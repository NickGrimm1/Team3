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
	void Run();
	void Terminate() { isRunning = false; }

	void		BroadPhaseCollisions();
	void        SortandSweep();
	void		NarrowPhaseCollisions();

	bool   count;
	int numPointsSimplex;


	
	Vector3 a, b, c, d;
	Vector3 support(PhysicsNode& shape1,PhysicsNode& shape2, Vector3 dir);
	bool triangle(Vector3& dir);
	bool tetrahedron(Vector3& dir);
	bool checkTetrahedron(const Vector3& ao,const Vector3& ab,const Vector3& ac,const Vector3& abc,Vector3& dir);
	bool GJK(PhysicsNode& shape1,PhysicsNode& shape2);
	bool containsOrigin( Vector3& dir);
    bool CollisionDetection(PhysicsNode& shape1,PhysicsNode& shape2);
	
    vector<Vector3> worldpoints1;
    vector<Vector3> worldpoints2;

	void	AddNode(PhysicsNode* n);

	void	RemoveNode(PhysicsNode* n);

	void	AddConstraint(Constraint* c);
	            
	void	RemoveConstraint(Constraint* c);

	void	AddDebugDraw(DebugDrawer* d);

	void	RemoveDebugDraw(DebugDrawer* d);

	void    DrawDebug();
private:
	PhysicsEngine() { }
	~PhysicsEngine() { }
	static PhysicsEngine* instance;
	bool isRunning;




	vector<PhysicsNode*> allNodes;
	vector<Constraint*> allSprings;
	vector<DebugDrawer*> allDebug;
	vector<PhysicsNode*> narrowlist;
};