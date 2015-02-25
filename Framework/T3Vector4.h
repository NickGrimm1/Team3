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

	~T3Vector4(void){}

	float x;
	float y;
	float z;
	float w;
};

