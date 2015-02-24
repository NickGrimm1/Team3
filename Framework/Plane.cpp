#include "Plane.h"

Plane::Plane(const T3Vector3 &normal, float distance, bool normalise) {
	if(normalise) {
		float length = T3Vector3::Dot(normal,normal);

		this->normal   = normal		/ length;
		this->distance = distance	/ length;
	}
	else{
		this->normal = normal;
		this->distance = distance;
	}
}

bool Plane::SphereInPlane(const T3Vector3 &position, float radius) const {
	if(T3Vector3::Dot(position,normal)+distance <= -radius) {
		return false;
	}

	return true;	
}

bool Plane::PointInPlane(const T3Vector3 &position) const {
	float test = T3Vector3::Dot(position,normal);
	float test2 = test + distance;

	if(T3Vector3::Dot(position,normal)+distance <= 0.0f) {
		return false;
	}

	return true;
}