#include "Headers.h"
#include "MouseController.h"
#include "SoundManager.h"

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

void MouseController::ProcessMouseState()
{
	auto state = mouse->GetState();
	if (state.leftButton)
		OnMousePress(LeftMouse);
	else
		OnMouseRelease(LeftMouse);
	if (state.middleButton)
		OnMousePress(MiddleMouse);
	else
		OnMouseRelease(MiddleMouse);
	if (state.rightButton)
		OnMousePress(RightMouse);
	else
		OnMouseRelease(RightMouse);

	if (state.positionMode == Mouse::MODE_RELATIVE)
		delta = Vector3((float)state.x, (float)state.y, 0) * 0.01f;
	else
		delta = Vector3();

	// TODO: Handle any "IsMousePressed" or "IsMouseReleased" events here. (LeftMouse, MiddleMouse, RightMouse)
}

void MouseController::OnMousePress(int key)
{
	if (IsMouseReleased(key) && !IsMousePressed(key))
	{
		RemoveReleased(key);
		AddPressed(key);

		if (key == LeftMouse)
		{
			if (Menu::currentMenu != NULL)
				Menu::currentMenu->OnMousePress(Vector2(GetX(), GetY()));
			else { /* We are currently in game and out of a menu. */ }
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
				Menu::currentMenu->OnMouseRelease(Vector2(GetX(), GetY()));
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
	mouse->SetMode(value ? Mouse::MODE_ABSOLUTE : Mouse::MODE_RELATIVE);
	locked = value;
}

Vector3 MouseController::GetDelta()
{
	return delta;
}

int MouseController::GetX()
{
	return mouse->GetState().x;
}

int MouseController::GetY()
{
	return mouse->GetState().y;
}