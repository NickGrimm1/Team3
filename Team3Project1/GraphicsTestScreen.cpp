#include "GraphicsTestScreen.h"
#include "Mesh.h"
#include "../Framework/Light.h"
#include "GameStateManager.h"
#include "FreeCamera.h"
#include "../Framework/ObjMesh.h"

//TODO - remove
#include <iostream>
#include "../Framework/BumpTexture.h"

GraphicsTestScreen::GraphicsTestScreen(void)
{
}

GraphicsTestScreen::~GraphicsTestScreen(void)
{
	delete quad;
	delete light;
}

void GraphicsTestScreen::LoadContent() {
	//Mesh* coneMesh = Mesh::GenerateCone(20);
	quad = GameStateManager::Assets()->LoadCylinder(this, 20);
	//cylinder = Mesh::GenerateCylinder(20);
	//cout << "Quad Obj = " << quad->GetVertexBuffer() << endl;
	
	ent = new DrawableEntity3D(
		quad, 
		NULL,
		GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"snowflake.png"), 
		GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"snowflake.png"),
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
	camera->SetPosition(Vector3(0,10.0f, 80.0f));
	camera->SetYaw(180.0f);
	GameStateManager::Graphics()->SetCamera(camera);
}

void GraphicsTestScreen::Update() { 
	//Matrix4 m = Matrix4::Rotation(0.016f, Vector3(0,1,0));
	//ent->AddRotation(Quaternion::FromMatrix(m));
}

void GraphicsTestScreen::KeyboardEvent(KeyboardEvents::EventType type, KeyboardEvents::Key key) {

	switch (type) {
	case KeyboardEvents::KEY_DOWN:
	case KeyboardEvents::KEY_HELD:
		switch (key) {

		case KeyboardEvents::KEYBOARD_W:
			camera->AddMovement(Vector3(0,0,1));
			break;
		case KeyboardEvents::KEYBOARD_S:
			camera->AddMovement(Vector3(0,0,-1));
			break;
		case KeyboardEvents::KEYBOARD_A:
			camera->AddMovement(Vector3(1,0,0));
			break;
		case KeyboardEvents::KEYBOARD_D:
			camera->AddMovement(Vector3(-1,0,0));
			break;
		case KeyboardEvents::KEYBOARD_SHIFT:
			camera->AddMovement(Vector3(0,1,0));
			break;
		case KeyboardEvents::KEYBOARD_SPACE:
			camera->AddMovement(Vector3(0,-1,0));
			break;
		}
		
	}
}
	
void GraphicsTestScreen::MouseMoved(Vector2& finish) {
	camera->AddPitch(-finish.y);
	camera->AddYaw(finish.x);
}