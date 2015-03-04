#if WINDOWS_BUILD
#include "Mouse.h"
#include "../Team3Project1/GameStateManager.h"

Mouse::Mouse(HWND &hwnd)	{
	ZeroMemory(buttons,	sizeof(bool) * MouseEvents::MOUSE_MAX);
	ZeroMemory(doubleClicks, sizeof(bool)  * MouseEvents::MOUSE_MAX);
	ZeroMemory(lastClickTime, sizeof(float) * MouseEvents::MOUSE_MAX);
	ZeroMemory(lastMouseDown, sizeof(float) * MouseEvents::MOUSE_MAX);

	lastWheel = 0;
	frameWheel = 0;
	sensitivity = 0.07f;	//Chosen for no other reason than it's a nice value for my Deathadder ;)
	clickLimit = 200.0f;
	doubleClickLimit = 400.f;

	rid.usUsagePage = HID_USAGE_PAGE_GENERIC; 
    rid.usUsage		= HID_USAGE_GENERIC_MOUSE; 
    rid.dwFlags		= RIDEV_INPUTSINK;   
    rid.hwndTarget	= hwnd;
    RegisterRawInputDevices(&rid, 1, sizeof(rid));
}

void Mouse::Update(RAWINPUT* raw, float msec)	
{
	if(isAwake)	{
		// Get the current position
		POINT cursorPos;
		GetCursorPos(&cursorPos);
		ScreenToClient(Window::GetWindow().GetHandle(), &cursorPos);
		position = T3Vector2(cursorPos.x, cursorPos.y);

		// Clamp the current position to bounds
		position.x = maximum(position.x, 0.0f);
		position.x = minimum(position.x, bounds.x);

		position.y = maximum(position.y, 0.0f);
		position.y = minimum(position.y, bounds.y);

		T3Vector2 mouseMove = position - lastPosition;
		if (mouseMove != 0)
		{
			// Convert to resolution-independent
			GameStateManager::Instance()->MouseMoved(T3Vector2(lastPosition.x / Window::GetWindow().GetScreenSize().x, lastPosition.y / Window::GetWindow().GetScreenSize().y), T3Vector2(position.x / Window::GetWindow().GetScreenSize().x, position.y / Window::GetWindow().GetScreenSize().y));
		}
		
		// If the mouse cursor isn't visible, lock to the center of the screen...
		if (!isCursorVisible)
			position.x = Window::GetWindow().GetScreenSize().x / 2;
			position.y = Window::GetWindow().GetScreenSize().y / 2;

		/*
		TODO: How framerate independent is this?
		*/
		if(raw->data.mouse.usButtonFlags & RI_MOUSE_WHEEL)	{		
			if(raw->data.mouse.usButtonData == 120) {
				frameWheel = 1;
				GameStateManager::Instance()->MouseScrolled(position, frameWheel);
			}
			else {
				frameWheel = -1;
				GameStateManager::Instance()->MouseScrolled(position, frameWheel);
			}
		}

		/*
		Oh, Microsoft...
		*/
		static int buttondowns [5] = {	RI_MOUSE_BUTTON_1_DOWN,
										RI_MOUSE_BUTTON_2_DOWN,
										RI_MOUSE_BUTTON_3_DOWN,
										RI_MOUSE_BUTTON_4_DOWN,
										RI_MOUSE_BUTTON_5_DOWN};

		static int buttonps [5] = {		RI_MOUSE_BUTTON_1_UP,
										RI_MOUSE_BUTTON_2_UP,
										RI_MOUSE_BUTTON_3_UP,
										RI_MOUSE_BUTTON_4_UP,
										RI_MOUSE_BUTTON_5_UP};
		
		T3Vector2 resIndependentPosition;
		resIndependentPosition.x = position.x / Window::GetWindow().GetScreenSize().x;
		resIndependentPosition.y = position.y / Window::GetWindow().GetScreenSize().y;

		for(int i = 0; i < MouseEvents::MOUSE_MAX; ++i) 
		{
			// Add the time on for clicks & double clicks.
			lastMouseDown[i] += msec;
			lastClickTime[i] += msec;

			if(raw->data.mouse.usButtonFlags & buttondowns[i])	
			{
				if (!buttons[i])
				{
					// Mouse Down
					GameStateManager::Instance()->MouseEvent(MouseEvents::BUTTON_DOWN, (MouseEvents::MouseButtons)i, resIndependentPosition);
					buttons[i] = true;
					lastMouseDown[i] = 0;
				}
				else
				{
					// Mouse Held
					GameStateManager::Instance()->MouseEvent(MouseEvents::BUTTON_HELD, (MouseEvents::MouseButtons)i, resIndependentPosition);
				}
			}
			else if(raw->data.mouse.usButtonFlags & buttonps[i])	
			{
				//The button has been released!
				buttons[i]		= false;
				GameStateManager::Instance()->MouseEvent(MouseEvents::BUTTON_UP, (MouseEvents::MouseButtons)i, resIndependentPosition);

				// Check if down then up is within time to count as a click
				if (lastMouseDown[i] < clickLimit)
				{
					GameStateManager::Instance()->MouseEvent(MouseEvents::BUTTON_CLICK, (MouseEvents::MouseButtons)i, resIndependentPosition);

					// Check if this is a double click
					if (lastClickTime[i] < doubleClickLimit)
					{
						GameStateManager::Instance()->MouseEvent(MouseEvents::BUTTON_DOUBLECLICK, (MouseEvents::MouseButtons)i, resIndependentPosition);
						lastClickTime[i] = 0;
					}
				}
			}
		}
		// If the mouse cursor isn't visible, lock to the center of the screen...
		if (!isCursorVisible)
			lastPosition = Window::GetWindow().GetScreenSize() / 2;
		else
			lastPosition = position;
	}
}

/*
Sets the mouse sensitivity (higher = mouse pointer moves more!)
Lower bounds checked.
*/
void	Mouse::SetMouseSensitivity(float amount)	{
	if(amount == 0.0f) {
		amount = 1.0f;
	}
	sensitivity = amount;	
}

/*
Sends the mouse to sleep, so it doesn't process any
movement or buttons until it receives a Wake()
*/
void Mouse::Sleep()	{
	isAwake = false;	//Bye bye for now
	ZeroMemory(buttons,		MouseEvents::MOUSE_MAX * sizeof(bool) );
}

/*
Forces the mouse pointer to a specific point in absolute space.
*/
void	Mouse::SetAbsolutePosition(unsigned int x, unsigned int y)	{
	position.x = (float)x;
	position.y = (float)y;
}

/*
Returns the mouse pointer position in absolute space.
*/
T3Vector2 Mouse::GetAbsolutePosition()	{
	return position;
}

/*
Returns how much the mouse has moved by since the last frame.
*/
void Mouse::SetAbsolutePositionBounds(unsigned int maximumX, unsigned int maximumY)	{
	bounds.x = (float)maximumX;
	bounds.y = (float)maximumY;	
}

/*
Has the mousewheel been moved since the last frame?
*/
bool	Mouse::WheelMoved()	{
	return frameWheel != 0;
};

/*
Get the mousewheel movement. Positive values mean the mousewheel
has moved up, negative down. Can be 0 (no movement)
*/
int		Mouse::GetWheelMovement()	{
	return (int)frameWheel;
}
#endif