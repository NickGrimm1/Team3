/**
<summary>Represents a drawable text string.

Author: Nick Grimm
Version: 0.0.5 06/02/2015.</summary>
*/
#pragma once
#include "DrawableEntity2D.h"
#include <string>

using namespace std;

class DrawableTexture2D : public DrawableEntity2D
{
public:
	/**
	<summary>Constructor.</summary>
	<param name='x'>The x position of the entity.</param>
	<param name='y'>The y position of the entity.</param>
	<param name='width'>The width of the entity.</param>
	<param name='height'>The height of the entity.</param>
	<param name='blendColor'>The colour. Default is White (1,1,1,1).</param>
	<param name='rotation'>The rotation in degrees from up. Default is 0.</param>
	<param name='origin'>The origin around which this object will rotate, relative to the object's draw area. Default is the center (0.5, 0.5).</param>
	<param name='font'>The font.</param>
	<param name='text'>The text.</param>
	*/
	DrawableTexture2D(float x, float y, float width, float height, const string& text, Font* font, float rotation = 0, Vector2 origin = Vector2(0.5f, 0.5f), Vector4 blendColor = Vector4(1.0f, 1.0f, 1.0f ,1.0f)) : DrawableEntity2D(x, y, width, height, DrawableType::Text, blendColor), font(font), text(text), rotation(rotation), origin(origin)
	{ }

	/**
	<summary>Gets the texture.</summary>
	*/
	Font* GetFont() const;
	/**
	<summary>Sets the texture.</summary>
	<param name='value'>The texture.</param>
	*/
	void SetFont(Font* value);
	/**
	<summary>Gets the text string.</summary>
	*/
	string& GetText() const;
	/**
	<summary>Sets the text string.</summary>
	<param name='value'>The text.</param>
	*/
	void SetText(const string& value);
	/**
	<summary>Gets the rotation.</summary>
	*/
	float GetRotation() const;
	/**
	<summary>Sets the rotation.</summary>
	<param name='value'>The rotation in degrees from up. Default is 0.</param>
	*/
	void SetRotation(const float value);
	/**
	<summary>Gets the origin of rotation.</summary>
	*/
	Vector2& GetOrigin() const;
	/**
	<summary>Sets the origin of rotation.</summary>
	<param name='value'>The origin around which this object will rotate, relative to the object's draw area. Default is the center (0.5, 0.5).</param>
	*/
	void SetOrigin(const Vector2& value);
protected:
	Font* font;
	string text;
	float rotation;
	Vector2 origin;
};