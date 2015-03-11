#include"CheckPoint.h"
#include "../Team3Project1/GameStateManager.h"
#include "../Framework/InertialMatrixHelper.h"


CheckPoint::CheckPoint(float size):
	carPhysicNode(carPhysicNode) 
{
	//mesh = NULL; // 
	mesh=GameStateManager::Assets()->LoadMesh(this, MESHDIR"cube.obj");
	T3Vector3 position =T3Vector3(50, -2, 0);
	boundingRadius = size;
	origin = position;
	rotation = Quaternion::EulerAnglesToQuaternion(0,0,0);
	scale = T3Vector3(0.15*size,size,10*size);
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
	physicsNode->SetXstart(physicsNode->GetPosition().x-1*size);
	physicsNode->SetXend(physicsNode->GetPosition().x+1*size); 
	physicsNode->SetType(type);
	physicsNode->SetCollisionVolume(new CollisionAABB(T3Vector3(0.15*size,size,10*size)));
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
	physicsNode->SetMass(5);

	physicsNode->SetMesh(GameStateManager::Assets()->LoadMesh(this, MESHDIR"cube.obj"));
	physicsNode->SetXstart(physicsNode->GetPosition().x-1*size);
	physicsNode->SetXend(physicsNode->GetPosition().x+1*size); 
	physicsNode->SetType(type);
	physicsNode->SetOrientation(rotation);
	physicsNode->SetCollisionVolume(new CollisionAABB(T3Vector3(0.15*size,size,10*size)));
	physicsNode->SetInverseInertia(InertialMatrixHelper::createImmovableInvInertial());

	physicsNode->SetIsCollide(true);
	physicsNode->Setcar_wheel(true);

    ConnectToSystems(); 

}


