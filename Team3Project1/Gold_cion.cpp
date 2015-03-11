#include"Gold_cion.h"
#include "../Team3Project1/GameStateManager.h"
#include "../Framework/InertialMatrixHelper.h"


Gold_cion::Gold_cion(float size):
	carPhysicNode(carPhysicNode) 
{
	mesh = GameStateManager::Assets()->LoadMesh(this, MESHDIR"cube.obj");
	T3Vector3 position =T3Vector3(100, -2, 0);
	boundingRadius = size;
	origin = position;
	rotation = Quaternion::EulerAnglesToQuaternion(0,0,0);
	scale = T3Vector3(0.3*size,0.3*size,0.3*size);
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
	

	physicsNode->SetCollisionVolume(new CollisionAABB(T3Vector3(0.3*size,0.3*size,0.3*size)));
	physicsNode->SetMesh(GameStateManager::Assets()->LoadMesh(this, MESHDIR"cube.obj"));
	physicsNode->SetXstart(physicsNode->GetPosition().x-1*size);
	physicsNode->SetXend(physicsNode->GetPosition().x+1*size); 
	physicsNode->SetIsCollide(true);
	physicsNode->Setcar_wheel(true);
	physicsNode->SetInverseInertia(InertialMatrixHelper::createCuboidInvInertial(5,size,size,size));
	physicsNode->SetInverseMass(1.0f);



    ConnectToSystems(); 

}

void Gold_cion::SetPhysics(float size,char type,T3Vector3 position,const Quaternion& rotation)
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
	physicsNode->SetCollisionVolume(new CollisionAABB(T3Vector3(size,size,size)));
//	physicsNode->SetCollisionVolume(new CollisionAABB(T3Vector3(0.1*size,size,10*size)));
	physicsNode->SetInverseInertia(InertialMatrixHelper::createImmovableInvInertial());

	physicsNode->SetIsCollide(true);
	physicsNode->Setcar_wheel(true);

    ConnectToSystems(); 
}