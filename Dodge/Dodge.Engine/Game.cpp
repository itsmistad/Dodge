#include "Headers.h"
#include "Game.h"

extern void ExitGame();

using Microsoft::WRL::ComPtr;

Game * Game::Current = NULL;

Game::Game() noexcept(false)
{
	Log::Initialize();

	Log::Debug("Loading configuration files...");
	Configs::Load();
	Log::Debug("Done.");

    m_deviceResources = std::make_unique<DX::DeviceResources>();
    m_deviceResources->RegisterDeviceNotify(this);
}

Game::~Game()
{
    if (m_deviceResources)
        m_deviceResources->WaitForGpu();
	if (audioEngine)
		audioEngine->Suspend();
	SoundManager::GetCurrent()->ResetLoops();
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
	yRot = xRot = 0;
	Current = this;

	Log::Debug("Initializing KeyboardController...");
	keyboardController = KeyboardController();
	keyboardController.Initialize();
	Log::Debug("Done.");

	Log::Debug("Initializing MouseController...");
	mouseController = MouseController();
	mouseController.Initialize(window);
	Log::Debug("Done.");

	Log::Debug("Initializing DirectX resources...");
	m_deviceResources->SetWindow(window, width, height);

	m_deviceResources->CreateDeviceResources();
	CreateDeviceDependentResources();

	m_deviceResources->CreateWindowSizeDependentResources();
	CreateWindowSizeDependentResources();

	AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
	eflags = eflags | AudioEngine_Debug;
#endif
	audioEngine = make_unique<AudioEngine>(eflags);
	retryAudio = false;
	Log::Debug("Done.");

	Log::Debug("Initializing SoundManager...");
	SoundManager::Initialize(audioEngine.get());
	auto sndmgr = SoundManager::GetCurrent();
	sndmgr->Add("hit_1", L"Assets/Sounds/Effects/hit_1.wav", Effect, false);
	sndmgr->Add("hit_2", L"Assets/Sounds/Effects/hit_2.wav", Effect, false);
	sndmgr->Add("hit_3", L"Assets/Sounds/Effects/hit_3.wav", Effect, false);
	sndmgr->Add("hit_4", L"Assets/Sounds/Effects/hit_4.wav", Effect, false);
	sndmgr->Add("jump", L"Assets/Sounds/Effects/jump.wav", Effect, false);
	sndmgr->Add("land", L"Assets/Sounds/Effects/land.wav", Effect, false);
	sndmgr->Add("ui_click", L"Assets/Sounds/Effects/ui_click.wav", Effect, false);

	sndmgr->Add("titlescreen", L"Assets/Sounds/Music/titlescreen.wav", Music, true);
	sndmgr->SetLoopVolume("titlescreen", sndmgr->GetConfiguredVolume(Music));
	sndmgr->Add("ingame", L"Assets/Sounds/Music/ingame.wav", Music, true);
	sndmgr->SetLoopVolume("ingame", sndmgr->GetConfiguredVolume(Music));
	
	Log::Debug("Done.");

	Log::Debug("Starting title loop...");
	sndmgr->Play("titlescreen");

	Log::Debug("Ready!");

	auto cap = Configs::GetVideo().frameCap;
	if (cap != -1 && cap > 30)
	{
		m_timer.SetFixedTimeStep(true);
		m_timer.SetTargetElapsedSeconds(1.0 / cap);
	}
}

void Game::InitializeFonts(ID3D12Device * device, ResourceUploadBatch * resourceUpload)
{
	Font::Default = new Font(
		move(make_unique<SpriteFont>(
			device,
			*resourceUpload,
			L"Assets/Fonts/roboto.spritefont",
			resourceDescriptors->GetCpuHandle(UIDescriptors::RobotoFont),
			resourceDescriptors->GetGpuHandle(UIDescriptors::RobotoFont))
		));

	versionText = Text(L"Dodge v" + DodgeVersion, TopRight, Vector2(), Font::Default, 16, Colors::Red, true, false, true);
	fpsText = Text(L"", TopRight, Vector2(0, versionText.GetHeight()), Font::Default, 14, Colors::Gold, true, false, true);
	mouseXText = Text(L"X: 0", TopLeft, Vector2(), Font::Default, 14, Colors::White, true, false, true);
	mouseYText = Text(L"Y: 0", TopLeft, Vector2(0, mouseXText.GetHeight()), Font::Default, 14, Colors::White, true, false, true);
}

