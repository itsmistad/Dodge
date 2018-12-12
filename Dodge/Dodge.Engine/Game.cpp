#include "Headers.h"
#include "Game.h"
#include "StringExtensions.hpp"

extern void ExitGame();

using Microsoft::WRL::ComPtr;

Game * Game::Current = NULL;

Game::Game() noexcept(false)
{
	Log::Initialize();

	Log::Debug("Loading configuration files...");
	Configs::Load();
	Log::Debug("Done.");

    m_deviceResources = std::make_unique<DX::DeviceResources>(DXGI_FORMAT_B8G8R8A8_UNORM,
		DXGI_FORMAT_D32_FLOAT,
		2,
		D3D_FEATURE_LEVEL_11_0,
		0x3);
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
	Current = this;

	Log::Debug("Initializing MouseController...");
	mouseController = MouseController();
	mouseController.Initialize(window);
	Log::Debug("Done.");

	Log::Debug("Initializing KeyboardController...");
	keyboardController = KeyboardController();
	keyboardController.Initialize(&mouseController);
	Log::Debug("Done.");


	Log::Debug("Initializing DirectX resources...");
	m_deviceResources->SetWindow(window, width, height);

	m_deviceResources->CreateDeviceResources();
	CreateDeviceDependentResources();

	Camera::Main = new Camera(m_deviceResources.get());

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
	sndmgr->Add("ui_click", L"Assets/Sounds/Effects/ui_click.wav", UI, false);

	sndmgr->Add("titlescreen", L"Assets/Sounds/Music/titlescreen.wav", Music, true);
	sndmgr->SetLoopVolume("titlescreen", sndmgr->GetConfiguredVolume(Music));
	sndmgr->Add("ingame", L"Assets/Sounds/Music/ingame.wav", Music, true);
	sndmgr->SetLoopVolume("ingame", sndmgr->GetConfiguredVolume(Music));
	
	Log::Debug("Done.");

	Log::Debug("Starting title loop...");
	sndmgr->Play("titlescreen");

	Log::Debug("Ready!");

	auto cap = Configs::GetVideo().frameCap;
	if (cap != -1 && cap >= 30)
	{
		m_timer.SetFixedTimeStep(true);
		m_timer.SetTargetElapsedSeconds(1.0 / cap);
	}

	radiansElapsed = 0;
}

