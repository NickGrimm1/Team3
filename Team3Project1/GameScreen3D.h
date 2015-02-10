/**
<summary>Abstract class for representing a 3D perspective screen within the game.
Holds entities that can be drawn and interacted with.

Author: Nick Grimm
Version: 0.0.1 03/02/2015.</summary>
*/
#pragma once
#include "GameScreen.h"
#include "../Framework/Vector3.h"
#include "../Framework/Vector4.h"

class GameScreen3D : GameScreen
{
	// TODO: Implement GameScreen3D.
	
	// SetPlayer
	// RemovePlayer
	// AddDrawable
	// RemoveDrawable
	// unsigned int AddPointLight(Vector3 lightPosition, float lightRadius, Vector4 diffuseColour, Vector4 specularColour, bool castsShadow);
	// unsigned int AddDirectionalLight(Vector3 lightDirection, Vector4 diffuseColour, Vector4 specularColour);
	// unsigned int AddSpotLight(Vector3 lightPosition, Vector3 lightTarget, Vector3 upVector, float lightRadius, float lightAngle, Vector4 diffuseColour, Vector4 specularColour, bool castsShadow);
	// Remove Light

	// Move these to Light.h? Probably...
	/*bool ChangeLightColour(unsigned int lightReference, Vector4 newDiffuseColour, Vector4 newSpecularColour);
	bool ChangeLightPosition(unsigned int lightReference, Vector3 newLightPosition);
	bool ChangeLightRadius(unsigned int lightReference, float newLightRadius);
	bool ChangeLightAngle(unsigned int lightReference, float newLightAngle);
	bool ChangeLightTarget(unsigned int lightReference, Vector3 newLightTarget, Vector3 newUpVector);*/
};