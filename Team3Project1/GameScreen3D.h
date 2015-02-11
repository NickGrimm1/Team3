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
#include "../Framework/Vector3.h"
#include "../Framework/Vector4.h"
#include "DrawableEntity3D.h"
#include <vector>
#include "../Framework/Light.h"
#include "../Framework/Camera.h"

using namespace std;

class GameScreen3D : GameScreen
{
public:
	void SetPlayer(DrawableEntity3D* value) { player = value; }
	void RemovePlayer() 
	{
		player = NULL; 
	}
	void AddDrawable(DrawableEntity3D* value);
	void RemoveDrawable(DrawableEntity3D* value);
	void AddLight(Light* value);
	void RemoveLight(Light* value);
	void SetCamera(Camera* value);
	virtual void Update()
	{
		if (camera != NULL)
			camera->UpdateCamera();
	}
	// Move these to Light.h? Probably...
	/*bool ChangeLightColour(unsigned int lightReference, Vector4 newDiffuseColour, Vector4 newSpecularColour);
	bool ChangeLightPosition(unsigned int lightReference, Vector3 newLightPosition);
	bool ChangeLightRadius(unsigned int lightReference, float newLightRadius);
	bool ChangeLightAngle(unsigned int lightReference, float newLightAngle);
	bool ChangeLightTarget(unsigned int lightReference, Vector3 newLightTarget, Vector3 newUpVector);*/
protected:
	DrawableEntity3D* player;
	vector<DrawableEntity3D*> drawables;
	vector<Light*> lights;
	Camera* camera;
};