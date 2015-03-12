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


//#define LINEAR_VELOCITY_DAMP 0.92
//#define ANGULAR_VELOCITY_DAMP 0.7
//#define LINEAR_VELOCITY_MIN 0.00001

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
		tireMesh=GameStateManager::Assets()->LoadMesh(this, MESHDIR"NovaTire.obj");
		collisionVertices = new Vertex[carMesh->GetNumVertices() + 4 * tireMesh->GetNumVertices()];
	}

	~VehiclePhysicsNode(void){}

	virtual void		Update(float msec) {
		
		/*FrontRightTire->SetPosition(car->BuildTransform()*T3Vector3(10,-2,8));
		FrontLeftTire->SetPosition(T3Vector3(car->GetPosition().x+10,car->GetPosition().y-2,car->GetPosition().z-5));
		BackRightTire->SetPosition(T3Vector3(car->GetPosition().x-11,car->GetPosition().y-2,car->GetPosition().z+8));
		BackLeftTire->SetPosition(T3Vector3(car->GetPosition().x-11,car->GetPosition().y-2,car->GetPosition().z-5));*/
		
		
		
	    PhysicsNode:: Update( msec);


		FrontRightTire->SetPosition(BuildTransform()*T3Vector3(size * 1.3f,size * -0.5f,  size * 2.6f));
		FrontLeftTire->SetPosition(BuildTransform()*T3Vector3(size *- 1.3f,size * -0.5f,size * 2.6f));
		BackRightTire->SetPosition(BuildTransform()*T3Vector3(size * 1.3f,size * -0.5f,size * -1.7f));
		BackLeftTire->SetPosition(BuildTransform()*T3Vector3(size * -1.3f,size * -0.5f,size * -1.7f));
		//FrontRightTire->SetPosition(/*Transform(temp)*/BuildTransform()*T3Vector3(size * 1.7,size * -0.5,  size * 1.3));
		//FrontLeftTire->SetPosition(BuildTransform()*T3Vector3(size *1.7,size * -0.5,size * -1.3));
		//BackRightTire->SetPosition(BuildTransform()*T3Vector3(size *-2.6,size * -0.5,size * 1.3));
		//BackLeftTire->SetPosition(BuildTransform()*T3Vector3(size * -2.6,size * -0.5,size * -1.3));
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
			collisionVertices[i++].SetPosition(T3Matrix4::Translation(T3Vector3(1.3f,-0.5f,2.6f)) * tireVertices[j].GetPosition());
			collisionVertices[i++].SetPosition(T3Matrix4::Translation(T3Vector3(-1.3f,-0.5f,2.6f)) * tireVertices[j].GetPosition());
			collisionVertices[i++].SetPosition(T3Matrix4::Translation(T3Vector3(1.3f,-0.5f,-1.7f)) * tireVertices[j].GetPosition());
			collisionVertices[i++].SetPosition(T3Matrix4::Translation(T3Vector3(-1.3f,-0.5f,-1.7f)) * tireVertices[j].GetPosition());
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