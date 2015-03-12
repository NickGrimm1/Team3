/**
Version History:
0.0.5 05/02/2015
0.0.6 06/02/2015
	Added Type.
*/

/**
<summary>Defines an object that can be drawn to the screen.

Author: Nick Grimm
Version: 0.0.6 06/02/2015.</summary>
*/
#pragma once
#include "T3Rectangle.h"
#include "../Framework/T3Vector4.h"

namespace DrawableType
{
	enum Type
	{
		Unknown,
		Texture,
		Text,
	};
}

class DrawableEntity2D : public T3Rectangle
{
public:
	/**
	<summary>Constructor.</summary>
	<param name='x'>The x position of the entity.</param>
	<param name='y'>The y position of the entity.</param>
	<param name='depth>The depth of the entity on screen.</param>
	<param name='width'>The width of the entity.</param>
	<param name='height'>The height of the entity.</param>
	<param name='blendColor'>The colour. Default is White (1,1,1,1).</param>
	*/
	DrawableEntity2D(float x, float y, int depth, float width, float height, DrawableType::Type type, float rotation = 0, T3Vector2 origin = T3Vector2(0.5f, 0.5f), T3Vector4 blendColor = T3Vector4(1.0f, 1.0f, 1.0f ,1.0f), bool trans = true) 
		: T3Rectangle(x, y, width, height),
		blendColor(blendColor), type(type), depth(depth), transparent(trans), origin(origin), rotation(rotation)
	{ }

	virtual ~DrawableEntity2D() {}

	/**
	<summary>Gets the drawable type of this object.</summary>
	*/
	DrawableType::Type GetType() { return type; }
	/**
	<summary>Gets the colour to blend this drawable with.</summary>
	*/
	T3Vector4 GetColor() const { return blendColor; }
	/**
	<summary>Sets the colour to blend this drawable with.</summary>
	<param name='value'>The colour.</param>
	*/
	void SetColor(const T3Vector4& value) { blendColor = value; }
	/**
	<summary>Gets the drawable entitys depth on screen. Allows it to be drawn behind/in front off other entities.</summary>
	*/
	int GetDepth() const {return depth;}
	void SetDepth(int value) { depth = value; }
	/**
	<summary>Sets the drawable entitys depth on screen.</summary>
	<param name='value'>The depth.</param>
	*/
	void SetColor(const int& value) { depth = value; }
	
	void SetTransparent(bool arg) { transparent = arg; }

	bool GetTransparent() { return transparent; }

	/**
	<summary>Gets the rotation.</summary>
	*/
	float GetRotation() const { return rotation; }
	/**
	<summary>Sets the rotation.</summary>
	<param name='value'>The rotation in degrees from up. Default is 0.</param>
	*/
	void SetRotation(const float value) {
		if (rotation > 360.0f) {
			rotation -= 360.0f;
		}
		else if (rotation < -360.0f) {
			rotation += 360.0f;
		}
		else rotation = value;
	}
	/**
	<summary>Gets the origin of rotation.</summary>
	*/
	T3Vector2 GetOrigin() const { return origin; }
	/**
	<summary>Sets the origin of rotation.</summary>
	<param name='value'>The origin around which this object will rotate, relative to the object's draw area. Default is the center (0.5, 0.5).</param>
	*/
	void SetOrigin(const T3Vector2& value) { origin = value; }

	friend std::ostream& operator<<(std::ostream& o, const DrawableEntity2D& v)
	{
		
		o << "DrawableEntity2D:" << std::endl;
		o << "  transparent: " << v.transparent << std::endl;
		o << "  depth: " << v.depth << std::endl;
		o << "  blendColor: " << v.blendColor << std::endl;
		o << "  type: " << v.type << std::endl;
		return o;
	}

protected:
	bool transparent;
	int depth;
	T3Vector4 blendColor;
	DrawableType::Type type;
	float rotation;
	T3Vector2 origin;
};