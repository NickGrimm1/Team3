#include "VehicleTestingScreen.h"


#include "VehicleTestingScreen.h"
#include "Mesh.h"
#include "../Framework/Light.h"
#include "GameStateManager.h"
#include "FreeCamera.h"
#include "../Framework/ObjMesh.h"
//#include "../Framework/Vehicle.h"


//TODO - remove
#include <iostream>
#include "../Framework/BumpTexture.h"

VehicleTestingScreen::VehicleTestingScreen(void)
{
}

VehicleTestingScreen::~VehicleTestingScreen(void)
{
	delete quad;
	delete light;
}

void VehicleTestingScreen::LoadContent() {
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


	gold_cion= new Gold_cion(5);
	gold_cion->SetPhysics(5);
	AddDrawable(gold_cion);


	checkpoint= new CheckPoint(10);
	checkpoint->SetPhysics(10);
	AddDrawable(checkpoint);
	
	VehiclePhysicsNode* vpn = new VehiclePhysicsNode();
	
	GameStateManager::Physics()->AddNode((PhysicsNode*)vpn);

	car = new Vehicle(5);
//	car->SetPhysics(5,(vpn->GetCar()));
	car->SetPhysics(5, vpn);
	AddDrawable(car);


	FrontRightTire = new Vehicle_Wheel(5);
	FrontRightTire->SetOriginPosition(T3Vector3(10, -2, 8));
	FrontRightTire->SetPhysics(5, (vpn->getRFW()));
	AddDrawable(FrontRightTire);


	FrontLeftTire = new Vehicle_Wheel(5);
	FrontLeftTire->SetOriginPosition(T3Vector3(10, -2, -5));
	FrontLeftTire->SetPhysics(5, (vpn->getLFW()));
	AddDrawable(FrontLeftTire);


    BackRightTire = new Vehicle_Wheel(5/*, car->GetCarNode()*/);
	BackRightTire->SetOriginPosition(T3Vector3(-11, -2, 8));
	BackRightTire->SetPhysics(5,(vpn->getBRW()));
	AddDrawable(BackRightTire);

	BackLeftTire = new Vehicle_Wheel(5/*, car->GetCarNode()*/);
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
	Speed_Rotate =  -0.4;
	tempPosition = T3Vector3(0,350,-800);
	PlayerPosition=T3Vector3(500,100,-800);

	camera->SetPosition(T3Vector3(0,10.0f, 80.0f));
	//camera->SetYaw(180.0f);
	GameStateManager::Graphics()->SetCamera(camera);
}

void VehicleTestingScreen::Update() { 

	if(car->GetX()>=15)
	{
		car->SetX(15);
		car->GetCarNode().SetForce(T3Vector3(0,0,0));
	
	}
		if(car->GetX()<=-15)
	{
		car->SetX(-15);
	
	}

	if(car->GetY()>=15)
	{
		car->SetY(15);
	
	}
	if(car->GetY()<=-15)
	{
		car->SetY(-15);
	
	}

	if(car->GetZ()>=15)
	{
		car->SetZ(15);
	
	}
	if(car->GetZ()<=-15)
	{
		car->SetZ(-15);
	
	}
	if(car->GetCarNode().GetLinearVelocity()==T3Vector3(0,0,0) && f!=0)
	{
	  f=0;
	}

	
	
	car->GetCarNode().SetXstart(car->GetCarNode().GetPosition().x-3*5);
	car->GetCarNode().SetXend(car->GetCarNode().GetPosition().x+3*5);
	checkpoint->GetPhysicsNode().SetXstart(checkpoint->GetPhysicsNode().GetPosition().x-1*10);
	checkpoint->GetPhysicsNode().SetXend(checkpoint->GetPhysicsNode().GetPosition().x+1*10);
	gold_cion->GetPhysicsNode().SetXstart(gold_cion->GetPhysicsNode().GetPosition().x-0.95*10);
	gold_cion->GetPhysicsNode().SetXend(gold_cion->GetPhysicsNode().GetPosition().x+0.95*10);

	
	
	
	//T3Matrix4 m = T3Matrix4::Rotation(0.016f, T3Vector3(0,1,0));
	//ent->AddRotation(Quaternion::FromMatrix(m));
}

