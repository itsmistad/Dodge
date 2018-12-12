#include "Headers.h"
#include "MouseController.h"
#include "Camera.h"
#include "SoundManager.h"
#include "StringExtensions.hpp"

MouseController * MouseController::Current = NULL;

MouseController::MouseController()
{
}

void MouseController::Initialize(HWND windowHandle)
{
	mouse = make_unique<Mouse>();
	mouse->SetWindow(windowHandle);
	for (int i = LeftMouse; i <= RightMouse; i++)
		AddReleased(i);
	locked = false;
	Current = this;
}

void MouseController::AddPressed(int key)
{
	pressed.push_back(key);
}

void MouseController::AddReleased(int key)
{
	released.push_back(key);
}

void MouseController::RemovePressed(int key)
{
	pressed.erase(remove(pressed.begin(), pressed.end(), key), pressed.end());
}

void MouseController::RemoveReleased(int key)
{
	released.erase(remove(released.begin(), released.end(), key), released.end());
}

void MouseController::ProcessMouseState(float elapsedTime, float deltaTime, DX::DeviceResources * deviceResources)
{
	auto state = mouse->GetState();
	auto gameplayConfig = Configs::GetGameplay();
	if (state.leftButton)
		OnMousePress(LeftMouse, deviceResources);
	else
		OnMouseRelease(LeftMouse);
	if (state.middleButton)
		OnMousePress(MiddleMouse, deviceResources);
	else
		OnMouseRelease(MiddleMouse);
	if (state.rightButton)
		OnMousePress(RightMouse, deviceResources);
	else
		OnMouseRelease(RightMouse);

	if (state.positionMode == Mouse::MODE_RELATIVE)
	{
		auto smoothFactor = gameplayConfig.mouseSmoothing; // The larger this value, the less the smoothing. This value represents the amount of progress covered per tick.
		if (Camera::Main->canFreeCam)
			smoothFactor = 7.f;
		delta = Vector2((float)state.x, (float)state.y) * 0.0005f * gameplayConfig.mouseSensitivity * smoothFactor * deltaTime;
		smoothedDelta = Vector2::Lerp(smoothedDelta, delta, 1.f / smoothFactor);
	}
	else
	{
		smoothedDelta = delta = Vector2();
		if (Menu::currentMenu == Menu::all["audio"] && Menu::currentMenu->currentAudioButton != NULL)
		{
			float newValue = 0.f;
			switch (Menu::currentMenu->currentAudio)
			{
			case MasterAudioButton:
				newValue = min(1.0f, max(tempMasterVolume + ((Menu::currentMenu->startingMouseY - (float)GetY()) / 500.f), 0.0f));
				Configs::Current->audioConfig.masterVolume = newValue;
				Menu::currentMenu->currentAudioButton->SetText(to_wstring_with_precision(Configs::Current->audioConfig.masterVolume), Font::Default, 14, Colors::White);
				break;
			case MusicAudioButton:
				newValue = min(1.0f, max(tempMusicVolume + ((Menu::currentMenu->startingMouseY - (float)GetY()) / 500.f), 0.0f));
				Configs::Current->audioConfig.musicVolume = newValue;
				Menu::currentMenu->currentAudioButton->SetText(to_wstring_with_precision(Configs::Current->audioConfig.musicVolume), Font::Default, 14, Colors::White);
				break;
			case SFXAudioButton:
				newValue = min(1.0f, max(tempEffectsVolume + ((Menu::currentMenu->startingMouseY - (float)GetY()) / 500.f), 0.0f));
				Configs::Current->audioConfig.effectsVolume = newValue;
				Menu::currentMenu->currentAudioButton->SetText(to_wstring_with_precision(Configs::Current->audioConfig.effectsVolume), Font::Default, 14, Colors::White);
				break;
			case UIAudioButton:
				newValue = min(1.0f, max(tempUiVolume + ((Menu::currentMenu->startingMouseY - (float)GetY()) / 500.f), 0.0f));
				Configs::Current->audioConfig.uiVolume = newValue;
				Menu::currentMenu->currentAudioButton->SetText(to_wstring_with_precision(Configs::Current->audioConfig.uiVolume), Font::Default, 14, Colors::White);
				break;
			}
			SoundManager::GetCurrent()->SetLoopVolume("titlescreen", SoundManager::GetCurrent()->GetConfiguredVolume(Music));
			SoundManager::GetCurrent()->SetLoopVolume("ingame", SoundManager::GetCurrent()->GetConfiguredVolume(Music));
		}
	}

	elapsedTime;
}

void MouseController::OnMousePress(int key, DX::DeviceResources * deviceResources)
{
	if (IsMouseReleased(key) && !IsMousePressed(key))
	{
		RemoveReleased(key);
		AddPressed(key);

		if (key == LeftMouse)
		{
			if (Menu::currentMenu != NULL)
			{
				Menu::currentMenu->startingMouseY = (float)GetY();
				Menu::currentMenu->OnMousePress(Vector2((float)GetX(), (float)GetY()));
				tempMasterVolume = Configs::GetAudio().masterVolume;
				tempEffectsVolume = Configs::GetAudio().effectsVolume;
				tempMusicVolume = Configs::GetAudio().musicVolume;
				tempUiVolume = Configs::GetAudio().uiVolume;
			}
			else 
			{
				auto tempMatrix = Matrix::CreateFromYawPitchRoll(Camera::Main->yaw, Camera::Main->pitch, 0);
				auto adjustedForward = Vector3::Transform(Vector3::Forward * 3 + Vector3(0, -.04f, 0), tempMatrix);
				ObjectManager::Current->Create(new Throwable(deviceResources,
					L"throwing_knife", Camera::Main->position + adjustedForward, Vector3(Camera::Main->pitch, Camera::Main->yaw - (3.f *XM_PI/2.f), 0), adjustedForward, 100.f, 0.5f));
			}
		}
	}
}

void MouseController::OnMouseRelease(int key)
{
	if (IsMousePressed(key) && !IsMouseReleased(key))
	{
		RemovePressed(key);
		AddReleased(key);

		if (key == LeftMouse)
		{
			if (Menu::currentMenu != NULL)
				Menu::currentMenu->OnMouseRelease(Vector2((float)GetX(), (float)GetY()));
		}
	}
}

bool MouseController::IsMousePressed(int key)
{
	return find(pressed.begin(), pressed.end(), key) != pressed.end();
}

bool MouseController::IsMouseReleased(int key)
{
	return find(released.begin(), released.end(), key) != released.end();
}

void MouseController::SetLock(bool value)
{
	mouse->SetMode(!value ? Mouse::MODE_ABSOLUTE : Mouse::MODE_RELATIVE);
	locked = value;
}

Vector2 MouseController::GetDelta()
{
	return smoothedDelta;
}

int MouseController::GetX()
{
	return mouse->GetState().x;
}

int MouseController::GetY()
{
	return mouse->GetState().y;
}