void Game::InitializeModels(ID3D12Device * device)
{
	m_model = Model::CreateFromSDKMESH(L"Assets/Models/game_map.sdkmesh", device);
}

void Game::InitializeTextures(ID3D12Device * device, ResourceUploadBatch * resourceUpload)
{
	crosshairImage = Image(device, resourceUpload, resourceDescriptors.get(),
		CrosshairImage,
		L"Assets/Images/General/crosshair.png",
		Center, Vector2(), .25f, Colors::White);
}

void Game::InitializeMenus(ID3D12Device * device, ResourceUploadBatch * resourceUpload)
{
	// TODO: Initialize menus here.
	mainMenu = Menu::Add("main", &mouseController);
	mainMenu.SetBackgroundImage(new Image(device, resourceUpload, resourceDescriptors.get(),
		BackgroundImage,
		L"Assets/Images/General/background.png",
		Vector2(), 1.0f, Colors::White, true, true));
	mainMenu.AddButton(
		new Button(device, resourceUpload, resourceDescriptors.get(),
			SettingsButton,
			L"",
			L"Assets/Images/Buttons/cog.png",
			BottomRight, Vector2(), Font::Default, .08f, 0, Colors::White,
	[]()->void {
		Menu::all["main"]->Hide();
		Menu::all["settings"]->Show();
	}));
	mainMenu.AddButton(
		new Button(device, resourceUpload, resourceDescriptors.get(),
			StartButton,
			L"Start",
			L"Assets/Images/Buttons/purple.png",
			Center, Vector2(), Font::Default, .5f, 14.f, Colors::White,
			[]()->void {
				Menu::currentMenu->Hide();
			}));
	mainMenu.AddImage(new Image(device, resourceUpload, resourceDescriptors.get(),
		LogoImage,
		L"Assets/Images/General/logo.png",
		TopCenter, Vector2(), .8f, Colors::White));
	mainMenu.Show();
	ingameMenu = Menu::Add("ingame", &mouseController);
}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    PIXBeginEvent(PIX_COLOR_DEFAULT, L"Update");

    float elapsedTime = float(timer.GetElapsedSeconds());

	// Update Audio
	if (retryAudio)
	{
		retryAudio = false;
		if (audioEngine->Reset())
			SoundManager::GetCurrent()->RestartLoops();
	}
	else if (!audioEngine->Update() && audioEngine->IsCriticalError())
		retryAudio = true;

	// Update Keyboard
	keyboardController.ProcessKeyStates();

	// Update Mouse
	mouseController.ProcessMouseState();

	// Update World
	yRot += mouseController.GetDelta().x;
	m_world = Matrix::CreateRotationY(yRot) * Matrix::CreateTranslation(Vector3(0, -25.f, 0));

	if (m_timer.GetFramesPerSecond() != 0)
		fpsText.content = to_wstring(m_timer.GetFramesPerSecond());

	mouseXText.content = L"X: " + to_wstring(mouseController.GetX());
	mouseYText.content = L"Y: " + to_wstring(mouseController.GetY());

    PIXEndEvent();
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    // Prepare the command list to render a new frame.
    m_deviceResources->Prepare();
    Clear();

    auto commandList = m_deviceResources->GetCommandList();
    PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Render");

    // BEGIN CUSTOM RENDERING CODE
	ID3D12DescriptorHeap* heaps[] = { resourceDescriptors->Heap() };
	commandList->SetDescriptorHeaps(_countof(heaps), heaps);
	
	Model::UpdateEffectMatrices(m_modelNormal, m_world, m_view, m_proj);
	m_model->Draw(m_deviceResources->GetCommandList(), m_modelNormal.cbegin());

	spriteBatch->Begin(commandList);
	crosshairImage.Draw(spriteBatch.get());
	// TODO: Draw extra menus here.
	mainMenu.Draw(spriteBatch.get());
	ingameMenu.Draw(spriteBatch.get());
	versionText.Draw(spriteBatch.get());
	fpsText.Draw(spriteBatch.get());
	if (Menu::currentMenu != NULL)
	{
		mouseXText.Draw(spriteBatch.get());
		mouseYText.Draw(spriteBatch.get());
	}
	spriteBatch->End();
	// END CUSTOM RENDERING CODE

    PIXEndEvent(commandList);

    // Show the new frame.
    PIXBeginEvent(m_deviceResources->GetCommandQueue(), PIX_COLOR_DEFAULT, L"Present");
    m_deviceResources->Present();
	m_graphicsMemory->Commit(m_deviceResources->GetCommandQueue());
    PIXEndEvent(m_deviceResources->GetCommandQueue());
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    auto commandList = m_deviceResources->GetCommandList();
    PIXBeginEvent(commandList, PIX_COLOR_DEFAULT, L"Clear");

    // Clear the views.
    auto rtvDescriptor = m_deviceResources->GetRenderTargetView();
    auto dsvDescriptor = m_deviceResources->GetDepthStencilView();
    commandList->OMSetRenderTargets(1, &rtvDescriptor, FALSE, &dsvDescriptor);
    commandList->ClearRenderTargetView(rtvDescriptor, Colors::CornflowerBlue, 0, nullptr);
    commandList->ClearDepthStencilView(dsvDescriptor, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

    // Set the viewport and scissor rect.
    auto viewport = m_deviceResources->GetScreenViewport();
    auto scissorRect = m_deviceResources->GetScissorRect();
    commandList->RSSetViewports(1, &viewport);
    commandList->RSSetScissorRects(1, &scissorRect);

    PIXEndEvent(commandList);
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
	audioEngine->Suspend();
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();
	audioEngine->Resume();
}

