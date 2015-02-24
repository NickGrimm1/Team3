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
	//cylinder = Mesh::GenerateCylinder(20);
	//cout << "Quad Obj = " << quad->GetVertexBuffer() << endl;
	
	ent = new DrawableEntity3D(
		quad, 
		NULL,
		GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"snowflake.png", 0), 
		GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"snowflake.png", 0),
		50.0f, 
		Vector3(0,0,0), 
		Quaternion::FromMatrix(Matrix4::Rotation(90.0f, Vector3(-1,0,0))),
		Vector3(50,50,50));
	AddDrawable(ent);

	/*ent = new DrawableEntity3D(
		cylinder, 
		NULL,
		GameStateManager::Assets()->LoadTexture(this, "D:\\Visual Studio 2012\\Projects\\Graphics For Games\\Textures\\calvin.bmp"), 
		NULL,
		10.0f, 
		Vector3(5,0,5), 
		Quaternion::EulerAnglesToQuaternion(0,0,0),
		Vector3(2,10,2));

	drawables.push_back(ent);
	GameStateManager::Graphics()->AddDrawable(ent);*/

	
	//cout << "Cone Mesh Obj = " << coneMesh->GetVertexBuffer() << endl;
	//SpotLight::SetConeMesh(coneMesh);
	//Mesh* circleMesh = Mesh::GenerateCircle(20);
	//cout << "Circle Mesh Obj = " << circleMesh->GetVertexBuffer() << endl;
	//SpotLight::SetCircleMesh(Mesh::GenerateCircle(20));
	//Mesh* sphereMesh = new OBJMesh(MESHDIR"sphere.obj");
	//PointLight::SetMesh(sphereMesh);


	//light = GameStateManager::Graphics()->AddSpotLight(Vector3(0, 0, 0), Vector3(0,1,0), Vector3(1,0,0), 1.0f, 45.0f, Vector4(1,1,1,1), Vector4(1,1,1,1), false);
	
	//PointLight* l = GameStateManager::Graphics()->AddPointLight(Vector3(0,5,0), 10, Vector4(1,1,1,1), Vector4(1,1,1,1), false); 

	camera = new FreeCamera();
	car = new Vehicle();
	Speed_Player = 2;
	f=0;
	temp2=Vector3(0,0,0);
	Speed_Rotate = -0.0004;
	tempPosition = Vector3(0,350,-800);
	PlayerPosition=Vector3(500,100,-800);

	camera->SetPosition(Vector3(0,10.0f, 80.0f));
	//camera->SetYaw(180.0f);
	GameStateManager::Graphics()->SetCamera(camera);
}

void RacerGame::Update() { 
	//Matrix4 m = Matrix4::Rotation(0.016f, Vector3(0,1,0));
	//ent->AddRotation(Quaternion::FromMatrix(m));
}

void RacerGame::KeyboardEvent(KeyboardEvents::EventType type, KeyboardEvents::Key key) {


	PlayerPosition = car->GetPlayer()->GetPhysicsNode().GetPosition();


	switch (type) {
	case KeyboardEvents::KEY_DOWN:
	case KeyboardEvents::KEY_HELD:
		switch (key) {

		case KeyboardEvents::KEYBOARD_W:
			{camera->AddMovement(Vector3(0,0,1));
			 f=f+0.01;
		     car->GetPlayer()->GetPhysicsNode().SetForce(Vector3(0,0,0.3+f));                                                                        
			 tempPosition.z = PlayerPosition.z-1000;
		     tempPosition.y = PlayerPosition.y+800;



			}
			
			break;
		case KeyboardEvents::KEYBOARD_S:
			{camera->AddMovement(Vector3(0,0,-1));
			 f=f+0.01;
		     car->GetPlayer()->GetPhysicsNode().SetForce(Vector3(0,0,-1-f)); 
		     tempPosition.z = PlayerPosition.z-1000;
		     tempPosition.y = PlayerPosition.y+800;



			}
			break;
		case KeyboardEvents::KEYBOARD_A:
			{camera->AddMovement(Vector3(1,0,0));
			if(car->GetPlayer()->GetPhysicsNode().GetOrientation().z>=-0.13)
			 {
			    car->GetPlayer()->GetPhysicsNode().SetAngularVelocity(Vector3(0,0,Speed_Rotate));
		     }			
		     temp1 = car->GetPlayer()->GetPhysicsNode().GetLinearVelocity();
		     temp1.x=0.1*Speed_Player;
		     car->GetPlayer()->GetPhysicsNode().SetLinearVelocity(temp1);
		     tempPosition.x = PlayerPosition.x;
			
			
			}
			break;
		case KeyboardEvents::KEYBOARD_D:
			{camera->AddMovement(Vector3(-1,0,0));
			  if(car->GetPlayer()->GetPhysicsNode().GetOrientation().z<=0.13)
			  {
			    car->GetPlayer()->GetPhysicsNode().SetAngularVelocity(Vector3(0,0,-Speed_Rotate));
		      }
		      temp1 = car->GetPlayer()->GetPhysicsNode().GetLinearVelocity();
		      temp1.x=-0.1*Speed_Player;
		      car->GetPlayer()->GetPhysicsNode().SetLinearVelocity(temp1);
		      tempPosition.x = PlayerPosition.x;

			}
			break;
		case KeyboardEvents::KEYBOARD_SHIFT:
			camera->AddMovement(Vector3(0,1,0));
			break;
		case KeyboardEvents::KEYBOARD_SPACE:
			{camera->AddMovement(Vector3(0,-1,0));
			 temp1 = car->GetPlayer()->GetPhysicsNode().GetLinearVelocity();
			 car->GetPlayer()->GetPhysicsNode().SetUseGravity(TRUE);
			 temp1.y = 0.4;
			 car->GetPlayer()->GetPhysicsNode().SetLinearVelocity(temp1);	
			
			}
			break;
		}
		
	}
}
	
void RacerGame::MouseMoved(Vector2& finish) {
	camera->AddPitch(-finish.y);
	camera->AddYaw(finish.x);
}