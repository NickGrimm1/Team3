#include "GameScreen2D.h"
#include "GameStateManager.h"
#include "MathHelper.h"

/**
<summary>Adds a drawable entity. Idempotent.</summary>
<param name="autoScreenDeviation">True if scroll deviations are calculated. Use false to add many entities before calculating (more efficient). Default is true.</param>
<param name="drawable">The entity.</param>
*/
void GameScreen2D::AddDrawable(DrawableEntity2D* drawable, bool autoScreenDeviation)
{
	for (unsigned int i = 0; i < drawables.size(); i++)
		if (drawables[i] == drawable)
			return;

	if (drawable->GetType() == DrawableType::Texture)
		GameStateManager::Graphics()->AddTextureToScene((DrawableTexture2D*)drawable);
	else if (drawable->GetType() == DrawableType::Text)
		GameStateManager::Graphics()->AddDrawableTextToScene((DrawableText2D*)drawable);

    AddEntity(drawable, autoScreenDeviation);
}
/**
<summary>Removes a drawable entity. Idempotent.</summary>
<param name="drawable">The entity.</param>
*/
void GameScreen2D::RemoveDrawable(DrawableEntity2D* drawable)
{
	for (vector<DrawableEntity2D*>::iterator i = drawables.begin(); i != drawables.end(); i++)
	{
		if (*i == drawable)
		{
			if (drawable->GetType() == DrawableType::Texture)
				GameStateManager::Graphics()->RemoveTextureFromScene((DrawableTexture2D*)drawable);
			else if (drawable->GetType() == DrawableType::Text)
				GameStateManager::Graphics()->RemoveDrawableTextFromScene((DrawableText2D*)drawable);

			drawables.erase(i);
			break;
		}
	}
	RemoveEntity(drawable);
}

/**
<summary>Notifies all screens in the stack of a mouse event.</summary>
<param name='type'>The event type.</param>
<param name='position'>The resolution independent co-ordinates of the mouse cursor.</param>
*/
void GameScreen2D::MouseEvent(MouseEvents::EventType type, MouseEvents::MouseButtons button, Vector2& position)
{
	// Check if input is being accepted
	if (inputEnabled)
	{
		// Check if input is within screen bounds
		if (MathHelper::Contains(position, *this))
		{
			// If type is click & button is left, iterate over clickables
			if (button == MouseEvents::MOUSE_LEFT && type == MouseEvents::BUTTON_CLICK)
			{
				for (unsigned int i = 0; i < clickables.size(); i++)
				{
					// Test if click is within bounds of clickables
					if (MathHelper::Contains(position, *clickables[i]))
					{
						// If it is, calculate relative co-ords of click and call Click(x, y)
						clickables[i]->Click((position.x - clickables[i]->x) / clickables[i]->width, (position.y - clickables[i]->y) / clickables[i]->height);
					}
				}
			}
		}
	}
}

/**
<summary>Notifies all screens in the stack that the mouse has moved.</summary>
<param name='start'>The resolution independent co-ordinates of the mouse cursor at the start of the frame.</param>
<param name='finish'>The resolution independent co-ordinates of the mouse cursor at the end of the frame.</param>
*/
void GameScreen2D::MouseMoved(Vector2& finish)
{
	// Check if input is being accepted
	if (inputEnabled)
	{
		// Check if input is within screen bounds
		if (MathHelper::Contains(finish, *this))
		{
			// Check if the finish location is within bounds of a selectable
			for (unsigned int i = 0; i < selectables.size(); i++)
			{
				// If it is, call Select()
				selectables[i]->Select();
				currentSelected = i;
			}
		}
	}
}
/**
<summary>Notifies all screens in the stack that the mouse scroll wheel has moved.</summary>
<param name='amount'>The amount of the movement.</param>
*/
void GameScreen2D::MouseScrolled(Vector2& position, int amount)
{
	// Check if input is being accepted
	if (inputEnabled)
	{
		// Check if input is within screen bounds
		if (MathHelper::Contains(position, *this))
		{
			// If it is, scroll vertically.
			Scroll(position, Vector2(position.x, position.y + amount));
		}
	}
}
/**
<summary>Notifies all screens in the stack of a keyboard event.</summary>
<param name='type'>The event type.</param>
<param name='key'>The key.</param>
*/
void GameScreen2D::KeyboardEvent(KeyboardEvents::EventType type, KeyboardEvents::Key key) 
{
	// Not implemented in 2D. Can be overridden for specific screens if neccessary.
}
/**
<summary>Notifies all screens in the stack of a gamepad event.</summary>
<param name='playerID'>The ID for the controller.</param>
<param name='type'>The event type.</param>
<param name='button'>The button.</param>
*/
void GameScreen2D::GamepadEvent(GamepadEvents::PlayerIndex playerID, GamepadEvents::EventType type, GamepadEvents::Button button)
{
	// Check if input is being accepted
	if (inputEnabled)
	{
		// Check for select actions
		if (type == GamepadEvents::BUTTON_PRESS)
		{
			if (button == GamepadEvents::INPUT_CROSS)
			{
				// Find the currently selected entity in clickables, and 'Click' it
				for (unsigned int i = 0; i < clickables.size(); i++)
				{
					if (clickables[i] == selectables[currentSelected])
						clickables[i]->Click(0.0f, 0.0f);
				}
			}

			// Check for DPad & Thumbstick movement for changing the selected item.

		}
	}
}
/**
<summary>Notifies all screens in the stack that an analogue control is displaced.</summary>
<param name='playerID'>The ID for the controller.</param>
<param name='analogueControl'>The control that is displaced.</param>
<param name='amount'>The amount of the displacement. For the triggers, only the x co-ordinate is used.</param>
*/
void GameScreen2D::GamepadAnalogueDisplacement(GamepadEvents::PlayerIndex playerID, GamepadEvents::AnalogueControl analogueControl, Vector2& amount)
{
	// Check if input is being accepted
	if (inputEnabled)
	{
		// Check for right stick movement
		if (analogueControl == GamepadEvents::RIGHT_STICK)
		{
			Scroll(Vector2(), amount);
		}
	}
}