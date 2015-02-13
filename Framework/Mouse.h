/******************************************************************************
Class:Mouse
Implements:InputDevice
Author:Rich Davison
Description:Windows RAW input mouse, with a couple of game-related enhancements

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "InputDevice.h"
#include "Vector2.h"

class InputListener;

namespace MouseEvents
{
	enum EventType
	{
		/**
		<summary>Button was down last frame, it is still down.</summary>
		*/
		BUTTON_DOWN,
		/**
		<summary>Button was up last frame, now it is down.</summary>
		*/
		BUTTON_UP,
		/**
		<summary>Button was clicked (Down then up in quick sucession).</summary>
		*/
		BUTTON_CLICK,
		/**
		<summary>Button was double-clicked.</summary>
		*/
		BUTTON_DOUBLECLICK,
		BUTTON_HELD,
	};
	//Presumably RAW input does actually support those fancy mice with greater
	//than 5 buttons in some capacity, but I have a 5 button mouse so I don't
	//care to find out how ;)
	enum MouseButtons{
		MOUSE_LEFT		= 0,
		MOUSE_RIGHT		= 1,
		MOUSE_MIDDLE	= 2,
		MOUSE_FOUR		= 3,
		MOUSE_FIVE		= 4,
		MOUSE_MAX		= 5
	};
}
class Mouse : public InputDevice	{
public:
	friend class Window;

	//Get the window position of the mouse pointer
	Vector2 GetAbsolutePosition();

	/**
	<summary>Sets the amount of time between two click events that is short enough to count as a click.</summary>
	*/
	void SetDoubleClickLimit(float msec);
	/**
	<summary>Sets the amount of time between a mouse-down and mouse-up event that is short enough to count as a click.</summary>
	*/
	void SetClickLimit(float msec);
	
	//Has the mouse wheel moved since the last update?
	bool WheelMoved();
	//Get the mousewheel movement. Positive means scroll up,
	//negative means scroll down, 0 means no movement.
	int GetWheelMovement();

	//Sets the mouse sensitivity. Currently only affects the 'relative'
	//(i.e FPS-style) mouse movement. Students! Maybe you'd like to
	//implement a 'MenuSensitivity' for absolute movement?
	void SetMouseSensitivity(float amount);

protected:
	Mouse(HWND &hwnd);
	~Mouse(void){}

	//Internal function that updates the mouse variables from a 
	//raw input 'packet'
	virtual void Update(RAWINPUT* raw, float msec);
	//Sends the mouse to sleep (i.e window has been alt-tabbed away etc)
	virtual void Sleep();

	//Set the mouse's current screen position. Maybe should be public?
	void SetAbsolutePosition(unsigned int x,unsigned int y);
	//Set the absolute screen bounds (<0 is always assumed dissallowed). Used
	//by the window resize routine...
	void SetAbsolutePositionBounds(unsigned int maxX, unsigned int maxY);

	//Current mouse position
	Vector2	position;
	//Current mouse position maximum bounds
	Vector2	bounds;
	// The position of the mouse last frame.
	Vector2	lastPosition;
	//Current button down state for each button
	bool buttons[MouseEvents::MOUSE_MAX];
	//Current doubleClick counter for each button
	bool doubleClicks[MouseEvents::MOUSE_MAX];
	//Counter to remember when last mouse click occured
	float lastMouseDown[MouseEvents::MOUSE_MAX];
	//Counter to remember when last mouse click occured
	float lastClickTime[MouseEvents::MOUSE_MAX];

	//last mousewheel updated position
	int	lastWheel;

	//Current mousewheel updated position
	int	frameWheel;

	//Max amount of ms between clicks count as a 'double click'
	float clickLimit;
	float doubleClickLimit;

	//Mouse pointer sensitivity. Set this negative to get a headache!
	float sensitivity;

	InputListener* listener;
};

