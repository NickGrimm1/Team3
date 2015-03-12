#include "GraphicsTestScreen.h"
#include "Mesh.h"
#include "../Framework/Light.h"
#include "GameStateManager.h"
#include "ChaseCamera.h"
#include "../Framework/ObjMesh.h"
#include "FreeCamera.h"
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
	//
	quad = GameStateManager::Assets()->LoadQuad(this);
	cylinder = GameStateManager::Assets()->LoadCylinder(this, 20);
	circle = GameStateManager::Assets()->LoadCircle(this, 20);
	Texture* grassTex = GameStateManager::Assets()->LoadTexture(this, "Grass_Color", 0);
#if WINDOWS_BUILD
	Texture* calvinTex = GameStateManager::Assets()->LoadTexture(this, "calvin", SOIL_FLAG_INVERT_Y);
#endif
#if PS3_BUILD
	Texture* calvinTex = GameStateManager::Assets()->LoadTexture(this, "calvin", 0);
#endif
//	Mesh* car = GameStateManager::Assets()->LoadMesh(this, MESHDIR"NovaCar.obj");
	
	DrawableEntity3D* ent;
	
	/*for (unsigned int i = 0; i < 8; i++) {
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
	}*/
	
#if WINDOWS_BUILD
	GameStateManager::Graphics()->GetRenderContext();
#endif

	track = new TrackSegment(T3Vector3(-400, 0, 0), T3Vector3(0,0,-400), T3Vector3(400,0,0), 10, 50.0);

#if WINDOWS_BUILD
	GameStateManager::Graphics()->DropRenderContext();
#endif
	ent = new DrawableEntity3D(
#if WINDOWS_BUILD
		GameStateManager::Assets()->LoadHeightmap(0, 15, true),
#endif
#if PS3_BUILD //TODO : PS3 Needs to draw a heightmap instead of a quad, change when heightmap fixed
		GameStateManager::Assets()->LoadQuad(this),
#endif
		NULL,
		grassTex,
		NULL,
		800.0f,
		T3Vector3(),
		Quaternion::EulerAnglesToQuaternion(0,0,0),
		T3Vector3(1,1,1));
	AddDrawable(ent);

	ent = new DrawableEntity3D(
		track,
		NULL,
		grassTex,
		NULL,
		800.0f,
		T3Vector3(0,0,0),
		Quaternion::EulerAnglesToQuaternion(0,0,0),
		T3Vector3(1,1,1));
	gameEntities.push_back(ent);
	AddDrawable(ent);

	ent = new DrawableEntity3D(
		cylinder, 
		NULL,
		calvinTex,
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
		calvinTex,
		NULL,
		30.0f, // needs same bounding radius as cylinder
		T3Vector3(35,30,35), 
		Quaternion::EulerAnglesToQuaternion(0,0,0),
		T3Vector3(15,1,15));
	gameEntities.push_back(ent);
	AddDrawable(ent);

#if WINDOWS_BUILD
	ent = new DrawableEntity3D(
		cylinder, 
		NULL,
#if WINDOWS_BUILD
		calvinTex,
#endif
#if PS3_BUILD
		nclTex,
#endif
		NULL,
		30.0f, // needs same bounding radius as cylinder
		track->GetTrackCentreLeft(),
		Quaternion::EulerAnglesToQuaternion(0,0,0),
		T3Vector3(5,15,5));
	gameEntities.push_back(ent);
	AddDrawable(ent);

	ent = new DrawableEntity3D(
		cylinder, 
		NULL,
#if WINDOWS_BUILD
		calvinTex,
#endif
#if PS3_BUILD
		nclTex,
#endif
		NULL,
		30.0f, // needs same bounding radius as cylinder
		track->GetTrackCentreRight(),
		Quaternion::EulerAnglesToQuaternion(0,0,0),
		T3Vector3(5,15,5));
	gameEntities.push_back(ent);
	AddDrawable(ent);
