#include "GamePad.h"
#include "GameStateManager.h"

GamePad::GamePad(GamepadEvents::PlayerIndex playerID)
	: playerID(playerID)
{
	for (unsigned int i = 0; i < GamepadEvents::PADBUTTONS_MAX; ++i)
		buttonHoldTimes[i] = 0;
	T3Vector2 previousRightStick = 0.0f;
	T3Vector2 previousLeftStick = 0.0f;
	float previousRightTrigger = 0;
	float previousLeftTrigger = 0;
}

void GamePad::Update(CellPadData& gamePadState, float msec)
{
	// query gamePadState for analogue movement
#pragma region Analogue
#pragma region Sticks
#pragma region Right Stick
	T3Vector2 rightStick = T3Vector2(gamePadState.button[4] / 255.0f - 0.5f, gamePadState.button[5] / 255.0f - 0.5f);
	if (rightStick > T3Vector2(DEADZONE, DEADZONE))
	{
		GameStateManager::Instance()->GamepadAnalogueDisplacement(playerID, GamepadEvents::RIGHT_STICK, rightStick);
		
		if (rightStick.x > 0.5f)
		{
			if (!previousRightStick.x > 0.5f)
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_DOWN, GamepadEvents::RIGHT_STICK_RIGHT);
			}
			else
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_HELD, GamepadEvents::RIGHT_STICK_RIGHT);
			}
			buttonHoldTimes[GamepadEvents::RIGHT_STICK_RIGHT] += msec;
		}
		else
		{
			if (previousRightStick.x > 0.5f)
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_UP, GamepadEvents::RIGHT_STICK_RIGHT);
				
				if (buttonHoldTimes[GamepadEvents::RIGHT_STICK_RIGHT] < PRESSLIMIT)
				{
					GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_PRESS, GamepadEvents::RIGHT_STICK_RIGHT);
				}
				buttonHoldTimes[GamepadEvents::RIGHT_STICK_RIGHT] = 0;
			}
		}

		if (rightStick.x < -0.5f)
		{
			if (!previousRightStick.x < -0.5f)
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_DOWN, GamepadEvents::RIGHT_STICK_LEFT);
			}
			else
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_HELD, GamepadEvents::RIGHT_STICK_LEFT);
			}
			buttonHoldTimes[GamepadEvents::RIGHT_STICK_LEFT] += msec;
		}
		else
		{
			if (previousRightStick.x < -0.5f)
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_UP, GamepadEvents::RIGHT_STICK_LEFT);
				
				if (buttonHoldTimes[GamepadEvents::RIGHT_STICK_LEFT] < PRESSLIMIT)
				{
					GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_PRESS, GamepadEvents::RIGHT_STICK_LEFT);
				}
				buttonHoldTimes[GamepadEvents::RIGHT_STICK_LEFT] = 0;
			}
		}


		if (rightStick.y < -0.5f)
		{
			if (!previousRightStick.y < -0.5f)
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_DOWN, GamepadEvents::RIGHT_STICK_DOWN);
			}
			else
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_HELD, GamepadEvents::RIGHT_STICK_DOWN);
			}
			buttonHoldTimes[GamepadEvents::RIGHT_STICK_DOWN] += msec;
		}
		else
		{
			if (previousRightStick.y < -0.5f)
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_UP, GamepadEvents::RIGHT_STICK_DOWN);
				
				if (buttonHoldTimes[GamepadEvents::RIGHT_STICK_DOWN] < PRESSLIMIT)
				{
					GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_PRESS, GamepadEvents::RIGHT_STICK_DOWN);
				}
				buttonHoldTimes[GamepadEvents::RIGHT_STICK_DOWN] = 0;
			}
		}

		if (rightStick.y > 0.5f)
		{
			if (!previousRightStick.y > 0.5f)
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_DOWN, GamepadEvents::RIGHT_STICK_UP);
			}
			else
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_HELD, GamepadEvents::RIGHT_STICK_UP);
			}
			buttonHoldTimes[GamepadEvents::RIGHT_STICK_UP] += msec;
		}
		else
		{
			if (previousRightStick.y > 0.5f)
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_UP, GamepadEvents::RIGHT_STICK_UP);
				
				if (buttonHoldTimes[GamepadEvents::RIGHT_STICK_UP] < PRESSLIMIT)
				{
					GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_PRESS, GamepadEvents::RIGHT_STICK_UP);
				}
				buttonHoldTimes[GamepadEvents::RIGHT_STICK_UP] = 0;
			}
		}
	}
