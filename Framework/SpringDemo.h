#pragma once
/**
 * Author: Callum Rhodes <c.g.rhodes@ncl.ac.uk>
 */
#include <vector>
#include "Constraint.h"
#include "DebugDrawer.h"
#include "T3Vector3.h"


class SpringDemo : public Constraint, public DebugDrawer {
public:
	SpringDemo();
	~SpringDemo() {}

	void Update(float msec);

	void DebugDraw();

private:
	std::vector<T3Vector3> positions;
	std::vector<T3Vector3> velocities;
	std::vector<T3Vector3> forces;

	int numBalls;
	float mass;
	float ks;
	float kd;
};