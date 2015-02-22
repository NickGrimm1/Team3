/**
Version History:
0.0.1 03/02/2015
1.0.0 05/02/2015
	Added all methods & operator overloads.
*/

/**
<summary>A 2 dimensional Vector.

Author: Nick Grimm
Version: 1.0.0 05/02/2015.</summary>
*/
#pragma once
#include "common.h"
#include <math.h>

class Vector2
{
public:
#pragma region Variables
	float x;
	float y;
#pragma endregion
#pragma region Constructors
	/**
	<summary>Constructor.</summary>
	<param name='x'>The x component. Default is 0.</param>
	<param name='y'>The y component. Default is 0.</param>
	*/
	Vector2(const float x = 0, const float y = 0) : x(x), y(y)
	{ }
	/**
	<summary>Destructor.</summary>
	*/
	~Vector2()
	{
	}
	/**
	<summary>Sets all components to zero.</summary>
	*/
	void ToZero()
	{
		x = 0;
		y = 0;
	}
#pragma endregion
#pragma region Operations
#pragma region Length and Comparisons
	/**
	<summary>Gets the length squared of this Vector. Useful for comparing two vectors while avoiding square root.</summary>
	*/
	inline float LengthSquared() const
	{
		return x * x + y * y; 
	}
	/**
	<summary>Gets the length (magnitude) the vector. For a faster comparison, always use length squared.</summary>
	*/
	inline float Length() const
	{
		return sqrt(LengthSquared());
	}
	/**
	<summary>Compares this vector with another.</summary>
	<param name='in'>The second vector.</param>
	<returns>true if this vector is shorter than the other.</returns>
	*/
	inline bool operator<(const Vector2 &in) const
	{
		return LengthSquared() < in.LengthSquared();
	}
	/**
	<summary>Compares this vector with another.</summary>
	<param name='in'>The second vector.</param>
	<returns>true if this vector is shorter than or equal in length to the other.</returns>
	*/
	inline bool operator<=(const Vector2 &in) const
	{
		return LengthSquared() <= in.LengthSquared();
	}
	/**
	<summary>Compares this vector with another.</summary>
	<param name='in'>The second vector.</param>
	<returns>true if this vector is longer than the other.</returns>
	*/
	inline bool operator>(const Vector2 &in) const
	{
		return LengthSquared() > in.LengthSquared();
	}
	/**
	<summary>Compares this vector with another.</summary>
	<param name='in'>The second vector.</param>
	<returns>true if this vector is longer than or equal in length to the other.</returns>
	*/
	inline bool operator>=(const Vector2 &in) const
	{
		return LengthSquared() >= in.LengthSquared();
	}
	/**
	<summary>Compares this vector with another.</summary>
	<param name='in'>The second vector.</param>
	<returns>true if this vector is equal to the other, within a tolerance of 10^-5.</returns>
	*/
	inline bool operator==(const Vector2 &in) const
	{
		return (abs(x - in.x < 0.00001f) && abs(y - in.y < 0.00001f));
	}
	/**
	<summary>Compares this vector with another.</summary>
	<param name='in'>The second vector.</param>
	<returns>true if this vector is not equal to the other, within a tolerance of 10^-5.</returns>
	*/
	inline bool operator!=(const Vector2 &in) const
	{
		return !(abs(x - in.x < 0.00001f) && abs(y - in.y < 0.00001f));
	}
#pragma endregion
#pragma region Addition
	/**
	<summary>Adds a vector to this one, without modifying the vector.</summary>
	<param name='in'>The vector to add.</param>
	<returns>The resulting vector.</returns>
	*/
	Vector2 Add(const Vector2 &in) const 
	{
		return Vector2(x + in.x, y + in.y);
	}
	/**
	<summary>Adds a vector to this one, modifying this.</summary>
	<param name='in'>The vector to add.</param>
	*/
	void AddTo(const Vector2 &in)
	{
		x += in.x;
		y += in.y;
	}
	/**
	<summary>Adds a vector to another without altering either.</summary>
	<param name='v1'>The first vector.</param>
	<param name='v2'>The second vector.</param>
	<returns>The resulting vector.</returns>
	*/
	static Vector2 Add(const Vector2 &v1, const Vector2 &v2)
	{
		return Vector2(v1.x + v2.x, v1.y + v2.y);
	}
	/**
	<summary>Adds a vector to this one, without modifying the vector.</summary>
	<param name='in'>The vector to add.</param>
	<returns>The resulting vector.</returns>
	*/
	Vector2 operator+(const Vector2 &in) const  
	{ 
		return Add(in); 
	}
	/**
	<summary>Adds a vector to this one, modifying this.</summary>
	<param name='in'>The vector to add.</param>
	*/
	void operator+=(const Vector2 &in) 
	{ 
		AddTo(in);
	}
#pragma endregion
#pragma region Subtraction
	/**
	<summary>Subtracts a vector from this one, without modifying the vector.</summary>
	<param name='in'>The vector to subtract.</param>
	<returns>The resulting vector.</returns>
	*/
	Vector2 Subtract(const Vector2 &in) const 
	{
		return Vector2(x - in.x, y - in.y);
	}
	/**
	<summary>Subtracts a vector from this one, modifying this.</summary>
	<param name='in'>The vector to subtract.</param>
	*/
	void SubtractFrom(const Vector2 &in)
	{
		x = x - in.x;
		y = y - in.y;
	}
	/**
	<summary>Subtracts the second vector from the first without altering either.</summary>
	<param name='v1'>The first vector.</param>
	<param name='v2'>The second vector.</param>
	<returns>The resulting vector.</returns>
	*/
	static Vector2 Subtract(const Vector2 &v1, const Vector2 &v2)
	{
		return Vector2(v1.x - v2.x, v1.y - v2.y);
	}
	/**
	<summary>Subtracts a vector from this one, without modifying the vector.</summary>
	<param name='in'>The vector to subtract.</param>
	<returns>The resulting vector.</returns>
	*/
	Vector2 operator-(const Vector2 &in) const 
	{
		return Subtract(in);
	}
	/**
	<summary>Subtracts a vector from this one, modifying this.</summary>
	<param name='in'>The vector to subtract.</param>
	*/
	void operator-=(const Vector2 &in)
	{
		SubtractFrom(in);
	}
#pragma endregion
#pragma region Simple Multiplication
	/**
	<summary>Multiplies this vector by a scalar, without modifying the vector.</summary>
	<param name='scalar'>The scalar.</param>
	<returns>The resulting vector.</returns>
	*/
	Vector2 Multiply(const float scalar) const
	{
		return Vector2(x * scalar, y * scalar);
	}
	/**
	<summary>Multiplies this vector by a scalar, modifying this.</summary>
	<param name='vector'>The vector.</param>
	<param name='scalar'>The scalar.</param>
	*/
	void MultiplyBy(const float scalar)
	{
		x *= scalar;
		y *= scalar;
	}
	/**
	<summary>Multiplies a vector by a scalar without altering the vector.</summary>
	<param name='vector'>The vector.</param>
	<param name='scalar'>The scalar.</param>
	<returns>The resulting vector.</returns>
	*/
	static Vector2 Multiply(const Vector2 &vector, const float scalar)
	{
		return Vector2(vector.x * scalar, vector.y * scalar);
	}
	/**
	<summary>Multiplies this vector by a scalar, without modifying the vector.</summary>
	<param name='scalar'>The scalar.</param>
	<returns>The resulting vector.</returns>
	*/
	Vector2 operator*(const float in) const 
	{
		return Multiply(in);
	}
	/**
	<summary>Multiplies this vector by a scalar, modifying this.</summary>
	<param name='vector'>The vector.</param>
	<param name='scalar'>The scalar.</param>
	*/
	void operator*=(const float in)
	{
		MultiplyBy(in);
	}
#pragma endregion
#pragma region Division
	/**
	<summary>Divides this vector by a scalar, without altering the vector.</summary>
	<param name='scalar'>The scalar.</param>
	<returns>The resulting vector.</returns>
	*/
	Vector2 Divide(const float scalar) const
	{
		if (scalar != 0)
			return Vector2(x / scalar, y / scalar);
		else
			throw DivideByZeroException();
	}
	/**
	<summary>Divides this vector by a scalar, modifying this.</summary>
	<param name='vector'>The vector.</param>
	<param name='scalar'>The scalar.</param>
	*/
	void DivideBy(const float scalar)
	{
		if (scalar != 0)
		{
			x /= scalar;
			y /= scalar;
		}
		else
			throw DivideByZeroException();
	}
	/**
	<summary>Divides a vector by a scalar without altering the vector.</summary>
	<param name='vector'>The vector.</param>
	<param name='scalar'>The scalar.</param>
	<returns>The resulting vector.</returns>
	*/
	static Vector2 Divide(const Vector2 &vector, const float scalar)
	{
		return Vector2(vector.x / scalar, vector.y / scalar);
	}
	/**
	<summary>Divides this vector by a scalar, without altering the vector.</summary>
	<param name='scalar'>The scalar.</param>
	<returns>The resulting vector.</returns>
	*/
	Vector2 operator/(const float in) const
	{
		return Divide(in);
	}
	/**
	<summary>Divides this vector by a scalar, modifying this.</summary>
	<param name='vector'>The vector.</param>
	<param name='scalar'>The scalar.</param>
	*/
	void operator/=(const float in)
	{
		DivideBy(in);
	}
#pragma endregion
#pragma region DotProduct
	/**
	<summary>Finds the dot product (Scalar Product) of the this vector and another vector.</summary>
	<param name='in'>The other vector.</param>
	<returns>The scalar product.</returns>
	*/
	float Dot(const Vector2 &in) const
	{
		return x * in.x + y * in.y;
	}
	/**
	<summary>Finds the dot product (Scalar Product) of the first vector and the second vector.</summary>
	<param name='v1'>The first vector.</param>
	<param name='v2'>The second vector.</param>
	<returns>The scalar product.</returns>
	*/
	static float Dot(const Vector2 &v1, const Vector2 &v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}
	/**
	<summary>Finds the dot product (Scalar Product) of the this vector and another vector.</summary>
	<param name='in'>The other vector.</param>
	<returns>The scalar product.</returns>
	*/
	float operator*(const Vector2 &in) const
	{
		return Dot(in);
	}
#pragma endregion
#pragma region Normals
	/**
	<summary>Normalizes this vector.</summary>
	*/
	void Normalise()
	{
		float l = Length();
		if (l != 0)
		{
			x /= l;
			y /= l;
		}
		else
			throw DivideByZeroException();
	}
	/**
	<summary>Finds the normal of the this vector without modifying it.</summary>
	<returns>The normal (unit) vector.</returns>
	*/
	Vector2 Normal() const
	{
		Vector2 output = Vector2(x, y);
		output.Normalise();
		return output;
	}
	/**
	<summary>Finds the normal of a vector.</summary>
	<param name='vector'>The vector.</param>
	<returns>The normal (unit) vector.</returns>
	*/
	static Vector2 Normal(const Vector2 &in)
	{
		Vector2 output = Vector2(in.x, in.y);
		output.Normalise();
		return output;
	}
#pragma endregion
#pragma region UnaryNegation
	/**
	<summary>Negates this vector without modifing it.</summary>
	<returns>A vector with each component of this negated.</returns>
	*/
	Vector2 operator-() const
	{
		return Vector2(-x, -y);
	}
	/**
	<summary>Negates this vector.</summary>
	*/
	void operator-()
	{
		x = -x;
		y = -y;
	}
#pragma endregion
#pragma endregion
#pragma region Standards
	/**
		<summary>A unit vector in positive X.</summary>
	*/
	static Vector2 UnitX()
	{
		return Vector2(1.0f);
	}
	/**
		<summary>A unit vector in positive Y.</summary>
	*/
	static Vector2 UnitY()
	{
		return Vector2(0.0f, 1.0f);
	}
#pragma endregion
};

