#include "GameScreen3D.h"
#include "GameStateManager.h"

GameScreen3D::~GameScreen3D()
{
	// Clear drawables.
	vector<DrawableEntity3D*>::iterator i = drawables.begin();
	while (i != drawables.end())
	{
		RemoveDrawable(*i);
	}
	// Clear lights
	vector<Light*>::iterator j = lights.begin();
	while (j != lights.end())
	{
		RemoveLight(*j);
	}
	// Clear Camera
	GameStateManager::Graphics()->SetCamera(NULL);
}
void GameScreen3D::AddDrawable(DrawableEntity3D* value) 
{
	for (unsigned int i = 0; i < drawables.size(); i++)
		if (drawables[i] == value)
			return;

	drawables.push_back(value);
	GameStateManager::Graphics()->AddDrawable(value);
}
void GameScreen3D::RemoveDrawable(DrawableEntity3D* value)
{
	for (vector<DrawableEntity3D*>::iterator i = drawables.begin(); i != drawables.end(); i++)
	{
		if (*i == value)
		{
			i = drawables.erase(i);
			GameStateManager::Graphics()->RemoveDrawable(value);
			break;
		}
	}
}
Light* GameScreen3D::AddPointLight(Vector3 lightPosition, float lightRadius, Vector4 diffuseColour, Vector4 specularColour, bool castsShadow)
{
	Light* l = GameStateManager::Graphics()->AddPointLight(lightPosition, lightRadius, diffuseColour, specularColour, castsShadow);
	lights.push_back(l);
	return l;
}
Light* GameScreen3D::AddDirectionalLight(Vector3 lightDirection, Vector4 diffuseColour, Vector4 specularColour)
{
	Light* l = GameStateManager::Graphics()->AddDirectionalLight(lightDirection, diffuseColour, specularColour);
	lights.push_back(l);
	return l;
}
Light* GameScreen3D::AddSpotLight(Vector3 lightPosition, Vector3 lightTarget, Vector3 upVector, float lightRadius, float lightAngle, Vector4 diffuseColour, Vector4 specularColour, bool castsShadow)
{
	Light* l = GameStateManager::Graphics()->AddSpotLight(lightPosition, lightTarget, upVector, lightRadius, lightAngle, diffuseColour, specularColour, castsShadow);
	lights.push_back(l);
	return l;
}
void GameScreen3D::RemoveLight(Light* value) 
{
	for (vector<Light*>::iterator i = lights.begin(); i != lights.end(); i++)
	{
		if (*i == value)
		{
			i = lights.erase(i);
			GameStateManager::Graphics()->RemoveLight(value);
			break;
		}
	}
}
void GameScreen3D::SetCamera(Camera* value)
{
	camera = value;
	GameStateManager::Graphics()->SetCamera(value);
}