#pragma once
#include "../Framework/T3Vector3.h"
#include "../Framework/T3Vector2.h"
#include "../Framework/T3Vector4.h"

namespace VertexAttributes
{
	enum Attributes
	{
		POSITION = 0,
		NORMAL = 1,
		COLOUR	= 2,
		TEXCOORD = 3,
		TANGENT	= 4,
		MAX = 5,
	};
}

class Vertex
{
public:
	float x, y, z; // Position
	float nX, nY, nZ; // Normal
#if WINDOWS_BUILD
	float r, g, b, a; // Color
#endif
#if PS3_BUILD
	int rgba; // Color
#endif
	float tX, tY; // Texture Coordinate
	float tnX, tnY, tnZ; // Tangent

	T3Vector3 GetPosition() const { return T3Vector3(x, y, z); }
	void SetPosition(const T3Vector3& value)
	{
		x = value.x;
		y = value.y;
		z = value.z;
	}
	T3Vector3 GetNormal() const { return T3Vector3(nX, nY, nZ); }
	void SetNormal(const T3Vector3& value)
	{
		nX = value.x;
		nY = value.y;
		nZ = value.z;
	}

	T3Vector4 GetColor() const 
	{
		#if WINDOWS_BUILD
		return T3Vector4(r, g, b, a);
#endif
		#if PS3_BUILD
		int a = rgba / (256 * 256 * 256);
		int b = rgba / (256 * 256) - a;
		int g = rgba / (256) - a - b;
		int r = rgba - a - b - g;

		return T3Vector4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f); 
#endif
	}
	void SetColor(const T3Vector4& value)
	{
		#if WINDOWS_BUILD
		r = value.x;
		g = value.y;
		b = value.z;
		a = value.w;
#endif
		#if PS3_BUILD
		rgba = value.x * 255 
			+ (256 + value.y * 255) 
			+ ((256 * 256) + value.z * 255)
			+ ((256 * 256 * 256) + value.z * 255);
		#endif
	}

	T3Vector2 GetTexCoord() const { return T3Vector2(tX, tY); }
	void SetTexCoord(const T3Vector2& value)
	{
		tX = value.x;
		tY = value.y;
	}
	T3Vector3 GetTangent() const { return T3Vector3(tnX, tnY, tnZ); }
	void SetTangent(const T3Vector3& value)
	{
		tnX = value.x;
		tnY = value.y;
		tnZ = value.z;
	}

	Vertex(T3Vector3 position = T3Vector3(), T3Vector3 normal = T3Vector3(), T3Vector4 color = T3Vector4(), T3Vector2 texCoord = T3Vector2(), T3Vector3 tangent = T3Vector3())
		: x(position.x), y(position.y), z(position.z),
		nX(normal.x), nY(normal.y), nZ(normal.z),
#if WINDOWS_BUILD
		r(color.x), g(color.y), b(color.z), a(color.w),
#endif


		tX(texCoord.x), tY(texCoord.y),
		tnX(tangent.x), tnY(tangent.y), tnZ(tangent.z)
	{
	#if PS3_BUILD
		SetColor(color);
	#endif 
	}
};
