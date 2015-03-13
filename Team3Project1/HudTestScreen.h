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
	void SetScreen(int point,int time,float speed);
	void SetTime(int value){time+=value;}
	int GetTime(){return time;}
	DrawableText2D* Playtime;
	DrawableText2D* Score;
	DrawableText2D* Speed;
	virtual void Update();
	Font* f;
protected:
	void LoadContent();
	void UnloadContent();
	int time;
	int score;
	float speed;
};

