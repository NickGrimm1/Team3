/*
Class:T3Vector4
Implements:
Author:Rich Davison
Description:VERY simple T3Vector4 class. Students are encouraged to modify this as necessary!

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*/
#pragma once
#include <iostream>
#include "T3Vector3.h"

class T3Vector4	{
public:
	T3Vector4(void) {
		x = y = z = w = 1.0f;
	}
	T3Vector4(float x, float y, float z, float w) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	T3Vector3 ToT3Vector3() {
		return T3Vector3(x, y, z);
	}

	T3Vector4 ToT3Vector4(T3Vector3 v3)
	{
		x = v3.x;
		y = v3.y;
		z = v3.z;
		w = 1.f;
		return T3Vector4(x,y,z,w);
	}

	~T3Vector4(void){}

	float x;
	float y;
	float z;
	float w;

	operator float*() const
	{
		float* output = new float[4];
		output[0] = x;
		output[1] = y;
		output[2] = z;
		output[3] = w;
		return output;
	}

	friend std::ostream& operator<<(std::ostream& o, const T3Vector4& v)
	{

		o << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
		return o;
	}
};

