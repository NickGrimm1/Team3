/******************************************************************************
Author:Rich Davison
Description: Some random variables and functions, for lack of a better place 
to put them.

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////

#pragma once

#define WEEK_2_CODE
#define WEEK_3_CODE

//It's pi(ish)...
static const float		PI = 3.14159265358979323846f;	

//It's pi...divided by 360.0f!
static const float		PI_OVER_360 = PI / 360.0f;

//Radians to degrees
static inline double RadToDeg(const double deg)	{
	return deg * 180.0 / PI;
};

//Degrees to radians
static inline double DegToRad(const double rad)	{
	return rad * PI / 180.0;
};
 //#if WINDOWS_BUILD
//I blame Microsoft...
#define maximum(a,b)    (((a) > (b)) ? (a) : (b))
#define minimum(a,b)    (((a) < (b)) ? (a) : (b))
//#endif
#if WINDOWS_BUILD
#define SHADERDIR	"../Resources/Shaders/"
#define MESHDIR		"../Resources/Meshes/"
#define TEXTUREDIR  "../Resources/Textures/"
#endif
#if PS3_BUILD
#define SHADERDIR	SYS_APP_HOME"/"
#define MESHDIR		SYS_APP_HOME"/"
#define TEXTUREDIR  SYS_APP_HOME"/"
#endif
#if WINDOWS_BUILD
#define SOUNDSDIR	"../Resources/Sounds/"
#endif


