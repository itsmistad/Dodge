#pragma once
#include "Headers.h"

class Keybind
{
private:
	wstring name;
public:
	int key;

	Keybind();
	Keybind(int key, wstring name);
	wstring GetName();
	void SetKey(int key);
};

class Keybinds
{
public:
	Keybind * forward, * backward, * left, * right, * jump, * sprint, * use, * escape, * enter;

	Keybinds();
};