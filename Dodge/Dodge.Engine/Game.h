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

// A basic game implementation that creates a D3D12 device and
// provides a game loop.
class Game : public IDeviceNotify
{
public:
	const wstring DodgeVersion = L"1.0.0";
	static Game * Current;

    Game() noexcept(false);
    ~Game();

    // Initialization and management
    void Initialize(HWND window, int width, int height);
	void InitializeFonts(ID3D12Device * device, ResourceUploadBatch * resourceUpload);
	void InitializeModels(ID3D12Device * device);
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

private:
    void Update(DX::StepTimer const& timer);
    void Render();

    void Clear();

    void CreateDeviceDependentResources();
    void CreateWindowSizeDependentResources();

    std::unique_ptr<DX::DeviceResources> m_deviceResources;
    DX::StepTimer m_timer;
	unique_ptr<EffectFactory> m_fxFactory;
	unique_ptr<GraphicsMemory> m_graphicsMemory;


	Matrix m_world;
	Matrix m_view;
	Matrix m_proj;
	unique_ptr<EffectTextureFactory> m_modelResources;
	unique_ptr<Model> m_model;
	vector<shared_ptr<IEffect>> m_modelNormal;


	unique_ptr<DescriptorHeap> resourceDescriptors;
	unique_ptr<SpriteBatch> spriteBatch;
	unique_ptr<AudioEngine> audioEngine;
	KeyboardController keyboardController;
	MouseController mouseController;

	Menu mainMenu, ingameMenu;
	Text fpsText, versionText, mouseXText, mouseYText;
	Image crosshairImage;
	DirectX::SimpleMath::Vector2 m_screenPos;
	DirectX::SimpleMath::Vector2 m_origin;
	bool retryAudio;
	float yRot, xRot;
};