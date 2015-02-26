#pragma once

/**
<summary>TrackSegment
Generates curved mesh based on a Spline and a width

Inherits Spline.
Methods available to draw 
1. The track boundarys
2. The track mesh

Author: Derek Kelly
Version: 0.0.1 16/02/2015</summary>
*/

#include "Spline.h"
#include "../Framework/T3Vector3.h"

class TrackSegment : public Spline
{
public:
	TrackSegment(const T3Vector3& a, const T3Vector3& b, const T3Vector3& c, unsigned int subdivisions, float trackWidth);
	virtual ~TrackSegment(void);

	void DrawSpline();
	void DrawTrackBoundaries();
	virtual void Draw();

protected:
	Vertex* trackMesh;
	unsigned int trackVBO;
};