void Game::InitializePlayers()
{
	ObjectManager::TotalTargets = 0;
	ObjectManager::Current->Create(new Player(m_deviceResources.get(),
		Vector3(-167, 122, 269), Vector3(0, XM_PI, 0), 2.f));
	ObjectManager::Current->Create(new Player(m_deviceResources.get(),
		Vector3(-127, 157, 247), Vector3(0, XM_PI, 0), 2.f));
	ObjectManager::Current->Create(new Player(m_deviceResources.get(),
		Vector3(-45, 110, 272), Vector3(0, XM_PI, 0), 2.f));
	ObjectManager::Current->Create(new Player(m_deviceResources.get(),
		Vector3(-88, 110, 342), Vector3(0, XM_PI, 0), 2.f));
	ObjectManager::Current->Create(new Player(m_deviceResources.get(),
		Vector3(-35, 38, 373), Vector3(0, XM_PI, 0), 2.f));
	ObjectManager::Current->Create(new Player(m_deviceResources.get(),
		Vector3(27, 133, 304), Vector3(0, XM_PI, 0), 2.f));
	ObjectManager::Current->Create(new Player(m_deviceResources.get(),
		Vector3(75, 78, 268), Vector3(0, XM_PI, 0), 2.f));
	ObjectManager::Current->Create(new Player(m_deviceResources.get(),
		Vector3(104, 160, 340), Vector3(0, XM_PI, 0), 2.f));
	ObjectManager::Current->Create(new Player(m_deviceResources.get(),
		Vector3(142, 145, 270), Vector3(0, XM_PI, 0), 2.f));
	ObjectManager::Current->Create(new Player(m_deviceResources.get(),
		Vector3(143, 145, -336), Vector3(0, 0, 0), 2.f));
	ObjectManager::Current->Create(new Player(m_deviceResources.get(),
		Vector3(105, 161, -274), Vector3(0, 0, 0), 2.f));
	ObjectManager::Current->Create(new Player(m_deviceResources.get(),
		Vector3(74, 79, -339), Vector3(0, 0, 0), 2.f));
	ObjectManager::Current->Create(new Player(m_deviceResources.get(),
		Vector3(26, 133, -306), Vector3(0, 0, 0), 2.f));
	ObjectManager::Current->Create(new Player(m_deviceResources.get(),
		Vector3(-35, 35, -234), Vector3(0, 0, 0), 2.f));
	ObjectManager::Current->Create(new Player(m_deviceResources.get(),
		Vector3(-46, 110, -335), Vector3(0, 0, 0), 2.f));
	ObjectManager::Current->Create(new Player(m_deviceResources.get(),
		Vector3(-90, 109, -263), Vector3(0, 0, 0), 2.f));
	ObjectManager::Current->Create(new Player(m_deviceResources.get(),
		Vector3(-127, 156, -356), Vector3(0, 0, 0), 2.f));
	ObjectManager::Current->Create(new Player(m_deviceResources.get(),
		Vector3(-167, 122, -345), Vector3(0, 0, 0), 2.f));
	ObjectManager::Current->Create(new Player(m_deviceResources.get(),
		Vector3(-303, 89, -29), Vector3(0, 1.f / 2.f * XM_PI, 0), 2.f));
	ObjectManager::Current->Create(new Player(m_deviceResources.get(),
		Vector3(-345, 92, 57), Vector3(0, 1.f / 2.f * XM_PI, 0), 2.f));
	ObjectManager::Current->Create(new Player(m_deviceResources.get(),
		Vector3(439, 103, -24), Vector3(0, 3.f / 2.f * XM_PI, 0), 2.f));
	ObjectManager::Current->Create(new Player(m_deviceResources.get(),
		Vector3(387, 128, 34), Vector3(0, 3.f / 2.f * XM_PI, 0), 2.f));
	ObjectManager::Current->Create(new Player(m_deviceResources.get(),
		Vector3(320, 120, 94), Vector3(0, 3.f / 2.f * XM_PI, 0), 2.f));
	ObjectManager::Current->Create(new Player(m_deviceResources.get(),
		Vector3(351, 95, -36), Vector3(0, 3.f / 2.f * XM_PI, 0), 2.f));
	ObjectManager::Current->Create(new Player(m_deviceResources.get(),
		Vector3(490, 88, 87), Vector3(0, 3.f / 2.f * XM_PI, 0), 2.f));
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

	versionText = Text(L"Dodge v" + dodgeVersion, TopRight, Vector2(), Font::Default, 16, Colors::Red, true, false, true);
	fpsText = Text(L"", TopRight, Vector2(0, versionText.GetHeight()), Font::Default, 14, Colors::Gold, true, false, true);
	mouseXText = Text(L"X: 0", TopLeft, Vector2(), Font::Default, 14, Colors::White, true, false, true);
	mouseYText = Text(L"Y: 0", TopLeft, Vector2(0, mouseXText.GetHeight()), Font::Default, 14, Colors::White, true, false, true);
	pitchText = Text(L"Pitch: 0", TopLeft, Vector2(), Font::Default, 14, Colors::White, true, false, true);
	yawText = Text(L"Yaw: 0", TopLeft, Vector2(0, pitchText.GetHeight()), Font::Default, 14, Colors::White, true, false, true);
	posXText = Text(L"X: 0", TopLeft, Vector2(0, yawText.displacement.y + yawText.GetHeight()), Font::Default, 14, Colors::White, true, false, true);
	posYText = Text(L"Y: 0", TopLeft, Vector2(0, posXText.displacement.y + posXText.GetHeight()), Font::Default, 14, Colors::White, true, false, true);
	posZText = Text(L"Z: 0", TopLeft, Vector2(0, posYText.displacement.y + posYText.GetHeight()), Font::Default, 14, Colors::White, true, false, true);
	targetsText = Text(L"0/0", TopCenter, Vector2(0, 100), Font::Default, 18, Colors::Red, true, false, true);
}

