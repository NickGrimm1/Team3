#include "Keyboard.h"
#include "../Team3Project1/InputListener.h"

Keyboard::Keyboard(HWND &hwnd)	{
	//Initialise the arrays to false!
	ZeroMemory(keyStates,  KeyboardEvents::KEYBOARD_MAX * sizeof(bool));
	ZeroMemory(holdStates, KeyboardEvents::KEYBOARD_MAX * sizeof(bool));

	//Tedious windows RAW input stuff
	rid.usUsagePage		= HID_USAGE_PAGE_GENERIC;		//The keyboard isn't anything fancy
    rid.usUsage			= HID_USAGE_GENERIC_KEYBOARD;	//but it's definitely a keyboard!
    rid.dwFlags			= RIDEV_INPUTSINK;				//Yes, we want to always receive RAW input...
    rid.hwndTarget		= hwnd;							//Windows OS window handle
    RegisterRawInputDevices(&rid, 1, sizeof(rid));		//We just want one keyboard, please!
}

/*
Updates variables controlling whether a keyboard key has been
held for multiple frames.
*/
void Keyboard::UpdateHolds()	{
	memcpy(holdStates,keyStates, KeyboardEvents::KEYBOARD_MAX * sizeof(bool));
}

/*
Sends the keyboard to sleep, so it doesn't process any
keypresses until it receives a Wake()
*/
void Keyboard::Sleep()	{
	isAwake = false;	//Night night!
	//Prevents incorrectly thinking keys have been held / pressed when waking back up
	ZeroMemory(keyStates,  KeyboardEvents::KEYBOARD_MAX * sizeof(bool));
	ZeroMemory(holdStates, KeyboardEvents::KEYBOARD_MAX * sizeof(bool));
}

/*
Updates the keyboard state with data received from the OS.
*/
void Keyboard::Update(RAWINPUT* raw)	{
	if(isAwake)	{
		DWORD key = (DWORD)raw->data.keyboard.VKey;

		//We should do bounds checking!
		if(key < KeyboardEvents::KEYBOARD_BACK || key > KeyboardEvents::KEYBOARD_MAX)	{
			return;
		}

		//First bit of the flags tag determines whether the key is down or up
		keyStates[key] = !(raw->data.keyboard.Flags & RI_KEY_BREAK);

		if (keyStates[key] && !holdStates[key])
			listener->KeyboardEvent(KeyboardEvents::KEY_DOWN, (KeyboardEvents::Key)key);
		else if (keyStates[key] && holdStates[key])
			listener->KeyboardEvent(KeyboardEvents::KEY_HELD, (KeyboardEvents::Key)key);
		else if (!keyStates[key] && holdStates[key])
			listener->KeyboardEvent(KeyboardEvents::KEY_UP, (KeyboardEvents::Key)key);
	}
}