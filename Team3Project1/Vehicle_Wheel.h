#pragma once
#include"../Framework/MyGame.h"
#include "../Framework/PhysicsNode.h"
#include "../Team3Project1/DrawableEntity3D.h"
#include "../Framework/GameEntity.h"

class Vehicle_Wheel : public GameEntity {
public:
	Vehicle_Wheel(float size/*,PhysicsNode * carPhysicNode*/);
	~Vehicle_Wheel(void);
	void AddMove_W();
	void SetPhysics(float size);
	void SetPhysics(float size,PhysicsNode * a);
	

	/*void SetSpeed_Player(int Speed){ Speed_Player=Speed;}
	int GetSpeed_Player(){return Speed_Player;}
	float GetSize(){return size;}*/

	/*void UpdatePlayer1(float msec);*/
	//void UpdatePlayer2(float msec);

	//T3Vector3 tempPosition;
protected:


	PhysicsNode * carPhysicNode;
	/*float f;
	float size;
	int Speed_Player;
	float Speed_Rotate;*/

};