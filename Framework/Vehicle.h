#pragma once
#include"MyGame.h"
#include "PhysicsNode.h"

class Vehicle {
public:
	Vehicle();
	~Vehicle(void);
	GameEntity* BuildPlayerEntity(float size, T3Vector3 pos);
	GameEntity* BuildBuffEntity(float radius, T3Vector3 pos);
	//void UpdatePlayer(float msec);
	GameEntity* GetPlayer(){return Player;}
	void SetSpeed_Player(int Speed){ Speed_Player=Speed;}
	int GetSpeed_Player(){return Speed_Player;}
	float GetSize(){return size;}

	void UpdatePlayer1(float msec);
	//void UpdatePlayer2(float msec);

	T3Vector3 tempPosition;
protected:
	T3Vector3 PlayerPosition;
	T3Vector3 temp,temp1,temp2;
	Mesh* PlayerMesh;
	Mesh* BuffMesh;
	GameEntity* Player; 
	GameEntity* Buff;
	float f;
	float size;
	int Speed_Player;
	float Speed_Rotate;
	unsigned int TT;
};