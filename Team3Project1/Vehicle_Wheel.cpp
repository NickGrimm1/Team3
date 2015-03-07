#include"Vehicle_Wheel.h"
#include "../Team3Project1/GameStateManager.h"
#include "../Framework/InertialMatrixHelper.h"


Vehicle_Wheel::Vehicle_Wheel(float size/*,PhysicsNode * carPhysicNode*/):
	carPhysicNode(carPhysicNode) 
{
	mesh = GameStateManager::Assets()->LoadMesh(this, MESHDIR"Nova Tire.obj");
	T3Vector3 position =T3Vector3(-11, -2, 7);
	boundingRadius = size;
	origin = position;
	rotation = Quaternion::EulerAnglesToQuaternion(0,0,0);
	scale = T3Vector3(size,size,size);
	texture = NULL;
	bumpTexture = NULL;
	shader = NULL;
	
	


}

Vehicle_Wheel::~Vehicle_Wheel(void){
	
}


void Vehicle_Wheel::AddMove_W()
{

	 


}

void Vehicle_Wheel::SetPhysics(float size)
{
    physicsNode = new PhysicsNode();
    physicsNode->SetUseGravity(true);
	physicsNode->SetPosition(origin);
	physicsNode->SetMass(5);
	physicsNode->SetCollisionVolume(new CollisionAABB(T3Vector3(size,size,size)));

	



    ConnectToSystems(); 

}

void Vehicle_Wheel::SetPhysics(float size,PhysicsNode * a)
{
    physicsNode=a;
	physicsNode->SetUseGravity(true);
	physicsNode->SetPosition(origin);
	physicsNode->SetMass(5);
	physicsNode->SetInverseMass(1.0f);
	physicsNode->SetInverseInertia(InertialMatrixHelper::createCuboidInvInertial(5.0f,size,size,size));
	physicsNode->SetCollisionVolume(new CollisionAABB(T3Vector3(size,size,size)));
	physicsNode->SetMesh(GameStateManager::Assets()->LoadMesh(this, MESHDIR"Nova Tire.obj"));
//	physicsNode->SetMesh(NULL);
	physicsNode->SetIsCollide(true);
	physicsNode->Setcar_wheel(false);



    ConnectToSystems(); 


}





























//void Vehicle_Wheel::UpdatePlayer1(float msec){
////void Vehicle::UpdatePlayer1(float msec,float forward, float sideways) {
//	PlayerPosition = Player->GetPhysicsNode().GetPosition();
//
//	if(Player->GetPhysicsNode().GetOrientation().z<-0.1||Player->GetPhysicsNode().GetOrientation().z>0.1){
//		Player->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,0,0));
//	}
//
//	if(Player->GetPhysicsNode().GetPosition().y<=100){
//		temp1 = Player->GetPhysicsNode().GetLinearVelocity();
//		temp1.y = 0;
//		Player->GetPhysicsNode().SetLinearVelocity(temp1);
//		Player->GetPhysicsNode().SetUseGravity(FALSE);
//	}
//
//	
//
//	tempPosition.z = PlayerPosition.z-1000;
//	tempPosition.y = PlayerPosition.y+800;
//	
//
//	
//	
//
//}