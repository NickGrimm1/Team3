/**
<summary> PS3 gamepad input handler, static
Author: Matthew Johnson
Version: 0.0.2 </summary>
**/
// TODO: Make sure that PC version doesn't try to compile this
#pragma once 
#include <stdio.h>
#include <math.h>
#include <string>
#include <iostream>
//#include <cell/pad.h> //ps3 sdk include 

#define SCREENX 720
#define SCREENY 480
#define DEADZONE 5.0f
#define SENSITIVITY 100.0f

typedef void (*PadInput)();

// Forward Declaration of InputListener
class InputListener;

namespace GamepadEvents
{
	enum PlayerIndex
	{
		ONE,
		TWO,
		THREE,
		FOUR,
	};
	enum EventType
	{
		BUTTON_UP,
		BUTTON_DOWN,
		BUTTON_PRESS,
		BUTTON_HELD,
	};
	// This allows us to call an alternative method for input handling when we want to know how much an analogue control has moved.
	enum AnalogueControl
	{
		LEFT_STICK,
		RIGHT_STICK,
		LEFT_TRIGGER,
		RIGHT_TRIGGER,
	};
	//All buttons are listed in the enum below so that 
	// the CellPadData struct from the ps3 API can be used
	// buttons listed in the same order as in that struct
	//making access easier.
	enum Button
	{
		INPUT_SELECT	= 0,
		INPUT_L3		= 1,
		INPUT_R3		= 2,
		INPUT_START		= 3,
		INPUT_UP		= 4, INPUT_RIGHT	= 5,
		INPUT_DOWN		= 6, INPUT_LEFT		= 7,
		INPUT_L2		= 8,
		INPUT_R2		= 9,
		INPUT_L1		= 10,
		INPUT_R1		= 11,
		INPUT_TRIANGLE	= 12, INPUT_CIRCLE	= 13,
		INPUT_CROSS		= 14, INPUT_SQUARE	= 15,
		PADBUTTONS_MAX
	};
}
class GamePad
{
public:
	static void Initialise();
	static void Destroy();
	static void UpdateJoypad();

	static void SetPadFunction(GamepadEvents::Button button, PadInput pad);
	static void GetPointerPosition(float &x, float &y);
	static void GetJoypadMovement(float &x, float &y);

	void SetInputListener(InputListener* l) { listener = l; }

protected:
	static float pointerPosX;
	static float pointerPosY;

	static float pointerRelativeX;
	static float pointerRelativeY;

	static PadInput padFunctions[GamepadEvents::PADBUTTONS_MAX];

	InputListener* listener;
};


