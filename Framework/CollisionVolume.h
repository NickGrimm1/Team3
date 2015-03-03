#pragma once

#include "T3Vector3.h"

/*
Rich: 
There are a few ways of integrating the collision volumes
discussed in this module into your little physics engine.
You could keep pointers to all types inside a physics node,
and check to see which ones are NULL. Or you could make all
collision volumes inherit from a base class, so we only need
a single pointer inside each Physics Node. We can then either
use a process of dynamic casting to determine which exact type
of volume the pointer is, or just cheat and use an enum member
variable (I do this in my own work, you can do whichever you
feel comfortable with!).
*/
enum CollisionVolumeType {
	COLLISION_SPHERE,
	COLLISION_AABB,
	COLLISION_PLANE,
	COLLISION_LINE,
	COLLISION_CAPSULE,
	COLLISION_VOL_BBAA
};

class CollisionVolume {
public:
	virtual CollisionVolumeType GetType() const { return type;}
	virtual void SetPosition(const T3Vector3& pos) {}; // define empty method so static entities like floors and walls can ignore updates

protected:
	CollisionVolumeType type;
};

class CollisionSphere : public CollisionVolume {
public:

	CollisionSphere(float radius): radius(radius) {}

	CollisionVolumeType GetType() const {
		return COLLISION_SPHERE;
	}

	float GetRadius() const {
		return radius;
	}

private:
	float radius;
};

class CollisionAABB : public CollisionVolume {
public:
	CollisionAABB(T3Vector3 halfDim): halfDim(halfDim) {}

	CollisionVolumeType GetType() const {
		return COLLISION_AABB;
	}

	T3Vector3 getHalfDimensions() const { return halfDim; }

private:
	T3Vector3 halfDim;
};


class CollisionBBAA : public CollisionVolume {
public:
	CollisionBBAA(T3Vector3 halfDim): halfDim(halfDim) {}

	CollisionVolumeType GetType() const {
		return COLLISION_VOL_BBAA;
	}

	T3Vector3 getHalfDimensions() const { return halfDim; }

private:
	T3Vector3 halfDim;
};


class CollisionPlane : public CollisionVolume {
public:
	CollisionPlane(const T3Vector3& n, float d) { // surface normal + distance from origin
		m_normal = n;
		m_distance = d;
		type = COLLISION_PLANE;
	}

	T3Vector3 GetNormal() const {
		return m_normal;
	}

	float GetDistance() const {
		return m_distance;
	}
	T3Vector3 m_normal; //Unit-length plane normal
	float	m_distance; //Distance from origin
};

class CollisionLine : public CollisionVolume {
public:
	CollisionLine(const T3Vector3& pos0, const T3Vector3& pos1) {
		m_pos0 = pos0;
		m_pos1 = pos1;
		type = COLLISION_LINE;
	}
	virtual void UpdateLine(const T3Vector3& pos0, const T3Vector3& pos1) {
		m_pos0 = pos0;
		m_pos1 = pos1;
	}
	T3Vector3 m_pos0;
	T3Vector3 m_pos1;
};

class CollisionCapsule : public CollisionLine { // Sphere Swept Line (approxs a cylinder)
public:
	CollisionCapsule(const T3Vector3& pos0, const T3Vector3& pos1, float r) : CollisionLine(pos0, pos1) {
		m_radius = r;
		type = COLLISION_CAPSULE;
	}
	
	virtual void UpdateCapsule(const T3Vector3& pos0, const T3Vector3& pos1, float radius) {
		m_pos0 = pos0;
		m_pos1 = pos1;
		m_radius = radius;
	}
	
	float m_radius;
};

class CollisionData {
public:
	T3Vector3 m_point;
	T3Vector3 m_normal;
	float m_penetration;
};