void Game::InitializeModels(ResourceUploadBatch * resourceUpload)
{
	RenderTargetState rtState(DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_FORMAT_D32_FLOAT);
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
	ObjectManager::Current = new ObjectManager();
	Object * mapObject = new Object(m_deviceResources.get(),
		L"Assets/Models/gamefloor.sdkmesh", Vector3(0, -1.0f, 0), Vector3::Zero, 1);
	mapObject->SetNormals(pd, pdAlpha);
	ObjectManager::Current->SetMap(mapObject);

	// Add all the player targets.
	InitializePlayers();

	ObjectManager::You = new Player(m_deviceResources.get(), Vector3(0, -10099, 0), Vector3());
	ObjectManager::You->SetNormals(pd, pdAlpha);

	resourceUpload;
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
	mainMenu->SetBackgroundImage(new Image(device, resourceUpload, resourceDescriptors.get(),
		BackgroundImage,
		L"Assets/Images/General/background-transparent.png",
		Vector2(), 1.0f, Colors::White, true, true));
	mainMenu->AddText(
		new Text(L"DODGE", Center, Vector2(0, -250), Font::Default, 60, Colors::Red, true, true, false)
	);
	mainMenu->AddButton(
		new Button(device, resourceUpload, resourceDescriptors.get(),
			SettingsButton,
			L"",
			L"Assets/Images/Buttons/cog.png",
			BottomRight, Vector2(), Font::Default, .08f, 0, Colors::White,
			[](Button * sender)->void {
				Menu::all["main"]->Hide();
				Menu::all["settings"]->Show();
				sender;
			}));
	mainMenu->AddButton(
		new Button(device, resourceUpload, resourceDescriptors.get(),
			StartButton,
			L"Start",
			L"Assets/Images/Buttons/purple.png",
			Center, Vector2(), Font::Default, .5f, 14, Colors::White,
			[](Button * sender)->void {
				ObjectManager::You->SetPosition(Vector3());
				ObjectManager::You->isGrounded = true;
				ObjectManager::TargetsKilled = 0;
				Camera::Main->position = Vector3(0, 5.6f, 0);
				Camera::Main->LookAt(Vector3(1, 0, 1));
				Menu::all["main"]->Hide();
				Game::Current->mouseController.SetLock(true);
				SoundManager::GetCurrent()->StopLoop("titlescreen");
				SoundManager::GetCurrent()->Play("ingame");
				Game::Current->hasStarted = true;
				sender;
			}));
	mainMenu->AddButton(
		new Button(device, resourceUpload, resourceDescriptors.get(),
			QuitGameButton,
			L"Quit",
			L"Assets/Images/Buttons/gray.png",
			Center, Vector2(0, 50), Font::Default, .5f, 14, Colors::White,
			[](Button * sender)->void {
				Log::Info("Exiting game... quitter.");
				ExitGame();
				sender;
			}));
	mainMenu->Show();

	gameoverMenu = Menu::Add("gameover", &mouseController);
	gameoverMenu->SetBackgroundImage(new Image(device, resourceUpload, resourceDescriptors.get(),
		BackgroundTransparentImage,
		L"Assets/Images/General/background-transparent.png",
		Vector2(), 1.0f, Colors::White, true, true));
	gameoverMenu->AddText(
		new Text(L"Nicely done!", Center, Vector2(0, -250), Font::Default, 40, Colors::Red, true, true, false)
	);
	gameoverMenu->AddText(
		new Text(L"Game Over", Center, Vector2(0, -200), Font::Default, 30, Colors::White, true, true, false)
	);
	gameoverMenu->AddButton(
		new Button(device, resourceUpload, resourceDescriptors.get(),
			BackToMainMenuButton,
			L"Quit",
			L"Assets/Images/Buttons/purple.png",
			Center, Vector2(), Font::Default, .7f, 14, Colors::White,
			[](Button * sender)->void {
				ObjectManager::Current->ResetField();
				Game::Current->InitializePlayers();
				Menu::all["gameover"]->Hide();
				Menu::all["main"]->Show();
				SoundManager::GetCurrent()->StopLoop("ingame");
				SoundManager::GetCurrent()->Play("titlescreen");
				ObjectManager::You->SetPosition(Vector3(0, -10099, 0));
				Game::Current->hasStarted = false;
				sender;
	}));

	settingsMenu = Menu::Add("settings", &mouseController);
	settingsMenu->AddText(
		new Text(L"Settings", Center, Vector2(0, -250), Font::Default, 40, Colors::White, true, true, false)
	);
	settingsMenu->SetBackgroundImage(new Image(device, resourceUpload, resourceDescriptors.get(),
		BackgroundTransparentImage,
		L"Assets/Images/General/background-transparent.png",
		Vector2(), 1.0f, Colors::White, true, true));
	settingsMenu->AddButton(
		new Button(device, resourceUpload, resourceDescriptors.get(),
			ControlsButton,
			L"Controls",
			L"Assets/Images/Buttons/gray.png",
			Center, Vector2(0, -50), Font::Default, .5f, 14, Colors::White,
			[](Button * sender)->void {
				Menu::all["settings"]->Hide();
				Menu::all["controls"]->Show();
				sender;
			}));
	settingsMenu->AddButton(
		new Button(device, resourceUpload, resourceDescriptors.get(),
			AudioButton,
			L"Audio",
			L"Assets/Images/Buttons/gray.png",
			Center, Vector2(), Font::Default, .5f, 14, Colors::White,
			[](Button * sender)->void {
				Menu::all["settings"]->Hide();
				Menu::all["audio"]->Show();
				sender;
			}));
	settingsMenu->AddButton(
		new Button(device, resourceUpload, resourceDescriptors.get(),
			PhysicsButton,
			L"Gameplay",
			L"Assets/Images/Buttons/gray.png",
			Center, Vector2(0, 50), Font::Default, .5f, 14, Colors::White,
			[](Button * sender)->void {
				Menu::all["settings"]->Hide();
				Menu::all["gameplay"]->Show();
				sender;
			}));
	settingsMenu->AddButton(
		new Button(device, resourceUpload, resourceDescriptors.get(),
			BackToMainMenuButton,
			L"Back",
			L"Assets/Images/Buttons/purple.png",
			Center, Vector2(0, 300), Font::Default, .7f, 14, Colors::White,
			[](Button * sender)->void {
				Menu::all["settings"]->Hide();
				if (Game::Current->hasStarted)
					Menu::all["ingame"]->Show();
				else
					Menu::all["main"]->Show();
				sender;
			}));

	controlsMenu = Menu::Add("controls", &mouseController);
	controlsMenu->SetBackgroundImage(new Image(device, resourceUpload, resourceDescriptors.get(),
		BackgroundTransparentImage,
		L"Assets/Images/General/background-transparent.png",
		Vector2(), 1.0f, Colors::White, true, true));
	controlsMenu->AddText(
		new Text(L"Controls", Center, Vector2(0, -250), Font::Default, 40, Colors::White, true, true, false)
	);
	controlsMenu->AddText(
		new Text(L"Forward", Center, Vector2(-200, -150), Font::Default, 20, Colors::White, true, false, true)
	);
	controlsMenu->AddText(
		new Text(L"Backward", Center, Vector2(-200, -100), Font::Default, 20, Colors::White, true, false, true)
	);
	controlsMenu->AddText(
		new Text(L"Left", Center, Vector2(-200, -50), Font::Default, 20, Colors::White, true, false, true)
	);
	controlsMenu->AddText(
		new Text(L"Right", Center, Vector2(-200, 0), Font::Default, 20, Colors::White, true, false, true)
	);
	controlsMenu->AddText(
		new Text(L"Jump", Center, Vector2(-200, 50), Font::Default, 20, Colors::White, true, false, true)
	);
	controlsMenu->AddText(
		new Text(L"Sprint", Center, Vector2(-200, 100), Font::Default, 20, Colors::White, true, false, true)
	);
	//controlsMenu->AddText(
	//	new Text(L"Use", Center, Vector2(-200, 150), Font::Default, 20, Colors::White, true, false, true)
	//);
	auto keybinds = Configs::GetKeybinds();
	controlsMenu->AddButton(
		new Button(device, resourceUpload, resourceDescriptors.get(),
			ForwardButton,
			keybinds.forward->GetName(),
			L"Assets/Images/Buttons/gray.png",
			Center, Vector2(0, -150), Font::Default, .5f, 14, Colors::White,
			[](Button * sender)->void {
				Menu::currentMenu->StartKeybindChange(sender, Configs::GetKeybinds().forward);
			}));
	controlsMenu->AddButton(
		new Button(device, resourceUpload, resourceDescriptors.get(),
			ForwardButton,
			keybinds.backward->GetName(),
			L"Assets/Images/Buttons/gray.png",
			Center, Vector2(0, -100), Font::Default, .5f, 14, Colors::White,
			[](Button * sender)->void {
				Menu::currentMenu->StartKeybindChange(sender, Configs::GetKeybinds().backward);
			}));
	controlsMenu->AddButton(
		new Button(device, resourceUpload, resourceDescriptors.get(),
			LeftButton,
			keybinds.left->GetName(),
			L"Assets/Images/Buttons/gray.png",
			Center, Vector2(0, -50), Font::Default, .5f, 14, Colors::White,
			[](Button * sender)->void {
				Menu::currentMenu->StartKeybindChange(sender, Configs::GetKeybinds().left);
			}));
	controlsMenu->AddButton(
		new Button(device, resourceUpload, resourceDescriptors.get(),
			RightButton,
			keybinds.right->GetName(),
			L"Assets/Images/Buttons/gray.png",
			Center, Vector2(0, 0), Font::Default, .5f, 14, Colors::White,
			[](Button * sender)->void {
				Menu::currentMenu->StartKeybindChange(sender, Configs::GetKeybinds().right);
			}));
	controlsMenu->AddButton(
		new Button(device, resourceUpload, resourceDescriptors.get(),
			JumpButton,
			keybinds.jump->GetName(),
			L"Assets/Images/Buttons/gray.png",
			Center, Vector2(0, 50), Font::Default, .5f, 14, Colors::White,
			[](Button * sender)->void {
				Menu::currentMenu->StartKeybindChange(sender, Configs::GetKeybinds().jump);
			}));
	controlsMenu->AddButton(
		new Button(device, resourceUpload, resourceDescriptors.get(),
			SprintButton,
			keybinds.sprint->GetName(),
			L"Assets/Images/Buttons/gray.png",
			Center, Vector2(0, 100), Font::Default, .5f, 14, Colors::White,
			[](Button * sender)->void {
		Menu::currentMenu->StartKeybindChange(sender, Configs::GetKeybinds().sprint);
	}));
	/*controlsMenu->AddButton(
		new Button(device, resourceUpload, resourceDescriptors.get(),
			UseButton,
			keybinds.use->GetName(),
			L"Assets/Images/Buttons/gray.png",
			Center, Vector2(0, 150), Font::Default, .5f, 14, Colors::White,
			[](Button * sender)->void {
				Menu::currentMenu->StartKeybindChange(sender, Configs::GetKeybinds().use);
			}));*/
	controlsMenu->AddButton(
		new Button(device, resourceUpload, resourceDescriptors.get(),
			BackToMainMenuButton,
			L"Back To Settings",
			L"Assets/Images/Buttons/purple.png",
			Center, Vector2(0, 300), Font::Default, .7f, 14, Colors::White,
			[](Button * sender)->void {
				Menu::all["controls"]->Hide();
				Menu::all["settings"]->Show();
				sender;
			}));

	audioMenu = Menu::Add("audio", &mouseController);
	audioMenu->AddText(
		new Text(L"Audio", Center, Vector2(0, -250), Font::Default, 40, Colors::White, true, true, false)
	);
	audioMenu->SetBackgroundImage(new Image(device, resourceUpload, resourceDescriptors.get(),
		BackgroundTransparentImage,
		L"Assets/Images/General/background-transparent.png",
		Vector2(), 1.0f, Colors::White, true, true));
	audioMenu->AddText(
		new Text(L"Master", Center, Vector2(-200, -50), Font::Default, 20, Colors::White, true, false, true)
	);
	audioMenu->AddText(
		new Text(L"Music", Center, Vector2(-200, 0), Font::Default, 20, Colors::White, true, false, true)
	);
	audioMenu->AddText(
		new Text(L"SFX", Center, Vector2(-200, 50), Font::Default, 20, Colors::White, true, false, true)
	);
	audioMenu->AddText(
		new Text(L"UI", Center, Vector2(-200, 100), Font::Default, 20, Colors::White, true, false, true)
	);
	audioMenu->AddButton(
		new Button(device, resourceUpload, resourceDescriptors.get(),
			MasterAudioButton,
			to_wstring_with_precision(Configs::GetAudio().masterVolume),
			L"Assets/Images/Buttons/gray.png",
			Center, Vector2(0, -50), Font::Default, .5f, 14, Colors::White,
			[](Button * sender)->void {
		Menu::currentMenu->StartVolumeChange(sender, MasterAudioButton);
			}));
	audioMenu->AddButton(
		new Button(device, resourceUpload, resourceDescriptors.get(),
			MusicAudioButton,
			to_wstring_with_precision(Configs::GetAudio().musicVolume),
			L"Assets/Images/Buttons/gray.png",
			Center, Vector2(0, 0), Font::Default, .5f, 14, Colors::White,
			[](Button * sender)->void {
		Menu::currentMenu->StartVolumeChange(sender, MusicAudioButton);
			}));
	audioMenu->AddButton(
		new Button(device, resourceUpload, resourceDescriptors.get(),
			SFXAudioButton,
			to_wstring_with_precision(Configs::GetAudio().effectsVolume),
			L"Assets/Images/Buttons/gray.png",
			Center, Vector2(0, 50), Font::Default, .5f, 14, Colors::White,
			[](Button * sender)->void {
		Menu::currentMenu->StartVolumeChange(sender, SFXAudioButton);
			}));
	audioMenu->AddButton(
		new Button(device, resourceUpload, resourceDescriptors.get(),
			UIAudioButton,
			to_wstring_with_precision(Configs::GetAudio().uiVolume),
			L"Assets/Images/Buttons/gray.png",
			Center, Vector2(0, 100), Font::Default, .5f, 14, Colors::White,
			[](Button * sender)->void {
		Menu::currentMenu->StartVolumeChange(sender, UIAudioButton);
	}));
	audioMenu->AddButton(
		new Button(device, resourceUpload, resourceDescriptors.get(),
			BackToMainMenuButton,
			L"Back To Settings",
			L"Assets/Images/Buttons/purple.png",
			Center, Vector2(0, 300), Font::Default, .7f, 14, Colors::White,
			[](Button * sender)->void {
				Menu::all["audio"]->Hide();
				Menu::all["settings"]->Show();
				sender;
			}));

	gameplayMenu = Menu::Add("gameplay", &mouseController);
	gameplayMenu->AddText(
		new Text(L"Gameplay", Center, Vector2(0, -250), Font::Default, 40, Colors::White, true, true, false)
	);
	gameplayMenu->AddText(
		new Text(L"The following settings cannot be changed from this menu.", Center, Vector2(0, -200), Font::Default, 12, Colors::SlateGray, true, true, false)
	);
	gameplayMenu->AddText(
		new Text(L"To make changes, modify the gameplay configuration and restart:", Center, Vector2(0, -175), Font::Default, 12, Colors::SlateGray, true, true, false)
	);
	gameplayMenu->AddText(
		new Text(L"./Config/Gameplay.json", Center, Vector2(0, -150), Font::Default, 12, Colors::LightGray, true, true, false)
	);
	gameplayMenu->SetBackgroundImage(new Image(device, resourceUpload, resourceDescriptors.get(),
		BackgroundTransparentImage,
		L"Assets/Images/General/background-transparent.png",
		Vector2(), 1.0f, Colors::White, true, true));
	gameplayMenu->AddText(
		new Text(L"Gravity", Center, Vector2(-100, -25), Font::Default, 14, Colors::White, true, false, true)
	);
	gameplayMenu->AddText(
		new Text(to_wstring_with_precision(Configs::GetGameplay().gravity, 2), Center, Vector2(100, -25), Font::Default, 14, Colors::SlateGray, true, false, true)
	);
	gameplayMenu->AddText(
		new Text(L"Walk Speed", Center, Vector2(-100, 0), Font::Default, 14, Colors::White, true, false, true)
	);
	gameplayMenu->AddText(
		new Text(to_wstring_with_precision(Configs::GetGameplay().walkSpeed, 2), Center, Vector2(100, 0), Font::Default, 14, Colors::SlateGray, true, false, true)
	);
	gameplayMenu->AddText(
		new Text(L"Sprint Speed", Center, Vector2(-100, 25), Font::Default, 14, Colors::White, true, false, true)
	);
	gameplayMenu->AddText(
		new Text(to_wstring_with_precision(Configs::GetGameplay().sprintSpeed, 2), Center, Vector2(100, 25), Font::Default, 14, Colors::SlateGray, true, false, true)
	);
	gameplayMenu->AddText(
		new Text(L"Jump Speed", Center, Vector2(-100, 50), Font::Default, 14, Colors::White, true, false, true)
	);
	gameplayMenu->AddText(
		new Text(to_wstring_with_precision(Configs::GetGameplay().jumpSpeed, 2), Center, Vector2(100, 50), Font::Default, 14, Colors::SlateGray, true, false, true)
	);
	gameplayMenu->AddText(
		new Text(L"Mouse Sensitivity", Center, Vector2(-100, 75), Font::Default, 14, Colors::White, true, false, true)
	);
	gameplayMenu->AddText(
		new Text(to_wstring_with_precision(Configs::GetGameplay().mouseSensitivity, 2), Center, Vector2(100, 75), Font::Default, 14, Colors::SlateGray, true, false, true)
	);
	gameplayMenu->AddText(
		new Text(L"Mouse Smoothing", Center, Vector2(-100, 100), Font::Default, 14, Colors::White, true, false, true)
	);
	gameplayMenu->AddText(
		new Text(to_wstring_with_precision(Configs::GetGameplay().mouseSmoothing, 2), Center, Vector2(100, 100), Font::Default, 14, Colors::SlateGray, true, false, true)
	);
	gameplayMenu->AddButton(
		new Button(device, resourceUpload, resourceDescriptors.get(),
			BackToMainMenuButton,
			L"Back To Settings",
			L"Assets/Images/Buttons/purple.png",
			Center, Vector2(0, 300), Font::Default, .7f, 14, Colors::White,
			[](Button * sender)->void {
				Menu::all["gameplay"]->Hide();
				Menu::all["settings"]->Show();
				sender;
			}));

	ingameMenu = Menu::Add("ingame", &mouseController);
	ingameMenu->AddText(
		new Text(L"What? Can't take the heat?!", Center, Vector2(0, -250), Font::Default, 50, Colors::Red, true, true, false)
	);
	ingameMenu->SetBackgroundImage(new Image(device, resourceUpload, resourceDescriptors.get(),
		BackgroundTransparentImage,
		L"Assets/Images/General/background-transparent.png",
		Vector2(), 1.0f, Colors::White, true, true));
	ingameMenu->AddButton(
		new Button(device, resourceUpload, resourceDescriptors.get(),
			BackToMainMenuButton,
			L"Quit",
			L"Assets/Images/Buttons/purple.png",
			Center, Vector2(), Font::Default, .5f, 14, Colors::White,
			[](Button * sender)->void {
				ObjectManager::Current->ResetField();
				Game::Current->InitializePlayers();
				Menu::all["ingame"]->Hide();
				Menu::all["main"]->Show();
				SoundManager::GetCurrent()->StopLoop("ingame");
				SoundManager::GetCurrent()->Play("titlescreen");
				ObjectManager::You->SetPosition(Vector3(0, -10099, 0));
				Game::Current->hasStarted = false;
				sender;
			}));
	ingameMenu->AddButton(
		new Button(device, resourceUpload, resourceDescriptors.get(),
			SettingsButton,
			L"",
			L"Assets/Images/Buttons/cog.png",
			BottomRight, Vector2(), Font::Default, .08f, 0, Colors::White,
			[](Button * sender)->void {
				Menu::all["ingame"]->Hide();
				Menu::all["settings"]->Show();
				sender;
			}));
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
    float deltaTime = elapsedTime / frameTimeConstant;

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
	keyboardController.ProcessKeyStates(deltaTime);

	// Update Mouse
	mouseController.ProcessMouseState(elapsedTime, deltaTime, m_deviceResources.get());

	// Update Camera
	if (!hasStarted)
	{
		radiansElapsed += (XM_PI / 1024) * deltaTime;
		Camera::Main->position = Vector3(cos(radiansElapsed) * 600, 400, sin(radiansElapsed) * 600);
		Camera::Main->LookAt(Vector3(0, 10, 0), true);
	}
	else if (Menu::currentMenu == NULL)
	{
		auto currentPitch = Camera::Main->pitch;
		auto projectedPitch = currentPitch - mouseController.GetDelta().y;
		projectedPitch = min(1.5f, max(projectedPitch, -1.3f));
		auto currentYaw = Camera::Main->yaw;
		auto projectedYaw = currentYaw - mouseController.GetDelta().x;
		if (projectedYaw < 0)
		{
			projectedYaw += 2 * XM_PI;
			Camera::Main->yaw += 2 * XM_PI;
		}
		else if (projectedYaw > 2 * XM_PI)
		{
			projectedYaw -= 2 * XM_PI;
			Camera::Main->yaw -= 2 * XM_PI;
	}
		Camera::Main->yaw = projectedYaw;
		Camera::Main->pitch = projectedPitch;
		Camera::Main->Update(elapsedTime, deltaTime);
		if (!Camera::Main->canFreeCam)
		{
			ObjectManager::You->SetRotation(Vector3(0, Camera::Main->yaw + XM_PI, 0));
			ObjectManager::You->SetPosition(Camera::Main->position - Vector3(0, 5.6f, 0.f));
		}
	}

	// Update Objects
	ObjectManager::Current->Update(deltaTime);
	ObjectManager::You->Update(deltaTime);

	// Update Texts
	if (m_timer.GetFramesPerSecond() != 0)
		fpsText.content = to_wstring(m_timer.GetFramesPerSecond());

	mouseXText.content = L"X: " + to_wstring(mouseController.GetX());
	mouseYText.content = L"Y: " + to_wstring(mouseController.GetY());

	pitchText.content = L"Pitch: " + to_wstring(Camera::Main->pitch * 180 / XM_PI) + L" degrees";
	yawText.content = L"Yaw: " + to_wstring(Camera::Main->yaw * 180 / XM_PI) + L" degrees";
	posXText.content = L"X: " + to_wstring(Camera::Main->position.x);
	posYText.content = L"Y: " + to_wstring(Camera::Main->position.y);
	posZText.content = L"Z: " + to_wstring(Camera::Main->position.z);
	targetsText.content = to_wstring(ObjectManager::TargetsKilled) + L"/" + to_wstring(ObjectManager::TotalTargets);

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

	spriteBatch->Begin(commandList);
	ObjectManager::Current->Draw(Camera::Main->viewMatrix, Camera::Main->perspectiveMatrix);
	ObjectManager::You->Draw(Camera::Main->viewMatrix, Camera::Main->perspectiveMatrix);

	crosshairImage.Draw(spriteBatch.get());
	mainMenu->Draw(spriteBatch.get());
	settingsMenu->Draw(spriteBatch.get());
	ingameMenu->Draw(spriteBatch.get());
	gameoverMenu->Draw(spriteBatch.get());
	audioMenu->Draw(spriteBatch.get());
	controlsMenu->Draw(spriteBatch.get());
	gameplayMenu->Draw(spriteBatch.get());
	versionText.Draw(spriteBatch.get());
	fpsText.Draw(spriteBatch.get());
	if (Menu::currentMenu != NULL)
	{
		mouseXText.Draw(spriteBatch.get());
		mouseYText.Draw(spriteBatch.get());
	}
	else
	{
		pitchText.Draw(spriteBatch.get());
		yawText.Draw(spriteBatch.get());
		posXText.Draw(spriteBatch.get());
		posYText.Draw(spriteBatch.get());
		posZText.Draw(spriteBatch.get());
		targetsText.Draw(spriteBatch.get());
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
    commandList->ClearRenderTargetView(rtvDescriptor, Colors::DeepSkyBlue, 0, nullptr);
    commandList->ClearDepthStencilView(dsvDescriptor, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

    // Set the viewport and scissor rect.
	auto size = m_deviceResources->GetOutputSize();
	D3D12_VIEWPORT viewport = { 0.0f, 0.0f,
		static_cast<float>(size.right), static_cast<float>(size.bottom),
		D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
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
		(float)m_deviceResources->GetOutputSize().right,
		(float)m_deviceResources->GetOutputSize().bottom);
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
	ResourceUploadBatch resourceUpload(device);
	resourceUpload.Begin();
	RenderTargetState rtState(m_deviceResources->GetBackBufferFormat(), m_deviceResources->GetDepthBufferFormat());
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
	auto size = m_deviceResources->GetOutputSize();
	D3D12_VIEWPORT viewport = { 0.0f, 0.0f,
		static_cast<float>(size.right), static_cast<float>(size.bottom),
		D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
	spriteBatch->SetViewport(viewport);
	InitializeModels(&resourceUpload);
	InitializeFonts(device, &resourceUpload);
	InitializeTextures(device, &resourceUpload);
	InitializeMenus(device, &resourceUpload);

	auto uploadResourcesFinished = resourceUpload.End(m_deviceResources->GetCommandQueue());
	uploadResourcesFinished.wait();
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
	Camera::Main->SetFOV(Camera::Main->GetFOV()); // Update the perspective
	auto size = m_deviceResources->GetOutputSize();
	D3D12_VIEWPORT viewport = { 0.0f, 0.0f,
		static_cast<float>(size.right), static_cast<float>(size.bottom),
		D3D12_MIN_DEPTH, D3D12_MAX_DEPTH };
	spriteBatch->SetViewport(viewport);
}

void Game::OnDeviceLost()
{
	m_graphicsMemory.reset();
	ObjectManager::Current->Reset();
	ObjectManager::You->Reset();
	Font::Default->Reset();
	mainMenu->Reset();
	settingsMenu->Reset();
	gameoverMenu->Reset();
	ingameMenu->Reset();
	audioMenu->Reset();
	gameplayMenu->Reset();
	controlsMenu->Reset();
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
