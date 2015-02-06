/**
Version History:
1.0.0 05/02/2015
1.0.1 07/02/2015
	Removed inheritance from DrawableEntity2D to prevent problems of multiple inheritance.
*/

/**
<summary>Defines an object that can respond to a single click.

Author: Nick Grimm
Version: 1.0.0 05/02/2015.</summary>
*/
#pragma once
#include "DrawableEntity2D.h"

class ClickableEntity2D
{
public:
	/**
	<summary>Constructor.</summary>
	<param name='actionOnClick'>Function pointer to the action to take on click.</param>m>
	*/
	ClickableEntity2D(void (*actionOnClick)(float, float)) : actionOnClick(actionOnClick)
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
	/**
	<summary>Sets the action to take on click.</summary>
	<param name='value'>Function pointer to the action to take on click.</param>m>
	*/
	void SetActionOnClick(void (*value)(float, float)) { actionOnClick = value; }
protected:
	void (*actionOnClick)(float x, float y);
};