#include "GraphicsTestScreen.h"
#include "Mesh.h"
#include "../Framework/Light.h"
#include "GameStateManager.h"
#include "FreeCamera.h"
#include "../Framework/ObjMesh.h"

//TODO - remove
#include <iostream>

GraphicsTestScreen::GraphicsTestScreen(void)
{
}

GraphicsTestScreen::~GraphicsTestScreen(void)
{
	// Unload Content

	// Remove Lights
	RemoveLight(spotLight);
	RemoveLight(pointLight);
	RemoveLight(directionalLight);
	delete pointLight;
	delete spotLight;
	delete directionalLight;

	// Remove Drawables/Game entities
	for (vector<DrawableEntity3D*>::iterator i = gameEntities.begin(); i != gameEntities.end(); i++) {
		RemoveDrawable(*i);
		delete *i;
	}
	gameEntities.clear();

	// Unload textures/meshes/assets
	GameStateManager::Assets()->UnloadQuad(this);
	GameStateManager::Assets()->UnloadCylinder(this, 20);
	GameStateManager::Assets()->UnloadMesh(this, MESHDIR"Nova Car.obj");
	GameStateManager::Assets()->UnloadTexture(this, TEXTUREDIR"Grass_Color.tga");
	GameStateManager::Assets()->UnloadTexture(this, TEXTUREDIR"calvin.bmp");
}

void GraphicsTestScreen::LoadContent() {
	
	quad = GameStateManager::Assets()->LoadQuad(this);
	DrawableEntity3D* ent;
	for (unsigned int i = 0; i < 8; i++) {
		for (unsigned int j = 0; j < 8; j++) {
			ent = new DrawableEntity3D(
				quad,
				NULL,
				GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"Grass_Color.tga", SOIL_FLAG_MIPMAPS),
				NULL,
				50.0f,
				Vector3(-350.0f + i * 100.0f,0,-350.0f + j * 100.0f),
				Quaternion::FromMatrix(Matrix4::Rotation(90.0f, Vector3(1,0,0))),
				Vector3(50,50,1));
			gameEntities.push_back(ent);
			AddDrawable(ent);
		}
	}
/*
	ent = new DrawableEntity3D(
		quad,
		NULL,
		GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"Grass_Color.tga", SOIL_FLAG_MIPMAPS),
		NULL,
		400.0f,
		Vector3(0,0,0),
		Quaternion::FromMatrix(Matrix4::Rotation(90.0f, Vector3(1,0,0))),
		Vector3(400,400,1));
		gameEntities.push_back(ent);
		AddDrawable(ent);
		*/

	cylinder = GameStateManager::Assets()->LoadCylinder(this, 20);
	ent = new DrawableEntity3D(
		cylinder, 
		NULL,
		GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"calvin.bmp", SOIL_FLAG_INVERT_Y), 
		NULL,
		30.0f, 
		Vector3(35,0,35), 
		Quaternion::EulerAnglesToQuaternion(0,0,0),
		Vector3(15,30,15));
	gameEntities.push_back(ent);
	AddDrawable(ent);

	circle = GameStateManager::Assets()->LoadCircle(this, 20);
	ent = new DrawableEntity3D(
		circle, 
		NULL,
		GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"calvin.bmp", SOIL_FLAG_INVERT_Y), 
		NULL,
		30.0f, // needs same bounding radius as cylinder
		Vector3(35,30,35), 
		Quaternion::EulerAnglesToQuaternion(0,0,0),
		Vector3(15,1,15));
	gameEntities.push_back(ent);
	AddDrawable(ent);

	Mesh* car = GameStateManager::Assets()->LoadMesh(this, MESHDIR"Nova Car.obj");
	ent = new DrawableEntity3D(
		GameStateManager::Assets()->LoadMesh(this, MESHDIR"Nova Car.obj"),
		NULL,
		NULL,
		NULL,
		25.0f,
		Vector3(-25, 20, 0),
		Quaternion::EulerAnglesToQuaternion(0,0,0),
		Vector3(5,5,5));
	gameEntities.push_back(ent);
	AddDrawable(ent);
	
	
	spotLight = GameStateManager::Graphics()->AddSpotLight(Vector3(-10, 40, -10), Vector3(35,0,35), Vector3(0,1,0), 2000.0f, 45.0f, Vector4(1,0,0,1), Vector4(0,0,1,1), true);
	spotLight = GameStateManager::Graphics()->AddSpotLight(Vector3(50, 40, 50), Vector3(35,0,35), Vector3(0,1,0), 2000.0f, 90.0f, Vector4(0.5,0.5,0.5,1), Vector4(0,0,1,1), true);
	pointLight = GameStateManager::Graphics()->AddPointLight(Vector3(-50,60,-50), 500, Vector4(1,0,1,1), Vector4(0,0.5,0,1), true); 
	pointLight = GameStateManager::Graphics()->AddPointLight(Vector3(50,60,50), 500, Vector4(0,1,0,1), Vector4(0,0.5,0,1), true); 
	
//	directionalLight = GameStateManager::Graphics()->AddDirectionalLight(Vector3(-1, -1, -1), Vector4(1,1,1,1), Vector4(0,0,0,1));

	camera = new FreeCamera();
	camera->SetPosition(Vector3(0,10,80));
	
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
			camera->AddMovement(Vector3(0,0,-1));
			break;
		case KeyboardEvents::KEYBOARD_S:
			camera->AddMovement(Vector3(0,0,1));
			break;
		case KeyboardEvents::KEYBOARD_A:
			camera->AddMovement(Vector3(-1,0,0));
			break;
		case KeyboardEvents::KEYBOARD_D:
			camera->AddMovement(Vector3(1,0,0));
			break;
		case KeyboardEvents::KEYBOARD_SHIFT:
			camera->AddMovement(Vector3(0,1,0));
			break;
		case KeyboardEvents::KEYBOARD_SPACE:
			camera->AddMovement(Vector3(0,-1,0));
			break;
		case KeyboardEvents::KEYBOARD_LEFT:
			camera->AddYaw(1);
			break;
		case KeyboardEvents::KEYBOARD_RIGHT:
			camera->AddYaw(-1);
			break;
		case KeyboardEvents::KEYBOARD_UP:
			camera->AddPitch(1);
			break;
		case KeyboardEvents::KEYBOARD_DOWN:
			camera->AddPitch(-1);
			break;
		case KeyboardEvents::KEYBOARD_Q:
			camera->AddRoll(-1);
			break;
		case KeyboardEvents::KEYBOARD_E:
			camera->AddRoll(1);
			break;
		case KeyboardEvents::KEYBOARD_1:
		GameStateManager::Graphics()->DrawDeferredLights(drawDeferredLights = !drawDeferredLights);
		break;
		}
		break;
	case KeyboardEvents::KEY_PRESS:
		switch (key) {

		case KeyboardEvents::KEYBOARD_ESCAPE:
			GameStateManager::Instance()->Exit();
			break;
		}
	}
}
	
void GraphicsTestScreen::MouseMoved(Vector2& finish) {
	//camera->AddPitch(-finish.y);
	//camera->AddYaw(finish.x);
}