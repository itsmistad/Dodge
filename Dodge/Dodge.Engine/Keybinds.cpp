#include "Headers.h"
#include "Keybinds.h"

/*
 * Codes in this file are from Microsoft's "Virtual-Key Codes" documentation.
 * For helping determining any key code, refer to https://docs.microsoft.com/en-us/windows/desktop/inputdev/virtual-key-codes
 */

Keybinds::Keybinds()
{
	forward = 0x57; // W
	backward = 0x53; // S
	left = 0x41; // A 
	right = 0x44; // D
	jump = VK_SPACE;
	sprint = VK_LSHIFT;
	use = 0x46; // F
	escape = VK_ESCAPE;
	enter = VK_RETURN;
}