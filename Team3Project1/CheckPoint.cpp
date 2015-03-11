#include"CheckPoint.h"
#include "../Team3Project1/GameStateManager.h"
#include "../Framework/InertialMatrixHelper.h"


CheckPoint::CheckPoint(float size):
	carPhysicNode(carPhysicNode) 
{
	//mesh = NULL; // 
	mesh=GameStateManager::Assets()->LoadMesh(this, MESHDIR"cube.obj");
	T3Vector3 position =T3Vector3(50.0f, -2.0f, 0.0f);
	boundingRadius = size;
	origin = position;
	rotation = Quaternion::EulerAnglesToQuaternion(0.0f,0.0f,0.0f);
	scale = T3Vector3(0.15f*size,size,10.0f*size);
	texture =NULL;
	bumpTexture = NULL;
	shader = NULL;
	


}

CheckPoint::~CheckPoint(void){
//	DisconnectFromSystems();
//
////	delete renderNode;
//	delete physicsNode;
}


void CheckPoint::SetPhysics(float size,char type)
{
    physicsNode = new PhysicsNode();
    physicsNode->SetUseGravity(false);
	physicsNode->SetPosition(origin);
	physicsNode->SetMass(5);

	physicsNode->SetMesh(GameStateManager::Assets()->LoadMesh(this, MESHDIR"cube.obj"));
	physicsNode->SetXstart(physicsNode->GetPosition().x-1.0f*size);
	physicsNode->SetXend(physicsNode->GetPosition().x+1.0f*size); 
	physicsNode->SetType(type);
	physicsNode->SetCollisionVolume(new CollisionAABB(T3Vector3(0.15f*size,size,10.0f*size)));
	physicsNode->SetInverseInertia(InertialMatrixHelper::createImmovableInvInertial());

	physicsNode->SetIsCollide(true);
	physicsNode->Setcar_wheel(true);

    ConnectToSystems(); 

}

void CheckPoint::SetPhysics(float size,char type,T3Vector3 position,const Quaternion& rotation)
{
    physicsNode = new PhysicsNode();
    physicsNode->SetUseGravity(false);
	physicsNode->SetPosition(position);
	physicsNode->SetMass(5.0f);
	physicsNode->SetOrientation(rotation);

	physicsNode->SetMesh(GameStateManager::Assets()->LoadMesh(this, MESHDIR"cube.obj"));
	physicsNode->SetXstart(physicsNode->GetPosition().x-1.0f*size);
	physicsNode->SetXend(physicsNode->GetPosition().x+1.0f*size); 
	physicsNode->SetType(type);
	
	physicsNode->SetCollisionVolume(new CollisionAABB(T3Vector3(0.15f*size,size,10.0f*size)));
	physicsNode->SetInverseInertia(InertialMatrixHelper::createImmovableInvInertial());

	physicsNode->SetIsCollide(true);
	physicsNode->Setcar_wheel(true);

    ConnectToSystems(); 

}


