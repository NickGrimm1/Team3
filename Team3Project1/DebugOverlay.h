#pragma once

#include "GameScreen2D.h"
#include "DrawableText2D.h"

class DebugOverlay : public GameScreen2D
{
public:
	DebugOverlay();
	~DebugOverlay();
	virtual void LoadContent();
	virtual void Update();
	virtual void UnloadContent();

#if WINDOWS_BUILD
	virtual void KeyboardEvent(KeyboardEvents::EventType type, KeyboardEvents::Key key) { }
	virtual void MouseMoved(T3Vector2& start, T3Vector2& finish);
	virtual void MouseEvent(MouseEvents::EventType type, MouseEvents::MouseButtons button, T3Vector2& position) { }
	virtual void MouseScrolled(T3Vector2& position, int amount) { }
#endif
	virtual void GamepadEvent(GamepadEvents::PlayerIndex playerID, GamepadEvents::EventType type, GamepadEvents::Button button) { }
	virtual void GamepadAnalogueDisplacement(GamepadEvents::PlayerIndex playerID, GamepadEvents::AnalogueControl analogueControl, T3Vector2& amount) { }
	virtual void GamepadDisconnect(GamepadEvents::PlayerIndex playerID) { }
private:
	DrawableText2D* renderFrameRate;
	DrawableText2D* physicsFrameRate;
	DrawableText2D* inputFrameRate;
	
	DrawableText2D* totalMemory;
	DrawableText2D* meshMemory;
	DrawableText2D* textureMemory;

	DrawableText2D* mouseX;
	DrawableText2D* mouseY;
};