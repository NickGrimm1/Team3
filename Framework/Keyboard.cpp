#include "Keyboard.h"
#include "../Team3Project1/GameStateManager.h"

Keyboard::Keyboard(HWND &hwnd)	{
	//Initialise the arrays to false!
	ZeroMemory(keyStates,  KeyboardEvents::KEYBOARD_MAX * sizeof(bool));
	ZeroMemory(prevStates, KeyboardEvents::KEYBOARD_MAX * sizeof(bool));
	ZeroMemory(lastKeyDown, KeyboardEvents::KEYBOARD_MAX * sizeof(float));

	//Tedious windows RAW input stuff
	rid.usUsagePage		= HID_USAGE_PAGE_GENERIC;		//The keyboard isn't anything fancy
    rid.usUsage			= HID_USAGE_GENERIC_KEYBOARD;	//but it's definitely a keyboard!
    rid.dwFlags			= RIDEV_INPUTSINK;				//Yes, we want to always receive RAW input...
    rid.hwndTarget		= hwnd;							//Windows OS window handle
    RegisterRawInputDevices(&rid, 1, sizeof(rid));		//We just want one keyboard, please!

	pressLimit = 200.0f;
}

/*
Sends the keyboard to sleep, so it doesn't process any
keypresses until it receives a Wake()
*/
void Keyboard::Sleep()	{
	isAwake = false;	//Night night!
	//Prevents incorrectly thinking keys have been held / pressed when waking back up
	ZeroMemory(keyStates,  KeyboardEvents::KEYBOARD_MAX * sizeof(bool));
	ZeroMemory(prevStates, KeyboardEvents::KEYBOARD_MAX * sizeof(bool));
	ZeroMemory(lastKeyDown, KeyboardEvents::KEYBOARD_MAX * sizeof(float));
}

/*
Updates the keyboard state with data received from the OS.
*/
void Keyboard::Update(RAWINPUT* raw, float msec)	{
	if(isAwake)	{
		DWORD key = (DWORD)raw->data.keyboard.VKey;

		//We should do bounds checking!
		if(key < KeyboardEvents::KEYBOARD_BACK || key > KeyboardEvents::KEYBOARD_MAX)	{
			return;
		}

		//First bit of the flags tag determines whether the key is down or up
		keyStates[key] = !(raw->data.keyboard.Flags & RI_KEY_BREAK);
		for (int i = 0; i < KeyboardEvents::KEYBOARD_MAX; i++)
			lastKeyDown[i] += msec;

		if (keyStates[key])
		{
			if (!prevStates[key])
			{
				GameStateManager::Instance()->KeyboardEvent(KeyboardEvents::KEY_DOWN, (KeyboardEvents::Key)key);
				lastKeyDown[key] = 0;
			}
			else
			{
				GameStateManager::Instance()->KeyboardEvent(KeyboardEvents::KEY_HELD, (KeyboardEvents::Key)key);
			}
		}
		else
		{
			if (prevStates[key])
			{
				GameStateManager::Instance()->KeyboardEvent(KeyboardEvents::KEY_UP, (KeyboardEvents::Key)key);

				if (lastKeyDown[key] < pressLimit)
				{
					GameStateManager::Instance()->KeyboardEvent(KeyboardEvents::KEY_PRESS, (KeyboardEvents::Key)key);
				}
			}
		}

		memcpy(prevStates, keyStates, KeyboardEvents::KEYBOARD_MAX * sizeof(bool));
	}
}