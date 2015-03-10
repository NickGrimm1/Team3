#pragma once

#include "Scoreboard.h"
#include "GameScreen2D.h"
#include "DrawableText2D.h"
#include "../Framework/GameTimer.h"

class HighScore : public GameScreen2D
{
public:
	HighScore(unsigned int highScore);
	virtual ~HighScore(void);

	virtual void LoadContent();
	virtual void Update();
	virtual void UnloadContent();

#if WINDOWS_BUILD
	virtual void KeyboardEvent(KeyboardEvents::EventType type, KeyboardEvents::Key key);
	
	// Input Listener methods - not implementing
	virtual void MouseEvent(MouseEvents::EventType type, MouseEvents::MouseButtons button, T3Vector2& position) {};
	virtual void MouseScrolled(T3Vector2& position, int amount) {};
	virtual void MouseMoved(T3Vector2& start, T3Vector2& finish) {};
#endif
	virtual void GamepadEvent(GamepadEvents::PlayerIndex playerID, GamepadEvents::EventType type, GamepadEvents::Button button);
	virtual void GamepadAnalogueDisplacement(GamepadEvents::PlayerIndex playerID, GamepadEvents::AnalogueControl analogueControl, T3Vector2& amount);
	virtual void GamepadDisconnect(GamepadEvents::PlayerIndex playerID) {}

protected:

	bool SubmitScore();
#if WINDOWS_BUILD
	Scoreboard* scoreboard;
#endif
	unsigned int currentPos;
	string name;
	DrawableText2D* nameEntities[3];
	DrawableText2D* cursor;
	DrawableText2D* smallCursor[3];
	DrawableText2D* bigCursor;
	unsigned int highScore;
	float lastRight;
	float lastLeft;
	float lastUp;
	float lastDown;
	GameTimer timer;
};

