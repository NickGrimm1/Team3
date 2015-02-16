#pragma once

#include "GameScreen3D.h"

class GraphicsTestScreen : public GameScreen3D
{
public:
	GraphicsTestScreen();
	virtual ~GraphicsTestScreen(void);

	virtual void LoadContent();


	// Input Listener methods - not implemented
	virtual void MouseEvent(MouseEvents::EventType type, MouseEvents::MouseButtons button, Vector2& position) {};
	virtual void MouseMoved(Vector2& finish) {};
	virtual void MouseScrolled(Vector2& position, int amount) {};
	virtual void KeyboardEvent(KeyboardEvents::EventType type, KeyboardEvents::Key key) {};
	virtual void GamepadEvent(GamepadEvents::PlayerIndex playerID, GamepadEvents::EventType type, GamepadEvents::Button button) {};
	virtual void GamepadAnalogueDisplacement(GamepadEvents::PlayerIndex playerID, GamepadEvents::AnalogueControl analogueControl, Vector2& amount) {};
	

private:
	Mesh* quad;
	SpotLight* light;
};
