#pragma once
#include "Headers.h"
#include "DeviceResources.h"
#include "StepTimer.h"
#include "SoundManager.h"
#include "KeyboardController.h"
#include "MouseController.h"
#include "Font.h"
#include "Text.h"
#include "Image.h"
#include "Menu.h"
#include "Camera.h"
#include "ObjectManager.h"

// A basic game implementation that creates a D3D12 device and
// provides a game loop.
class Game : public IDeviceNotify
{
public:
	const wstring dodgeVersion = L"1.1";
	const float frameTimeConstant = 0.0166666666666667f;
	static Game * Current;
	bool hasStarted = false;

    Game() noexcept(false);
    ~Game();

    // Initialization and management
    void Initialize(HWND window, int width, int height);
	void InitializePlayers();
	void InitializeModels(ResourceUploadBatch * resourceUpload);
	void InitializeFonts(ID3D12Device * device, ResourceUploadBatch * resourceUpload);
	void InitializeTextures(ID3D12Device * device, ResourceUploadBatch * resourceUpload);
	void InitializeMenus(ID3D12Device * device, ResourceUploadBatch * resourceUpload);

    // Basic game loop
    void Tick();

    // IDeviceNotify
    virtual void OnDeviceLost() override;
    virtual void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowMoved();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height );
	Vector2 GetCurrentSize();

	void OnNewAudioDevice() { retryAudio = true; }

	KeyboardController keyboardController;
	MouseController mouseController;
private:
    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    std::unique_ptr<DX::DeviceResources> m_deviceResources;
    DX::StepTimer m_timer;
	unique_ptr<GraphicsMemory> m_graphicsMemory;

	unique_ptr<DescriptorHeap> resourceDescriptors;
	unique_ptr<SpriteBatch> spriteBatch;
	unique_ptr<AudioEngine> audioEngine;

	Throwable * currentThrowable;
	Menu * mainMenu, * gameoverMenu, * ingameMenu, * settingsMenu, * controlsMenu, * audioMenu, *gameplayMenu;
	Text fpsText, versionText, mouseXText, mouseYText, pitchText, yawText, posXText, posYText, posZText, targetsText;
	Image crosshairImage;
	float radiansElapsed, currentPitchSpeed, currentYawVelocity;
	bool retryAudio;
};