/**
Version History:
0.0.1 03/02/2015
1.0.0 05/02/2015
	Added all methods.
*/

/**
<summary>A 2 dimensional axis aligned rectangle.

Author: Nick Grimm
Version: 1.0.0 05/02/2015.</summary>
*/
#pragma once
#include "../Framework/Vector2.h"

class Rectangle
{
public:
#pragma region Variables
	float x;
	float y;
	float width;
	float height;
#pragma endregion
#pragma region Constructors
	/**
	<summary>Constructor.</summary>
	<param name='x'>The x position.</param>
	<param name='y'>The y position.</param>
	<param name='width'>The width.</param>
	<param name='hieght'>The height.</param>
	*/
	Rectangle(float x, float y, float width, float height) : x(x), y(y), width(width), height(height)
	{ }
	/**
	<summary>Destructor.</summary>
	*/
	~Rectangle() {}
#pragma endregion
#pragma region Methods
	/**
	<summary>Gets the x co-ordinate of the right side of the rectangle.</summary>
	*/
	float GetRight() const { return x + width; }
	/**
	<summary>Gets the y co-ordinate of the top of the rectangle.</summary>
	*/
	float GetTop() const { return y + height; }
	/**
	<summary>Gets the position of the center of the rectangle.</summary>
	*/
	Vector2 GetCenter() const { return Vector2(x + width * 0.5f, y + height * 0.5f); }
	/**
	<summary>Gets an array of the corners of the rectangle. Clockwise from bottom left.</summary>
	*/
	Vector2* GetCorners() const 
	{
		Vector2 output[4] = { Vector2(x, y), Vector2(x, GetTop()), Vector2(GetRight(), GetTop()), Vector2(GetRight(), y) };
		return output;
	}
#pragma endregion
};