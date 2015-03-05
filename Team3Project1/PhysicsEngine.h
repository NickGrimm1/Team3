/**
<summary>Static Singleton.
The game's physics engine.

Author: Nick Grimm
Version: 0.0.1 03/02/2015.</summary>
*/
#pragma once
#include "Thread.h"
#include "../Framework/PhysicsNode.h"
#include "../Framework/Constraint.h"
#include "../Framework/DebugDrawer.h"
#include "../Team3Project1/DrawableEntity3D.h"

#include <vector>
#include <algorithm> 

using std::vector;

class PhysicsEngine : public Thread
{
	// TODO: Implement Physics Engine.
public:
	static bool Initialize(PhysicsEngine*& out) 
	{
		if (instance == NULL)
			instance = new PhysicsEngine();

		out = instance;
		return true; 
	}
	static void Destroy()
	{
		if (instance != NULL)
			delete instance;
		instance = NULL;
	}

#if WINDOWS_BUILD
	void Run();
#endif
#if PS3_BUILD
	static void Run(uint64_t arg);
#endif

	void Terminate() { isRunning = false; }

	void		BroadPhaseCollisions();
	void        SortandSweep();
	void		NarrowPhaseCollisions();

	bool   count;
	int numPointsSimplex;

	int GetFrameRate() { return frameRate; }

	
	T3Vector3 a, b, c, d;
	T3Vector3 support(PhysicsNode& shape1,PhysicsNode& shape2, T3Vector3 dir);
	bool triangle(T3Vector3& dir);
	bool tetrahedron(T3Vector3& dir);
	bool checkTetrahedron(const T3Vector3& ao,const T3Vector3& ab,const T3Vector3& ac,const T3Vector3& abc,T3Vector3& dir);
	bool GJK(PhysicsNode& shape1,PhysicsNode& shape2);
	bool containsOrigin( T3Vector3& dir);
    bool CollisionDetection(PhysicsNode& shape1,PhysicsNode& shape2);
	
	//sam
	bool   check;
	//sam

    vector<T3Vector3> worldpoints1;
    vector<T3Vector3> worldpoints2;

	void	AddNode(PhysicsNode* n);

	void	RemoveNode(PhysicsNode* n);

	void	AddConstraint(Constraint* c);
	            
	void	RemoveConstraint(Constraint* c);

	void	AddDebugDraw(DebugDrawer* d);

	void	RemoveDebugDraw(DebugDrawer* d);

	void    DrawDebug();
private:
#if WINDOWS_BUILD
	PhysicsEngine()
		: PHYSICS_TIME(1000.0f / 120)
	{
		frameRate = 0;
		check=true;
	}
#endif
#if PS3_BUILD
	PhysicsEngine()
		: Thread(Run), PHYSICS_TIME(1000.0f / 120)
	{
		frameRate = 0;
	}
#endif

	~PhysicsEngine() { }
	static PhysicsEngine* instance;

	vector<PhysicsNode*> allNodes;
	vector<Constraint*> allSprings;
	vector<DebugDrawer*> allDebug;
	vector<PhysicsNode*> narrowlist;

	const float PHYSICS_TIME;
	float lastFrameTimeStamp;
	int frameRate;

	void ThreadRun();
};