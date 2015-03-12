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
	  //  car = new VehiclePhysicsNode(size);
		FrontLeftTire = new PhysicsNode();
		FrontRightTire = new PhysicsNode();
		BackLeftTire = new PhysicsNode();
		BackRightTire = new PhysicsNode();
		carMesh=GameStateManager::Assets()->LoadMesh(this, MESHDIR"CarPhysics.obj");
		tireMesh=GameStateManager::Assets()->LoadMesh(this, MESHDIR"NovaTire.obj");
		collisionVertices = new Vertex[carMesh->GetNumVertices() + 4 * tireMesh->GetNumVertices()];
		f=0;
	}

	~VehiclePhysicsNode(void){}

	virtual void		Update(float msec) {
		

	    PhysicsNode:: Update( msec);

		FrontRightTire->SetPosition(BuildTransform()*T3Vector3(size * 1.3f,size * -0.5f,  size * 2.6f));
		FrontLeftTire->SetPosition(BuildTransform()*T3Vector3(size *- 1.3f,size * -0.5f,size * 2.6f));
		BackRightTire->SetPosition(BuildTransform()*T3Vector3(size * 1.3f,size * -0.5f,size * -1.7f));
		BackLeftTire->SetPosition(BuildTransform()*T3Vector3(size * -1.3f,size * -0.5f,size * -1.7f));
	
		UpdatePhysicsMesh();


		if(this->GetLinearVelocity()==T3Vector3(0,0,0) && f!=0)
	{
	  f=0;
	}


		if(this->GetLinearVelocity().x>0)
	{
		FrontRightTire->SetAngularVelocity(T3Vector3(-this->GetLinearVelocity().Length()*0.5f,0,0) );
		FrontLeftTire->SetAngularVelocity(T3Vector3(-this->GetLinearVelocity().Length()*0.5f,0,0));
		BackRightTire->SetAngularVelocity(T3Vector3(-this->GetLinearVelocity().Length()*0.5f,0,0));
		BackLeftTire->SetAngularVelocity(T3Vector3(-this->GetLinearVelocity().Length()*0.5f,0,0));
	}
	if(this->GetLinearVelocity().x<0)
	{
	    FrontRightTire->SetAngularVelocity(T3Vector3(this->GetLinearVelocity().Length()*0.5f,0,0));
		FrontLeftTire->SetAngularVelocity(T3Vector3(this->GetLinearVelocity().Length()*0.5f,0,0));
		BackRightTire->SetAngularVelocity(T3Vector3(this->GetLinearVelocity().Length()*0.5f,0,0));
		BackLeftTire->SetAngularVelocity(T3Vector3(this->GetLinearVelocity().Length()*0.5f,0,0));
	
	}


	};
	

	void SetF(float F) {f=F;}
	float GetF()  {return f;}

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

	PhysicsNode * FrontRightTire;
	PhysicsNode * FrontLeftTire;
    PhysicsNode * BackRightTire;
	PhysicsNode * BackLeftTire;
	Mesh* tireMesh;
	Mesh* carMesh;
	Vertex* collisionVertices;
	float size;
	float f;



};