/**
<summary>Represents a 2 dimensional selectable entity. Reacts to select events (Mouse Hover etc).

Author: Nick Grimm
Version: 1.0.0 07/02/2015.</summary>
*/
#pragma once
#include "DrawableEntity2D.h"
//#include "GameScreen2D.h"
//#include "MainMenu.h"
#include <vector>

class GameScreen2D;

using namespace std;

class SelectableEntity2D : public T3Rectangle
{
public:
	/**
	<summary>Constructor.</summary>
	<param name='actionOnClick'>Function pointer to the action to take on click.</param>m>
	*/
	SelectableEntity2D(float x, float y, float width, float height, void (GameScreen2D::*actionOnSelect)(), void (GameScreen2D::*actionOnUnselect)(), GameScreen2D* arg ) 
		: T3Rectangle(x, y, width, height), actionOnSelect(actionOnSelect), actionOnUnselect(actionOnUnselect), arg(arg)
	{ }
	/**
	<summary>Process the action this object takes when it is clicked.</summary>
	<param name='x'>The x position of the click in resolution independent co-ordinates, relative to the boundaries of the object.</param>
	<param name='y'>The y position of the click in resolution independent co-ordinates, relative to the boundaries of the object.</param>
	*/
	void Select()
	{
		(arg->*actionOnSelect)();
	}

	void UnSelect()
	{
		(arg->*actionOnUnselect)();
	}
	/**
	<summary>Sets the action to take on select.</summary>
	<param name='value'>Function pointer to the action to take on select.</param>m>
	*/

	void SetActionOnSelect(void (GameScreen2D::*value)()) { actionOnSelect = value; }
	void SetActionOnUnSelect(void (GameScreen2D::*value)()) { actionOnUnselect = value; }
	vector<DrawableEntity2D*> GetDrawables() { return drawables; }
protected:
	GameScreen2D* arg;
	void (GameScreen2D::*actionOnSelect)();
	void (GameScreen2D::*actionOnUnselect)();
	vector<DrawableEntity2D*> drawables;
};
