#pragma once
#include "KeyboardController.h"
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
	Vector3 delta;
	bool locked;

	void AddPressed(int key);
	void AddReleased(int key);
	void RemovePressed(int key);
	void RemoveReleased(int key);
	void OnMousePress(int key);
	void OnMouseRelease(int key);
	bool IsMousePressed(int key);
	bool IsMouseReleased(int key);
public:
	MouseController();
	void Initialize(HWND windowHandle);
	void ProcessMouseState();
	void SetLock(bool value);
	Vector3 GetDelta();
	int GetX();
	int GetY();
};