#pragma endregion
#pragma region Left Stick
	T3Vector2 leftStick = T3Vector2(gamePadState.button[6] / 255.0f - 0.5f, gamePadState.button[7] / 255.0f - 0.5f);
	if (leftStick > T3Vector2(DEADZONE, DEADZONE))
	{
		GameStateManager::Instance()->GamepadAnalogueDisplacement(playerID, GamepadEvents::LEFT_STICK, leftStick);
		
		if (leftStick.x > 0.5f)
		{
			if (!previousLeftStick.x > 0.5f)
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_DOWN, GamepadEvents::LEFT_STICK_RIGHT);
			}
			else
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_HELD, GamepadEvents::LEFT_STICK_RIGHT);
			}
			buttonHoldTimes[GamepadEvents::LEFT_STICK_RIGHT] += msec;
		}
		else
		{
			if (previousLeftStick.x > 0.5f)
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_UP, GamepadEvents::LEFT_STICK_RIGHT);
				
				if (buttonHoldTimes[GamepadEvents::LEFT_STICK_RIGHT] < PRESSLIMIT)
				{
					GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_PRESS, GamepadEvents::LEFT_STICK_RIGHT);
				}
				buttonHoldTimes[GamepadEvents::LEFT_STICK_RIGHT] = 0;
			}
		}

		if (leftStick.x < -0.5f)
		{
			if (!previousLeftStick.x < -0.5f)
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_DOWN, GamepadEvents::LEFT_STICK_LEFT);
			}
			else
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_HELD, GamepadEvents::LEFT_STICK_LEFT);
			}
			buttonHoldTimes[GamepadEvents::LEFT_STICK_LEFT] += msec;
		}
		else
		{
			if (previousLeftStick.x < -0.5f)
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_UP, GamepadEvents::LEFT_STICK_LEFT);
				
				if (buttonHoldTimes[GamepadEvents::LEFT_STICK_LEFT] < PRESSLIMIT)
				{
					GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_PRESS, GamepadEvents::LEFT_STICK_LEFT);
				}
				buttonHoldTimes[GamepadEvents::LEFT_STICK_LEFT] = 0;
			}
		}


		if (leftStick.y < -0.5f)
		{
			if (!previousLeftStick.y < -0.5f)
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_DOWN, GamepadEvents::LEFT_STICK_DOWN);
			}
			else
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_HELD, GamepadEvents::LEFT_STICK_DOWN);
			}
			buttonHoldTimes[GamepadEvents::LEFT_STICK_DOWN] += msec;
		}
		else
		{
			if (previousLeftStick.y < -0.5f)
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_UP, GamepadEvents::LEFT_STICK_DOWN);
				
				if (buttonHoldTimes[GamepadEvents::LEFT_STICK_DOWN] < PRESSLIMIT)
				{
					GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_PRESS, GamepadEvents::LEFT_STICK_DOWN);
				}
				buttonHoldTimes[GamepadEvents::LEFT_STICK_DOWN] = 0;
			}
		}

		if (leftStick.y > 0.5f)
		{
			if (!previousLeftStick.y > 0.5f)
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_DOWN, GamepadEvents::LEFT_STICK_UP);
			}
			else
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_HELD, GamepadEvents::LEFT_STICK_UP);
			}
			buttonHoldTimes[GamepadEvents::LEFT_STICK_UP] += msec;
		}
		else
		{
			if (previousLeftStick.y > 0.5f)
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_UP, GamepadEvents::LEFT_STICK_UP);
				
				if (buttonHoldTimes[GamepadEvents::LEFT_STICK_UP] < PRESSLIMIT)
				{
					GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_PRESS, GamepadEvents::LEFT_STICK_UP);
				}
				buttonHoldTimes[GamepadEvents::LEFT_STICK_UP] = 0;
			}
		}
	}
