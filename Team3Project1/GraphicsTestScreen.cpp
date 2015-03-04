
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
	gameEntities.clear();
}

void GraphicsTestScreen::LoadContent() {
	
	// Use of this method: It's abstract, so you must implement it.
	//
	// Use this to call GameStateManager::Assets()->Load* for meshes, textures etc. Note the reference to this in each Load call.
	// This is so the AssetManager knows who is using this object - It does not matter how many times Load* is called, the AssetManager will not add extra user counts.
	//
	// Note the use below of a single pointer to the object ent, which is then used many times over to add new objects to the scene.
	// This is perfectly fine practice - internally the GameScreen holds all the pointers and will clean up on exit.
	// If you need to remove or change an entity before the game screen is disposed of (A pick-up perhaps) then it will require it's own unique named variable.

	quad = GameStateManager::Assets()->LoadQuad(this);
	cylinder = GameStateManager::Assets()->LoadCylinder(this, 20);
	circle = GameStateManager::Assets()->LoadCircle(this, 20);
	std::cout << "GraphicsTestScreen: Load ncl.tga" << std::endl;
	Texture* grassTex = GameStateManager::Assets()->LoadTexture(this, "/ncl.tga", 0);
//	Texture* calvinTex = GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"calvin.bmp", SOIL_FLAG_INVERT_Y);

	DrawableEntity3D* ent;
	for (unsigned int i = 0; i < 8; i++) {
		for (unsigned int j = 0; j < 8; j++) {
			ent = new DrawableEntity3D(
				quad,
				NULL,
				grassTex,
				NULL,
				50.0f,
				T3Vector3(-350.0f + i * 100.0f,0,-350.0f + j * 100.0f),
				Quaternion::FromMatrix(T3Matrix4::Rotation(90.0f, T3Vector3(1,0,0))),
				T3Vector3(50,50,1));
			gameEntities.push_back(ent);
			AddDrawable(ent);
		}
	}

	ent = new DrawableEntity3D(
		cylinder, 
		NULL,
		grassTex,
		NULL,
		30.0f, 
		T3Vector3(35,0,35), 
		Quaternion::EulerAnglesToQuaternion(0,0,0),
		T3Vector3(15,30,15));
	gameEntities.push_back(ent);
	AddDrawable(ent);


	ent = new DrawableEntity3D(
		circle, 
		NULL,
		grassTex,
		NULL,
		30.0f, // needs same bounding radius as cylinder
		T3Vector3(35,30,35), 
		Quaternion::EulerAnglesToQuaternion(0,0,0),
		T3Vector3(15,1,15));
	gameEntities.push_back(ent);
	AddDrawable(ent);

	Mesh* car = GameStateManager::Assets()->LoadMesh(this, MESHDIR"Nova Car.obj");
	ent = new DrawableEntity3D(
		GameStateManager::Assets()->LoadMesh(this, MESHDIR"Nova Car.obj"),
		NULL,
		NULL,
		NULL,
		25.0f,
		T3Vector3(-25, 20, 0),
		Quaternion::EulerAnglesToQuaternion(0,0,0),
		T3Vector3(5,5,5));
	gameEntities.push_back(ent);
	AddDrawable(ent);
	
	
	AddSpotLight(T3Vector3(-10, 40, -10), T3Vector3(35,0,35), T3Vector3(0,1,0), 2000.0f, 45.0f, T3Vector4(1,0,0,1), T3Vector4(0,0,1,1), true);
	AddSpotLight(T3Vector3(50, 40, 50), T3Vector3(35,0,35), T3Vector3(0,1,0), 2000.0f, 90.0f, T3Vector4(0.5,0.5,0.5,1), T3Vector4(0,0,1,1), true);
	AddPointLight(T3Vector3(-50,60,-50), 500, T3Vector4(1,0,1,1), T3Vector4(0,0.5,0,1), true); 
	AddPointLight(T3Vector3(50,60,50), 500, T3Vector4(0,1,0,1), T3Vector4(0,0.5,0,1), true); 
	
//	directionalLight = GameStateManager::Graphics()->AddDirectionalLight(T3Vector3(-1, -1, -1), T3Vector4(1,1,1,1), T3Vector4(0,0,0,1));

	camera = new FreeCamera();
	camera->SetPosition(T3Vector3(0,5,0));
	
	SetCamera(camera);
}
void GraphicsTestScreen::UnloadContent()
{
	// Use of this method: It's abstract, so you must implement it.
	//
	// Use this to call GameStateManager::Assets()->Unload* for meshes, textures etc that were loaded in LoadContent(). Not the reference to this in Unload*.
	// In LoadContent() above, LoadTexture was called many times upon Grass_Color.tga and calvin.bmp. - the reference to this ensures that it only needs to be unloaded by this method once.
	//
	// IMPORTANT: DO NOT DIRECTLY DELETE ASSETS, always call GameStateManager::Assets()->Unload*. It is possible to have multiple GameScreens - the asset may still be in use elsewhere.
	//
	// Remember to clean up any pointers you may have added either here or in the destructor - however DO NOT DELETE pointers that have been added to Entities/Drawables.
	// Entity pointer(s) will be handled automatically by the parent destructor.
	//
	// RemoveDrawable() etc will also be called automatically upon destruction so calling it is uneccessary.

	GameStateManager::Assets()->UnloadQuad(this);
	GameStateManager::Assets()->UnloadCylinder(this, 20);
	GameStateManager::Assets()->UnloadCircle(this, 20);
	GameStateManager::Assets()->UnloadTexture(this, TEXTUREDIR"Grass_Color.jpg"),
	GameStateManager::Assets()->UnloadTexture(this, TEXTUREDIR"calvin.bmp"),
	GameStateManager::Assets()->UnloadMesh(this, MESHDIR"Nova Car.obj");
}


void GraphicsTestScreen::Update() { 
	//T3Matrix4 m = T3Matrix4::Rotation(0.016f, T3Vector3(0,1,0));
	//ent->AddRotation(Quaternion::FromMatrix(m));
}
#if WINDOWS_BUILD
void GraphicsTestScreen::KeyboardEvent(KeyboardEvents::EventType type, KeyboardEvents::Key key) {

	switch (type) {
	case KeyboardEvents::KEY_DOWN:
	case KeyboardEvents::KEY_HELD:
		switch (key) {

		case KeyboardEvents::KEYBOARD_W:
			camera->AddMovement(T3Vector3(0,0,-1));
			break;
		case KeyboardEvents::KEYBOARD_S:
			camera->AddMovement(T3Vector3(0,0,1));
			break;
		case KeyboardEvents::KEYBOARD_A:
			camera->AddMovement(T3Vector3(-1,0,0));
			break;
		case KeyboardEvents::KEYBOARD_D:
			camera->AddMovement(T3Vector3(1,0,0));
			break;
		case KeyboardEvents::KEYBOARD_SHIFT:
			camera->AddMovement(T3Vector3(0,1,0));
			break;
		case KeyboardEvents::KEYBOARD_SPACE:
			camera->AddMovement(T3Vector3(0,-1,0));
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
	
void GraphicsTestScreen::MouseMoved(T3Vector2& finish) {
	camera->AddPitch(-finish.y);
	camera->AddYaw(finish.x);
}

#endif