#include "Headers.h"
#include "KeyboardController.h"
#include "SoundManager.h"

KeyboardController::KeyboardController() { }

void KeyboardController::Initialize()
{
	keyboard = make_unique<Keyboard>();
	for (int i = 0x08; i <= 0xFE; i++)
		AddReleased(i);
	modelScale = 0;
}

void KeyboardController::AddPressed(int key)
{
	pressed.push_back(key);
}

void KeyboardController::AddReleased(int key)
{
	released.push_back(key);
}

void KeyboardController::RemovePressed(int key)
{
	pressed.erase(remove(pressed.begin(), pressed.end(), key), pressed.end());
}

void KeyboardController::RemoveReleased(int key)
{
	released.erase(remove(released.begin(), released.end(), key), released.end());
}

void KeyboardController::ProcessKeyStates()
{
	auto state = keyboard->GetState();
	for (int i = 0x08; i <= 0xFE; i++)
	{
		if (state.IsKeyDown((Keyboard::Keys)i))
			OnKeyPress(i);
		if (state.IsKeyUp((Keyboard::Keys)i))
			OnKeyRelease(i);
	}

	// TODO: Handle any "IsKeyPressed" or "IsKeyReleased" events here.
	auto keybind = Configs::GetKeybinds();
	auto sndmgr = SoundManager::GetCurrent();
	if (IsKeyPressed(keybind.forward))
	{
		//TODO: Go forward
		Log::Debug("going forward");
	}
	if (IsKeyPressed(keybind.backward))
	{
		//TODO: Go backward
	}
	if (IsKeyPressed(keybind.left))
	{
		//TODO: Go left
	}
	if (IsKeyPressed(keybind.right))
	{
		//TODO: Go right
	}
	if (IsKeyPressed(keybind.sprint))
	{
		//TODO: Sprint
	}
}

void KeyboardController::OnKeyPress(int key)
{
	if (IsKeyReleased(key) && !IsKeyPressed(key))
	{
		RemoveReleased(key);
		AddPressed(key);

		// TODO: Handle any "OnKeyPress" events here.
		auto keybinds = Configs::GetKeybinds();
		auto sndmgr = SoundManager::GetCurrent();
		if (key == keybinds.escape)
		{
			// Do something...
		}

		if (Menu::currentMenu == NULL)
		{
			if (key == keybinds.use)
			{
				sndmgr->Play("powerup");
				//TODO: Activate power up
			}
			if (key == keybinds.jump)
			{
				sndmgr->Play("jump");
				//TODO: Jump
			}
			if (key == keybinds.escape)
				Menu::all["ingame"]->Show();
		}
		else
		{
			if (key == keybinds.escape && Menu::currentMenu->name == "ingame")
				Menu::all["ingame"]->Hide();
		}
	}
}

void KeyboardController::OnKeyRelease(int key)
{
	if (IsKeyPressed(key) && !IsKeyReleased(key))
	{
		RemovePressed(key);
		AddReleased(key);

		// TODO: Handle any "OnKeyRelease" events here.
	}
}

bool KeyboardController::IsKeyPressed(int key)
{
	return find(pressed.begin(), pressed.end(), key) != pressed.end();
}

bool KeyboardController::IsKeyReleased(int key)
{
	return find(released.begin(), released.end(), key) != released.end();
}