void Game::OnWindowMoved()
{
    auto r = m_deviceResources->GetOutputSize();
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);
}

void Game::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height)
{
    width = Configs::GetVideo().windowWidth;
    height = Configs::GetVideo().windowHeight;
}

Vector2 Game::GetCurrentSize()
{
	return Vector2(
		m_deviceResources->GetOutputSize().right,
		m_deviceResources->GetOutputSize().bottom);
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
    auto device = m_deviceResources->GetD3DDevice();

	m_graphicsMemory = make_unique<GraphicsMemory>(device);
	resourceDescriptors = make_unique<DescriptorHeap>(device,
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE,
		UIDescriptors::Count);
	m_fxFactory = make_unique<EffectFactory>(device);
	ResourceUploadBatch resourceUpload(device);
	resourceUpload.Begin();
	RenderTargetState rtState(m_deviceResources->GetBackBufferFormat(),
		m_deviceResources->GetDepthBufferFormat());
	EffectPipelineStateDescription pd(
		nullptr,
		CommonStates::Opaque,
		CommonStates::DepthDefault,
		CommonStates::CullClockwise,
		rtState);
	EffectPipelineStateDescription pdAlpha(
		nullptr,
		CommonStates::AlphaBlend,
		CommonStates::DepthDefault,
		CommonStates::CullClockwise,
		rtState);
	SpriteBatchPipelineStateDescription spd(rtState, &CommonStates::NonPremultiplied);
	spriteBatch = make_unique<SpriteBatch>(device, resourceUpload, spd);
	auto viewport = m_deviceResources->GetScreenViewport();

	spriteBatch->SetViewport(viewport);
	InitializeModels(device);
	InitializeFonts(device, &resourceUpload);
	InitializeTextures(device, &resourceUpload);
	InitializeMenus(device, &resourceUpload);

	auto uploadResourcesFinished = resourceUpload.End(m_deviceResources->GetCommandQueue());
	uploadResourcesFinished.wait();

	m_modelNormal = m_model->CreateEffects(*m_fxFactory, pd, pdAlpha);
	m_world = Matrix::Identity;
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
	m_view = Matrix::CreateLookAt(Vector3::Zero + Vector3(0, 1.8, 0),
		Vector3(30, 1.8, 30), Vector3::UnitY);
	auto size = m_deviceResources->GetOutputSize();
	m_proj = Matrix::CreatePerspectiveFieldOfView(80.f * XM_PI / 180.f,
		(float)(size.right / size.bottom), 0.1f, 10000.f);
	D3D12_VIEWPORT viewport = { 0.0f, 0.0f,
		static_cast<float>(size.right), static_cast<float>(size.bottom),
		D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
	spriteBatch->SetViewport(viewport);
}

void Game::OnDeviceLost()
{
	m_graphicsMemory.reset();
	m_fxFactory.reset();
	m_modelResources.reset();
	m_model.reset();
	m_modelNormal.clear();
	Font::Default->Reset();
	mainMenu.Reset();
	ingameMenu.Reset();
	crosshairImage.Reset();
	spriteBatch.reset();
	resourceDescriptors.reset();
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion
