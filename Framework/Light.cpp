/**
<summary>Light
Implements a Light class that allows creation of Spot, Point and Directional lights for traditional and deferred rendering

Author: Derek Kelly
Version: 0.0.1 04/02/2015</summary>
*/

#include "Light.h"
#include "Shader.h"

T3Vector3 Light::GetPosition() const {
	return position;
}

void Light::SetPosition(T3Vector3 pos) {
	position = pos;
}

T3Vector3 Light::GetDirection() const {
	return direction;
}

void Light::SetDirection(T3Vector3 dir) {
	direction = dir;
}

float Light::GetRadius() const {
	return radius;
}

void Light::SetRadius(float rad) {
	radius = rad;
}

T3Vector4 Light::GetDiffuseColour() const {
	return diffuseColour;
}

void Light::SetDiffuseColour(T3Vector4 col) {
	diffuseColour = col;
}

T3Vector4 Light::GetSpecularColour() const {
	return specularColour;
}

void Light::SetSpecularColour(T3Vector4 spec) {
	specularColour = spec;
}

float Light::GetAngle() const {
	return angle;
}

void Light::SetAngle(float a) {
	angle = a;
}

unsigned int Light::GetType() const {
	return type;
}

void Light::BindLight() const {
	GLint shaderObject = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &shaderObject);
	glUniform4fv(glGetUniformLocation(shaderObject, "lightColour"), 1, (float*) &diffuseColour);
	glUniform4fv(glGetUniformLocation(shaderObject, "lightSpecColour"), 1, (float*) &specularColour);
	glUniform3fv(glGetUniformLocation(shaderObject, "lightPos"), 1, (float*) &position);
	glUniform3fv(glGetUniformLocation(shaderObject, "lightDir"), 1, (float*) &direction);
	glUniform1f(glGetUniformLocation(shaderObject, "lightRadius"), radius);
	glUniform1f(glGetUniformLocation(shaderObject, "lightAngle"), angle);
	glUniform1i(glGetUniformLocation(shaderObject, "lightType"), type);
}

void Light::BindLight(unsigned int i) const { 
	char buffer[20];
	GLint shaderObject = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &shaderObject);
	sprintf_s(buffer, 20, "lightColour[%d]", i);
	glUniform4fv(glGetUniformLocation(shaderObject, buffer), 1, (float*) &diffuseColour);
	sprintf_s(buffer, 20, "lightSpecColour[%d]", i);
	glUniform4fv(glGetUniformLocation(shaderObject, buffer), 1, (float*) &specularColour);
	sprintf_s(buffer, 20, "lightPos[%d]", i);
	glUniform3fv(glGetUniformLocation(shaderObject, buffer), 1, (float*) &position);
	sprintf_s(buffer, 20, "lightDir[%d]", i);
	glUniform3fv(glGetUniformLocation(shaderObject, buffer), 1, (float*) &direction);
	sprintf_s(buffer, 20, "lightRadius[%d]", i);
	glUniform1f(glGetUniformLocation(shaderObject, buffer), radius);
	sprintf_s(buffer, 20, "lightAngle[%d]", i);
	glUniform1f(glGetUniformLocation(shaderObject, buffer), angle);
	sprintf_s(buffer, 20, "lightType[%d]", i);
	glUniform1i(glGetUniformLocation(shaderObject, buffer), type);
}

PointLight::PointLight() {
	type = POINT_LIGHT_TYPE;
	position = T3Vector3(0,0,0);
	radius = 0.0f;
}

PointLight::PointLight(T3Vector3 pos, T3Vector4 col, T3Vector4 spec, float rad, unsigned int shadowTex) {
	type = POINT_LIGHT_TYPE;
	position = pos;
	direction = T3Vector3(0,0,0);
	diffuseColour = col;
	specularColour = spec;
	radius = rad; 
	angle = 0;
	shadowTexID = shadowTex;
}

T3Matrix4 PointLight::GetViewMatrix(T3Vector3 target) {
	return T3Matrix4::BuildViewMatrix(position, target);
}

T3Matrix4 PointLight::GetProjectionMatrix() {
	return T3Matrix4::Perspective(1.0f, radius, 1, 90.0f);
}

T3Matrix4 PointLight::GetModelMatrix() {
	return T3Matrix4::Translation(position) *	
			T3Matrix4::Scale(T3Vector3(radius, radius, radius));
}

DirectionalLight::DirectionalLight(T3Vector3 dir, T3Vector4 col, T3Vector4 spec, unsigned int shadowTex) {
	type = DIRECTIONAL_LIGHT_TYPE;
	position = T3Vector3(0,0,0);
	direction = dir;
	direction.Normalise();
	diffuseColour = col;
	specularColour = spec;
	radius = 0; 
	angle = 0;
	shadowTexID = shadowTex;
}

T3Vector3 DirectionalLight::boundingMin = T3Vector3(0,0,0);
T3Vector3 DirectionalLight::boundingMax = T3Vector3(0,0,0);

