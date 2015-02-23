#pragma once
#include "../Framework/Vector3.h"
#include "../Framework/Vector2.h"
#include "../Framework/Vector4.h"

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

	Vector3 GetPosition() const { return Vector3(x, y, z); }
	void SetPosition(const Vector3& value)
	{
		x = value.x;
		y = value.y;
		z = value.z;
	}
	Vector3 GetNormal() const { return Vector3(nX, nY, nZ); }
	void SetNormal(const Vector3& value)
	{
		nX = value.x;
		nY = value.y;
		nZ = value.z;
	}
#if WINDOWS_BUILD
	Vector4 GetColor() const { return Vector4(r, g, b, a); }
	void SetColor(const Vector4& value)
	{
		r = value.x;
		g = value.y;
		b = value.z;
		a = value.w;
	}
#endif
	#if PS3_BUILD
	Vector4 GetColor() const 
	{
		int a = rgba / (256 * 256 * 256);
		int b = rgba / (256 * 256) - a;
		int g = rgba / (256) - a - b;
		int r = rgba - a - b - g;

		return Vector4(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f); 
	}
	void SetColor(const Vector4& value)
	{
		rgba = value.x * 255 
			+ (256 + value.y * 255) 
			+ ((256 * 256) + value.z * 255)
			+ ((256 * 256 * 256) + value.z * 255);
	}
	#endif
	Vector2 GetTexCoord() const { return Vector2(tX, tY); }
	void SetTexCoord(const Vector2& value)
	{
		tX = value.x;
		tY = value.y;
	}
	Vector3 GetTangent() const { return Vector3(tnX, tnY, tnZ); }
	void SetTangent(const Vector3& value)
	{
		tnX = value.x;
		tnY = value.y;
		tnZ = value.z;
	}

	Vertex(Vector3 position = Vector3(), Vector3 normal = Vector3(), Vector4 color = Vector4(), Vector2 texCoord = Vector2(), Vector3 tangent = Vector3())
		: x(position.x), y(position.y), z(position.z),
		nX(normal.x), nY(normal.y), nZ(normal.z),
#if WINDOWS_BUILD
		r(color.x), g(color.y), b(color.z), a(color.w),
#endif
#if PS3_BUILD
#endif
		tX(texCoord.x), tY(texCoord.y),
		tnX(tangent.x), tnY(tangent.y), tnZ(tangent.z)
	{ }
};
