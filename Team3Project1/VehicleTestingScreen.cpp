


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
	

	T3Vector3 sp1= T3Vector3(-100,0,0);
	T3Vector3 sp2= T3Vector3(0,0,0);
	T3Vector3 sp3= T3Vector3(100,0,0);
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
		road->SetOriginPosition(T3Vector3(0,-20,0));
		road->SetRotation(Quaternion::EulerAnglesToQuaternion(0,0,0));
		road->SetScale(T3Vector3(1,1,1));
		road->GetPhysicsNode().SetMass(5);
		road->GetPhysicsNode().SetMesh(trackr);
		road->GetPhysicsNode().SetInverseMass(0);
		road->GetPhysicsNode().SetUseGravity(false);
		road->GetPhysicsNode().SetIsCollide(true);
	    road->GetPhysicsNode().Setcar_wheel(true);
		road->GetPhysicsNode().Setplanecollision(true);
		road->GetPhysicsNode().SetPosition(T3Vector3(0,-20,0));
		road->GetPhysicsNode().SetInverseInertia(InertialMatrixHelper::createImmovableInvInertial());
		road->ConnectToSystems();
		AddDrawable(road);





	
	//SpotLight::SetConeMesh(coneMesh);
	//Mesh* circleMesh = Mesh::GenerateCircle(20);
	//SpotLight::SetCircleMesh(Mesh::GenerateCircle(20));
	//Mesh* sphereMesh = new OBJMesh(MESHDIR"sphere.obj");
	//PointLight::SetMesh(sphereMesh);

	
	//light = GameStateManager::Graphics()->AddSpotLight(T3Vector3(0, 0, 0), T3Vector3(0,1,0), T3Vector3(1,0,0), 1.0f, 45.0f, T3Vector4(1,1,1,1), T3Vector4(1,1,1,1), false);
	
	//PointLight* l = GameStateManager::Graphics()->AddPointLight(T3Vector3(0,5,0), 10, T3Vector4(1,1,1,1), T3Vector4(1,1,1,1), false); 

	camera = new FreeCamera();


	//gold_cion= new Gold_cion(5);
	//gold_cion->SetPhysics(5);
	//AddDrawable(gold_cion);


	//checkpoint= new CheckPoint(10);
	//checkpoint->SetPhysics(10,'c');
	//AddDrawable(checkpoint);
	
	float size = 5;
	//GameStateManager::Physics()->AddNode((PhysicsNode*)vpn);

	
	car = new Vehicle(size);
	ChaseCamera* chasecamera = new ChaseCamera(car,T3Vector3(0,40,130),0,0,0);
	//ChaseCamera* chasecamera= new ChaseCamera(car,T3Vector3(0,2,20),0,0,0);

	AddDrawable(car);
	AddDrawable(car->GetFrontRightTire());
	AddDrawable(car->GetFrontLeftTire());
	AddDrawable(car->GetBackRightTire());
	AddDrawable(car->GetBackLeftTire());


	//f=0;
	b=0;
	
	Speed_Rotate =  0.4f;
	angular=0.1f;
	
	camera->SetPosition(T3Vector3(0,10.0f, 80.0f));
	//camera->SetYaw(180.0f);
	GameStateManager::Graphics()->SetCamera(camera);
#if WINDOWS_BUILD
	GameStateManager::Audio()->SetListener(car);
#endif
}

void VehicleTestingScreen::Update() { 



	


//	FrontLeftTire->GetPhysicsNode().SetOrientation(car->GetCarNode().GetOrientation());
//	FrontRightTire->GetPhysicsNode().SetOrientation(car->GetCarNode().GetOrientation());
//	BackLeftTire->GetPhysicsNode().SetOrientation(car->GetCarNode().GetOrientation());
//	BackRightTire->GetPhysicsNode().SetOrientation(car->GetCarNode().GetOrientation());
	
	
	//car->GetCarNode().SetXstart(car->GetCarNode().GetPosition().x-3*5 +10);
	//car->GetCarNode().SetXend(car->GetCarNode().GetPosition().x+3*5+10);
//	checkpoint->GetPhysicsNode().SetXstart(checkpoint->GetPhysicsNode().GetPosition().x-1*10);
//	checkpoint->GetPhysicsNode().SetXend(checkpoint->GetPhysicsNode().GetPosition().x+1*10);
	//gold_cion->GetPhysicsNode().SetXstart(gold_cion->GetPhysicsNode().GetPosition().x-1*10+10);
	//gold_cion->GetPhysicsNode().SetXend(gold_cion->GetPhysicsNode().GetPosition().x+1*10+10);
	//FrontRightTire->GetPhysicsNode().SetXstart(FrontRightTire->GetPhysicsNode().GetPosition().x-1*5);
	//FrontRightTire->GetPhysicsNode().SetXend(FrontRightTire->GetPhysicsNode().GetPosition().x+1*5);

	//FrontLeftTire->GetPhysicsNode().SetXstart(FrontLeftTire->GetPhysicsNode().GetPosition().x-1*5);
	//FrontLeftTire->GetPhysicsNode().SetXend(FrontLeftTire->GetPhysicsNode().GetPosition().x+1*5);

	//BackRightTire->GetPhysicsNode().SetXstart(BackRightTire->GetPhysicsNode().GetPosition().x-1*5);
	//BackRightTire->GetPhysicsNode().SetXend(BackRightTire->GetPhysicsNode().GetPosition().x+1*5);
	//BackLeftTire->GetPhysicsNode().SetXstart(BackLeftTire->GetPhysicsNode().GetPosition().x-1*5);
	//BackLeftTire->GetPhysicsNode().SetXend(BackLeftTire->GetPhysicsNode().GetPosition().x+1*5);

	
	
	//T3Matrix4 m = T3Matrix4::Rotation(0.016f, T3Vector3(0,1,0));
	//ent->AddRotation(Quaternion::FromMatrix(m));
}

