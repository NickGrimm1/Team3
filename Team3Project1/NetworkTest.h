#pragma once
#ifdef WINDOWS_BUILD

#include "GameScreen3D.h"
#include "GameStateManager.h"

class ServerSocket;
class TcpSocket;
class BaseSocket;

class NetworkTest : public GameScreen3D
{
public:
	NetworkTest(void);
	virtual ~NetworkTest(void);

	virtual void LoadContent();
	virtual void Update();
	virtual void UnloadContent();

	// Input Listener methods - not implementing
	virtual void KeyboardEvent(KeyboardEvents::EventType type, KeyboardEvents::Key key) {}
	virtual void MouseMoved(T3Vector2& start, T3Vector2& finish) {}
	virtual void MouseEvent(MouseEvents::EventType type, MouseEvents::MouseButtons button, T3Vector2& position) {};
	virtual void MouseScrolled(T3Vector2& position, int amount) {};
	virtual void GamepadEvent(GamepadEvents::PlayerIndex playerID, GamepadEvents::EventType type, GamepadEvents::Button button) {};
	virtual void GamepadAnalogueDisplacement(GamepadEvents::PlayerIndex playerID, GamepadEvents::AnalogueControl analogueControl, T3Vector2& amount) {};

protected:
	ServerSocket* server;
	bool connected;
	BaseSocket* connection;	
};

#endif