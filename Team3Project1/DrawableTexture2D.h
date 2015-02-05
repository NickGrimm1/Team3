/**
<summary>Represents a drawable image.

Author: Nick Grimm
Version: 0.0.5 05/02/2015.</summary>
*/
#pragma once
#include "DrawableEntity2D.h"

class DrawableTexture2D : public DrawableEntity2D
{
public:
	/**
	<summary>Constructor.</summary>
	<param name='x'>The x position of the entity.</param>
	<param name='y'>The y position of the entity.</param>
	<param name='width'>The width of the entity.</param>
	<param name='height'>The height of the entity.</param>
	<param name='blendColor'>The colour. Default is White (1,1,1,1).</param>
	<param name='rotation'>The rotation in degrees from up. Default is 0.</param>
	<param name='origin'>The origin around which this object will rotate, relative to the object's draw area. Default is the center (0.5, 0.5).</param>
	<param name='texture'>The texture.</param>
	*/
	DrawableTexture2D(float x, float y, float width, float height, Texture* texture, float rotation = 0, Vector2 origin = Vector2(0.5f, 0.5f), Vector4 blendColor = Vector4(1.0f, 1.0f, 1.0f ,1.0f)) : DrawableEntity2D(x, y, width, height, blendColor), texture(texture), rotation(rotation), origin(origin)
	{ }

	/**
	<summary>Gets the texture.</summary>
	*/
	Texture* GetTexture() const;
	/**
	<summary>Sets the texture.</summary>
	<param name='value'>The texture.</param>
	*/
	void SetTexture(Texture* value);
	/**
	<summary>Gets the rotation.</summary>
	*/
	float GetRotation() const;
	/**
	<summary>Sets the rotation.</summary>
	<param name='value'>The rotation in degrees from up. Default is 0.</param>
	*/
	void SetRotation(const float value);
	/**
	<summary>Gets the origin of rotation.</summary>
	*/
	Vector2& GetOrigin() const;
	/**
	<summary>Sets the origin of rotation.</summary>
	<param name='value'>The origin around which this object will rotate, relative to the object's draw area. Default is the center (0.5, 0.5).</param>
	*/
	void SetOrigin(const Vector2& value);
protected:
	Texture* texture;
	float rotation;
	Vector2 origin;
};