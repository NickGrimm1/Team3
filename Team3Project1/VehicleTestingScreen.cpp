


#include "VehicleTestingScreen.h"
#include "Mesh.h"
#include "../Framework/Light.h"
#include "GameStateManager.h"
#include "FreeCamera.h"
#include "ChaseCamera.h"
#include "../Framework/ObjMesh.h"

//#include "../Framework/Vehicle.h"
#include "TrackSegment.h"
#include "../Framework/InertialMatrixHelper.h"


//TODO - remove
#include <iostream>
#include "../Framework/BumpTexture.h"

VehicleTestingScreen::VehicleTestingScreen(void)
{
}

VehicleTestingScreen::~VehicleTestingScreen(void)
{
}

void VehicleTestingScreen::LoadContent() {
	//Mesh* coneMesh = Mesh::GenerateCone(20);
	quad = GameStateManager::Assets()->LoadCylinder(this, 20);
	cylinder = GameStateManager::Assets()->LoadCylinder(this, 20);
	//cout << "Quad Obj = " << quad->GetVertexBuffer() << endl;
	
	ent = new DrawableEntity3D(
		quad, 
		NULL,
		GameStateManager::Assets()->LoadTexture(this, "Grass_Color", 0), 
		GameStateManager::Assets()->LoadTexture(this, "snowflake", 0),
		50.0f, 
		T3Vector3(0,-20,0), 
		Quaternion::FromMatrix(T3Matrix4::Rotation(90.0f, T3Vector3(1,0,0))),
		T3Vector3(50,50,1));
	//AddDrawable(ent);
	

	T3Vector3 sp1= T3Vector3(100,0,0);
	T3Vector3 sp2= T3Vector3(200,0,0);
	T3Vector3 sp3= T3Vector3(300,0,0);
	SplinePoint.push_back(sp1);
	SplinePoint.push_back(sp2);
	SplinePoint.push_back(sp3);
 Texture* grassTex2 = GameStateManager::Assets()->LoadTexture(this, "water", 0);
	GameStateManager::Graphics()->GetRenderContext();
	TrackSegment* trackr = new TrackSegment(SplinePoint[0],SplinePoint[1],SplinePoint[2], 5, 50.0);
	//push back track
	TrackSegmentVector.push_back(trackr);
	GameStateManager::Graphics()->DropRenderContext();
	PhysicsNode* proad = new PhysicsNode();
	GameEntity* road= new GameEntity(proad);
		road->SetMesh(trackr);
		road->SetShader(NULL);
		road->SetTexture(grassTex2);
		road->SetBumpTexture(NULL);
		road->SetBoundingRadius(800.0f);
		road->SetOriginPosition(T3Vector3(-200,-20,0));
		road->SetRotation(Quaternion::EulerAnglesToQuaternion(0,0,0));
		road->SetScale(T3Vector3(1,1,1));
		road->GetPhysicsNode().SetMass(5);
		road->GetPhysicsNode().SetMesh(trackr);
		road->GetPhysicsNode().SetInverseMass(0);
		road->GetPhysicsNode().SetUseGravity(false);
		road->GetPhysicsNode().SetIsCollide(true);
	    road->GetPhysicsNode().Setcar_wheel(true);
		road->GetPhysicsNode().Setplanecollision(true);
		road->GetPhysicsNode().SetPosition(T3Vector3(-200,-20,0));
		road->GetPhysicsNode().SetInverseInertia(InertialMatrixHelper::createImmovableInvInertial());
		road->ConnectToSystems();
		AddDrawable(road);





	
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


	//checkpoint= new CheckPoint(10);
	//checkpoint->SetPhysics(10,'c');
	//AddDrawable(checkpoint);
	
	unsigned int size = 5;
	VehiclePhysicsNode* vpn = new VehiclePhysicsNode(size);
	
	//GameStateManager::Physics()->AddNode((PhysicsNode*)vpn);

	
	car = new Vehicle(size);
	
	//ChaseCamera* chasecamera= new ChaseCamera(car,T3Vector3(0,2,20),0,0,0);

//	car->SetPhysics(5,(vpn->GetCar()));
	car->SetPhysics(size, vpn);
	AddDrawable(car);


	FrontRightTire = new Vehicle_Wheel(size);
	//FrontRightTire->SetOriginPosition(T3Vector3(2.5, -0.5, 1.5));
	FrontRightTire->SetPhysics(size, (vpn->getRFW()));
	AddDrawable(FrontRightTire);


	FrontLeftTire = new Vehicle_Wheel(size);
	//FrontLeftTire->SetOriginPosition(T3Vector3(10, -2, -5));
	FrontLeftTire->SetPhysics(size, (vpn->getLFW()));
	AddDrawable(FrontLeftTire);


    BackRightTire = new Vehicle_Wheel(size/*, car->GetCarNode()*/);
	//BackRightTire->SetOriginPosition(T3Vector3(-11, -2, 8));
	BackRightTire->SetPhysics(size,(vpn->getBRW()));
	AddDrawable(BackRightTire);

	BackLeftTire = new Vehicle_Wheel(size/*, car->GetCarNode()*/);
	//BackLeftTire->SetOriginPosition(T3Vector3(-11, -2, -5));
	BackLeftTire->SetPhysics(size, (vpn->getBLW()));
	AddDrawable(BackLeftTire);


/*
	Mesh* carMesh = GameStateManager::Assets()->LoadMesh(this, MESHDIR"NovaCar.obj");
	ent = new DrawableEntity3D(
		GameStateManager::Assets()->LoadMesh(this, MESHDIR"NovaCar.obj"),
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


	
	f=0;
	
	Speed_Rotate =  -0.4;
	

	camera->SetPosition(T3Vector3(0,10.0f, 80.0f));
	//camera->SetYaw(180.0f);
	GameStateManager::Graphics()->SetCamera(camera);
#if WINDOWS_BUILD
	GameStateManager::Audio()->SetListener(car);
#endif
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

	
	
	car->GetCarNode().SetXstart(car->GetCarNode().GetPosition().x-3*5 +10);
	car->GetCarNode().SetXend(car->GetCarNode().GetPosition().x+3*5+10);
//	checkpoint->GetPhysicsNode().SetXstart(checkpoint->GetPhysicsNode().GetPosition().x-1*10);
//	checkpoint->GetPhysicsNode().SetXend(checkpoint->GetPhysicsNode().GetPosition().x+1*10);
	gold_cion->GetPhysicsNode().SetXstart(gold_cion->GetPhysicsNode().GetPosition().x-1*10+10);
	gold_cion->GetPhysicsNode().SetXend(gold_cion->GetPhysicsNode().GetPosition().x+1*10+10);
	FrontRightTire->GetPhysicsNode().SetXstart(FrontRightTire->GetPhysicsNode().GetPosition().x-1*5);
	FrontRightTire->GetPhysicsNode().SetXend(FrontRightTire->GetPhysicsNode().GetPosition().x+1*5);

	FrontLeftTire->GetPhysicsNode().SetXstart(FrontLeftTire->GetPhysicsNode().GetPosition().x-1*5);
	FrontLeftTire->GetPhysicsNode().SetXend(FrontLeftTire->GetPhysicsNode().GetPosition().x+1*5);

	BackRightTire->GetPhysicsNode().SetXstart(BackRightTire->GetPhysicsNode().GetPosition().x-1*5);
	BackRightTire->GetPhysicsNode().SetXend(BackRightTire->GetPhysicsNode().GetPosition().x+1*5);
	BackLeftTire->GetPhysicsNode().SetXstart(BackLeftTire->GetPhysicsNode().GetPosition().x-1*5);
	BackLeftTire->GetPhysicsNode().SetXend(BackLeftTire->GetPhysicsNode().GetPosition().x+1*5);

	
	
	//cout<<car->GetCarNode().GetPosition().y<<endl;
	//T3Matrix4 m = T3Matrix4::Rotation(0.016f, T3Vector3(0,1,0));
	//ent->AddRotation(Quaternion::FromMatrix(m));
}

#if WINDOWS_BUILD
void VehicleTestingScreen::KeyboardEvent(KeyboardEvents::EventType type, KeyboardEvents::Key key) {
			float E=0.707106829f;
			//cout<<"ttt "<<car->GetCarNode().GetOrientation().y<<" ttt"<<endl;
			//cout<<"ttt "<<((car->GetCarNode().GetOrientation().y)+E)<<" ttt"<<endl;
	switch (type) {
	case KeyboardEvents::KEY_DOWN:
	case KeyboardEvents::KEY_HELD:
		switch (key) {

		case KeyboardEvents::KEYBOARD_W:
		{
				//camera->AddMovement(T3Vector3(1,0,0));

		
			   f=f+5;
				//  car->GetCarNode().SetForce(T3Vector3(10+f,0,0));
			   if((car->GetCarNode().GetOrientation().y+E)==0)
		       {
				   car->GetCarNode().SetForce(T3Vector3(f /*+ 100.0f*/,0,0));   
					//		    f=f+100;
		       }
			//  cout<<car->GetCarNode().GetLinearVelocity().x<<"   "<<car->GetCarNode().GetLinearVelocity().y<<"   "<<car->GetCarNode().GetLinearVelocity().z<<"   "<<endl;

			   //cout<<car->GetCarNode().GetLinearVelocity().Length()<<endl;
				if((car->GetCarNode().GetOrientation().y+E)<0)
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

				if((car->GetCarNode().GetOrientation().y+E)>0)
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



				if((car->GetCarNode().GetOrientation().y+E)==0)
		{
		  
		 car->GetCarNode().SetForce(T3Vector3(-f ,0,0));   
	

//		    f=f+100;
		}
		if((car->GetCarNode().GetOrientation().y+E)<0)
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

		if((car->GetCarNode().GetOrientation().y+E)>0)
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

			if((car->GetCarNode().GetOrientation().y+E)<=0.6){

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


				if((car->GetCarNode().GetOrientation().y+E)>=-0.6){
		
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
			case KeyboardEvents::KEYBOARD_I:
			camera->AddMovement(T3Vector3(0,0,-1));
			break;
		case KeyboardEvents::KEYBOARD_K:
			camera->AddMovement(T3Vector3(0,0,1));
			break;
		case KeyboardEvents::KEYBOARD_J:
			camera->AddMovement(T3Vector3(-1,0,0));
			break;
		case KeyboardEvents::KEYBOARD_L:
			camera->AddMovement(T3Vector3(1,0,0));
			break;
		case KeyboardEvents::KEYBOARD_SHIFT:
			camera->AddMovement(T3Vector3(0,1,0));
			break;
		case KeyboardEvents::KEYBOARD_SPACE:
			{
				camera->AddMovement(T3Vector3(0,-1,0));
			}
			break;

		case KeyboardEvents::KEYBOARD_N:
			{
				car->GetCarNode().SetPosition(car->GetCarNode().GetPosition() + T3Vector3(0.0f, -0.2f, 0.0f));	
			}
			break;
		case KeyboardEvents::KEYBOARD_H:
			{
				car->GetCarNode().SetPosition(car->GetCarNode().GetPosition() + T3Vector3(0.0f, 0.2f, 0.0f));
			
			}
			break;

		case KeyboardEvents::KEYBOARD_B:
			{
			//	car->GetCarNode().SetOrientation(Quaternion::AxisAngleToQuaterion(T3Vector3(0, 0, 1), 20));
			  temp1 =  car->GetCarNode().GetLinearVelocity();
			  car->GetCarNode().SetUseGravity(TRUE);
			  temp1.y = -1.4;
			  car->GetCarNode().SetLinearVelocity(temp1);
			}
			break;
		}
		
	}
}
	
void VehicleTestingScreen::MouseMoved(T3Vector2& star, T3Vector2& finish) {
T3Vector2 amount = finish - star;
	
	camera->AddPitch(10*(-amount.y));
	camera->AddYaw(10*(amount.x));
}

#endif
