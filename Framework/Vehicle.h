#pragma once
#include"MyGame.h"
#include "PhysicsNode.h"
#include "../Team3Project1/DrawableEntity3D.h"
#include "../Framework/GameEntity.h"




class Vehicle : public GameEntity {
public:
	Vehicle(float size);
	~Vehicle(void);

	void SetPhysics(float size,PhysicsNode * a);


	void BuildPlayerEntity(float size, T3Vector3 pos);
	//GameEntity* BuildBuffEntity(float radius, T3Vector3 pos);
	//void UpdatePlayer(float msec);
	GameEntity* GetPlayer(){return Player;}
	void SetSpeed_Player(int Speed){ Speed_Player=Speed;}
	int GetSpeed_Player(){return Speed_Player;}
	float GetSize(){return size;}

      void	Update(float msec);
	//void UpdatePlayer2(float msec);

	T3Vector3 tempPosition;
	T3Vector3 GetMaxSpeed() {return maxSpeed; }
	PhysicsNode&	GetCarNode()	{ return *car;}
	virtual void			ConnectToSystems(){
		if(car) {
		GameStateManager::Physics()->AddNode(car);
		car->SetTarget(this);
	}

	}

protected:
	float maxSpeed;



	PhysicsNode* car;


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