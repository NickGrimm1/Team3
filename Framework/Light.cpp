/**
<summary>Light
Implements a Light class that allows creation of Spot, Point and Directional lights for traditional and deferred rendering

Author: Derek Kelly
Version: 0.0.1 04/02/2015</summary>
*/

#include "Light.h"
#include "Shader.h"

Vector3 Light::GetPosition() const {
	return position;
}

void Light::SetPosition(Vector3 pos) {
	position = pos;
}

Vector3 Light::GetDirection() const {
	return direction;
}

void Light::SetDirection(Vector3 dir) {
	direction = dir;
}

float Light::GetRadius() const {
	return radius;
}

void Light::SetRadius(float rad) {
	radius = rad;
}

Vector4 Light::GetDiffuseColour() const {
	return diffuseColour;
}

void Light::SetDiffuseColour(Vector4 col) {
	diffuseColour = col;
}

Vector4 Light::GetSpecularColour() const {
	return specularColour;
}

void Light::SetSpecularColour(Vector4 spec) {
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

void Light::SetType(unsigned int t) {
	type = t;
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
	position = Vector3(0,0,0);
	radius = 0.0f;
}

PointLight::PointLight(Vector3 pos, Vector4 col, Vector4 spec, float rad, unsigned int shadowTex) {
	type = POINT_LIGHT_TYPE;
	position = pos;
	direction = Vector3(0,0,0);
	diffuseColour = col;
	specularColour = spec;
	radius = rad; 
	angle = 0;
	shadowTexID = shadowTex;
}

Matrix4 PointLight::GetViewMatrix(Vector3 target) {
	return Matrix4::BuildViewMatrix(position, target);
}

Matrix4 PointLight::GetProjectionMatrix() {
	return Matrix4::Perspective(1.0f, radius, 1, 45.0f);
}

Matrix4 PointLight::GetModelMatrix() {
	return Matrix4::Translation(position) *	
			Matrix4::Scale(Vector3(radius, radius, radius));
}

DirectionalLight::DirectionalLight(Vector3 dir, Vector4 col, Vector4 spec, unsigned int shadowTex) {
	type = DIRECTIONAL_LIGHT_TYPE;
	position = Vector3(0,0,0);
	direction = dir;
	direction.Normalise();
	diffuseColour = col;
	specularColour = spec;
	radius = 0; 
	angle = 0;
	shadowTexID = shadowTex;
}

// TODO: Check these & assign
float DirectionalLight::znear = 0;
float DirectionalLight::zfar = 0;
float DirectionalLight::right = 0;
float DirectionalLight::left = 0;
float DirectionalLight::top = 0;
float DirectionalLight::bottom = 0;

Matrix4 DirectionalLight::GetViewMatrix(Vector3 target) {
	Matrix4 m = Matrix4::BuildViewMatrix(target - (direction * 100.0f), direction);
	return m;
}

Matrix4 DirectionalLight::GetProjectionMatrix() {
	Matrix4 m = Matrix4::Orthographic(znear, zfar, right, left, top, bottom);
	return m;
}

void DirectionalLight::UpdateLightVolume(float n, float f, float r, float l, float t, float b) {
	znear = n;
	zfar = f;
	right = r;
	left = l;
	top = t;
	bottom = b;
}

Matrix4 DirectionalLight::GetModelMatrix() {
	return Matrix4();
}

SpotLight::SpotLight(Vector3 pos, Vector3 target, Vector3 up_vec, Vector4 col, Vector4 spec, float spot_rad, float spot_angle, unsigned int shadowTex) {
	type = SPOTLIGHT_LIGHT_TYPE;
	position = pos;
	direction = Vector3(target.x - pos.x, target.y - pos.y, target.z - pos.z); //remove once nick sorts out Vector subtraction
	direction.Normalise();
	diffuseColour = col;
	specularColour = spec;
	radius = spot_rad; 
	angle = spot_angle;
	up = up_vec; // an up vector for rotating the spot light
	up.Normalise();
	shadowTexID = shadowTex;
}

Matrix4 SpotLight::GetViewMatrix(Vector3 target) { //Ignore target spot lights, we have a defined direction + position
	return Matrix4::BuildViewMatrix(position, position + direction);
}

Matrix4 SpotLight::GetProjectionMatrix() { //Ignore target spot lights, we have a defined direction + position
	float cone_base_radius = tan(angle / 2.0f * PI / 180.0f) * radius;
	Matrix4 shadowProjMatrix = Matrix4::Perspective(25.0f, radius, 1, angle); // move projection closer to object increase performance of z-buffer
	return shadowProjMatrix;
}

Matrix4 SpotLight::GetModelMatrix() {
	// Calculate model matrices for deferred rendering
	float cone_base_radius = tan(angle / 2.0f * PI / 180.0f) * radius;
		
	Vector3 s = Vector3::Cross(up, direction);
	s.Normalise();
	Vector3 u = Vector3::Cross(s, direction);
	u.Normalise();
	Matrix4 Rotation = Matrix4::Rotation(u, direction, s);

	return Matrix4::Translation(position) * Rotation * Matrix4::Scale(Vector3(cone_base_radius, radius, cone_base_radius)); 
}

Matrix4 SpotLight::GetBaseModelMatrix() {
	float cone_base_radius = tan(angle / 2.0f * PI / 180.0f) * radius;
		
	Vector3 s = Vector3::Cross(up, direction);
	s.Normalise();
	Vector3 u = Vector3::Cross(s, direction);
	u.Normalise();
	Matrix4 Rotation = Matrix4::Rotation(u, direction, s);

	return Matrix4::Translation(position) * Rotation * Matrix4::Translation(Vector3(0, radius, 0)) * 
		Matrix4::Scale(Vector3(cone_base_radius, radius, cone_base_radius)) * Matrix4::Rotation(180.0f, Vector3(1,0,0)); 
}

/* Spot lights - emits a cone of light angle of alpha
A fragment will be lit if
1. Within spotlight radius (not beyond max distance)
2. within cone of light
dot(a,b) = |a| * |b| * cos angle
calc dot product between cones central axis and vector from cone origin and fragment (normalised)
if greater than cos alpha, then within cone.
*/

bool SpotLight::IsInSpotlight(Vector3 world_pos, Light* light) {
	//Vector3 light_to_world = (world_pos - light->GetPosition());
	Vector3 light_to_world = Vector3(world_pos.x - light->GetPosition().x, world_pos.y - light->GetPosition().y, world_pos.z - light->GetPosition().z);
	light_to_world.Normalise();
	float dotprod = Vector3::Dot(light_to_world, light->GetDirection());
	float coscone = cos(light->GetAngle() / 2.0f * PI / 180.0f); //cause expects radians
	if (coscone <= dotprod) { // inside angle of cone, determine whether beyond radius
		//float dist = (world_pos - light->GetPosition()).Length();
		float dist = light_to_world.Length();
		return (dist < light->GetRadius());
	}
	return false; // not in cone shape
}