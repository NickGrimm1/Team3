#pragma once
#include "GameScreen3D.h"
#include "TrackSegment.h"
#include "Heightmap.h"

class GraphicsTestScreen : public GameScreen3D
{
public:
	GraphicsTestScreen();
	virtual ~GraphicsTestScreen(void);

	virtual void LoadContent();
	virtual void Update();
	virtual void UnloadContent();

	// Input Listener methods - implemented
#if WINDOWS_BUILD
	virtual void KeyboardEvent(KeyboardEvents::EventType type, KeyboardEvents::Key key);
	virtual void MouseMoved(T3Vector2& start, T3Vector2& finish);

	// Input Listener methods - not implementing
	virtual void MouseEvent(MouseEvents::EventType type, MouseEvents::MouseButtons button, T3Vector2& position) {};
	virtual void MouseScrolled(T3Vector2& position, int amount) {};
#endif
	virtual void GamepadEvent(GamepadEvents::PlayerIndex playerID, GamepadEvents::EventType type, GamepadEvents::Button button) {};
	virtual void GamepadAnalogueDisplacement(GamepadEvents::PlayerIndex playerID, GamepadEvents::AnalogueControl analogueControl, T3Vector2& amount) {};
	virtual void GamepadDisconnect(GamepadEvents::PlayerIndex playerID) {}
private:
	Mesh* quad;
	Mesh* cylinder;
	Mesh* circle;
	Mesh* car;
	SpotLight* spotLight;
	DirectionalLight* directionalLight;
	PointLight* pointLight;
	Camera* camera;
	TrackSegment* track;
	vector<DrawableEntity3D*> gameEntities;
	bool drawDeferredLights;
	DrawableEntity3D* heightMap;
	DrawableEntity3D* heightMapFront;
	DrawableEntity3D* heightMapBack;
	DrawableEntity3D* heightMapLeft;
	DrawableEntity3D* heightMapRight;
};

