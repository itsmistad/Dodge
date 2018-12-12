#pragma once
#include "Headers.h"
#include "MouseController.h"

class KeyboardController
{
private:
	vector<int> pressed, released;
	unique_ptr<Keyboard> keyboard;
	MouseController * mouseController;

	void AddPressed(int key);
	void AddReleased(int key);
	void RemovePressed(int key);
	void RemoveReleased(int key);
	void OnKeyPress(int key);
	void OnKeyRelease(int key);
	bool IsKeyPressed(int key);
	bool IsKeyReleased(int key);
public:
	float modelScale;

	KeyboardController();
	void Initialize(MouseController * mouseController);
	void ProcessKeyStates(float deltaTime);
};