T3Matrix4 DirectionalLight::GetViewMatrix(T3Vector3 target) {
	// Take target as average of scene bounding box
	T3Vector3 sceneCentre = T3Vector3((boundingMin.x + boundingMax.x) / 2.0f,
									(boundingMin.y + boundingMax.y) / 2.0f,
									(boundingMin.z + boundingMax.z) / 2.0f);

	T3Matrix4 m = T3Matrix4::BuildViewMatrix(sceneCentre - (direction * 100.0f), sceneCentre, T3Vector3(0,1,0));
	return m;
}

T3Matrix4 DirectionalLight::GetProjectionMatrix() {
	// Translate scene AABB into camera viewspace
	T3Matrix4 camMatrix = DirectionalLight::GetViewMatrix(T3Vector3(0,0,0));
	T3Vector3 camMin = camMatrix * boundingMin;
	T3Vector3 camMax = camMatrix * boundingMax;
	
	T3Matrix4 m = T3Matrix4::Orthographic(1, camMax.z, camMax.x, camMin.x, camMax.y, camMin.y);
//	T3Matrix4 m = T3Matrix4::Perspective(1.0f, 10000.0, 1, 90); // move projection closer to object increase performance of z-buffer
	return m;
}

void DirectionalLight::UpdateLightVolume(T3Vector3& min, T3Vector3& max) {
	boundingMin = min;
	boundingMax = max;
}

T3Matrix4 DirectionalLight::GetModelMatrix() {
	return T3Matrix4();
}

SpotLight::SpotLight(T3Vector3 pos, T3Vector3 target, T3Vector3 up_vec, T3Vector4 col, T3Vector4 spec, float spot_rad, float spot_angle, unsigned int shadowTex) {
	type = SPOTLIGHT_LIGHT_TYPE;
	position = pos;
	direction = T3Vector3(target.x - pos.x, target.y - pos.y, target.z - pos.z); //remove once nick sorts out Vector subtraction
	direction.Normalise();
	diffuseColour = col;
	specularColour = spec;
	radius = spot_rad; 
	angle = spot_angle;
	up = up_vec; // an up vector for rotating the spot light
	up.Normalise();
	shadowTexID = shadowTex;
}

T3Matrix4 SpotLight::GetViewMatrix(T3Vector3 target) { //Ignore target spot lights, we have a defined direction + position
	return T3Matrix4::BuildViewMatrix(position, position + direction, up);
}

T3Matrix4 SpotLight::GetProjectionMatrix() {
	return T3Matrix4::Perspective(1.0f, radius, 1, angle); // move projection closer to object increase performance of z-buffer
}

T3Matrix4 SpotLight::GetModelMatrix() {
	// Calculate model matrices for deferred rendering
	float cone_base_radius = tan(angle / 2.0f * PI / 180.0f) * radius;
		
	T3Vector3 s = T3Vector3::Cross(up, direction);
	s.Normalise();
	T3Vector3 u = T3Vector3::Cross(s, direction);
	u.Normalise();
	T3Matrix4 Rotation = T3Matrix4::Rotation(u, direction, s);

	return T3Matrix4::Translation(position) * Rotation * T3Matrix4::Scale(T3Vector3(cone_base_radius, radius, cone_base_radius)); 
}

T3Matrix4 SpotLight::GetBaseModelMatrix() {
	float cone_base_radius = tan(angle / 2.0f * PI / 180.0f) * radius;
		
	T3Vector3 s = T3Vector3::Cross(up, direction);
	s.Normalise();
	T3Vector3 u = T3Vector3::Cross(s, direction);
	u.Normalise();
	T3Matrix4 Rotation = T3Matrix4::Rotation(u, direction, s);

	return T3Matrix4::Translation(position) * Rotation * T3Matrix4::Translation(T3Vector3(0, radius, 0)) * 
		T3Matrix4::Scale(T3Vector3(cone_base_radius, radius, cone_base_radius)) * T3Matrix4::Rotation(180.0f, T3Vector3(1,0,0)); 
}

/* Spot lights - emits a cone of light angle of alpha
A fragment will be lit if
1. Within spotlight radius (not beyond max distance)
2. within cone of light
dot(a,b) = |a| * |b| * cos angle
calc dot product between cones central axis and vector from cone origin and fragment (normalised)
if greater than cos alpha, then within cone.
*/

bool SpotLight::IsInSpotlight(T3Vector3 world_pos, Light* light) {
	//T3Vector3 light_to_world = (world_pos - light->GetPosition());
	T3Vector3 light_to_world = T3Vector3(world_pos.x - light->GetPosition().x, world_pos.y - light->GetPosition().y, world_pos.z - light->GetPosition().z);
	light_to_world.Normalise();
	float dotprod = T3Vector3::Dot(light_to_world, light->GetDirection());
	float coscone = cos(light->GetAngle() / 2.0f * PI / 180.0f); //cause expects radians
	if (coscone <= dotprod) { // inside angle of cone, determine whether beyond radius
		//float dist = (world_pos - light->GetPosition()).Length();
		float dist = light_to_world.Length();
		return (dist < light->GetRadius());
	}
	return false; // not in cone shape
}