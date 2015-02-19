#pragma once

#include "GameScreen2D.h"

class HudTestScreen : public GameScreen2D
{
public:
	HudTestScreen(void);
	virtual ~HudTestScreen(void);

protected:
	void LoadContent();
};

