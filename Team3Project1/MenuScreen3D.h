#pragma once
#include "GameScreen3D.h"
class MenuScreen3D : public GameScreen3D
{
public:
	MenuScreen3D(void);
	virtual ~MenuScreen3D(void);

	virtual void LoadContent(void);
	virtual void Update(void);
	virtual void UnloadContent(void);

#if WINDOWS_BUILD
	virtual void KeyboardEvent(KeyboardEvents::EventType type, KeyboardEvents::Key key) {};
	virtual void MouseMoved(T3Vector2& start, T3Vector2& finish) {};
	virtual void MouseEvent(MouseEvents::EventType type, MouseEvents::MouseButtons button, T3Vector2& position) {};
	virtual void MouseScrolled(T3Vector2& position, int amount) {};
#endif
	virtual void GamepadEvent(GamepadEvents::PlayerIndex playerID, GamepadEvents::EventType type, GamepadEvents::Button button) {};
	virtual void GamepadAnalogueDisplacement(GamepadEvents::PlayerIndex playerID, GamepadEvents::AnalogueControl analogueControl, T3Vector2& amount) {};
	virtual void GamepadDisconnect(GamepadEvents::PlayerIndex playerID) {}

private:
	
	Camera* camera;
};