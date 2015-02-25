/**
Version History:
0.0.1 03/02/2015
0.0.5 11/02/2015
	Implemented Methods.
*/

/**
<summary>Abstract class for representing a 3D perspective screen within the game.
Holds entities that can be drawn and interacted with.

Author: Nick Grimm
Version: 0.0.5 11/02/2015.</summary>
*/
#pragma once
#include "GameScreen.h"
#include "../Framework/T3Vector3.h"
#include "../Framework/T3Vector4.h"
#include "DrawableEntity3D.h"
#include <vector>
#include "../Framework/Light.h"
#include "../Framework/Camera.h"

using namespace std;

class GameScreen3D : public GameScreen
{
public:
	GameScreen3D(float x = 0.0f, float y = 0.0f, float width = 1.0f, float height = 1.0f)
		: GameScreen(x, y, width, height)
	{ }
	virtual ~GameScreen3D();
	void SetPlayer(DrawableEntity3D* value) { player = value; }
	void RemovePlayer() 
	{
		player = NULL; 
	}
	void AddDrawable(DrawableEntity3D* value);
	void RemoveDrawable(DrawableEntity3D* value);
	Light* AddPointLight(T3Vector3 lightPosition, float lightRadius, T3Vector4 diffuseColour, T3Vector4 specularColour, bool castsShadow);
	Light* AddDirectionalLight(T3Vector3 lightDirection, T3Vector4 diffuseColour, T3Vector4 specularColour);
	Light* AddSpotLight(T3Vector3 lightPosition, T3Vector3 lightTarget, T3Vector3 upVector, float lightRadius, float lightAngle, T3Vector4 diffuseColour, T3Vector4 specularColour, bool castsShadow);
	void RemoveLight(Light* value);
	void SetCamera(Camera* value);
	virtual void Update()
	{
		if (camera != NULL)
			camera->UpdateCamera();
	}
	// Move these to Light.h? Probably...
	/*bool ChangeLightColour(unsigned int lightReference, T3Vector4 newDiffuseColour, T3Vector4 newSpecularColour);
	bool ChangeLightPosition(unsigned int lightReference, T3Vector3 newLightPosition);
	bool ChangeLightRadius(unsigned int lightReference, float newLightRadius);
	bool ChangeLightAngle(unsigned int lightReference, float newLightAngle);
	bool ChangeLightTarget(unsigned int lightReference, T3Vector3 newLightTarget, T3Vector3 newUpVector);*/
private:
	DrawableEntity3D* player;
	vector<DrawableEntity3D*> drawables;
	vector<Light*> lights;
	Camera* camera;
};