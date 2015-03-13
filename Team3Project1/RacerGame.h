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
class AudioTestClass;
class Vehicle;
class RacerGame : public GameScreen3D
{
public:
	#if WINDOWS_BUILD
	AudioTestClass* audio;
#endif
	RacerGame(unsigned int lowestScore, GamepadEvents::PlayerIndex playerController);
	virtual ~RacerGame(void);
	virtual void LoadContent();
	virtual void UnloadContent();
	virtual void Update();
	static int update;
	virtual void Pause();
	virtual void Resume();
	// Input Listener methods - not implemented
#if WINDOWS_BUILD
	virtual void MouseEvent(MouseEvents::EventType type, MouseEvents::MouseButtons button, T3Vector2& position) {};
	virtual void MouseMoved(T3Vector2& start, T3Vector2& finish);
	virtual void MouseScrolled(T3Vector2& position, int amount) {};
	virtual void KeyboardEvent(KeyboardEvents::EventType type, KeyboardEvents::Key key);
#endif
	virtual void GamepadEvent(GamepadEvents::PlayerIndex playerID, GamepadEvents::EventType type, GamepadEvents::Button button);
	virtual void GamepadAnalogueDisplacement(GamepadEvents::PlayerIndex playerID, GamepadEvents::AnalogueControl analogueControl, T3Vector2& amount);
	virtual void GamepadDisconnect(GamepadEvents::PlayerIndex playerID)
	{
		if (isControllerControlled)
		{
			playerController = GamepadEvents::PLAYERINDEX_MAX;
			GameStateManager::Pause();
		}
	}
	void SetScore(int value){score+=value;}
	float GetScore(){return score;}
	void SetTime(int value){Time+=value;}
	float GetTime(){return Time;}
	void SetPlayTime(int value){PlayTime+=value;}
	int GetPlayTime(){return PlayTime;}
	void SettimeOrScore(int value){timeOrScore+=value;}
	int GettimeOrScore(){return timeOrScore;}
	void SetMaxSpeed(float value){maxSpeed+=value;}
	float GetMaxSpeed(){return maxSpeed;}
	void SetMinSpeed(float value);
	float GetMinSpeed(){return minSpeed;}

	void GameOver();

	void CreateTrack();
	void DeleteTrack();
	void Start();
	float GetCreateAngle();
	float f;
	float b;
	int start;
	float maxSpeed;
	float minSpeed;
	int Speed_Player;
	float Speed_Rotate;
	T3Vector3 tempPosition;
	T3Vector3 temp,temp1,temp2;
	T3Vector3 PlayerPosition;
	Texture* scoreTexture;
	Texture* timeTexture;
	static float g;
	static float gx;
	TrackSegment* Strack;
	vector<T3Vector3> SplinePoint;
	vector<GameEntity*> allEntities;
	vector<TrackSegment*> TrackSegmentVector;
	vector<GameEntity*> checkPoint;
	vector<Gold_cion*> pickup;
	virtual void carFall() {SetPlayTime(-GetPlayTime()-1);}
	 virtual void CollisionBetween(GameEntity* obj1, GameEntity* obj2) {
		  if(obj1->GetType()=='g')
		  {
			  if(obj1->GetPhysicsNode().GetIsCollide()==false){
				  if(GettimeOrScore()>3){
					  SetPlayTime(4);
					  SetMinSpeed(10);
					  SetMaxSpeed(10);
			}
			   CreateTrack();
	            checkPoint[0]->DisconnectFromSystems();
	delete checkPoint[0];
	checkPoint.erase(checkPoint.begin());
			  }
		  }
			 if(obj1->GetType()=='d')
		  {
			   DeleteTrack();
			   checkPoint[0]->DisconnectFromSystems();
	delete checkPoint[0];
	checkPoint.erase(checkPoint.begin());
			    SettimeOrScore(1);
		  }
			 if(obj1->GetType()=='p')
		  {
#ifdef WINDOWS_BUILD
			  Sound* coins;
			  coins=GameStateManager::Audio()->GetSound(SOUNDSDIR"coins.wav");
			  GameStateManager::Audio()->PlaySoundA(coins, obj1->GetOriginPosition(), false);
#endif			  
	  		  obj1->GetPhysicsNode().SetIsCollide(false);
			  SetScore(1);
			  cout<< "total point ="<<GetScore()<<endl; 
			  GameStateManager::Graphics()->RemoveDrawable(obj1);
	          obj1->DisconnectFromSystems();
			  pickup.erase(pickup.begin());
		  }
			 if(obj1->GetType()=='t')
		  {
#if WINDOWS_BUILD
			  Sound* time;
			  time=GameStateManager::Audio()->GetSound(SOUNDSDIR"time.wav");
			  GameStateManager::Audio()->PlaySoundA(time, obj1->GetOriginPosition(), false);
#endif
			  obj1->GetPhysicsNode().SetIsCollide(false);
			  if(GetScore()<40){
			  SetPlayTime(2);
			  }
			  if(GetScore()>=40){
				  SetMinSpeed(-40.0f);
			  }
			  GameStateManager::Graphics()->RemoveDrawable(obj1);
	          obj1->DisconnectFromSystems();
		  }
   }
private:
	Mesh* quad;
	Mesh* cylinder;
	unsigned int score;
	float Time;
	int PlayTime;
	int timeOrScore;
	#if WINDOWS_BUILD
	SpotLight* light;
#endif
	ChaseCamera* chasecamera;
	Vehicle* car;
	CheckPoint * checkpoint;
	CheckPoint * checkpoint2;
	Vehicle_Wheel * FrontRightTire;
	Vehicle_Wheel * FrontLeftTire;
	Vehicle_Wheel * BackRightTire;
	Vehicle_Wheel * BackLeftTire;
	HudTestScreen* hud; 
	DrawableEntity3D* ent;
	DrawableEntity3D* ent2;
	bool isplaystartegine;
	bool isplaylowspdegine;
	bool isplaymidspdegine;
	bool moveenginesound;
	bool carspeediszero;
	bool gameOver;
	bool isPaused;
	unsigned int lowScore;
#if WINDOWS_BUILD
	SoundEmitter* Engine0;
	SoundEmitter* Engine1;
#endif
	GamepadEvents::PlayerIndex playerController;
	bool isControllerControlled;
};

