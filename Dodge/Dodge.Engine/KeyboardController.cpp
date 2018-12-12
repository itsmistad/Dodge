#include "Headers.h"
#include "KeyboardController.h"
#include "Camera.h"
#include "SoundManager.h"
#include "ObjectManager.h"
#include "Menu.h"

KeyboardController::KeyboardController() { }

void KeyboardController::Initialize(MouseController * _mouseController)
{
	keyboard = make_unique<Keyboard>();
	for (int i = 0x08; i <= 0xFE; i++)
		AddReleased(i);
	modelScale = 0;
	mouseController = _mouseController;
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

void KeyboardController::ProcessKeyStates(float deltaTime)
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
	auto keybinds = Configs::GetKeybinds();
	//auto sndmgr = SoundManager::GetCurrent();
	if (Menu::currentMenu == NULL)
	{
		auto camera = Camera::Main;
		auto you = ObjectManager::You;
		camera->movementVelocity = Vector3();
		if (IsKeyPressed(keybinds.forward->key))
		{
			if (IsKeyPressed(keybinds.sprint->key))
				camera->movementVelocity += camera->GetForward() * Configs::GetGameplay().sprintSpeed;
			else
				camera->movementVelocity += camera->GetForward() * Configs::GetGameplay().walkSpeed * (camera->canFreeCam ? .2f : 1.f);
		}
		if (IsKeyPressed(keybinds.backward->key))
		{
			camera->movementVelocity += camera->GetForward() * -Configs::GetGameplay().walkSpeed * (camera->canFreeCam ? .2f : 1.f);
		}
		if (IsKeyPressed(keybinds.left->key))
		{
			camera->movementVelocity += camera->GetRight() * -Configs::GetGameplay().walkSpeed * (camera->canFreeCam ? .2f : .6f);
		}
		if (IsKeyPressed(keybinds.right->key))
		{
			camera->movementVelocity += camera->GetRight() * Configs::GetGameplay().walkSpeed * (camera->canFreeCam ? .2f : .6f);
		}
		if (IsKeyPressed(keybinds.jump->key))
		{
			if (camera->canFreeCam)
				camera->movementVelocity += camera->GetUp() * Configs::GetGameplay().jumpSpeed * 0.1f;
		}
		if (IsKeyPressed(VK_LCONTROL))
		{
			if (camera->canFreeCam)
				camera->movementVelocity -= camera->GetUp() * Configs::GetGameplay().jumpSpeed * 0.1f;
		}
		if (camera->movementVelocity != Vector3::Zero)
			you->isWalking = true;

		deltaTime;
	}
}

void KeyboardController::OnKeyPress(int key)
{
	if (IsKeyReleased(key) && !IsKeyPressed(key))
	{
		RemoveReleased(key);
		AddPressed(key);

		auto keybinds = Configs::GetKeybinds();
		auto sndmgr = SoundManager::GetCurrent();
		auto camera = Camera::Main;
		auto you = ObjectManager::You;

		if (Menu::currentMenu == NULL)
		{
			if (key == keybinds.use->key)
			{
				sndmgr->Play("powerup");
				//TODO: Activate power up
			}
			if (key == keybinds.jump->key && !camera->canFreeCam && you->isGrounded && !you->isJumping)
			{
				sndmgr->Play("jump");
				you->isGrounded = false;
				you->isJumping = true;
				camera->jumpVelocity = Vector3(0, Configs::GetGameplay().jumpSpeed, 0);
			}
			if (key == keybinds.escape->key)
			{
				Menu::all["ingame"]->Show();
				mouseController->SetLock(false);
			}
			if (key == VK_F1)
			{
				camera->canFreeCam = !camera->canFreeCam;
				if (camera->canFreeCam)
				{
					camera->oldPitch = camera->pitch;
					camera->oldYaw = camera->yaw;
					camera->oldPosition = camera->position;
					camera->gravitationalVelocity = Vector3();
					camera->jumpVelocity = Vector3();
				}
				else
				{
					camera->pitch = camera->oldPitch;
					camera->yaw = camera->oldYaw;
					camera->position = camera->oldPosition;
				}
			}
			if (key == keybinds.sprint->key && IsKeyPressed(keybinds.forward->key) ||
				key == keybinds.forward->key && IsKeyPressed(keybinds.sprint->key))
			{
				camera->SetGoalFOV(camera->GetDefaultFOV() + 10.f);
				you->isSprinting = true;
			}
		}
		else
		{
			if (Menu::currentMenu->currentControlButton != NULL)
				Menu::currentMenu->EndKeybindChange(key);
			if (key == keybinds.escape->key && Menu::currentMenu->name == "ingame")
			{
				Menu::all["ingame"]->Hide();
				mouseController->SetLock(true);
			}
		}
	}
}

void KeyboardController::OnKeyRelease(int key)
{
	if (IsKeyPressed(key) && !IsKeyReleased(key))
	{
		RemovePressed(key);
		AddReleased(key);

		auto keybinds = Configs::GetKeybinds();
		auto you = ObjectManager::You;

		if (Menu::currentMenu == NULL)
		{
			if (key == keybinds.sprint->key ||
				key == keybinds.forward->key && IsKeyPressed(keybinds.sprint->key))
			{
				Camera::Main->SetGoalFOV(Camera::Main->GetDefaultFOV());
				you->isSprinting = false;
			}
		}
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