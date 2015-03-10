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

#include "Mesh.h"
#include "../Framework/T3Vector3.h"

class Spline : public Mesh {
public:
	Spline(const T3Vector3& a, const T3Vector3& b, const T3Vector3& c, unsigned int subdivisions);
	virtual ~Spline();

	T3Vector3 GetAvgPosition() {return (ctrlPoints[0] + ctrlPoints[1] + ctrlPoints[2]) / 3.0f;}

	void DrawControlPoints();
	virtual void Draw();

	unsigned int GetSegments() const {return segments;}
protected:
	T3Vector3* ctrlPoints;
	unsigned int segments;
	unsigned int controlVBO;
};