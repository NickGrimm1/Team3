/**
<summary>Defines an object that can respond to a single click.

Author: Nick Grimm
Version: 1.0.0 05/02/2015.</summary>
*/
#pragma once
#include "DrawableEntity2D.h"

class ClickableEntity2D : public DrawableEntity2D
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
	ClickableEntity2D(float x, float y, float width, float height, void (*actionOnClick)(float, float), Vector4 blendColor = Vector4(1.0f, 1.0f, 1.0f ,1.0f)) : DrawableEntity2D(x, y, width, height, blendColor), actionOnClick(actionOnClick)
	{ }
	/**
	<summary>Process the action this object takes when it is clicked.</summary>
	<param name='x'>The x position of the click in resolution independent co-ordinates, relative to the boundaries of the object.</param>
	<param name='y'>The y position of the click in resolution independent co-ordinates, relative to the boundaries of the object.</param>
	*/
	void Click(float x, float y)
	{
		actionOnClick(x, y);
	}
protected:
	void (*actionOnClick)(float x, float y);
};