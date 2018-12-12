#pragma once
#include "Headers.h"
#include "ObjectManager.h"
#include "Menu.h"

enum MouseButtons
{
	LeftMouse = 1, MiddleMouse, RightMouse
};

class MouseController
{
private:
	vector<int> pressed, released;
	unique_ptr<Mouse> mouse;
	Vector2 delta, smoothedDelta;
	bool locked;
	float tempMasterVolume, tempEffectsVolume, tempMusicVolume, tempUiVolume;

	void AddPressed(int key);
	void AddReleased(int key);
	void RemovePressed(int key);
	void RemoveReleased(int key);
	void OnMousePress(int key, DX::DeviceResources * deviceResources);
	void OnMouseRelease(int key);
	bool IsMousePressed(int key);
	bool IsMouseReleased(int key);
public:
	static MouseController * Current;

	MouseController();
	void Initialize(HWND windowHandle);
	void ProcessMouseState(float elapsedTime, float deltaTime, DX::DeviceResources * deviceResources);
	void SetLock(bool value);
	Vector2 GetDelta();
	int GetX();
	int GetY();
};