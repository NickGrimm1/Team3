#if WINDOWS_BUILD
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
	float GetCurrentTimer(){return CurrentTime;}

	// Input Listener methods - implemented
	virtual void KeyboardEvent(KeyboardEvents::EventType type, KeyboardEvents::Key key);
	
	// Input Listener methods - not implementing
	virtual void MouseMoved(T3Vector2& start, T3Vector2& finish) {};
	virtual void MouseEvent(MouseEvents::EventType type, MouseEvents::MouseButtons button, T3Vector2& position) {};
	virtual void MouseScrolled(T3Vector2& position, int amount) {};
	virtual void GamepadEvent(GamepadEvents::PlayerIndex playerID, GamepadEvents::EventType type, GamepadEvents::Button button) {};
	virtual void GamepadAnalogueDisplacement(GamepadEvents::PlayerIndex playerID, GamepadEvents::AnalogueControl analogueControl, T3Vector2& amount) {};
	virtual void GamepadDisconnect(GamepadEvents::PlayerIndex playerID){}
private:
	Sound* BGM1;
	Sound* BGM2;
	Sound* BGM3;
	Sound* laser;
	Sound* laser1;
	Sound* laser2;
	Sound* laser3;
	GameTimer timer;
	float CurrentTime;
	bool bgm1;
	bool bgm2;
	bool bgm3;
	bool pressonce;
	bool pressonce2;
	bool pressonce3;
};
#endif

