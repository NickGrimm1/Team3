/**
Version History:
0.0.1 03/02/2015
1.0.0 05/02/2015
	Added all methods & operator overloads.
*/

/**
<summary>A 3 dimensional Vector.

Author: Nick Grimm
Version: 1.0.0 05/02/2015.</summary>
*/
#pragma once
#include "common.h"
#include <math.h>

class Vector3
{
public:
#pragma region Variables
	float x;
	float y;
	float z;
#pragma endregion
#pragma region Constructors
	/**
	<summary>Constructor.</summary>
	<param name='x'>The x component. Default is 0.</param>
	<param name='y'>The y component. Default is 0.</param>
	<param name='z'>The z component. Default is 0.</param>
	*/
	Vector3(const float x = 0, const float y = 0, const float z = 0) : x(x), y(y), z(z)
	{ }
	/**
	<summary>Destructor.</summary>
	*/
	~Vector3()
	{
	}
	/**
	<summary>Sets all components to zero.</summary>
	*/
	void ToZero()
	{
		x = 0;
		y = 0;
		z = 0;
	}
#pragma endregion
#pragma region Operations
#pragma region Length and Comparisons
	/**
	<summary>Gets the length squared of this Vector. Useful for comparing two vectors while avoiding square root.</summary>
	*/
	inline float LengthSquared() const
	{
		return x * x + y * y + z * z; 
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
	inline bool operator<(const Vector3 &in) const
	{
		return LengthSquared() < in.LengthSquared();
	}
	/**
	<summary>Compares this vector with another.</summary>
	<param name='in'>The second vector.</param>
	<returns>true if this vector is shorter than or equal in length to the other.</returns>
	*/
	inline bool operator<=(const Vector3 &in) const
	{
		return LengthSquared() <= in.LengthSquared();
	}
	/**
	<summary>Compares this vector with another.</summary>
	<param name='in'>The second vector.</param>
	<returns>true if this vector is longer than the other.</returns>
	*/
	inline bool operator>(const Vector3 &in) const
	{
		return LengthSquared() > in.LengthSquared();
	}
	/**
	<summary>Compares this vector with another.</summary>
	<param name='in'>The second vector.</param>
	<returns>true if this vector is longer than or equal in length to the other.</returns>
	*/
	inline bool operator>=(const Vector3 &in) const
	{
		return LengthSquared() >= in.LengthSquared();
	}
	/**
	<summary>Compares this vector with another.</summary>
	<param name='in'>The second vector.</param>
	<returns>true if this vector is equal to the other, within a tolerance of 10^-5.</returns>
	*/
	inline bool operator==(const Vector3 &in) const
	{
		return (abs(x - in.x < 0.00001f) && abs(y - in.y < 0.00001f) && abs(z - in.z < 0.00001f));
	}
	
	/**
	<summary>Compares this vector with another.</summary>
	<param name='in'>The second vector.</param>
	<returns>true if this vector is not equal to the other, within a tolerance of 10^-5.</returns>
	*/
	inline bool operator!=(const Vector3 &in) const
	{
		return !(abs(x - in.x < 0.00001f) && abs(y - in.y < 0.00001f) && abs(z - in.z < 0.00001f));
	}
#pragma endregion
#pragma region Addition
	/**
	<summary>Adds a vector to this one, without modifying the vector.</summary>
	<param name='in'>The vector to add.</param>
	<returns>The resulting vector.</returns>
	*/
	Vector3 Add(const Vector3 &in) const 
	{
		return Vector3(x + in.x, y + in.y, z + in.z);
	}
	/**
	<summary>Adds a vector to this one, modifying this.</summary>
	<param name='in'>The vector to add.</param>
	*/
	void AddTo(const Vector3 &in)
	{
		x += in.x;
		y += in.y;
		z += in.z;
	}
	/**
	<summary>Adds a vector to another without altering either.</summary>
	<param name='v1'>The first vector.</param>
	<param name='v2'>The second vector.</param>
	<returns>The resulting vector.</returns>
	*/
	static Vector3 Add(const Vector3 &v1, const Vector3 &v2)
	{
		return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
	}
	/**
	<summary>Adds a vector to this one, without modifying the vector.</summary>
	<param name='in'>The vector to add.</param>
	<returns>The resulting vector.</returns>
	*/
	Vector3 operator+(const Vector3 &in) const  
	{ 
		return Add(in); 
	}
	/**
	<summary>Adds a vector to this one, modifying this.</summary>
	<param name='in'>The vector to add.</param>
	*/
	void operator+=(const Vector3 &in) 
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
	Vector3 Subtract(const Vector3 &in) const 
	{
		return Vector3(in.x - x, in.y - y, in.z - z);
	}
	/**
	<summary>Subtracts a vector from this one, modifying this.</summary>
	<param name='in'>The vector to subtract.</param>
	*/
	void SubtractFrom(const Vector3 &in)
	{
		x = in.x - x;
		y = in.y - y;
		z = in.z - z;
	}
	/**
	<summary>Subtracts the first vector from the second without altering either.</summary>
	<param name='v1'>The first vector.</param>
	<param name='v2'>The second vector.</param>
	<returns>The resulting vector.</returns>
	*/
	static Vector3 Subtract(const Vector3 &v1, const Vector3 &v2)
	{
		return Vector3(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
	}
	/**
	<summary>Subtracts a vector from this one, without modifying the vector.</summary>
	<param name='in'>The vector to subtract.</param>
	<returns>The resulting vector.</returns>
	*/
	Vector3 operator-(const Vector3 &in) const 
	{
		return Subtract(in);
	}
	/**
	<summary>Subtracts a vector from this one, modifying this.</summary>
	<param name='in'>The vector to subtract.</param>
	*/
	void operator-=(const Vector3 &in)
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
	Vector3 Multiply(const float scalar) const
	{
		return Vector3(x * scalar, y * scalar, z * scalar);
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
		z *= scalar;
	}
	/**
	<summary>Multiplies a vector by a scalar without altering the vector.</summary>
	<param name='vector'>The vector.</param>
	<param name='scalar'>The scalar.</param>
	<returns>The resulting vector.</returns>
	*/
	static Vector3 Multiply(const Vector3 &vector, const float scalar)
	{
		return Vector3(vector.x * scalar, vector.y * scalar, vector.z * scalar);
	}
	/**
	<summary>Multiplies this vector by a scalar, without modifying the vector.</summary>
	<param name='scalar'>The scalar.</param>
	<returns>The resulting vector.</returns>
	*/
	Vector3 operator*(const float in) const 
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
	Vector3 Divide(const float scalar) const
	{
		if (scalar != 0)
			return Vector3(x / scalar, y / scalar, z / scalar);
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
			z /= scalar;
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
	static Vector3 Divide(const Vector3 &vector, const float scalar)
	{
		return Vector3(vector.x / scalar, vector.y / scalar, vector.z / scalar);
	}
	/**
	<summary>Divides this vector by a scalar, without altering the vector.</summary>
	<param name='scalar'>The scalar.</param>
	<returns>The resulting vector.</returns>
	*/
	Vector3 operator/(const float in) const
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
#pragma region CrossProduct
	/**
	<summary>Finds the cross product (Vector Product) of this and another vector.</summary>
	<param name='in'>The other vector.</param>
	<returns>The resulting vector.</returns>
	*/
	Vector3 Cross(const Vector3 &in) const
	{
		return Vector3(y * in.z - z * in.y, z * in.x - x * in.z, x * in.y - y * in.x);
	}
	/**
	<summary>Finds the cross product (Vector Product) of the first vector and the second vector.</summary>
	<param name='v1'>The first vector.</param>
	<param name='v2'>The second vector.</param>
	<returns>The resulting vector.</returns>
	*/
	static Vector3 Cross(const Vector3 &v1, const Vector3 &v2)
	{
		return Vector3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
	}

	static Vector3	doubleCross(const Vector3& v1, const Vector3& v2)
{
	return Cross(Cross(v1, v2), v1);
}
	/**
	<summary>Finds the cross product (Vector Product) of this and another vector.</summary>
	<param name='in'>The other vector.</param>
	<returns>The resulting vector.</returns>
	*/
	Vector3 operator%(const Vector3 &in)
	{
		return Cross(in);
	}
#pragma endregion
#pragma region DotProduct
	/**
	<summary>Finds the dot product (Scalar Product) of the this vector and another vector.</summary>
	<param name='in'>The other vector.</param>
	<returns>The scalar product.</returns>
	*/
	float Dot(const Vector3 &in) const
	{
		return x * in.x + y * in.y + z * in.z;
	}
	/**
	<summary>Finds the dot product (Scalar Product) of the first vector and the second vector.</summary>
	<param name='v1'>The first vector.</param>
	<param name='v2'>The second vector.</param>
	<returns>The scalar product.</returns>
	*/
	static float Dot(const Vector3 &v1, const Vector3 &v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}
	/**
	<summary>Finds the dot product (Scalar Product) of the this vector and another vector.</summary>
	<param name='in'>The other vector.</param>
	<returns>The scalar product.</returns>
	*/
	float operator*(const Vector3 &in) const
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
			z /= l;
		}
		//else
			//throw DivideByZeroException();
	}
	/**
	<summary>Finds the normal of the this vector without modifying it.</summary>
	<returns>The normal (unit) vector.</returns>
	*/
	Vector3 Normal() const
	{
		Vector3 output = Vector3(x, y, z);
		output.Normalise();
		return output;
	}
	/**
	<summary>Finds the normal of a vector.</summary>
	<param name='vector'>The vector.</param>
	<returns>The normal (unit) vector.</returns>
	*/
	static Vector3 Normal(const Vector3 &in)
	{
		Vector3 output = Vector3(in.x, in.y, in.z);
		output.Normalise();
		return output;
	}
	/**
	<summary>Finds a normalized (unit) vector of the cross product (Vector Product) of the this vector and another vector (The orthonormal).</summary>
	<param name='in'>The other vector.</param>
	<returns>The unit cross-product vector.</returns>
	*/
	Vector3 UnitOrthogonal(const Vector3 &in) const
	{
		Vector3 output = Cross(in);
		output.Normalise();
		return output;
	}
	/**
	<summary>Finds a normalized (unit) vector of the cross product (Vector Product) of the two vectors (The othernormal.</summary>
	<param name='v1'>The first vector.</param>
	<param name='v2'>The second vector.</param>
	<returns>The unit cross-product vector.</returns>
	*/
	static Vector3 UnitOrthogonal(const Vector3 &v1, const Vector3 &v2) 
	{
		Vector3 output = Cross(v1, v2);
		output.Normalise();
		return output;
	}
#pragma endregion
#pragma region UnaryNegation
	/**
	<summary>Negates this vector without modifing it.</summary>
	<returns>A vector with each component of this negated.</returns>
	*/
	Vector3 operator-() const
	{
		return Vector3(-x, -y, -z);
	}
	/**
	<summary>Negates this vector.</summary>
	*/
	void operator-()
	{
		x = -x;
		y = -y;
		z = -z;
	}
#pragma endregion
#pragma endregion
#pragma region Standards
	/**
		<summary>A unit vector in positive X.</summary>
	*/
	static Vector3 UnitX()
	{
		return Vector3(1.0f);
	}
	/**
		<summary>A unit vector in positive Y.</summary>
	*/
	static Vector3 UnitY()
	{
		return Vector3(0.0f, 1.0f);
	}
	/**
		<summary>A unit vector in positive Z.</summary>
	*/
	static Vector3 UnitZ()
	{
		return Vector3(0.0f, 0.0f, 1.0f);
	}
#pragma endregion
};

