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

	for(int i = 0; i < NUM_FUNCTIONS; ++i) //num functions may be supposed to be PADBUTTONS_MAX
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
}


#endif