#include"CheckPoint.h"
#include "../Team3Project1/GameStateManager.h"


CheckPoint::CheckPoint(float size):
	carPhysicNode(carPhysicNode) 
{
	mesh = GameStateManager::Assets()->LoadMesh(this, MESHDIR"cube.obj");
	T3Vector3 position =T3Vector3(50, -2, 0);
	boundingRadius = size;
	origin = position;
	rotation = Quaternion::EulerAnglesToQuaternion(0,0,0);
	scale = T3Vector3(size,size,size);
	texture =NULL;
	bumpTexture = NULL;
	shader = NULL;
	
	


}

CheckPoint::~CheckPoint(void){
	
}



void CheckPoint::SetPhysics(float size)
{
    physicsNode = new PhysicsNode();
    physicsNode->SetUseGravity(false);
	physicsNode->SetPosition(origin);
	physicsNode->SetMass(5);

	physicsNode->SetCollisionVolume(new CollisionAABB(T3Vector3(size,size,size)));
	physicsNode->SetMesh(GameStateManager::Assets()->LoadMesh(this, MESHDIR"cube.obj"));
	physicsNode->SetIsCollide(false);



    ConnectToSystems(); 

}


