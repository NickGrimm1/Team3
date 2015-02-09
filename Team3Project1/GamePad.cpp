#include "GamePad.h"
#if _PS3DEBUG
float GamePad::pointerPosX = 0.0f;
float GamePad::pointerPosY = 0.0f;

float GamePad::pointerRelativeX = 0.0f;
float GamePad::pointerRelativeY = 0.0f;

PadInput GamePad::padFunctions[GamepadEvents::PADBUTTONS_MAX];

void GamePad::Initialise()
{
	
	if(cellPadInit(1) != CELL_PAD_OK)
	{
		std::cout <<"cellPadInit failed!" << std::endl;
	}

	for(int i = 0; i < GamepadEvents::PADBUTTONS_MAX/*NUM_FUNCTIONS*/; ++i) //num functions may be supposed to be PADBUTTONS_MAX
	{
		padFunctions[i] = NULL;
	}
	
}

void GamePad::Destroy()
{
	cellPadEnd();
}

void GamePad::GetPointerPosition(float &x, float &y)
{
	x = pointerPosX;
	y = pointerPosY;
}

void GamePad::GetJoypadMovement(float &x, float &y)
{
	x = pointerRelativeX;
	y = pointerRelativeY;
}


void GamePad::SetPadFunction(GamepadEvents::Button button, PadInput function)
{
	padFunctions[button] = function;
}

void GamePad::UpdateJoypad()
{
	CellPadData data;
	cellPadGetData(0,&data);
	//2 ifs and for loops iterate through all buttons and call appropriate methods in response.
	if(data.len > 0) //pad status has changed
	{
		if(data.button[2]) //one of these buttons has changed
		{
			for(int i = 0; i < GamepadEvents::PADBUTTONS_MAX / 2; ++i)
			{
				if(padFunctions[i])
				{
					padFunctions[i](); //call the right function.
				}
			}
		}
	}
	if(data.button[3])
	{
		for(int i = 0; i < GamepadEvents::PADBUTTONS_MAX / 2; ++i)
		{
			if(data.button[3] & (1 << i))
			{
				if(padFunctions[GamepadEvents::PADBUTTONS_MAX / 2 + i])
				{
					padFunctions[GamepadEvents::PADBUTTONS_MAX / 2 + i]();
				}
			}
		}
	}

	//ensure centred analogue sticks don't register movement
	pointerRelativeX = data.button[6] * 2 - 255.0f;
	if(fabs(pointerRelativeX) < DEADZONE)
	{
		pointerRelativeX = 0;
	}
	pointerRelativeY = data.button[7] * 2 - 255.0f;
	if(fabs(pointerRelativeY) < DEADZONE)
	{
		pointerRelativeY = 0;
	}

	//use SENSITIVITY to fine tune pointer movement
	//then ensure pointer remains within bounds of screen
	//can read this in from renderer later
	pointerPosX += (pointerRelativeX / SENSITIVITY);
	pointerPosY += (pointerRelativeY / SENSITIVITY);

	if(pointerPosX < 0)
	{
		pointerPosX = 0;
	}
	else if(pointerPosX > SCREENX)
	{
		pointerPosX = SCREENX;
	}
	
	if(pointerPosY < 0)
	{
		pointerPosY = 0;
	}
	else if(pointerPosY > SCREENY)
	{
		pointerPosY = SCREENY;
	}

	cellPadClearBuf;
}


#endif