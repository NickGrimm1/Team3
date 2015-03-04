#pragma once

#include "GameScreen2D.h"

class HudTestScreen : public GameScreen2D
{
public:
	HudTestScreen(void);
	virtual ~HudTestScreen(void);
	virtual void GamepadDisconnect(GamepadEvents::PlayerIndex playerID) {};
protected:
	void LoadContent();
	void UnloadContent();
};

