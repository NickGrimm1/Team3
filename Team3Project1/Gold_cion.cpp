#include"Gold_cion.h"
#include "../Team3Project1/GameStateManager.h"


Gold_cion::Gold_cion(float size):
	carPhysicNode(carPhysicNode) 
{
	mesh = GameStateManager::Assets()->LoadMesh(this, MESHDIR"sphere.obj");
	T3Vector3 position =T3Vector3(100, -2, 0);
	boundingRadius = size;
	origin = position;
	rotation = Quaternion::EulerAnglesToQuaternion(0,0,0);
	scale = T3Vector3(size,size,size);
	texture =NULL;
	bumpTexture = NULL;
	shader = NULL;
	
	


}

Gold_cion::~Gold_cion(void){
	
}



void Gold_cion::SetPhysics(float size)
{
    physicsNode = new PhysicsNode();
    physicsNode->SetUseGravity(false);
	physicsNode->SetPosition(origin);
	physicsNode->SetMass(5);

	physicsNode->SetCollisionVolume(new CollisionAABB(T3Vector3(size,size,size)));
	physicsNode->SetMesh(GameStateManager::Assets()->LoadMesh(this, MESHDIR"sphere.obj"));
	physicsNode->SetXstart(physicsNode->GetPosition().x-0.95*size);
	physicsNode->SetXend(physicsNode->GetPosition().x+0.95*size); 
	physicsNode->SetIsCollide(false);



    ConnectToSystems(); 

}