#endif
	//ent = new DrawableEntity3D(
	//	car,
	//	NULL,
	//	NULL,
	//	NULL,
	//	25.0f,
	//	T3Vector3(-25, 20, 0),
	//	Quaternion::EulerAnglesToQuaternion(0,0,0),
	//	T3Vector3(5,5,5));
	//gameEntities.push_back(ent);
	//AddDrawable(ent);
	
	//bool enableShadows = false;
	//AddSpotLight(T3Vector3(-10, 40, -10), T3Vector3(35,0,35), T3Vector3(0,1,0), 2000.0f, 45.0f, T3Vector4(1,0,0,1), T3Vector4(0,0,1,1), enableShadows);
	//AddSpotLight(T3Vector3(50, 40, 50), T3Vector3(35,0,35), T3Vector3(0,1,0), 2000.0f, 90.0f, T3Vector4(0.5,0.5,0.5,1), T3Vector4(0,0,1,1), enableShadows);
	//AddPointLight(T3Vector3(-50,60,-50), 500, T3Vector4(1,0,1,1), T3Vector4(0,0.5,0,1), enableShadows); 
	//AddPointLight(T3Vector3(50,60,50), 500, T3Vector4(0,1,0,1), T3Vector4(0,0.5,0,1), enableShadows); 
	//
	//directionalLight = GameStateManager::Graphics()->AddDirectionalLight(T3Vector3(-1, -1, -1), T3Vector4(1,1,1,1), T3Vector4(0,0,0,1));

	DrawableEntity3D* myEnt = new DrawableEntity3D(
		quad,
		NULL,
		GameStateManager::Assets()->LoadTexture(this,"ncl",0),
		NULL,
		25.0f,
		T3Vector3(0,5,10),
		Quaternion::EulerAnglesToQuaternion(0,0,0),
		T3Vector3(5,5,5));
	gameEntities.push_back(myEnt);
	AddDrawable(myEnt);

	//SetPlayer(ent);
	camera = new FreeCamera();//ChaseCamera(myEnt, T3Vector3(0, 2, 50), 0, 0, 0);
	camera->SetPosition(T3Vector3(0,10,80));
	
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

	//delete track;

	GameStateManager::Assets()->UnloadQuad(this);
	GameStateManager::Assets()->UnloadCylinder(this, 20);
	GameStateManager::Assets()->UnloadCircle(this, 20);
	GameStateManager::Assets()->UnloadTexture(this, "Grass_Color"),
	GameStateManager::Assets()->UnloadTexture(this, "calvin"),
	GameStateManager::Assets()->UnloadMesh(this, "NovaCar");
}


void GraphicsTestScreen::Update() { 
	//T3Matrix4 m = T3Matrix4::Rotation(0.016f, T3Vector3(0,1,0));
	//ent->AddRotation(Quaternion::FromMatrix(m));

	//GameStateManager::Assets()->LoadHeightmap();
}
#if WINDOWS_BUILD
void GraphicsTestScreen::KeyboardEvent(KeyboardEvents::EventType type, KeyboardEvents::Key key) {

	switch (type) {
	case KeyboardEvents::KEY_DOWN:
	case KeyboardEvents::KEY_HELD:
		switch (key) {

		case KeyboardEvents::KEYBOARD_W:
			GetPlayer()->Move(T3Vector3(0,0,-1));
			break;
		case KeyboardEvents::KEYBOARD_S:
			GetPlayer()->Move(T3Vector3(0,0,1));
			break;
		case KeyboardEvents::KEYBOARD_A:
			GetPlayer()->Move(T3Vector3(-1,0,0));
			break;
		case KeyboardEvents::KEYBOARD_D:
			GetPlayer()->Move(T3Vector3(1,0,0));
			break;
		case KeyboardEvents::KEYBOARD_SHIFT:
			GetPlayer()->Move(T3Vector3(0,1,0));
			break;
		case KeyboardEvents::KEYBOARD_SPACE:
			GetPlayer()->Move(T3Vector3(0,-1,0));
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
		}
		break;
	case KeyboardEvents::KEY_PRESS:
		switch (key) {
		case KeyboardEvents::KEYBOARD_1:
			GameStateManager::Graphics()->DrawDeferredLights(drawDeferredLights = !drawDeferredLights);
			break;
		case KeyboardEvents::KEYBOARD_ESCAPE:
			GameStateManager::Instance()->Exit();
			break;
		}
	}
}
	
void GraphicsTestScreen::MouseMoved(T3Vector2& start, T3Vector2& finish) {
	camera->AddPitch(-finish.y);
	camera->AddYaw(-finish.x);
}
#endif