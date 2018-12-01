#pragma once
#include "Headers.h"
#include "Menu.h"

class KeyboardController
{
private:
	vector<int> pressed, released;
	unique_ptr<Keyboard> keyboard;

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
	void Initialize();
	void ProcessKeyStates();
};