#pragma endregion
#pragma endregion
#pragma region Triggers
#pragma region Right Trigger
	T3Vector2 rightTrigger = gamePadState.button[19] / 255.0f;
	if (rightTrigger > DEADZONE)
	{
		GameStateManager::Instance()->GamepadAnalogueDisplacement(playerID, GamepadEvents::RIGHT_TRIGGER, rightTrigger);
		if (rightTrigger > 0.5f)
		{
			if (!previousRightTrigger > 0.5f)
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_DOWN, GamepadEvents::INPUT_R2);
			}
			else
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_HELD, GamepadEvents::INPUT_R2);
			}
			buttonHoldTimes[GamepadEvents::INPUT_R2] += msec;
		}
		else
		{
			if (previousRightTrigger > 0.5f)
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_UP, GamepadEvents::INPUT_R2);
				
				if (buttonHoldTimes[GamepadEvents::INPUT_R2] < PRESSLIMIT)
				{
					GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_PRESS, GamepadEvents::INPUT_R2);
				}
				buttonHoldTimes[GamepadEvents::INPUT_R2] = 0;
			}
		}
	}
#pragma endregion
#pragma region Left Trigger
	T3Vector2 leftTrigger = gamePadState.button[18] / 255.0f;
	if (leftTrigger > DEADZONE)
	{
		GameStateManager::Instance()->GamepadAnalogueDisplacement(playerID, GamepadEvents::RIGHT_TRIGGER, leftTrigger);
		if (leftTrigger > 0.5f)
		{
			if (!previousLeftTrigger > 0.5f)
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_DOWN, GamepadEvents::INPUT_R2);
			}
			else
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_HELD, GamepadEvents::INPUT_R2);
			}
			buttonHoldTimes[GamepadEvents::INPUT_R2] += msec;
		}
		else
		{
			if (previousLeftTrigger > 0.5f)
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_UP, GamepadEvents::INPUT_R2);
				
				if (buttonHoldTimes[GamepadEvents::INPUT_R2] < PRESSLIMIT)
				{
					GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_PRESS, GamepadEvents::INPUT_R2);
				}
				buttonHoldTimes[GamepadEvents::INPUT_R2] = 0;
			}
		}
	}
#pragma endregion
#pragma endregion
#pragma endregion
	// query gamePadState for button action
#pragma region Buttons
	int halfInput = 8;
	for(int i = 0; i < halfInput; ++i)
	{
		if(gamePadState.button[2] & (1 << i)) 
		{
			GamepadEvents::Button button = (GamepadEvents::Button)i;
			// Button is down, check for first
			if (!(previousGamePadState.button[2] & (1 << i)))
			{			
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_DOWN, button);
			}
			else
			{
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_HELD, button);
			}
			buttonHoldTimes[i] += msec;
		}
		else
		{
			// button is up, check if it was down
			if (previousGamePadState.button[2] & (1 << i))
			{
				GamepadEvents::Button button = (GamepadEvents::Button)i;
				GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_UP, button);
				if (buttonHoldTimes[i] < PRESSLIMIT)
				{
					GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_PRESS, button);
				}
			}
			buttonHoldTimes[i] = 0;
		}

		if(gamePadState.button[3] & (1 << i)) 
		{
			GamepadEvents::Button button = (GamepadEvents::Button)(halfInput + i);
			// Button is down, check for first
			if (!(previousGamePadState.button[2] & (1 << i)))
			{
				// Special case - L2 & R2 are analogue
				if (button == GamepadEvents::INPUT_L2 || button == GamepadEvents::INPUT_R2)
				{
				}
				else
				{
					if (!(previousGamePadState.button[2] & (1 << i)))
					{			
						GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_DOWN, button);
					}
					else
					{
						GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_HELD, button);
					}
					buttonHoldTimes[halfInput + i] += msec;
				}
			}
			else
			{
				// Special case - L2 & R2 are analogue
				if (button == GamepadEvents::INPUT_L2 || button == GamepadEvents::INPUT_R2)
				{
				}
				else
				{
					// button is up, check if it was down
					if (previousGamePadState.button[2] & (1 << i))
					{
						GamepadEvents::Button button = (GamepadEvents::Button)(halfInput +i);
						GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_UP, button);
						if (buttonHoldTimes[i] < PRESSLIMIT)
						{
							GameStateManager::Instance()->GamepadEvent(playerID, GamepadEvents::BUTTON_PRESS, button);
						}
					}
					buttonHoldTimes[halfInput + i] = 0;
				}
			}
		}
	}
#pragma endregion
	// Set for next frame
	previousGamePadState = gamePadState;
}