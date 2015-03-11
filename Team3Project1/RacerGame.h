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
#include "Gold_cion.h"
#include "HudTestScreen.h"
#include "GameStateManager.h"
//#include "../Framework/SoundManager.h"

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

	void SetScore(int value){score+=value;}
	float GetScore(){return score;}
	void SetTime(int value){Time+=value;}
	float GetTime(){return Time;}
	
	void SetPlayTime(int value){PlayTime+=value;}
	int GetPlayTime(){return PlayTime;}

	void SettimeOrScore(int value){timeOrScore+=value;}
	int GettimeOrScore(){return timeOrScore;}

	void CreateTrack();
	void DeleteTrack();
	void Start();
	float GetCreateAngle();
	float f;
	float b;
	int Speed_Player;
	float Speed_Rotate;
	T3Vector3 tempPosition;
	T3Vector3 temp,temp1,temp2;
	T3Vector3 PlayerPosition;



	//sam
	Texture* scoreTexture;
	Texture* timeTexture;
	HudTestScreen* hud;
	static float g;
	static float gx;
	TrackSegment* Strack;
#if WINDOWS_BUILD	
	vector<T3Vector3> SplinePoint;
	vector<DrawableEntity3D*> allEntities;
	vector<TrackSegment*> TrackSegmentVector;
	vector<GameEntity*> checkPoint;
#endif 
	 virtual void CollisionBetween(GameEntity* obj1, GameEntity* obj2) {
		 cout<<obj1->GetType();
		  cout<<" collisionbetween ";
		  cout<<obj2->GetType()<<endl;
		  if(obj1->GetType()=='g')
		  {
			  obj1->GetPhysicsNode().SetIsCollide(false);
			  cout<<"set = true ok"<<endl;
			  RacerGame::update=1;
		  }
		    if(obj2->GetType()=='g')
		  {
			  obj2->GetPhysicsNode().SetIsCollide(false);
			  cout<<"set2 = true ok"<<endl;
			
			  RacerGame::update=1;
			  
		  }
			 if(obj2->GetType()=='d')
		  {
			  obj2->GetPhysicsNode().SetIsCollide(false);
			  cout<<"delete = true ok"<<endl;
			  RacerGame::update=2;
			    SettimeOrScore(1);
			  
		  }
			  if(obj2->GetType()=='p')
		  {
			  obj2->GetPhysicsNode().SetIsCollide(false);
			  cout<<"point get = true ok"<<endl;
			  SetScore(1);
			 // SoundManager::AddSound(SOUNDSDIR"Tokyo Drift2.wav");
			 // GameStateManager::Audio()->PlaySoundW(GameStateManager::Audio()->GetSound(SOUNDSDIR"Tokyo Drift2.wav"),SOUNDPRIORITY_ALWAYS);
			  cout<< "total point ="<<GetScore()<<endl; 
			  GameStateManager::Graphics()->RemoveDrawable(obj2);
	          obj2->DisconnectFromSystems();
		  }
			   if(obj2->GetType()=='t')
		  {
			  obj2->GetPhysicsNode().SetIsCollide(false);
			  cout<<"time get = true ok"<<endl;
			  SettimeOrScore(-(GettimeOrScore()));
			  SetPlayTime(30);
			  cout<< "total time ="<<GetTime()<<endl; 
			  GameStateManager::Graphics()->RemoveDrawable(obj2);
	          obj2->DisconnectFromSystems();
		  }
			   		  
   }
	//sam



private:
	Mesh* quad;
	Mesh* cylinder;
	int score;
	float Time;
	int PlayTime;
	int timeOrScore;
	#if WINDOWS_BUILD

	SpotLight* light;
#endif
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

