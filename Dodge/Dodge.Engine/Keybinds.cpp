#include "Headers.h"
#include "Keybinds.h"

/*
 * Codes in this file are from Microsoft's "Virtual-Key Codes" documentation.
 * For helping determining any key code, refer to https://docs.microsoft.com/en-us/windows/desktop/inputdev/virtual-key-codes
 */

Keybinds::Keybinds()
{
	forward = new Keybind(0x57, L"W");
	backward = new Keybind(0x53, L"S");
	left = new Keybind(0x41, L"A");
	right = new Keybind(0x44, L"D");
	jump = new Keybind(VK_SPACE, L"Space");
	sprint = new Keybind(VK_LSHIFT, L"Left Shift");
	use = new Keybind(0x46, L"F");
	escape = new Keybind(VK_ESCAPE, L"Escape");
	enter = new Keybind(VK_RETURN, L"Enter");
}

Keybind::Keybind() { }

Keybind::Keybind(int key, wstring name)
{
	this->key = key;
	this->name = name;
}

wstring Keybind::GetName()
{
	return name;
}

void Keybind::SetKey(int _key)
{
	key = _key;
	switch (key)
	{
	case VK_BACK:
		name = L"Backspace";
		break;
	case VK_TAB:
		name = L"Tab";
		break;
	case VK_CLEAR:
		name = L"Clear";
		break;
	case VK_RETURN:
		name = L"Enter";
		break;
	case VK_LSHIFT:
		name = L"Left Shift";
		break;
	case VK_RSHIFT:
		name = L"Right Shift";
		break;
	case VK_SHIFT:
		name = L"Shift";
		break;
	case VK_LCONTROL:
		name = L"Left Ctrl";
		break;
	case VK_RCONTROL:
		name = L"Right Ctrl";
		break;
	case VK_CONTROL:
		name = L"Ctrl";
		break;
	case VK_LMENU:
		name = L"Left Alt";
		break;
	case VK_RMENU:
		name = L"Right Alt";
		break;
	case VK_MENU:
		name = L"Alt";
		break;
	case VK_PAUSE:
		name = L"Pause";
		break;
	case VK_CAPITAL:
		name = L"Caps Lock";
		break;
	case VK_ESCAPE:
		name = L"Escape";
		break;
	case VK_SPACE:
		name = L"Space";
		break;
	case VK_PRIOR:
		name = L"Page Up";
		break;
	case VK_NEXT:
		name = L"Page Down";
		break;
	case VK_END:
		name = L"End";
		break;
	case VK_HOME:
		name = L"Home";
		break;
	case VK_LEFT:
		name = L"Left";
		break;
	case VK_RIGHT:
		name = L"Right";
		break;
	case VK_UP:
		name = L"Up";
		break;
	case VK_DOWN:
		name = L"Down";
		break;
	case VK_INSERT:
		name = L"Insert";
		break;
	case VK_DELETE:
		name = L"Delete";
		break;
	case VK_NUMPAD0:
		name = L"Numpad 0";
		break;
	case VK_NUMPAD1:
		name = L"Numpad 1";
		break;
	case VK_NUMPAD2:
		name = L"Numpad 2";
		break;
	case VK_NUMPAD3:
		name = L"Numpad 3";
		break;
	case VK_NUMPAD4:
		name = L"Numpad 4";
		break;
	case VK_NUMPAD5:
		name = L"Numpad 5";
		break;
	case VK_NUMPAD6:
		name = L"Numpad 6";
		break;
	case VK_NUMPAD7:
		name = L"Numpad 7";
		break;
	case VK_NUMPAD8:
		name = L"Numpad 8";
		break;
	case VK_NUMPAD9:
		name = L"Numpad 9";
		break;
	case VK_MULTIPLY:
		name = L"Numpad *";
		break;
	case VK_ADD:
		name = L"Numpad +";
		break;
	case VK_SUBTRACT:
		name = L"Numpad -";
		break;
	case VK_DECIMAL:
		name = L"Numpad .";
		break;
	case VK_DIVIDE:
		name = L"Numpad /";
		break;
	case VK_F1:
		name = L"F1";
		break;
	case VK_F2:
		name = L"F2";
		break;
	case VK_F3:
		name = L"F3";
		break;
	case VK_F4:
		name = L"F4";
		break;
	case VK_F5:
		name = L"F5";
		break;
	case VK_F6:
		name = L"F6";
		break;
	case VK_F7:
		name = L"F7";
		break;
	case VK_F8:
		name = L"F8";
		break;
	case VK_F9:
		name = L"F9";
		break;
	case VK_F10:
		name = L"F10";
		break;
	case VK_F11:
		name = L"F11";
		break;
	case VK_F12:
		name = L"F12";
		break;
	default:
		name = wchar_t(key);
		break;
	}
}