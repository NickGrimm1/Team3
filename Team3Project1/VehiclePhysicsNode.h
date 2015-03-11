#pragma once

#include "../Framework/Quaternion.h"
#include "../Framework/T3Vector3.h"
#include "../Framework/T3Matrix4.h"
#include "../Framework/SceneNode.h"
#include "../Team3Project1/DrawableEntity3D.h"
#include "../Framework/PhysicsNode.h"
#include "../Framework/CollisionVolume.h"
#include "../Framework/GameEntity.h"
#include "../Framework/Vehicle.h"
#include "../Team3Project1/RacerGame.h"


#define LINEAR_VELOCITY_DAMP 0.988
#define ANGULAR_VELOCITY_DAMP 0.998
#define LINEAR_VELOCITY_MIN 0.00001

class VehiclePhysicsNode : public PhysicsNode	
{
public:

	VehiclePhysicsNode(float size) : size(size){
		//car = new PhysicsNode();
		FrontLeftTire = new PhysicsNode();
		FrontRightTire = new PhysicsNode();
		BackLeftTire = new PhysicsNode();
		BackRightTire = new PhysicsNode();
		carMesh=GameStateManager::Assets()->LoadMesh(this, MESHDIR"CarPhysics.obj");
		tireMesh=GameStateManager::Assets()->LoadMesh(this, MESHDIR"Nova Tire.obj");
		collisionVertices = new Vertex[carMesh->GetNumVertices() + 4 * tireMesh->GetNumVertices()];
		
		cout << "car: " << carMesh->GetNumVertices() + 4 * tireMesh->GetNumVertices() << endl;
	}

	~VehiclePhysicsNode(void){}

	virtual void		Update(float msec) {
		
		/*FrontRightTire->SetPosition(car->BuildTransform()*T3Vector3(10,-2,8));
		FrontLeftTire->SetPosition(T3Vector3(car->GetPosition().x+10,car->GetPosition().y-2,car->GetPosition().z-5));
		BackRightTire->SetPosition(T3Vector3(car->GetPosition().x-11,car->GetPosition().y-2,car->GetPosition().z+8));
		BackLeftTire->SetPosition(T3Vector3(car->GetPosition().x-11,car->GetPosition().y-2,car->GetPosition().z-5));*/
		
		PhysicsNode::Update(msec);
		
		
	/*	FrontRightTire->SetPosition(car->BuildTransform()*T3Vector3(10,-2,8));
		FrontLeftTire->SetPosition(car->BuildTransform()*T3Vector3(10,-2,-5));
		BackRightTire->SetPosition(car->BuildTransform()*T3Vector3(-11,-2,8));
		BackLeftTire->SetPosition( car->BuildTransform()*T3Vector3(-11,-2,-5));*/

		FrontRightTire->SetPosition(BuildTransform()*T3Vector3(size * 1.3,size * -0.5,  size * 2.6));
		FrontLeftTire->SetPosition(BuildTransform()*T3Vector3(size *- 1.3,size * -0.5,size * 2.6));
		BackRightTire->SetPosition(BuildTransform()*T3Vector3(size * 1.3,size * -0.5,size * -1.7));
		BackLeftTire->SetPosition(BuildTransform()*T3Vector3(size * -1.3,size * -0.5,size * -1.7));

		UpdatePhysicsMesh();
	};
	PhysicsNode* GetCar(){return car;}
	PhysicsNode* getLFW(){return FrontLeftTire;}
	PhysicsNode* getRFW(){return FrontRightTire;}
	PhysicsNode* getBLW(){return BackLeftTire;}
	PhysicsNode* getBRW(){return BackRightTire;}

	Vertex* UpdatePhysicsMesh() {
		

		memcpy(collisionVertices, carMesh->GetVertices(), sizeof(Vertex) * carMesh->GetNumVertices());
		
		Vertex* tireVertices = tireMesh->GetVertices();

		unsigned int i = carMesh->GetNumVertices();

		for (unsigned int j = 0; j < tireMesh->GetNumVertices(); j++) {
			collisionVertices[i++].SetPosition(T3Matrix4::Translation(T3Vector3(1.3,-0.5,2.6)) * tireVertices[j].GetPosition());
			collisionVertices[i++].SetPosition(T3Matrix4::Translation(T3Vector3(-1.3,-0.5,2.6)) * tireVertices[j].GetPosition());
			collisionVertices[i++].SetPosition(T3Matrix4::Translation(T3Vector3(1.3,-0.5,-1.7)) * tireVertices[j].GetPosition());
			collisionVertices[i++].SetPosition(T3Matrix4::Translation(T3Vector3(-1.3,-0.5,-1.7)) * tireVertices[j].GetPosition());
		}
		

		return  collisionVertices;
		
	}

	virtual Vertex* GetPhysicsVertex() {return collisionVertices;}
	virtual int GetNunmberVertex() {return carMesh->GetNumVertices() + 4 * tireMesh->GetNumVertices();}

//	virtual Vertex* GetPhysicsVertex() {return carMesh->GetVertices();}
//	virtual int GetNunmberVertex() {return carMesh->GetNumVertices();}
private:
	PhysicsNode * car;
	PhysicsNode * FrontRightTire;
	PhysicsNode * FrontLeftTire;
    PhysicsNode * BackRightTire;
	PhysicsNode * BackLeftTire;
	Mesh* tireMesh;
	Mesh* carMesh;
	Vertex* collisionVertices;
	float size;



};