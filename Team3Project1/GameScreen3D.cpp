#include "GameScreen3D.h"
#include "GameStateManager.h"

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
void GameScreen3D::AddLight(Light* value) 
{
	for (unsigned int i = 0; i < lights.size(); i++)
		if (lights[i] == value)
			return;

	lights.push_back(value);
	GameStateManager::Graphics()->AddLight(value);
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