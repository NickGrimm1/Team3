#include "RacerGame.h"


#include "RacerGame.h"
#include "Mesh.h"
#include "../Framework/Light.h"
#include "GameStateManager.h"
#include "FreeCamera.h"
#include "../Framework/ObjMesh.h"
//#include "../Framework/Vehicle.h"


//TODO - remove
#include <iostream>
#include "../Framework/BumpTexture.h"

RacerGame::RacerGame(void)
{
}

RacerGame::~RacerGame(void)
{
	delete quad;
	delete light;
}

void RacerGame::LoadContent() {
	//Mesh* coneMesh = Mesh::GenerateCone(20);
	quad = GameStateManager::Assets()->LoadCylinder(this, 20);
	cylinder = GameStateManager::Assets()->LoadCylinder(this, 20);
	//cout << "Quad Obj = " << quad->GetVertexBuffer() << endl;
	
	ent = new DrawableEntity3D(
		quad, 
		NULL,
		GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"Grass_Color.tga", 0), 
		GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"snowflake.png", 0),
		50.0f, 
		T3Vector3(0,0,0), 
		Quaternion::FromMatrix(T3Matrix4::Rotation(90.0f, T3Vector3(1,0,0))),
		T3Vector3(50,50,1));
	



	
	//cout << "Cone Mesh Obj = " << coneMesh->GetVertexBuffer() << endl;
	//SpotLight::SetConeMesh(coneMesh);
	//Mesh* circleMesh = Mesh::GenerateCircle(20);
	//cout << "Circle Mesh Obj = " << circleMesh->GetVertexBuffer() << endl;
	//SpotLight::SetCircleMesh(Mesh::GenerateCircle(20));
	//Mesh* sphereMesh = new OBJMesh(MESHDIR"sphere.obj");
	//PointLight::SetMesh(sphereMesh);

	
	//light = GameStateManager::Graphics()->AddSpotLight(T3Vector3(0, 0, 0), T3Vector3(0,1,0), T3Vector3(1,0,0), 1.0f, 45.0f, T3Vector4(1,1,1,1), T3Vector4(1,1,1,1), false);
	
	//PointLight* l = GameStateManager::Graphics()->AddPointLight(T3Vector3(0,5,0), 10, T3Vector4(1,1,1,1), T3Vector4(1,1,1,1), false); 

	camera = new FreeCamera();
	checkpoint= new CheckPoint(10);
	checkpoint->SetPhysics(10);
	AddDrawable(checkpoint);
	
	VehiclePhysicsNode* vpn = new VehiclePhysicsNode();
	
	GameStateManager::Physics()->AddNode(vpn);

	car = new Vehicle(5);
	car->SetPhysics(5,(vpn->GetCar()));
	AddDrawable(car);


	FrontRightTire = new Vehicle_Wheel(5);
	FrontRightTire->SetOriginPosition(T3Vector3(10, -2, 8));
	FrontRightTire->SetPhysics(5, (vpn->getRFW()));
	AddDrawable(FrontRightTire);


	FrontLeftTire = new Vehicle_Wheel(5);
	FrontLeftTire->SetOriginPosition(T3Vector3(10, -2, -5));
	FrontLeftTire->SetPhysics(5, (vpn->getLFW()));
	AddDrawable(FrontLeftTire);


    BackRightTire = new Vehicle_Wheel(5/*,car->GetPhysicsNode()*/);
	BackRightTire->SetOriginPosition(T3Vector3(-11, -2, 8));
	BackRightTire->SetPhysics(5,(vpn->getBRW()));
	AddDrawable(BackRightTire);

	BackLeftTire = new Vehicle_Wheel(5/*,car->GetPhysicsNode()*/);
	BackLeftTire->SetOriginPosition(T3Vector3(-11, -2, -5));
	BackLeftTire->SetPhysics(5, (vpn->getBLW()));
	AddDrawable(BackLeftTire);


/*
	Mesh* carMesh = GameStateManager::Assets()->LoadMesh(this, MESHDIR"Nova Car.obj");
	ent = new DrawableEntity3D(
		GameStateManager::Assets()->LoadMesh(this, MESHDIR"Nova Car.obj"),
		NULL,
		NULL,
		NULL,
		25.0f,
		T3Vector3(-25, 20, 0),
		Quaternion::EulerAnglesToQuaternion(0,0,0),
		T3Vector3(5,5,5));
	//gameEntities.push_back(ent);
	AddDrawable(ent);
	*/


	Speed_Player = 2;
	f=0;
	temp2=T3Vector3(0,0,0);
	Speed_Rotate = -0.004;
	tempPosition = T3Vector3(0,350,-800);
	PlayerPosition=T3Vector3(500,100,-800);

	camera->SetPosition(T3Vector3(0,10.0f, 80.0f));
	//camera->SetYaw(180.0f);
	GameStateManager::Graphics()->SetCamera(camera);
}

void RacerGame::Update() { 
	//T3Matrix4 m = T3Matrix4::Rotation(0.016f, T3Vector3(0,1,0));
	//ent->AddRotation(Quaternion::FromMatrix(m));
}

