#include"Gold_cion.h"
#include "../Team3Project1/GameStateManager.h"
#include "../Framework/InertialMatrixHelper.h"


Gold_cion::Gold_cion(float size):
	carPhysicNode(carPhysicNode) 
{
	mesh = GameStateManager::Assets()->LoadMesh(this, MESHDIR"cube.obj");
	T3Vector3 position =T3Vector3(100.0f, -2.0f, 0.0f);
	boundingRadius = size;
	origin = position;
	rotation = Quaternion::EulerAnglesToQuaternion(0,0,0);
	scale = T3Vector3(0.3f*size,0.3f*size,0.3f*size);
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
    physicsNode->SetInverseMass(0);
	

	physicsNode->SetCollisionVolume(new CollisionAABB(T3Vector3(0.3f*size,0.3f*size,0.3f*size)));
	physicsNode->SetMesh(GameStateManager::Assets()->LoadMesh(this, MESHDIR"cube.obj"));
	physicsNode->SetXstart(physicsNode->GetPosition().x-1.0f*size);
	physicsNode->SetXend(physicsNode->GetPosition().x+1.0f*size); 
	physicsNode->SetIsCollide(true);
	physicsNode->Setcar_wheel(true);
	physicsNode->SetInverseInertia(InertialMatrixHelper::createCuboidInvInertial(5.0f,size,size,size));



    ConnectToSystems(); 

}

void Gold_cion::SetPhysics(float size,char type,T3Vector3 position,const Quaternion& rotation)
{
	 physicsNode = new PhysicsNode();
    physicsNode->SetUseGravity(false);
	physicsNode->SetPosition(position);
	physicsNode->SetInverseMass(0);

	physicsNode->SetMesh(GameStateManager::Assets()->LoadMesh(this, MESHDIR"cube.obj"));
	physicsNode->SetXstart(physicsNode->GetPosition().x-1.0f*size);
	physicsNode->SetXend(physicsNode->GetPosition().x+1.0f*size); 
	physicsNode->SetType(type);
	physicsNode->SetOrientation(rotation);
	physicsNode->SetCollisionVolume(new CollisionAABB(T3Vector3(size,size,size)));
//	physicsNode->SetCollisionVolume(new CollisionAABB(T3Vector3(0.1*size,size,10*size)));
	physicsNode->SetInverseInertia(InertialMatrixHelper::createImmovableInvInertial());

	physicsNode->SetIsCollide(true);
	physicsNode->Setcar_wheel(true);

    ConnectToSystems(); 
}