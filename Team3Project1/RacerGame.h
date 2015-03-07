#pragma once

#include "GameScreen3D.h"
#include "../Framework/Vehicle.h"
#include "../Framework/MyGame.h"
#include "FreeCamera.h"
#include "ChaseCamera.h"
#include "Vehicle_Wheel.h"
#include "VehiclePhysicsNode.h"
#include "CheckPoint.h"
#include "TrackSegment.h"
class RacerGame : public GameScreen3D
{
public:
	RacerGame(void);
	virtual ~RacerGame(void);
	virtual void LoadContent();
	virtual void UnloadContent() {}
	virtual void Update();
	static int update;


	// Input Listener methods - not implemented
#if WINDOWS_BUILD
	virtual void MouseEvent(MouseEvents::EventType type, MouseEvents::MouseButtons button, T3Vector2& position) {};
	virtual void MouseMoved(T3Vector2& start, T3Vector2& finish);
	virtual void MouseScrolled(T3Vector2& position, int amount) {};
	virtual void KeyboardEvent(KeyboardEvents::EventType type, KeyboardEvents::Key key);
#endif
	virtual void GamepadEvent(GamepadEvents::PlayerIndex playerID, GamepadEvents::EventType type, GamepadEvents::Button button) {};
	virtual void GamepadAnalogueDisplacement(GamepadEvents::PlayerIndex playerID, GamepadEvents::AnalogueControl analogueControl, T3Vector2& amount) {};
	virtual void GamepadDisconnect(GamepadEvents::PlayerIndex playerID){}

	float f;
	int Speed_Player;
	float Speed_Rotate;
	T3Vector3 tempPosition;
	T3Vector3 temp,temp1,temp2;
	T3Vector3 PlayerPosition;

	//sam
	static float g;
	static float gx;
	TrackSegment* Strack;
	
	vector<T3Vector3> SplinePoint;
	vector<TrackSegment*> TrackSegmentVector;
	 virtual void CollisionBetween(GameEntity* obj1, GameEntity* obj2) {
		 cout<<obj1->GetType();
		  cout<<" collisionbetween ";
		 cout<<obj2->GetType()<<endl;
		  if(obj1->GetType()=='g')
		  {
			  obj1->GetPhysicsNode().SetIsCollide(true);
			  cout<<"set = true ok"<<endl;
			  RacerGame::update=1;
		  }
   }
	//sam


private:
	Mesh* quad;
	Mesh* cylinder;
	SpotLight* light;
	FreeCamera* camera;
	ChaseCamera* chasecamera;
	Vehicle* car;

	CheckPoint * checkpoint;
	CheckPoint * checkpoint2;
	Vehicle_Wheel * FrontRightTire;
	Vehicle_Wheel * FrontLeftTire;
	Vehicle_Wheel * BackRightTire;
	Vehicle_Wheel * BackLeftTire;
	
	DrawableEntity3D* ent;
	DrawableEntity3D* ent2;
};

