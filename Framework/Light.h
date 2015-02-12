#pragma once

/**
<summary>Light
Implements a Light class that allows creation of Spot, Point and Directional lights for traditional and deferred rendering

Author: Derek Kelly
Version: 0.0.1 04/02/2015</summary>
*/

#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"

class Mesh;

#define POINT_LIGHT_TYPE 0
#define DIRECTIONAL_LIGHT_TYPE 1
#define SPOTLIGHT_LIGHT_TYPE 2

class Light
{
public:

	Light(void){}
	~Light(void){}

	Vector3 GetPosition() const;
	void SetPosition(Vector3 pos);

	Vector3 GetDirection() const;
	void SetDirection(Vector3 dir);

	float GetRadius() const;
	void SetRadius(float rad);

	Vector4 GetDiffuseColour() const;
	void SetDiffuseColour(Vector4 col);

	Vector4 GetSpecularColour() const;
	void SetSpecularColour(Vector4 spec);

	float GetAngle() const;
	void SetAngle(float a);

	unsigned int GetType() const; // POINT, SPOT or DIRECTIONAL
	void SetType(unsigned int t);

	virtual Matrix4 GetViewMatrix(Vector3 target) = 0; // returns the view matrix for the camera
	virtual Matrix4 GetProjectionMatrix() = 0; // returns the projection matrix for the camera
	
	virtual void DrawLightDeferred(Vector3 camera_pos) = 0;
	
	void BindLight() const; // Binds the light source's data into the currently bound shader object
	void BindLight(unsigned int i) const; // Binds the light source's data into the currently bound shader object at the requested array index

	unsigned int GetShadowTexture() const {return shadowTexID;}

protected:
	Mesh* m; // mesh for use in deferred rendering
	Vector3 position;
	Vector3 direction; // for spot/directional lights
	Vector4 diffuseColour;
	Vector4 specularColour; // separate specular colour
	float radius;
	float angle; // spot lights only
	unsigned int type;
	unsigned int shadowTexID;
};

class PointLight : public Light {
public:
	PointLight();
	PointLight(Vector3 pos, Vector4 col, Vector4 spec, float rad, unsigned int shadowTex);

	virtual Matrix4 GetViewMatrix(Vector3 target);
	virtual Matrix4 GetProjectionMatrix();
	virtual void DrawLightDeferred(Vector3 camera_pos);
	static void SetMesh(Mesh* m) {lightMesh = m;}
protected:
	static Mesh* lightMesh;
};

class DirectionalLight : public Light { // Only limited implementation
public:
	DirectionalLight(Vector3 dir, Vector4 col, Vector4 spec);
	virtual Matrix4 GetViewMatrix(Vector3 target);
	virtual Matrix4 GetProjectionMatrix();
	virtual void DrawLightDeferred(Vector3 camera_pos);
	static void UpdateLightVolume(float n, float f, float r, float l, float t, float b);
private:
	static float znear, zfar, right, left, top, bottom;
};

class SpotLight	: public Light {
public:
	SpotLight(Vector3 pos, Vector3 target, Vector3 up_vec, Vector4 col, Vector4 spec, float spot_rad, float spot_angle, unsigned int shadowTex);

	virtual Matrix4 GetViewMatrix(Vector3 target); // Ignore target, spot lights have a defined direction + position
	virtual Matrix4 GetProjectionMatrix();
	virtual void DrawLightDeferred(Vector3 camera_pos);
	static bool IsInSpotlight(Vector3 world_pos, Light* l);
	static void SetConeMesh(Mesh* m) {lightCone = m;}
	static void SetCircleMesh(Mesh* m) {lightBase = m;} 
	Vector3 GetUp() {return up;}
protected:
	static Mesh* lightCone;
	static Mesh* lightBase;
	Vector3 up;
};