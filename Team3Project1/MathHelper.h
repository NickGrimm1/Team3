/**
<summary>Static namespace containing helper methods for various
mathematical functions & algorithms.

Author: Nick Grimm
Version: 0.0.5 10/02/2015.</summary>
*/
#pragma once
#include "../Framework/T3Vector2.h"
#include "T3Rectangle.h"

namespace MathHelper
{
	/**
	<summary>Checks if a point is within a rectangle.</summary>
	<param name='point'>The point.</param>
	<param name='container'>The rectangle.</param>
	<returns>True if the point is inside the rectangle.</returns>
	*/
	bool Contains(T3Vector2 point, T3Rectangle container)
	{
		if (point.x >= container.x && point.x <= container.GetRight()
			&& point.y >= container.y && point.y <= container.GetTop())
			return true;
		return false;
	}

	/**
	<summary>Linearly interpolates between 2 floats.</summary>
	<param name='f1'>The first float.</param>
	<param name='f2'>The second (target) float.</param>
	<param name='amount'>The amount to interpolate.</param>
	<returns>The final value of f1 moved amount towards f2.</returns>
	*/
	float Lerp(float f1, float f2, float amount)
	{
		return f1 + (f2 - f1) * amount;
	}
	/**
	<summary>Linearly interpolates between 2 3D vectors.</summary>
	<param name='v1'>The first vector.</param>
	<param name='v2'>The second (target) vector.</param>
	<param name='amount'>The amount to interpolate.</param>
	<returns>The final value of v1 moved amount towards v2.</returns>
	*/
	T3Vector3 Lerp(const T3Vector3& v1, const T3Vector3& v2, float amount)
	{
		return T3Vector3(Lerp(v1.x, v2.x, amount), Lerp(v1.y, v2.y, amount), Lerp(v1.z, v2.z, amount));
	}
	/**
	<summary>Linearly interpolates between 2 floats.</summary>
	<param name='f1'>The first float.</param>
	<param name='f2'>The second (target) float.</param>
	<param name='amount'>The amount to interpolate.</param>
	<returns>The final value of f1 moved amount towards f2.</returns>
	*/
	T3Matrix4& Lerp(T3Matrix4 m1, T3Matrix4 m2, float amount)
	{
		T3Matrix4 out;
		for (int i = 0; i < 16; i++)
			out.values[i] = m1.values[i] + (m2.values[i] - m1.values[i]) * amount;
		return out;
	}
	float Clamp(float f, float min = 0.0f, float max = 1.0f)
	{
		if (f < min)
			return min;
		if (f > max)
			return max;
		return f;
	}
};