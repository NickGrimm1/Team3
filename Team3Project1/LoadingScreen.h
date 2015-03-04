#pragma once

#include "Mesh.h"
#include "Texture.h"
#include "GameScreen2D.h"
#include "DrawableTexture2D.h"

class LoadingScreen : public GameScreen2D
{
public:
	LoadingScreen() {}
	virtual ~LoadingScreen() {}

	virtual void LoadContent();
	virtual void Update();
	virtual void UnloadContent();

	// Input Listener methods - not implementing
#if WINDOWS_BUILD
	virtual void KeyboardEvent(KeyboardEvents::EventType type, KeyboardEvents::Key key) {}
	virtual void MouseMoved(T3Vector2& finish) {}
	virtual void MouseEvent(MouseEvents::EventType type, MouseEvents::MouseButtons button, T3Vector2& position) {};
	virtual void MouseScrolled(T3Vector2& position, int amount) {};
#endif	
	virtual void GamepadEvent(GamepadEvents::PlayerIndex playerID, GamepadEvents::EventType type, GamepadEvents::Button button) {};
	virtual void GamepadAnalogueDisplacement(GamepadEvents::PlayerIndex playerID, GamepadEvents::AnalogueControl analogueControl, T3Vector2& amount) {};
	virtual void GamepadDisconnect(GamepadEvents::PlayerIndex playerID) {};
protected:
	Texture* refreshTex;
	DrawableTexture2D* refreshIcon;
	bool isLoading;
};
