/**
<summary>Static namespace containing helper methods for various
mathematical functions & algorithms.

Author: Nick Grimm
Version: 0.0.5 10/02/2015.</summary>
*/
#pragma once
#include "../Framework/Vector2.h"
#include "T3Rectangle.h"

namespace MathHelper
{
	/**
	<summary>Checks if a point is within a rectangle.</summary>
	<param name='point'>The point.</param>
	<param name='container'>The rectangle.</param>
	<returns>True if the point is inside the rectangle.</returns>
	*/
	bool Contains(Vector2 point, T3Rectangle container)
	{
		if (point.x >= container.x && point.x <= container.GetRight()
			&& point.y >= container.y && point.y <= container.GetTop())
			return true;
		return false;
	}
};