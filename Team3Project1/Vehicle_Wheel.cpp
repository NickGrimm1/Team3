#include"Vehicle_Wheel.h"
#include "../Team3Project1/GameStateManager.h"
#include "../Framework/InertialMatrixHelper.h"


Vehicle_Wheel::Vehicle_Wheel(float size/*,PhysicsNode * carPhysicNode*/):
	carPhysicNode(carPhysicNode) 
{
	mesh = GameStateManager::Assets()->LoadMesh(this, MESHDIR"NovaTire.obj");
	T3Vector3 position =T3Vector3(-11, -2, 7);
	boundingRadius = size;
	origin = position;
	rotation = Quaternion::EulerAnglesToQuaternion(0,-90,0);
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
    physicsNode->SetUseGravity(false);
	physicsNode->SetPosition(origin);
	physicsNode->SetMass(5);
	physicsNode->SetCollisionVolume(new CollisionAABB(T3Vector3(size,size,size)));

	



    ConnectToSystems(); 

}

void Vehicle_Wheel::SetPhysics(float size,PhysicsNode * a)
{
    physicsNode=a;
	physicsNode->SetUseGravity(false);
	physicsNode->SetPosition(origin);
	physicsNode->SetMass(5);
	physicsNode->SetInverseMass(1.0f);
	physicsNode->SetInverseInertia(InertialMatrixHelper::createCuboidInvInertial(5.0f,size,size,size));
	physicsNode->SetCollisionVolume(new CollisionAABB(T3Vector3(size,size,size)));
	//physicsNode->SetMesh(GameStateManager::Assets()->LoadMesh(this, MESHDIR"Nova Tire.obj"));
	physicsNode->SetOrientation(Quaternion::EulerAnglesToQuaternion(0,-90,0));
	physicsNode->SetIsCollide(true);
	physicsNode->Setcar_wheel(false);
	physicsNode->Setplanecollision(false);



    ConnectToSystems(); 


}

Quaternion Vehicle_Wheel::GetRotation() const {
	return rotation; ///Quaternion::FromMatrix(T3Matrix4::Rotation(90, T3Vector3(0,-1,0)) * rotation.ToMatrix());
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