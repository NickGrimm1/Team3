#pragma once
#include "../Framework/Vector3.h"
#include "../Framework/Vector2.h"
#include "../Framework/Vector4.h"

class Vertex
{
public:
	Vector3 position;
	Vector3 normal;
	Vector4 color;
	Vector2 texCoord;
	Vector3 tangent;

	Vertex(Vector3 position = Vector3(), Vector3 normal = Vector3(), Vector4 color = Vector4(), Vector2 texCoord = Vector2(), Vector3 tangent = Vector3())
		: position(position), normal(normal), color(color), texCoord(texCoord), tangent(tangent)
	{ }
};
