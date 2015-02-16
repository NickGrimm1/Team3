#pragma once

/**
<summary>Spline
Implements a quadratic bezier spline based on three control points.

Inherits Mesh.
Methods available to draw 
1. The spline control points
2. The generated spline

Author: Derek Kelly
Version: 0.0.1 16/02/2015</summary>
*/

#include "../Framework/Mesh.h"
#include "../Framework/Vector3.h"

class Spline : public Mesh {
public:
	Spline(const Vector3& a, const Vector3& b, const Vector3& c, unsigned int subdivisions);
	virtual ~Spline();

	void DrawControlPoints();
	virtual void Draw();


protected:
	Vector3* ctrlPoints;
	unsigned int segments;
	GLuint controlVBO;
};