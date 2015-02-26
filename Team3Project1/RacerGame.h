#pragma once

#include "GameScreen3D.h"
#include "../Framework/Vehicle.h"
#include "../Framework/MyGame.h"
#include "FreeCamera.h"

class RacerGame : public GameScreen3D
{
public:
	RacerGame(void);
	virtual ~RacerGame(void);
	virtual void LoadContent();
	virtual void Update();


	// Input Listener methods - not implemented
#if WINDOWS_BUILD
	virtual void MouseEvent(MouseEvents::EventType type, MouseEvents::MouseButtons button, T3Vector2& position) {};
	virtual void MouseMoved(T3Vector2& finish);
	virtual void MouseScrolled(T3Vector2& position, int amount) {};
	virtual void KeyboardEvent(KeyboardEvents::EventType type, KeyboardEvents::Key key);
#endif	
	virtual void GamepadEvent(GamepadEvents::PlayerIndex playerID, GamepadEvents::EventType type, GamepadEvents::Button button) {};
	virtual void GamepadAnalogueDisplacement(GamepadEvents::PlayerIndex playerID, GamepadEvents::AnalogueControl analogueControl, T3Vector2& amount) {};
	
	float f;
	int Speed_Player;
	float Speed_Rotate;
	T3Vector3 tempPosition;
	T3Vector3 temp,temp1,temp2;
	T3Vector3 PlayerPosition;
private:
	Mesh* quad;
	Mesh* cylinder;
	SpotLight* light;
	FreeCamera* camera;
	Vehicle* car;
	
	DrawableEntity3D* ent;
};