#if WINDOWS_BUILD
void VehicleTestingScreen::KeyboardEvent(KeyboardEvents::EventType type, KeyboardEvents::Key key) {
			
			
	switch (type) {
	case KeyboardEvents::KEY_DOWN:
	case KeyboardEvents::KEY_HELD:
		switch (key) {

		case KeyboardEvents::KEYBOARD_W:
		{
				//camera->AddMovement(T3Vector3(1,0,0));

			car->GetVehiclePhysicsNode()->SetF(car->GetVehiclePhysicsNode()->GetF() +0.4f);
            if(car->GetVehiclePhysicsNode()->GetF()>100)
			{
			  car->GetVehiclePhysicsNode()->SetF(100);
			}
		
			/*f=f+0.4f;
				   if(f>100){
				   f=100;
		       }*/

		
				   T3Matrix4 m4 = car->GetCarNode().GetOrientation().ToMatrix();

				   car->GetCarNode().SetLinearVelocity( m4 * T3Matrix4::Rotation(90,T3Vector3(0,1,0))*car->GetVehiclePhysicsNode()->GetF());
			
				   //FrontRightTire->GetPhysicsNode().SetAngularVelocity(T3Vector3(4,0,0));
				
			
			 break;
		}	
		case KeyboardEvents::KEYBOARD_S:
			{//camera->AddMovement(T3Vector3(0,0,1));

				 b=b+0.2f;
				   if(b>50){
				   b=50;
		}
           T3Matrix4 m4 = car->GetCarNode().GetOrientation().ToMatrix();
		  car->GetCarNode().SetLinearVelocity( m4 *T3Matrix4::Rotation(90,T3Vector3(0,1,0))*-b);

				



			}
			break;
		case KeyboardEvents::KEYBOARD_A:
			{//camera->AddMovement(T3Vector3(1,0,0));


				if(car->GetCarNode().GetLinearVelocity().x>=0)
			{

				car->GetCarNode().SetAngularVelocity(T3Vector3(0,Speed_Rotate,0));
				car->GetFrontLeftTire()->GetPhysicsNode().SetOrientation(car->GetCarNode().GetOrientation());
				car->GetFrontRightTire()->GetPhysicsNode().SetOrientation(car->GetCarNode().GetOrientation());
				car->GetBackLeftTire()->GetPhysicsNode().SetOrientation(car->GetCarNode().GetOrientation());
				car->GetBackRightTire()->GetPhysicsNode().SetOrientation(car->GetCarNode().GetOrientation());


		        

		}
				else
			{
					 car->GetCarNode().SetAngularVelocity(T3Vector3(0,-Speed_Rotate,0));
					 car->GetFrontLeftTire()->GetPhysicsNode().SetOrientation(car->GetCarNode().GetOrientation());
					 car->GetFrontRightTire()->GetPhysicsNode().SetOrientation(car->GetCarNode().GetOrientation());
					 car->GetBackLeftTire()->GetPhysicsNode().SetOrientation(car->GetCarNode().GetOrientation());
				     car->GetBackRightTire()->GetPhysicsNode().SetOrientation(car->GetCarNode().GetOrientation());
				
				}
			
		

		}		

		
			
			break;
		case KeyboardEvents::KEYBOARD_D:
			{//camera->AddMovement(T3Vector3(-1,0,0));
			
		if(car->GetCarNode().GetLinearVelocity().x>=0)
			  {
			 car->GetFrontLeftTire()->GetPhysicsNode().SetOrientation(car->GetCarNode().GetOrientation());
		     car->GetFrontRightTire()->GetPhysicsNode().SetOrientation(car->GetCarNode().GetOrientation());
			 car->GetBackLeftTire()->GetPhysicsNode().SetOrientation(car->GetCarNode().GetOrientation());
		     car->GetBackRightTire()->GetPhysicsNode().SetOrientation(car->GetCarNode().GetOrientation());
			  car->GetCarNode().SetAngularVelocity(T3Vector3(0,-Speed_Rotate,0));
		      }
		else
		{
		
		    car->GetFrontLeftTire()->GetPhysicsNode().SetOrientation(car->GetCarNode().GetOrientation());
		    car->GetFrontRightTire()->GetPhysicsNode().SetOrientation(car->GetCarNode().GetOrientation());
			car->GetBackLeftTire()->GetPhysicsNode().SetOrientation(car->GetCarNode().GetOrientation());
		    car->GetBackRightTire()->GetPhysicsNode().SetOrientation(car->GetCarNode().GetOrientation());
			  car->GetCarNode().SetAngularVelocity(T3Vector3(0,Speed_Rotate,0));


		}			


	


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
			  temp1.y = -1.4f;
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
