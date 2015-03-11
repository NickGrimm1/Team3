#pragma once

#include "GameScreen2D.h"
#include "DrawableText2D.h"
#include "GameStateManager.h"
#include <sstream>

class HudTestScreen : public GameScreen2D
{
public:
	HudTestScreen(void);
	virtual ~HudTestScreen(void);
	virtual void GamepadDisconnect(GamepadEvents::PlayerIndex playerID){}
	void SetScreen(int point,int time);
	void SetTime(int value){time+=value;}
	int GetTime(){return time;}
	DrawableText2D* Playtime;
	DrawableText2D* Score;
	virtual void Update();
	Font* f;
protected:
	void LoadContent();
	void UnloadContent();
	int time;
	int score;
};

