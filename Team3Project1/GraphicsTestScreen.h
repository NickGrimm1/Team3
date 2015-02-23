#pragma once

#include "GameScreen3D.h"

class GraphicsTestScreen : public GameScreen3D
{
public:
	GraphicsTestScreen();
	virtual ~GraphicsTestScreen(void);

	virtual void LoadContent();
	virtual void Update();
	virtual void UnloadContent();

	// Input Listener methods - implemented
	virtual void KeyboardEvent(KeyboardEvents::EventType type, KeyboardEvents::Key key);
	virtual void MouseMoved(Vector2& finish);

	// Input Listener methods - not implementing
	virtual void MouseEvent(MouseEvents::EventType type, MouseEvents::MouseButtons button, Vector2& position) {};
	virtual void MouseScrolled(Vector2& position, int amount) {};
	virtual void GamepadEvent(GamepadEvents::PlayerIndex playerID, GamepadEvents::EventType type, GamepadEvents::Button button) {};
	virtual void GamepadAnalogueDisplacement(GamepadEvents::PlayerIndex playerID, GamepadEvents::AnalogueControl analogueControl, Vector2& amount) {};

private:
	Mesh* quad;
	Mesh* cylinder;
	Mesh* circle;
	Mesh* car;
	SpotLight* spotLight;
	DirectionalLight* directionalLight;
	PointLight* pointLight;
	Camera* camera;
	vector<DrawableEntity3D*> gameEntities;
	bool drawDeferredLights;
};

