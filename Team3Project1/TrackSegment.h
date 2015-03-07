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

	T3Vector3 GetTrackCentreLeft() const;
	T3Vector3 GetTrackCentreRight() const;

	virtual Vertex* GetVertices() const {return trackMesh;}
	virtual unsigned int GetNumVertices() const {return numVertices * 2;}

protected:
	Vertex* trackMesh;
	unsigned int trackVBO;
};

