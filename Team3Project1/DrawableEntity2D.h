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
#include "../Framework/T3Vector4.h"

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
	<param name='depth>The depth of the entity on screen.</param>
	<param name='width'>The width of the entity.</param>
	<param name='height'>The height of the entity.</param>
	<param name='blendColor'>The colour. Default is White (1,1,1,1).</param>
	*/
	DrawableEntity2D(float x, float y, int depth, float width, float height, DrawableType::Type type, T3Vector4 blendColor = T3Vector4(1.0f, 1.0f, 1.0f ,1.0f)) : T3Rectangle(x, y, width, height), blendColor(blendColor), type(type), depth(depth)
	{ }

	virtual ~DrawableEntity2D() {}

	/**
	<summary>Gets the drawable type of this object.</summary>
	*/
	DrawableType::Type GetType() { return type; }
	/**
	<summary>Gets the colour to blend this drawable with.</summary>
	*/
	T3Vector4 GetColor() const { return blendColor; }
	/**
	<summary>Sets the colour to blend this drawable with.</summary>
	<param name='value'>The colour.</param>
	*/
	void SetColor(const T3Vector4& value) { blendColor = value; }
	/**
	<summary>Gets the drawable entitys depth on screen. Allows it to be drawn behind/in front off other entities.</summary>
	*/
	int GetDepth() const {return depth;}
	/**
	<summary>Sets the drawable entitys depth on screen.</summary>
	<param name='value'>The depth.</param>
	*/
	void SetColor(const int& value) { depth = value; }
	
protected:
	int depth;
	T3Vector4 blendColor;
	DrawableType::Type type;
};