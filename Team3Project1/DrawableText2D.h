/**
Version History:
0.0.5 05/02/2015
0.0.6 12/02/2015
	Actually implemented getters & setters.
*/

/**
<summary>Represents a drawable text string.

Author: Nick Grimm
Version: 0.0.6 12/02/2015.</summary>
*/
#pragma once
#include "DrawableEntity2D.h"
#include <string>
#include "Font.h"

using namespace std;

class DrawableText2D : public DrawableEntity2D
{
public:
	/**
	<summary>Constructor.</summary>
	<param name='x'>The x position of the entity.</param>
	<param name='y'>The y position of the entity.</param>
	<param name='depth>The depth of the entity on screen.</param>
	<param name='width'>The width of the entity.</param>
	<param name='height'>The height of the entity.</param>
	<param name='text'>The text.</param>
	<param name='font'>The font.</param>
	<param name='rotation'>The rotation in degrees from up (Clockwise). Default is 0.</param>
	<param name='origin'>The origin around which this object will rotate, relative to the object's draw area. Default is the center (0.5, 0.5).</param>
	<param name='blendColor'>The colour. Default is White (1,1,1,1).</param>	
	*/
	DrawableText2D(float x, float y, int depth, float width, float height, const string& text, Font* font, float rotation = 0, T3Vector2 origin = T3Vector2(0.5f, 0.5f), T3Vector4 blendColor = T3Vector4(1.0f, 1.0f, 1.0f ,1.0f), bool transparent = true) : DrawableEntity2D(x, y, depth, width, height, DrawableType::Text, blendColor, transparent), font(font), text(text), rotation(rotation), origin(origin)
	{ }

	virtual ~DrawableText2D() {
		text.clear();
	}
	/**
	<summary>Gets the font.</summary>
	*/
	Font* GetFont() const { return font; }
	/**
	<summary>Sets the font.</summary>
	<param name='value'>The font.</param>
	*/
	void SetFont(Font* value) { font = value; }
	/**
	<summary>Gets the text string.</summary>
	*/
	string GetText() const { return text; }
	/**
	<summary>Sets the text string.</summary>
	<param name='value'>The text.</param>
	*/
	void SetText(const string& value) { text = value; }
	/**
	<summary>Gets the rotation.</summary>
	*/
	float GetRotation() const { return rotation; }
	/**
	<summary>Sets the rotation.</summary>
	<param name='value'>The rotation in degrees from up. Default is 0.</param>
	*/
	void SetRotation(const float value) { rotation = value; }
	/**
	<summary>Gets the origin of rotation.</summary>
	*/
	T3Vector2 GetOrigin() const { return origin; }
	/**
	<summary>Sets the origin of rotation.</summary>
	<param name='value'>The origin around which this object will rotate, relative to the object's draw area. Default is the center (0.5, 0.5).</param>
	*/
	void SetOrigin(const T3Vector2& value) { origin = value; }
protected:
	Font* font;
	string text;
	float rotation;
	T3Vector2 origin;
};