/**
<summary>Defines an object that can be drawn to the screen.

Author: Nick Grimm
Version: 0.0.5 10/02/2015.</summary>
*/
#pragma once
#include "../Framework/Mesh.h"
#include "../Framework/Texture.h"

class DrawableEntity3D
{
public:
	/**
	<summary>Constructor.</summary>
	<param name='mesh'>The model mesh.</param>
	<param name='shader'>The shader to use.</param>
	<param name='texture'>The texture to use.</param>
	<param name='bumpTexture'>The bump map to use.</param>
	<param name='boundingRadius'>The bounding radius of the object. Denotes the object's maximum size for view frustrum culling.</param>
	<param name='origin'>The origin position of the model in world space relative to its parent. Default is (0, 0, 0).</param>
	<param name='rotation'>The rotation of the model in world space relative to its parent. Default is (0, 0, 0).</param>
	<param name='scale'>The scale of the model in world space relative to its parent. Default is (0, 0, 0).</param>
	*/
	DrawableEntity3D(Mesh* Mesh, Shader* shader, Texture* texture, Texture* bumpTexture, float boundingRadius, const Vector3& origin = Vector3(), const Quaternion& rotation = Quaternion(), const Vector3& scale = Vector3()) 
		: mesh(mesh), shader(shader), texture(texture), origin(origin), rotation(rotation), scale(scale), boundingRadius(boundingRadius), bumpTexture(bumpTexture)
	{ }
	
	/**
	<summary>Gets the model mesh for this entity.</summary>
	*/
	Mesh* GetMesh() const { return mesh; }
	/**
	<summary>Sets the model mesh for this entity.</summary>
	<param name='value'>The mesh.</param>
	*/
	void SetMesh(Mesh* value) { mesh = value; }
	/**
	<summary>Gets the shader to use for this entity.</summary>
	*/
	Shader* GetShader() const { return shader; }
	/**
	<summary>Sets the shader to use for this entity.</summary>
	<param name='value'>The shader.</param>
	*/
	void SetShader(Shader* value) { shader = value; }
	/**
	<summary>Gets the texture to use for this entity.</summary>
	*/
	Texture* GetTexture() const { return texture; }
	/**
	<summary>Sets the texture to use for this entity.</summary>
	<param name='value'>The texture.</param>
	*/
	void SetTexture(Texture* value) { texture = value; }
	/**
	<summary>Gets the origin position of this entity relative to its parent.</summary>
	*/
	Vector3 GetOriginPosition() const { return origin; }
	/**
	<summary>Sets the origin position of this entity relative to its parent.</summary>
	<param name='value'>The position.</param>
	*/
	void SetOriginPosition(const Vector3& value) { origin = value; }
	/**
	<summary>Gets the scale of this entity relative to its parent.</summary>
	*/
	Vector3 GetScale() const { return scale; }
	/**
	<summary>Sets the scale of this entity relative to its parent.</summary>
	<param name='value'>The scale.</param>
	*/
	void SetScale(const Vector3& value) { scale = value; }
	/**
	<summary>Gets the rotation of this entity relative to its parent.</summary>
	<param name='value'>The rotation.</param>
	*/
	Quaternion GetRotation() const { return rotation; }
	/**
	<summary>Sets the rotation of this entity relative to its parent.</summary>
	<param name='value'>The rotation.</param>
	*/
	void SetRotation(const Quaternion& value) { rotation = value; }
	
	/**
	<summary>Adds the specified amount of movement to this object's position relative to its parent.</summary>
	<param name='amount'>The amount to move by.</param>
	*/
	void Move(const Vector3& amount) { origin += amount; }
	/**
	<summary>Adds the specified amount or rotation to this object relative to its parent.</summary>
	<param name='value'>The amount to rotate by.</param>
	*/
	void AddRotation(const Quaternion& amount) { rotation = rotation * amount; }
	/**
	<summary>Adds the specified amount of scale to this object relative to its parent.</summary>
	<param name='value'>The amount to scale by.</param>
	*/
	void AddScale(const Vector3& amount) { scale += amount; }
protected:
	Mesh* mesh;
	Shader* shader;
	Texture* texture;
	Vector3 scale;
	Vector3 origin;
	Quaternion rotation;
	float boundingRadius;
	Texture* bumpTexture;
};