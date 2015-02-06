#include "GameScreen2D.h"
#include "GameStateManager.h"

/**
<summary>Notifies all screens in the stack of a mouse event.</summary>
<param name='type'>The event type.</param>
<param name='position'>The resolution independent co-ordinates of the mouse cursor.</param>
*/
void GameScreen2D::MouseEvent(MouseEvents::EventType type, MouseEvents::MouseButtons button, Vector2& position)
{
	// Check if input is being accepted
	// Check if input is within screen bounds
	// If type is click & button is left, iterate over clickables
	// Test if click is within bounds of clickables
	// If it is, calculate relative co-ords of click and call Click(x, y)
}

/**
<summary>Adds a drawable entity. Idempotent.</summary>
<param name="autoScreenDeviation">True if scroll deviations are calculated. Use false to add many entities before calculating (more efficient). Default is true.</param>
<param name="drawable">The entity.</param>
*/
void GameScreen2D::AddDrawable(DrawableEntity2D* drawable, bool autoScreenDeviation)
{
	for (int i = 0; i < drawables.size(); i++)
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