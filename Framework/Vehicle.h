#pragma once
#include"MyGame.h"
#include "PhysicsNode.h"
#include "../Team3Project1/DrawableEntity3D.h"
#include "../Framework/GameEntity.h"
#include "../Team3Project1/GameStateManager.h"
#include "../Team3Project1/Vehicle_Wheel.h"
#include "../Team3Project1/VehiclePhysicsNode.h"


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


	//float GetX() {return car->GetLinearVelocity().x;}
	//void SetX(float x) {
	//	T3Vector3 temp = car->GetLinearVelocity();
	//	temp.x = x;
	//	car->SetLinearVelocity(temp);
	//}

	//float GetY() {return car->GetLinearVelocity().y;}
	//void SetY(float y) {
	//	T3Vector3 temp1 = car->GetLinearVelocity();
	//	temp1.y = y;
	//	car->SetLinearVelocity(temp1);
	//}

	//float GetZ() {return car->GetLinearVelocity().z;}
	//void SetZ(float z) {
	//	T3Vector3 temp2 = car->GetLinearVelocity();
	//	temp2.z = z;
	//	car->SetLinearVelocity(temp2);
	//}

	virtual Quaternion GetRotation() const;
	Vehicle_Wheel* GetFrontRightTire() {return FrontRightTire;}
	Vehicle_Wheel* GetFrontLeftTire() {return FrontLeftTire;}
	Vehicle_Wheel* GetBackRightTire() {return BackRightTire;}
	Vehicle_Wheel* GetBackLeftTire() {return BackLeftTire;}

	VehiclePhysicsNode * GetVehiclePhysicsNode(){ return vpn;}
	
protected:
	T3Vector3 maxSpeed;


	PhysicsNode* car;

	VehiclePhysicsNode* vpn;
    Vehicle_Wheel *FrontRightTire;
	Vehicle_Wheel *FrontLeftTire;
	Vehicle_Wheel *BackRightTire;
	Vehicle_Wheel *BackLeftTire;

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