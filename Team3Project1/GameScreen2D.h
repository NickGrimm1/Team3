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
#include "ClickableEntity2D.h"

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
	void AddEntity(T3Rectangle* entity, bool autoScreenDeviation = true)
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
    void RemoveEntity(T3Rectangle* entity)
    {
		for (vector<T3Rectangle*>::iterator i = entities.begin(); i != entities.end(); i++)
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
    void AddDrawable(DrawableEntity2D* drawable, bool autoScreenDeviation = true);
	/**
	<summary>Removes a drawable entity. Idempotent.</summary>
    <param name="drawable">The entity.</param>
	*/
    void RemoveDrawable(DrawableEntity2D* drawable);
#pragma endregion
#pragma region Selectables
	/**
	<summary>Adds a selectable entity. Also adds to drawables. Idempotent.</summary>
	<param name="autoScreenDeviation">True if scroll deviations are calculated. Use false to add many entities before calculating (more efficient). Default is true.</param>
    <param name="selectable">The entity.</param>
	*/
	void AddSelectable(SelectableEntity2D* selectable, bool autoScreenDeviation = true)
    {
		for (int i = 0; i < selectables.size(); i++)
			if (selectables[i] == selectable)
				return;

		selectables.push_back(selectable);
		for (int i = 0 ;i < selectable->GetDrawables().size(); i++)
			AddDrawable(selectable->GetDrawables()[i], false);
        AddEntity(selectable, autoScreenDeviation);
    }
	/**
	<summary>Removes a selectable entity. Provides an option to leave the entity in draw, but remove interactivity. Idempotent.</summary>
	<param name="removeFromDraw">Remove the entity from draw. Default is true.</param>
    <param name="selectable">The entity.</param>
	*/
	void RemoveSelectable(ClickableEntity2D* selectable, bool removeFromDraw = true)
    {
        for (vector<SelectableEntity2D*>::iterator i = selectables.begin(); i != selectables.end(); i++)
		{
			if (*i == selectable)
			{
				selectables.erase(i);
				break;
			}
		}

        if (removeFromDraw)
			for (int i = 0 ;i < selectable->GetDrawables().size(); i++)
				RemoveDrawable(selectable->GetDrawables()[i]);
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
		AddSelectable(clickable);
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
		RemoveSelectable(clickable, removeFromDraw);
    }
#pragma endregion
protected:
	/**
	<summary>Gets the maximum screen deviations for scroll.</summary>
	*/
	void GetMaxScreenDeviation()
	{
		maxDeviationUp = 0;
		maxDeviationDown = 0;
		maxDeviationLeft = 0;
		maxDeviationRight = 0;

		for (int i = 0; i < entities.size(); i++)
		{
			float excessUp = GetTop() + entities[i]->GetTop();
			float excessRight = GetRight() - entities[i]->GetRight();

			if (entities[i]->y < maxDeviationDown)
			{
				maxDeviationDown = entities[i]->y;
			}
			if (entities[i]->x < maxDeviationLeft)
			{
				maxDeviationLeft = entities[i]->x;
			}

			if (excessUp > maxDeviationUp)
			{
				maxDeviationUp = excessUp;
			}
			if (excessRight < maxDeviationRight)
			{
				maxDeviationRight = excessRight;
			}
		}
	}
	float maxDeviationUp;
	float maxDeviationDown;
	float maxDeviationLeft;
	float maxDeviationRight;
	/**
	<summary>Notifies all screens in the stack of a mouse event.</summary>
	<param name='type'>The event type.</param>
	<param name='position'>The resolution independent co-ordinates of the mouse cursor.</param>
	*/
	virtual void MouseEvent(MouseEvents::EventType type, MouseEvents::MouseButtons button, Vector2& position);
private:
	vector<T3Rectangle*> entities;
	vector<DrawableEntity2D*> drawables;
	vector<ClickableEntity2D*> clickables;
	vector<SelectableEntity2D*> selectables;
	int currentSelected;
};