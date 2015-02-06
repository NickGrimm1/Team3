/**
<summary>Represents a 2 dimensional selectable entity. Reacts to select events (Mouse Hover etc).

Author: Nick Grimm
Version: 1.0.0 07/02/2015.</summary>
*/
#pragma once
#include "DrawableEntity2D.h"
#include <vector>

using namespace std;

class SelectableEntity2D : public T3Rectangle
{
public:
	/**
	<summary>Constructor.</summary>
	<param name='actionOnClick'>Function pointer to the action to take on click.</param>m>
	*/
	SelectableEntity2D(float x, float y, float width, float height, void (*actionOnSelect)()) : T3Rectangle(x, y, width, height), actionOnSelect(actionOnSelect)
	{ }
	/**
	<summary>Process the action this object takes when it is clicked.</summary>
	<param name='x'>The x position of the click in resolution independent co-ordinates, relative to the boundaries of the object.</param>
	<param name='y'>The y position of the click in resolution independent co-ordinates, relative to the boundaries of the object.</param>
	*/
	void Select()
	{
		actionOnSelect();
	}
	/**
	<summary>Sets the action to take on select.</summary>
	<param name='value'>Function pointer to the action to take on select.</param>m>
	*/
	void SetActionOnSelect(void (*value)()) { actionOnSelect = value; }
	vector<DrawableEntity2D*> GetDrawables() { return drawables; }
protected:
	void (*actionOnSelect)();
	vector<DrawableEntity2D*> drawables;
};
