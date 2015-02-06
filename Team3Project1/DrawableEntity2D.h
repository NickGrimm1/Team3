/**
Version History:
0.0.5 05/02/2015
0.0.6 06/02/2015
	Added Type.
*/

/**
<summary>Defines an object that can be drawn to the screen.

Author: Nick Grimm
Version: 0.0.6 06/02/2015.</summary>
*/
#pragma once
#include "T3Rectangle.h"
#include "../Framework/Vector4.h"

namespace DrawableType
{
	enum Type
	{
		Texture,
		Text,
	};
}

class DrawableEntity2D : public T3Rectangle
{
public:
	/**
	<summary>Constructor.</summary>
	<param name='x'>The x position of the entity.</param>
	<param name='y'>The y position of the entity.</param>
	<param name='width'>The width of the entity.</param>
	<param name='height'>The height of the entity.</param>
	<param name='blendColor'>The colour. Default is White (1,1,1,1).</param>
	*/
	DrawableEntity2D(float x, float y, float width, float height, DrawableType::Type type, Vector4 blendColor = Vector4(1.0f, 1.0f, 1.0f ,1.0f)) : T3Rectangle(x, y, width, height), blendColor(blendColor), type(type)
	{ }

	/**
	<summary>Gets the drawable type of this object.</summary>
	*/
	DrawableType::Type GetType() { return type; }
	/**
	<summary>Gets the colour to blend this drawable with.</summary>
	*/
	Vector4 GetColor() const { return blendColor; }
	/**
	<summary>Sets the colour to blend this drawable with.</summary>
	<param name='value'>The colour.</param>
	*/
	void SetColor(const Vector4& value) { blendColor = value; }
protected:
	Vector4 blendColor;
	DrawableType::Type type;
};