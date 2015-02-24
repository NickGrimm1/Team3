#pragma once

#include "GameScreen3D.h"
#include "../Framework/Vehicle.h"
#include "../Framework/MyGame.h"

class RacerGame : public GameScreen3D
{
public:
	RacerGame(void);
	virtual ~RacerGame(void);
	virtual void LoadContent();
	virtual void Update();


	// Input Listener methods - not implemented
	virtual void MouseEvent(MouseEvents::EventType type, MouseEvents::MouseButtons button, Vector2& position) {};
	virtual void MouseMoved(Vector2& finish);
	virtual void MouseScrolled(Vector2& position, int amount) {};
	virtual void KeyboardEvent(KeyboardEvents::EventType type, KeyboardEvents::Key key);
	virtual void GamepadEvent(GamepadEvents::PlayerIndex playerID, GamepadEvents::EventType type, GamepadEvents::Button button) {};
	virtual void GamepadAnalogueDisplacement(GamepadEvents::PlayerIndex playerID, GamepadEvents::AnalogueControl analogueControl, Vector2& amount) {};
	
	float f;
	int Speed_Player;
	float Speed_Rotate;
	Vector3 tempPosition;
	Vector3 temp,temp1,temp2;
	Vector3 PlayerPosition;
private:
	Mesh* quad;
	Mesh* cylinder;
	SpotLight* light;
	Camera* camera;
	Vehicle* car;
	
	DrawableEntity3D* ent;
};