void VehicleTestingScreen::KeyboardEvent(KeyboardEvents::EventType type, KeyboardEvents::Key key) {


	switch (type) {
	case KeyboardEvents::KEY_DOWN:
	case KeyboardEvents::KEY_HELD:
		switch (key) {

		case KeyboardEvents::KEYBOARD_W:
		{
				//camera->AddMovement(T3Vector3(1,0,0));

		
				
			   f=f+5;
				//  car->GetCarNode().SetForce(T3Vector3(10+f,0,0));
			   if(car->GetCarNode().GetOrientation().y==0)
		       {
				   car->GetCarNode().SetForce(T3Vector3(f /*+ 100.0f*/,0,0));   
					//		    f=f+100;
		       }
			//  cout<<car->GetCarNode().GetLinearVelocity().x<<"   "<<car->GetCarNode().GetLinearVelocity().y<<"   "<<car->GetCarNode().GetLinearVelocity().z<<"   "<<endl;

			   //cout<<car->GetCarNode().GetLinearVelocity().Length()<<endl;
				if(car->GetCarNode().GetOrientation().y<0)
				{
					float angle=-0.5/90*car->GetCarNode().GetOrientation().y;
					float l=  car->GetCarNode().GetLinearVelocity().Length();
					float x=l*sin(angle/2/3.14);
					float z=l*cos(angle/2/3.14);
				    f=f+5;
					//Player->GetPhysicsNode().SetForce(Vector3(0,0,0.3+f));  
					 car->GetCarNode().SetForce(T3Vector3(x+f,0,z+f));
					/*	tempPosition.x = PlayerPosition.x;*/
						/*tempPosition.z = PlayerPosition.z-1000;
						tempPosition.y = PlayerPosition.y+800;
					*/
		
				}

				if(car->GetCarNode().GetOrientation().y>0)
				{
					float angle=0.5/90*car->GetCarNode().GetOrientation().y;
					float l=  car->GetCarNode().GetLinearVelocity().Length();
					float x=-l*sin(angle/2/3.14);
					float z=l*cos(angle/2/3.14);
					f=f+5;
					 car->GetCarNode().SetForce(T3Vector3(z+f,0,x-f));	
				}
			
				 /*	tempPosition.z = PlayerPosition.z-1000;
					 tempPosition.y = PlayerPosition.y+800;*/

			
		}
			 break;
			
		case KeyboardEvents::KEYBOARD_S:
			{//camera->AddMovement(T3Vector3(0,0,1));
			/* f=f+100;
		      car->GetCarNode().SetForce(T3Vector3(-10-f,0,0)); */
		  /*   tempPosition.z = PlayerPosition.z-1000;
		     tempPosition.y = PlayerPosition.y+800;*/



				 f=f+5;
			//  car->GetCarNode().SetForce(T3Vector3(10+f,0,0));



				if(car->GetCarNode().GetOrientation().y==0)
		{
		  
		 car->GetCarNode().SetForce(T3Vector3(-f ,0,0));   
	

//		    f=f+100;
		}
		if(car->GetCarNode().GetOrientation().y<0)
		{
		float angle=-0.5/90*FrontRightTire->GetPhysicsNode().GetOrientation().y;

		float l=  car->GetCarNode().GetLinearVelocity().Length();

		float x=l*sin(angle/2/3.14);
		float z=l*cos(angle/2/3.14);
	    f=f+5;
		//Player->GetPhysicsNode().SetForce(Vector3(0,0,0.3+f));  
		 car->GetCarNode().SetForce(T3Vector3(-(x+f),0,-(z+f)));
		
	
	/*	tempPosition.x = PlayerPosition.x;*/
		/*tempPosition.z = PlayerPosition.z-1000;
		tempPosition.y = PlayerPosition.y+800;
*/
		
		}

		if(car->GetCarNode().GetOrientation().y>0)
		{
		float angle=0.5/90*FrontRightTire->GetPhysicsNode().GetOrientation().y;

		float l=  car->GetCarNode().GetLinearVelocity().Length();

		float x=-l*sin(angle/2/3.14);
		float z=l*cos(angle/2/3.14);
	    f=f+5;

		 car->GetCarNode().SetForce(T3Vector3(-(z+f),0,-(x-f)));
	

				
		}



			}
			break;
		case KeyboardEvents::KEYBOARD_A:
			{//camera->AddMovement(T3Vector3(1,0,0));
			/*if( car->GetCarNode().GetOrientation().z>=-0.13)
			 {
			     car->GetCarNode().SetAngularVelocity(T3Vector3(0,0,Speed_Rotate));
		     }			
		     temp1 =  car->GetCarNode().GetLinearVelocity();
		     temp1.x=0.1*Speed_Player;
		      car->GetCarNode().SetLinearVelocity(temp1);
		     tempPosition.x = PlayerPosition.x;
			*/
		    //	f=f+100;

			if( car->GetCarNode().GetOrientation().y<=0.6){

				// car->GetCarNode().SetOrientation( car->GetCarNode().GetOrientation()+Quaternion::FromMatrix(T3Matrix4::Rotation(30,T3Vector3(0,-1,0))));

			 FrontLeftTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,-Speed_Rotate-0.001,0));
			/*if( FrontLeftTire->GetPhysicsNode().GetOrientation().y>=0.2||FrontRightTire->GetPhysicsNode().GetOrientation().y>=0.2)
			{
			FrontLeftTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,0,0));
			FrontRightTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,0,0));
			}*/
		     FrontRightTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,-Speed_Rotate-0.001,0));
			 BackLeftTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,-Speed_Rotate,0));
		     BackRightTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,-Speed_Rotate,0));
			  car->GetCarNode().SetAngularVelocity(T3Vector3(0,-Speed_Rotate,0));

		
			// cout<< car->GetCarNode().GetOrientation().y<<endl;
			/* float angle=-0.5/90*FrontRightTire->GetPhysicsNode().GetOrientation().y;

		    float l=  car->GetCarNode().GetLinearVelocity().Length();

	    	float x=l*sin(angle/2/3.14);
	   float z=l*cos(angle/2/3.14);
	       f=f+100;*/
		////Player->GetPhysicsNode().SetForce(Vector3(0,0,0.3+f));  
		// car->GetCarNode().SetForce(T3Vector3(x+f,0,z+f));
		//	 cout<< car->GetCarNode().GetOrientation().y<<endl;
			 
		
		
			 	
		
		/*float angle=-0.5/90*FrontRightTire->GetPhysicsNode().GetOrientation().y;
		float l=  car->GetCarNode().GetLinearVelocity().Length();

		float x=l*sin(angle/2/3.14);
		float z=l*cos(angle/2/3.14);
	    f=f+5;

		 car->GetCarNode().SetForce(T3Vector3(x+f,0,-(z+f)));*/
		}		
		
			
			}
			break;
		case KeyboardEvents::KEYBOARD_D:
			{//camera->AddMovement(T3Vector3(-1,0,0));
			/*  if( car->GetCarNode().GetOrientation().z<=0.13)
			  {
			     car->GetCarNode().SetAngularVelocity(T3Vector3(0,0,-Speed_Rotate));
		      }
		      temp1 =  car->GetCarNode().GetLinearVelocity();
		      temp1.x=-0.1*Speed_Player;
		       car->GetCarNode().SetLinearVelocity(temp1);
		      tempPosition.x = PlayerPosition.x;*/


				if( car->GetCarNode().GetOrientation().y>=-0.6){
		
			 FrontLeftTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,Speed_Rotate-0.001,0));
		     FrontRightTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,Speed_Rotate-0.001,0));
			 BackLeftTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,Speed_Rotate,0));
		     BackRightTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(0,Speed_Rotate,0));
			  car->GetCarNode().SetAngularVelocity(T3Vector3(0,Speed_Rotate,0));
			// cout<< car->GetCarNode().GetOrientation().y<<endl;
		}			
				/*float angle=0.5/90*FrontRightTire->GetPhysicsNode().GetOrientation().y;

		       float l=  car->GetCarNode().GetLinearVelocity().Length();

		     float x=-l*sin(angle/2/3.14);
		float z=l*cos(angle/2/3.14);
	    f=f+5;

		 car->GetCarNode().SetForce(T3Vector3(z+f,0,-(x-f)));*/
	


			}
			break;
		case KeyboardEvents::KEYBOARD_SHIFT:
			camera->AddMovement(T3Vector3(0,1,0));
			break;
		case KeyboardEvents::KEYBOARD_SPACE:
			{camera->AddMovement(T3Vector3(0,-1,0));
			 temp1 =  car->GetCarNode().GetLinearVelocity();
			  car->GetCarNode().SetUseGravity(TRUE);
			 temp1.y = 0.4;
			  car->GetCarNode().SetLinearVelocity(temp1);	
			
			}
			break;
		}
		
	}
}
	
void VehicleTestingScreen::MouseMoved(T3Vector2& finish) {
	camera->AddPitch(-finish.y);
	camera->AddYaw(finish.x);
}