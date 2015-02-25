#pragma once

/**
<summary>Light
Implements a Light class that allows creation of Spot, Point and Directional lights for traditional and deferred rendering

Author: Derek Kelly
Version: 0.0.1 04/02/2015</summary>
*/

#include "T3Vector3.h"
#include "T3Vector4.h"
#include "T3Matrix4.h"

#define POINT_LIGHT_TYPE 0
#define DIRECTIONAL_LIGHT_TYPE 1
#define SPOTLIGHT_LIGHT_TYPE 2

class Light
{
public:

	Light(void){}
	~Light(void){}

	T3Vector3 GetPosition() const;
	void SetPosition(T3Vector3 pos);

	T3Vector3 GetDirection() const;
	void SetDirection(T3Vector3 dir);

	float GetRadius() const;
	void SetRadius(float rad);

	T3Vector4 GetDiffuseColour() const;
	void SetDiffuseColour(T3Vector4 col);

	T3Vector4 GetSpecularColour() const;
	void SetSpecularColour(T3Vector4 spec);

	float GetAngle() const;
	void SetAngle(float a);

	unsigned int GetType() const; // POINT, SPOT or DIRECTIONAL

	virtual T3Matrix4 GetModelMatrix() = 0; // returns the model matrix for the camera
	virtual T3Matrix4 GetViewMatrix(T3Vector3 target) = 0; // returns the view matrix for the camera
	virtual T3Matrix4 GetProjectionMatrix() = 0; // returns the projection matrix for the camera
	
	void BindLight() const; // Binds the light source's data into the currently bound shader object
	void BindLight(unsigned int i) const; // Binds the light source's data into the currently bound shader object at the requested array index

	unsigned int GetShadowTexture() const {return shadowTexID;}
	void SetShadowTexture(unsigned int tex) {shadowTexID = tex;}
	
protected:
	T3Vector3 position;
	T3Vector3 direction; // for spot/directional lights
	T3Vector4 diffuseColour;
	T3Vector4 specularColour; // separate specular colour
	float radius;
	float angle; // spot lights only
	unsigned int type;
	unsigned int shadowTexID;
};

class PointLight : public Light {
public:
	PointLight();
	PointLight(T3Vector3 pos, T3Vector4 col, T3Vector4 spec, float rad, unsigned int shadowTex);

	virtual T3Matrix4 GetViewMatrix(T3Vector3 target);
	virtual T3Matrix4 GetProjectionMatrix();
	virtual T3Matrix4 GetModelMatrix();
};

class DirectionalLight : public Light {
public:
	DirectionalLight(T3Vector3 dir, T3Vector4 col, T3Vector4 spec, unsigned int shadowTex);
	virtual T3Matrix4 GetViewMatrix(T3Vector3 target);
	virtual T3Matrix4 GetProjectionMatrix();
	virtual T3Matrix4 GetModelMatrix();
	static void UpdateLightVolume(T3Vector3& min, T3Vector3& max);
private:
	static T3Vector3 boundingMin;
	static T3Vector3 boundingMax;
};

class SpotLight	: public Light {
public:
	SpotLight(T3Vector3 pos, T3Vector3 target, T3Vector3 up_vec, T3Vector4 col, T3Vector4 spec, float spot_rad, float spot_angle, unsigned int shadowTex);

	virtual T3Matrix4 GetViewMatrix(T3Vector3 target); // Ignore target, spot lights have a defined direction + position
	virtual T3Matrix4 GetProjectionMatrix();
	virtual T3Matrix4 GetModelMatrix();
	T3Matrix4 GetBaseModelMatrix(); // The model matrix for the cone base
	static bool IsInSpotlight(T3Vector3 world_pos, Light* l);
	T3Vector3 GetUp() {return up;}
protected:
	T3Vector3 up;
};