void RacerGame::KeyboardEvent(KeyboardEvents::EventType type, KeyboardEvents::Key key) {


	PlayerPosition = car->GetPhysicsNode().GetPosition();


	switch (type) {
	case KeyboardEvents::KEY_DOWN:
	case KeyboardEvents::KEY_HELD:
		switch (key) {

		case KeyboardEvents::KEYBOARD_W:
			{
				//camera->AddMovement(T3Vector3(1,0,0));
			 f=f+1;
			// car->GetPhysicsNode().SetForce(T3Vector3(10+f,0,0));



				if(FrontRightTire->GetPhysicsNode().GetOrientation().y==0)
		{
		  
		car->GetPhysicsNode().SetForce(T3Vector3(f /*+ 100.0f*/,0,0));   
	

//		    f=f+100;
		}
		if(FrontRightTire->GetPhysicsNode().GetOrientation().y<0)
		{
		float angle=-0.5/90*FrontRightTire->GetPhysicsNode().GetOrientation().y;

		float l= FrontRightTire->GetPhysicsNode().GetLinearVelocity().Length();

		float x=l*sin(angle/2/3.14);
		float z=l*cos(angle/2/3.14);
	    f=f+100;
		//Player->GetPhysicsNode().SetForce(Vector3(0,0,0.3+f));  
		car->GetPhysicsNode().SetForce(T3Vector3(x+f,0,z+f));
		
	
	/*	tempPosition.x = PlayerPosition.x;*/
		/*tempPosition.z = PlayerPosition.z-1000;
		tempPosition.y = PlayerPosition.y+800;
*/
		
		}

		if(FrontRightTire->GetPhysicsNode().GetOrientation().y>0)
		{
		float angle=0.5/90*FrontRightTire->GetPhysicsNode().GetOrientation().y;

		float l= car->GetPhysicsNode().GetLinearVelocity().Length();

		float x=-l*sin(angle/2/3.14);
		float z=l*cos(angle/2/3.14);
	    f=f+100;

		car->GetPhysicsNode().SetForce(T3Vector3(z+f,0,x-f));
	

				
		}
			
		/*	tempPosition.z = PlayerPosition.z-1000;
		     tempPosition.y = PlayerPosition.y+800;*/


			}
			 break;
			
		case KeyboardEvents::KEYBOARD_S:
			{//camera->AddMovement(T3Vector3(0,0,1));
			 f=f+100;
		     car->GetPhysicsNode().SetForce(T3Vector3(-10-f,0,0)); 
		  /*   tempPosition.z = PlayerPosition.z-1000;
		     tempPosition.y = PlayerPosition.y+800;*/



			}
			break;
		case KeyboardEvents::KEYBOARD_A:
			{//camera->AddMovement(T3Vector3(1,0,0));
			/*if(car->GetPhysicsNode().GetOrientation().z>=-0.13)
			 {
			    car->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,0,Speed_Rotate));
		     }			
		     temp1 = car->GetPhysicsNode().GetLinearVelocity();
		     temp1.x=0.1*Speed_Player;
		     car->GetPhysicsNode().SetLinearVelocity(temp1);
		     tempPosition.x = PlayerPosition.x;
			*/
		    //	f=f+100;

			if(car->GetPhysicsNode().GetOrientation().y<=0.8){
			 FrontLeftTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,-Speed_Rotate+0.002,0));
			if( FrontLeftTire->GetPhysicsNode().GetOrientation().y>=0.2||FrontRightTire->GetPhysicsNode().GetOrientation().y>=0.2)
			{
			FrontLeftTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,0,0));
			FrontRightTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,0,0));
			}
		     FrontRightTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,-Speed_Rotate+0.002,0));
			 BackLeftTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,-Speed_Rotate,0));
		     BackRightTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,-Speed_Rotate,0));
			 car->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,-Speed_Rotate,0));
			 cout<<car->GetPhysicsNode().GetOrientation().y<<endl;
		 
		//	 	
		//
		//float angle=-0.5/90*FrontRightTire->GetPhysicsNode().GetOrientation().z;

		//float l= car->GetPhysicsNode().GetLinearVelocity().Length();

		//float x=l*sin(angle/2/3.14);
		//float z=l*cos(angle/2/3.14);
	 //   f=f+100;
		//Player->GetPhysicsNode().SetForce(Vector3(0,0,0.3+f));  
	//	car->GetPhysicsNode().SetForce(T3Vector3(x+f,0,z+f));
		}		
		
			
			}
			break;
		case KeyboardEvents::KEYBOARD_D:
			{//camera->AddMovement(T3Vector3(-1,0,0));
			/*  if(car->GetPhysicsNode().GetOrientation().z<=0.13)
			  {
			    car->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,0,-Speed_Rotate));
		      }
		      temp1 = car->GetPhysicsNode().GetLinearVelocity();
		      temp1.x=-0.1*Speed_Player;
		      car->GetPhysicsNode().SetLinearVelocity(temp1);
		      tempPosition.x = PlayerPosition.x;*/


				if(FrontRightTire->GetPhysicsNode().GetOrientation().y>=-0.8){
		
			 FrontLeftTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,Speed_Rotate-0.002,0));
		     FrontRightTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,Speed_Rotate-0.002,0));
			 BackLeftTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,Speed_Rotate,0));
		     BackRightTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,Speed_Rotate,0));
			 car->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,Speed_Rotate,0));
		}			


			}
			break;
		case KeyboardEvents::KEYBOARD_SHIFT:
			camera->AddMovement(T3Vector3(0,1,0));
			break;
		case KeyboardEvents::KEYBOARD_SPACE:
			{camera->AddMovement(T3Vector3(0,-1,0));
			 temp1 = car->GetPhysicsNode().GetLinearVelocity();
			 car->GetPhysicsNode().SetUseGravity(TRUE);
			 temp1.y = 0.4;
			 car->GetPhysicsNode().SetLinearVelocity(temp1);	
			
			}
			break;
		}
		
	}
}
	
void RacerGame::MouseMoved(T3Vector2& star, T3Vector2& finish) {
	camera->AddPitch(-finish.y);
	camera->AddYaw(finish.x);
}