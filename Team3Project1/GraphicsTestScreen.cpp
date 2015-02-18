#include "GraphicsTestScreen.h"
#include "Mesh.h"
#include "../Framework/Light.h"
#include "GameStateManager.h"

GraphicsTestScreen::GraphicsTestScreen(void)
{
	LoadContent(); // TODO - I assume the asset manager will call this automatically
}

GraphicsTestScreen::~GraphicsTestScreen(void)
{
	delete quad;
	delete light;
}

void GraphicsTestScreen::LoadContent() {
	quad = Mesh::GenerateQuad();
	DrawableEntity3D* ent = new DrawableEntity3D(
		quad, 
		NULL,
		GameStateManager::Assets()->LoadTexture(this, "D:\\Visual Studio 2012\\Projects\\Graphics For Games\\Textures\\Blend Swap\\Grass\\Grass_Color.tga"),
		NULL, 
		50.0f, 
		Vector3(0,0,0), 
		Quaternion::EulerAnglesToQuaternion(0,0,0), 
		Vector3(10,10,10));

	drawables.push_back(ent);
	GameStateManager::Graphics()->AddDrawable(drawables[0]);

	SpotLight::SetConeMesh(Mesh::GenerateCone(20));
	SpotLight::SetCircleMesh(Mesh::GenerateCircle(20));
	light = new SpotLight(Vector3(0, 50, 0), Vector3(0,0,0), Vector3(1,0,0), Vector4(1,1,1,1), Vector4(1,1,1,1), 1000.0f, 45.0f, 0);
	GameStateManager::Graphics()->AddLight(light);

	Camera* camera = new Camera();
	camera->SetPosition(Vector3(0, -10, -10));
	GameStateManager::Graphics()->SetCamera(camera);
}