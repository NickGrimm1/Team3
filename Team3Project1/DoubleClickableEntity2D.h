/**
<summary>Defines an object that can respond to a double click.

Author: Nick Grimm
Version: 0.0.5 05/02/2015.</summary>
*/
#pragma once
#include "ClickableEntity2D.h"

class DoubleClickableEntity2D : public ClickableEntity2D
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
	DoubleClickableEntity2D(float x, float y, float width, float height, void (*actionOnClick)(float, float), void (*actionOnDoubleClick)(float, float), Vector4 blendColor = Vector4(1.0f, 1.0f, 1.0f ,1.0f)) : ClickableEntity2D(x, y, width, height, actionOnClick, blendColor), actionOnDoubleClick(actionOnDoubleClick)
	{ }
	/**
	<summary>Process the action this object takes when it is clicked.</summary>
	<param name='x'>The x position of the click in resolution independent co-ordinates, relative to the boundaries of the object.</param>
	<param name='y'>The y position of the click in resolution independent co-ordinates, relative to the boundaries of the object.</param>
	*/
	void DoubleClick(float x, float y)
	{
		actionOnDoubleClick(x, y);
	}
protected:
	void (*actionOnDoubleClick)(float x, float y);
};