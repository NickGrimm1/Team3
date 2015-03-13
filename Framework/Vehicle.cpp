#include"Vehicle.h"
#include "../Team3Project1/GameStateManager.h"
#include "InertialMatrixHelper.h"


Vehicle::Vehicle(float size) {
	//mesh = GameStateManager::Assets()->LoadMesh(this, MESHDIR"NovaCar.obj");
	mesh = GameStateManager::Assets()->LoadMesh(this, MESHDIR"NovaCar.obj");
//	BuffMesh = new OBJMesh(MESHDIR"ico.obj");
	
	//TT =  SOIL_load_OGL_texture ("../../Texture/footballpitch.jpg", SOIL_LOAD_AUTO , SOIL_CREATE_NEW_ID , SOIL_FLAG_MIPMAPS);
//	PlayerPosition=T3Vector3(500,100,-800);

	T3Vector3 position =T3Vector3(0, 6, 0);
	boundingRadius = size;
	origin = position;
	rotation = Quaternion::EulerAnglesToQuaternion(0,-90,0);
	scale = T3Vector3(size,size,size);
	texture = NULL;
	bumpTexture = NULL;
	shader = NULL;
	vpn = new VehiclePhysicsNode(size);
	SetPhysics(size, vpn);



	FrontRightTire=new Vehicle_Wheel(size);
	FrontRightTire->SetPhysics(size, (vpn->getRFW()));
	FrontLeftTire=new Vehicle_Wheel(size);
	FrontLeftTire->SetPhysics(size, (vpn->getLFW()));
	BackRightTire=new Vehicle_Wheel(size);
	BackRightTire->SetPhysics(size,(vpn->getBRW()));
	BackLeftTire=new Vehicle_Wheel(size);
	BackLeftTire->SetPhysics(size, (vpn->getBLW()));





	maxSpeed=T3Vector3(15.0f,15.0f,15.0f);



	Speed_Player = 5;
	f=0.0f;
	temp2=T3Vector3(0.0f,0.0f,0.0f);
	Speed_Rotate = -0.0004f;

	tempPosition = T3Vector3(0.0f,350.0f,-800.0f);
}

Vehicle::~Vehicle(void){
	delete PlayerMesh;
	delete Player;
}

void	Vehicle::Update(float msec)
{
}
								 
void Vehicle::SetPhysics(float size,PhysicsNode * a)
{
    car=a;
	car->SetUseGravity(true);
	car->SetPosition(origin);
	car->SetMass(5);
	car->SetInverseMass(1.0f);
	car->SetCollisionVolume(new CollisionAABB(T3Vector3(size ,size ,size )));
	//car->SetMesh(GameStateManager::Assets()->LoadMesh(this, MESHDIR"Carphysics.obj"));
	car->SetOrientation(Quaternion::EulerAnglesToQuaternion(0.0f,-90.0f,0.0f));
	//car->SetMesh(GameStateManager::Assets()->LoadMesh(this, MESHDIR"CarPhysics.obj"));
	//Quaternion Test=car->GetOrientation();
	//car->SetOrientation(Quaternion::EulerAnglesToQuaternion(0,-90,0));
	//Quaternion Test2=car->GetOrientation();
	car->SetXstart(car->GetPosition().x-3.0f*size);
	car->SetXend(car->GetPosition().x+3.0f*size);                                                                                         
	car->SetInverseInertia(InertialMatrixHelper::createCuboidInvInertial(5.0f,size,size,size));

	car->SetIsCollide(true);
	car->SetIsDrop(false);
	car->Setcar_wheel(true);
	car->Setplanecollision(false);



    ConnectToSystems(); 


}

Quaternion Vehicle::GetRotation() const {
	return rotation; //Quaternion::FromMatrix(rotation.ToMatrix() * T3Matrix4::Rotation(90, T3Vector3(0,1,0)));
}