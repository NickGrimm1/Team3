/**
<summary> PS3 gamepad input handler, static
Author: Matthew Johnson
Version: 0.0.2 </summary>
**/
// TODO: Make sure that PC version doesn't try to compile this
#pragma once 
#include "../Framework/T3Vector2.h"
#if PS3_BUILD
#include <cell/pad.h>
#endif
#if WINDOWS_BUILD
#include <Xinput.h>
#pragma comment(lib, "XInput.lib")
#endif

#define DEADZONE 0.05f
#define PRESSLIMIT 200.0f

typedef void (*PadInput)();

// Forward Declaration of InputListener//
class InputListener;

namespace GamepadEvents
{
	enum PlayerIndex
	{
		PLAYERINDEX_ONE,
		PLAYERINDEX_TWO,
		PLAYERINDEX_THREE,
		PLAYERINDEX_FOUR,
		PLAYERINDEX_MAX,
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
#if PS3_BUILD
		INPUT_SELECT			= 0,
		INPUT_L3_STICK_PRESS	= 1,
		INPUT_R3_STICK_PRESS	= 2,
		INPUT_START				= 3,
		INPUT_DPAD_UP			= 4, 
		INPUT_DPAD_RIGHT		= 5,
		INPUT_DPAD_DOWN			= 6, 
		INPUT_DPAD_LEFT			= 7,
		INPUT_L2_TRIGGER		= 8,
		INPUT_R2_TRIGGER		= 9,
		INPUT_L1_SHOULDER		= 10,
		INPUT_R1_SHOULDER		= 11,
		INPUT_TRIANGLE_Y		= 12, 
		INPUT_CIRCLE_B			= 13,
		INPUT_CROSS_A			= 14, 
		INPUT_SQUARE_X			= 15,
#endif
#if WINDOWS_BUILD
		INPUT_DPAD_UP			= 0,
		INPUT_DPAD_DOWN			= 1,
		INPUT_DPAD_LEFT			= 2,
		INPUT_DPAD_RIGHT		= 3,
		INPUT_START				= 4,
		INPUT_SELECT			= 5,
		INPUT_L3_STICK_PRESS	= 6,
		INPUT_R3_STICK_PRESS	= 7,
		INPUT_L1_SHOULDER		= 8,
		INPUT_R1_SHOULDER		= 9,
		INPUT_L2_TRIGGER		= 10,
		INPUT_R2_TRIGGER		= 11,
		INPUT_CROSS_A			= 12,
		INPUT_CIRCLE_B			= 13,
		INPUT_SQUARE_X			= 14,
		INPUT_TRIANGLE_Y		= 15,
#endif
		RIGHT_STICK_UP			= 16,
		RIGHT_STICK_RIGHT		= 17,
		RIGHT_STICK_DOWN		= 18,
		RIGHT_STICK_LEFT		= 19,
		LEFT_STICK_UP			= 20,
		LEFT_STICK_RIGHT		= 21,
		LEFT_STICK_DOWN			= 22,
		LEFT_STICK_LEFT			= 23,
		PADBUTTONS_MAX			= 24,
	};
}
class GamePad
{
public:
	GamePad(GamepadEvents::PlayerIndex playerID);
	void Update(float msec);
	GamepadEvents::PlayerIndex GetPlayerID() const { return playerID; }
private:
	GamepadEvents::PlayerIndex playerID;
#if PS3_BUILD
	CellPadData previousGamePadState;
#endif
#if WINDOWS_BUILD
	XINPUT_STATE previousGamePadState;
#endif
	float buttonHoldTimes[GamepadEvents::PADBUTTONS_MAX];
	T3Vector2 previousRightStick;
	T3Vector2 previousLeftStick;
	float previousRightTrigger;
	float previousLeftTrigger;
};


