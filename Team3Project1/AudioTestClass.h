#pragma once
#include "../Framework/SoundManager.h"
#include "GameScreen3D.h"
#include "GameScreen2D.h"
class AudioTestClass: public GameScreen3D
{
public:
	AudioTestClass(void);
	virtual  ~AudioTestClass(void);

	virtual void LoadContent();
	virtual void Update();
	virtual void UnloadContent() {}

	// Input Listener methods - implemented
	virtual void KeyboardEvent(KeyboardEvents::EventType type, KeyboardEvents::Key key);
	
	// Input Listener methods - not implementing
	virtual void MouseMoved(T3Vector2& start, T3Vector2& finish) {};
	virtual void MouseEvent(MouseEvents::EventType type, MouseEvents::MouseButtons button, T3Vector2& position) {};
	virtual void MouseScrolled(T3Vector2& position, int amount) {};
	virtual void GamepadEvent(GamepadEvents::PlayerIndex playerID, GamepadEvents::EventType type, GamepadEvents::Button button) {};
	virtual void GamepadAnalogueDisplacement(GamepadEvents::PlayerIndex playerID, GamepadEvents::AnalogueControl analogueControl, T3Vector2& amount) {};

private:
	Sound* laser;
	Sound* laser1;
	Sound* laser2;
};

