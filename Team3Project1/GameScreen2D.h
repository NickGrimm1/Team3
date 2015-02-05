/**
Version History:
0.0.5 03/02/2015
0.0.6 05/02/2015
	Added Methods to add/remove entities. Needs to integrate with GraphicsEngine still.
*/

/**
<summary>Abstract class for representing a 2D orthographic screen within the game.
Holds entities that can be drawn and interacted with.

Author: Nick Grimm
Version: 0.0.5 05/02/2015.</summary>
*/
#pragma once
#include "GameScreen.h"
#include <vector>
#include "DoubleClickableEntity2D.h"

using namespace std;

class GameScreen2D : GameScreen
{
	// TODO: Implement GameScreen 2D.
public:
#pragma region Entity
	/**
	<summary>Adds an entity to the screen. Does not add to any other collection. Use addDrawable(...) to draw etc. Idempotent.</summary>
	<param name="autoScreenDeviation">True if scroll deviations are calculated. Use false to add many entities before calculating (more efficient). Default is true.</param>
    <param name="entity">The entity.</param>
	*/
	void AddEntity(Rectangle* entity, bool autoScreenDeviation = true)
    {
		for (int i = 0; i < entities.size(); i++)
			if (entities[i] == entity)
				return;

        entity->width = entity->width / 1.0f * width;
        entity->height = entity->height / 1.0f * height;
        entity->x = x + (entity->x / 1.0f * width);
        entity->y = y + (entity->y / 1.0f * height);
		entities.push_back(entity);
        if (autoScreenDeviation)
        {
            GetMaxScreenDeviation();
        }
    }
	/**
	<summary>Removes this entity from the screen.</summary>
    <param name="entity">The entity.</param>
	*/
    void RemoveEntity(Rectangle* entity)
    {
		for (vector<Rectangle*>::iterator i = entities.begin(); i != entities.end(); i++)
		{
			if (*i == entity)
			{
				// TODO: Ensure that this is also removed from Graphics & Input.
				entities.erase(i);
				break;
			}
		}
        GetMaxScreenDeviation();
    }
#pragma endregion
#pragma region Drawables
	/**
	<summary>Adds a drawable entity. Idempotent.</summary>
	<param name="autoScreenDeviation">True if scroll deviations are calculated. Use false to add many entities before calculating (more efficient). Default is true.</param>
    <param name="drawable">The entity.</param>
	*/
    void AddDrawable(DrawableEntity2D* drawable, bool autoScreenDeviation = true)
    {
		for (int i = 0; i < drawables.size(); i++)
			if (drawables[i] == drawable)
				return;

        AddEntity(drawable, autoScreenDeviation);
    }
	/**
	<summary>Removes a drawable entity. Idempotent.</summary>
    <param name="drawable">The entity.</param>
	*/
    void RemoveDrawable(DrawableEntity2D* drawable)
    {
		for (vector<DrawableEntity2D*>::iterator i = drawables.begin(); i != drawables.end(); i++)
		{
			if (*i == drawable)
			{
				// TODO: Ensure that this is also removed from Graphics & Input.
				drawables.erase(i);
				break;
			}
		}

        GetMaxScreenDeviation();
        Scroll(Vector2(), Vector2());
    }
#pragma endregion
#pragma region Clickables
	/**
	<summary>Adds a clickable entity. Also adds to drawables. Idempotent.</summary>
	<param name="autoScreenDeviation">True if scroll deviations are calculated. Use false to add many entities before calculating (more efficient). Default is true.</param>
    <param name="clickable">The entity.</param>
	*/
	void AddClickable(ClickableEntity2D* clickable, bool autoScreenDeviation = true)
    {
        for (int i = 0; i < clickables.size(); i++)
			if (clickables[i] == clickable)
				return;

		clickables.push_back(clickable);
        AddDrawable(clickable, false);
        AddEntity(clickable, autoScreenDeviation);
    }
	/**
	<summary>Removes a clickable entity. Provides an option to leave the entity in draw, but remove interactivity. Idempotent.</summary>
	<param name="removeFromDraw">Remove the entity from draw. Default is true.</param>
    <param name="clickable">The entity.</param>
	*/
	void RemoveClickable(ClickableEntity2D* clickable, bool removeFromDraw = true)
    {
        for (vector<ClickableEntity2D*>::iterator i = clickables.begin(); i != clickables.end(); i++)
		{
			if (*i == clickable)
			{
				clickables.erase(i);
				break;
			}
		}

        if (removeFromDraw)
            RemoveDrawable(clickable);
    }
#pragma endregion
#pragma region DoubleClickables
	/**
	<summary>Adds a double clickable entity. Also adds to clickables. Idempotent.</summary>
	<param name="autoScreenDeviation">True if scroll deviations are calculated. Use false to add many entities before calculating (more efficient). Default is true.</param>
    <param name="doubleClickable">The entity.</param>
	*/
	void AddDoubleClickable(DoubleClickableEntity2D* doubleClickable, bool autoScreenDeviation = true)
    {
		for (int i = 0; i < doubleClickables.size(); i++)
		if (doubleClickables[i] == doubleClickable)
			return;

		doubleClickables.push_back(doubleClickable);
        AddClickable(doubleClickable, autoScreenDeviation);
    }
	/**
	<summary>Removes a double clickable entity. Also removes from clickables. Provides an option to leave the entity in draw, but remove interactivity. Idempotent.</summary>
	<param name="removeFromDraw">True if no longer drawable. Default is true.</param>
    <param name="doubleClickable">The entity.</param>
	*/
	void RemoveDoubleClickable(DoubleClickableEntity2D* doubleClickable, bool removeFromDraw = true)
    {
        for (vector<DoubleClickableEntity2D*>::iterator i = doubleClickables.begin(); i != doubleClickables.end(); i++)
		{
			if (*i == doubleClickable)
			{
				doubleClickables.erase(i);
				break;
			}
		}

        RemoveClickable(doubleClickable);
        if (removeFromDraw)
        {
            RemoveDrawable(doubleClickable);
        }
    }
#pragma endregion
private:
	vector<Rectangle*> entities;
	vector<DrawableEntity2D*> drawables;
	vector<ClickableEntity2D*> clickables;
	vector<DoubleClickableEntity2D*> doubleClickables;
};