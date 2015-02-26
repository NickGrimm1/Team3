/******************************************************************************
Class:MyGame
Implements:GameEntity
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>, Callum Rhodes <c.g.rhodes@ncl.ac.uk> and YOU!
Description: This is an example of a derived game class - all of your game 
specific things will go here. Such things would include setting up the game 
environment (level, enemies, pickups, whatever), and keeping track of what the
gameplay state is (Is the player dead? Has he/she won the game? And so on).

To get you started, there's a few static functions included that will create 
create some simple game objects - ideally your game object loading would be
handled by config files or somesuch, but for a simple demo game, these 
functions are a good start. Don't be afraid to modify/delete/add more of these!

Finally, there's some calls to the OGLRenderer debug draw functions that have
been added to the course this year. These are really useful for debugging the
state of your game in a graphical way - i.e drawing a line from a character
to the item it is aiminimumg at, or pointing in the direction it is moving, or
for drawing debug bounding volumes to see when objects collide. Feel free
to add more of these debug functions - how they are implemented is covered
in the code definitions for them, and also in the extra introduction 
document on the research website.

Have fun!


-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////



#pragma once
#include "GameEntity.h"
#include "SoundEntity.h"
#include "GameClass.h"
#include "Camera.h"
#include "CubeRobot.h"

#include "SoundSystem.h"
#include "SoundEmitter.h"
#include "SoundManager.h"
class Vehicle;
class Debuff;
class MyGame : public GameClass	{
public:
	MyGame();
	~MyGame(void);

	virtual void UpdateGame(float msec);
	static int newt ;
	static int dt ;
	static int st ;
	void GT(float y);
	void GT2(float y);
	void GT3(float y);
	static int g;

protected:
	GameEntity* BuildRobotEntity();

	GameEntity* BuildCubeEntity(float size);

	GameEntity* BuildCubeEntityout(float size);

	GameEntity* BuildSphereEntity(float radius);

	GameEntity* BuildQuadEntity(float size);

	GameEntity* BuildVehicleEntity(float size,T3Vector3 pos, T3Vector3 vel);
#if WINDOWS_BUILD
	SoundEntity* Background_Music;

	SoundEntity* BuildSoundEntity ();
#endif
	Vehicle* Car;
	Debuff*  DB;
	Mesh* cube;
	Mesh* quad;
	Mesh* sphere;
	Mesh* newCar;
	
    int v;
	float Xstart;
	float Xend;
	float Ystart;
	float Yend;
	
	
};

