#pragma once
#include"MyGame.h"

class Vehicle {
public:
	Vehicle();
	~Vehicle(void);
	GameEntity* BuildPlayerEntity(float size, Vector3 pos);
	GameEntity* BuildBuffEntity(float radius, Vector3 pos);
	void UpdatePlayer(float msec);

	Vector3 tempPosition;
protected:
	Vector3 PlayerPosition;
	Vector3 temp,temp1;
	Mesh* PlayerMesh;
	Mesh* BuffMesh;
	GameEntity* Player; 
	GameEntity* Buff;

	int Speed_Player;
	float Speed_Rotate;
	
};