#if WINDOWS_BUILD
#pragma once
#include "../Framework/SoundManager.h"
#include "../Framework/SoundEmitter.h"
#include "../Framework/Vehicle.h"
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
	SoundEmitter* Getsoundemitter(){return a;}
private:
	Sound* BGM1;
	Sound* BGM2;
	Sound* Coins;
	Sound* Engine_start;
	Sound* Ready;
	Sound* Over;
	SoundEmitter* a;
	GameTimer timer;
	float CurrentTime;
	bool bgm1;
	bool bgm2;
	bool ready;
	bool coins;
	bool engine_start;
	bool pressonce;
	bool pressonce2;
	Vehicle* car;
};
#endif

