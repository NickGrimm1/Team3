#include "PhysicsEngine.h"
#include "RacerGame.h"
#include "../Framework/CollisionHelper.h"
#include <algorithm>

PhysicsEngine* PhysicsEngine::instance = NULL;

#if WINDOWS_BUILD
void PhysicsEngine::Run()
{
	ThreadRun();
}
#endif
#if PS3_BUILD
void PhysicsEngine::Run(uint64_t arg)
{
	std::cout << "Physics Thread Started! " << std::endl;
	//instance->ThreadRun();
	std::cout << "Physics Thread Ended! " << std::endl;
	sys_ppu_thread_exit(0);
}
#endif

void PhysicsEngine::ThreadRun()
{
	while (isRunning)
	{
#if WINDOWS_BUILD
		while (Window::GetWindow().GetTimer()->GetMS() - lastFrameTimeStamp < PHYSICS_TIME) { ; } // Fix the timestep
		float msec = Window::GetWindow().GetTimer()->GetMS() - lastFrameTimeStamp;
		lastFrameTimeStamp = Window::GetWindow().GetTimer()->GetMS();
#endif
#if PS3_BUILD
		float msec = 8;
#endif
		frameRate = (int)(1000.0f / msec);

	   // NarrowPhaseCollisions();

		narrowlist.clear();

		for(vector<Constraint*>::iterator i = allSprings.begin(); i != allSprings.end(); ++i) {
			(*i)->Update(msec);
		}

		for(vector<PhysicsNode*>::iterator i = allNodes.begin(); i != allNodes.end(); ++i) {
			(*i)->Update(msec);
			narrowlist.push_back((*i));
		}
		BroadPhaseCollisions();
	}
}

T3Vector3 PhysicsEngine::support(PhysicsNode& shape1,PhysicsNode& shape2, T3Vector3 dir)
{
	
	T3Vector3 p1=shape1.GetFathestPointInDirection(shape1,dir);
	T3Vector3 p2=shape2.GetFathestPointInDirection(shape2,T3Vector3(-dir.x,-dir.y,-dir.z));

	T3Vector3 p3=p1-p2;

	return p3;


}

 bool PhysicsEngine::CollisionDetection(PhysicsNode& shape1,PhysicsNode& shape2)
{

	worldpoints1.clear();
	worldpoints2.clear();
	Vertex * vertex1=shape1.GetPhysicsMesh()->GetVertices();
	int number1=shape1.GetPhysicsMesh()->GetNumVertices();
	for(int i=0;i<number1;i++)
	{
		worldpoints1.push_back(T3Matrix4::Translation(shape1.GetTarget()->GetOriginPosition()) * shape1.GetTarget()->GetRotation().ToMatrix() * T3Matrix4::Scale(shape1.GetTarget()->GetScale()) * vertex1[i].GetPosition());
	}
	shape1.SetWorldPoints(worldpoints1);

	//Vertex * vertex2=shape2.GetTarget()->GetMesh()->GetVertices();
	Vertex * vertex2=shape2.GetPhysicsMesh()->GetVertices();
	int number2=shape2.GetPhysicsMesh()->GetNumVertices();
	for(int i=0;i<number2;i++)
	{
		worldpoints2.push_back(T3Matrix4::Translation(shape2.GetTarget()->GetOriginPosition()) * shape2.GetTarget()->GetRotation().ToMatrix() * T3Matrix4::Scale(shape2.GetTarget()->GetScale()) * vertex2[i].GetPosition());
	
	}
	shape2.SetWorldPoints(worldpoints2);

	 T3Vector3 dir = T3Vector3(1, 1, 1);
		
	 c = support(shape1, shape2, dir);
		 	
	 dir = c * -1.0f;//negative direction

	 b = support(shape1, shape2, dir);

	 if (T3Vector3::Dot(dir,b) < 0)
	 {
		 return false;
	 }
	 dir = T3Vector3::doubleCross(c - b, b* -1.0f);
	 
	 numPointsSimplex = 2; //begin with 2 points in simplex
	
	 int steps = 0;//avoid infinite loop
	 while (steps<50)
	 {
		 a = support(shape1, shape2, dir);
		 if (T3Vector3::Dot(dir,a) < 0)
		 {
			 return false;
		 }
		 else
		 {
			 
			 if (containsOrigin(dir))
			 {

				 	
				 return true;
			 }
		 }
		 steps++;

	 }
	
	 return false;
}



 



bool PhysicsEngine::containsOrigin(T3Vector3& dir)
{

	 if (numPointsSimplex == 2)
	 {
		return triangle(dir);
	 }
	 else if (numPointsSimplex == 3)
	 {
		return tetrahedron(dir);
	 }
	
	 return false;
	
	}


bool PhysicsEngine::triangle(T3Vector3& dir)
 {
	 T3Vector3 ao = T3Vector3(-a.x, -a.y, -a.z);
	 T3Vector3 ab = b - a;
	 T3Vector3 ac = c - a;
	 T3Vector3 abc = T3Vector3::Cross(ab, ac);

	 //point is can't be behind/in the direction of B,C or BC

	
	 T3Vector3 ab_abc = T3Vector3::Cross(ab, abc);
	 // is the origin away from ab edge? in the same plane
	 //if a0 is in that direction than
	 if (T3Vector3::Dot(ao,ab_abc) > 0)
	 {
		//change points
		c = b;
		b = a;

		//dir is not ab_abc because it's not point towards the origin hjfe
		dir = T3Vector3::doubleCross(ab,ao);

		//direction change; can't build tetrahedron
		return false;
	 }

	
	 T3Vector3 abc_ac = T3Vector3::Cross(abc, ac); 

	 // is the origin away from ac edge? or it is in abc?
	 //if a0 is in that direction than
	 if (T3Vector3::Dot(ao,abc_ac) > 0)
	 {
		//keep c the same
		b = a;

		//dir is not abc_ac because it's not point towards the origin
		dir = T3Vector3::doubleCross(ac, ao);
				
		//direction change; can't build tetrahedron
		return false;
	 }

	 //now can build tetrahedron; check if it's above or below
	 if (T3Vector3::Dot(ao,abc) > 0)
	 {
		 //base of tetrahedron
		 d = c;
		 c = b;
		 b = a;

		 //new direction
		 dir = abc;
	 }
	 else
	 {
		 //upside down tetrahedron
		 d = b;
		 b = a;
		 dir = abc * -1.0f;
	 }

	 numPointsSimplex = 3;
	
	 return false;
 }

 bool PhysicsEngine::tetrahedron(T3Vector3& dir)
 {
	 T3Vector3 ao = a* -1.0f;//0-a
	 T3Vector3 ab = b - a;
	 T3Vector3 ac = c - a;
	 
	 //build abc triangle
	 T3Vector3 abc = T3Vector3::Cross(ab, ac);

	 //CASE 1
	 if (T3Vector3::Dot(ao,abc) > 0)
	 {
		 //in front of triangle ABC
		 //we don't have to change the ao,ab,ac,abc meanings
		   return checkTetrahedron(ao,ab,ac,abc,dir);
	 }
	 

	 //CASE 2:
	 
	 T3Vector3 ad = d - a;

	 //build acd triangle
	  T3Vector3 acd = T3Vector3::Cross(ac, ad);

	 //same direaction with ao
	  if (T3Vector3::Dot(ao,acd) > 0)
	 {

		 //in front of triangle ACD
		 b = c;
		 c = d;
		 ab = ac;
		 ac = ad;
		 abc = acd;

		  return checkTetrahedron(ao, ab, ac, abc, dir);
	 }

	 //build adb triangle
	 T3Vector3 adb = T3Vector3::Cross(ad, ab);

	 //same direaction with ao
	 if (T3Vector3::Dot(ao,adb) > 0)
	 {

		 //in front of triangle ADB

		 c = b;
		 b = d;

		 ac = ab;
		 ab = ad;

		 abc = adb;
		 return checkTetrahedron(ao, ab, ac, abc, dir);
	 }

				/*	OGLRenderer::DrawDebugLine(DEBUGDRAW_PERSPECTIVE, a, b, T3Vector3(1, 0, 1), T3Vector3(1, 0, 1));
					OGLRenderer::DrawDebugLine(DEBUGDRAW_PERSPECTIVE, c, b, T3Vector3(1, 0, 1), T3Vector3(1, 0, 1));
					OGLRenderer::DrawDebugLine(DEBUGDRAW_PERSPECTIVE, a, c, T3Vector3(1, 0, 1), T3Vector3(1, 0, 1));
					OGLRenderer::DrawDebugLine(DEBUGDRAW_PERSPECTIVE, a, d, T3Vector3(1, 1, 1), T3Vector3(1, 1, 1));
					OGLRenderer::DrawDebugLine(DEBUGDRAW_PERSPECTIVE, b, d, T3Vector3(1, 1, 1), T3Vector3(1, 1, 1));
					OGLRenderer::DrawDebugLine(DEBUGDRAW_PERSPECTIVE, c, d, T3Vector3(1, 1, 1), T3Vector3(1, 1, 1));*/

	 //origin in tetrahedron
	 return true;

 }

 bool PhysicsEngine::checkTetrahedron(const T3Vector3& ao,
									  const T3Vector3& ab,
									  const T3Vector3& ac,
									  const T3Vector3& abc,
									  T3Vector3& dir)
 {
	 
	//almost the same like triangle checks
	T3Vector3 ab_abc = T3Vector3::Cross(ab, abc);

	if (T3Vector3::Dot(ao,ab_abc) > 0)
	 {
		 c = b;
		 b = a;

		 //dir is not ab_abc because it's not point towards the origin;
		 //ABxA0xAB direction we are looking for
		 dir = T3Vector3::doubleCross(ab, ao);
		 
		 //build new triangle
		 // d will be lost
		 numPointsSimplex = 2;

		 return false;
	 }

	 T3Vector3 acp = T3Vector3::Cross(abc, ac);

	 if (T3Vector3::Dot(ao,acp) > 0)
	 {
		 b = a;

		 //dir is not abc_ac because it's not point towards the origin;
		 //ACxA0xAC direction we are looking for
		 dir = T3Vector3::doubleCross(ac, ao);
		 
		 //build new triangle
		 // d will be lost
		 numPointsSimplex = 2;

		 return false;
	 }

	 //build new tetrahedron with new base
	 d = c;
	 c = b;
	 b = a;

	 dir = abc;

	 numPointsSimplex = 3;

	 return false;
 }




//void  PhysicsEngine::SortandSweep()
//{
//
//	
//
//	sort(narrowlist.begin(),narrowlist.end(), [](PhysicsNode* xleft, PhysicsNode* xright){return xleft->GetPosition().x < xright->GetPosition().x;});
//
//	for( vector<PhysicsNode*>::iterator i=narrowlist.begin(); i <narrowlist.end(); ++i) 
//	{
//		for( vector<PhysicsNode*>::iterator j= i +1; j !=narrowlist.end(); ++j) 
//		{
//			if((*i)->GetXend() > (*j)->GetXstart())
//			{
//				    PhysicsNode& first =*(*i);
//				    PhysicsNode& second =*(*j);
//
//
//
//
//
//
//
//
//
//				
//				  /*  switch((*i)->GetCollisionVolume()->GetType()) 
//					{*/
//								/* case COLLISION_VOL_SPHERE:
//				
//								 {
//									  {
//											   switch((*j)->GetCollisionVolume()->GetType()) 
//											 {
//												case COLLISION_VOL_PLANE:
//												CollisionData data;
//												if (CollisionHelper::PlaneSphereCollision(second, first, &data)) 
//											   {
//													CollisionHelper::AddCollisionImpulse(second, first, data);
//											   }
//				        						continue;
//											 }
//										   }
//
//											 switch((*j)->GetCollisionVolume()->GetType())  
//										   {
//												case COLLISION_VOL_SPHERE:
//												CollisionData data;
//												if (CollisionHelper::SphereSphereCollision(first, second, &data))
//												{
//					     						  CollisionHelper::AddCollisionImpulse(first, second, data);
//												}
//												continue;
//										   }
//						 
//										 
//								 }
//				
//								case COLLISION_VOL_PLANE:
//								switch((*j)->GetCollisionVolume()->GetType()) 
//								{
//									 case COLLISION_VOL_SPHERE:
//				    					CollisionData data;
//									  if (CollisionHelper::PlaneSphereCollision(first, second, &data)) 
//									  {
//										CollisionHelper::AddCollisionImpulse(first, second, data);
//									  }
//										continue;
//								  }*/
//
//								/*case COLLISION_VOL_AABB:
//								switch((*j)->GetCollisionVolume()->GetType()) 
//								{
//									 case COLLISION_VOL_AABB:
//				    					
//									  if (CollisionHelper::AABBCollision(first, second )) 
//									  {
//									   first.SetLinearVelocity(T3Vector3(0,0,0));      
//									   first.SetForce(T3Vector3(0,0,0));
//									    second.SetLinearVelocity(T3Vector3(0,0,0));
//								       second.SetForce(T3Vector3(0,0,0));
//									  }
//									 
//										continue;
//								  }*/
//						}
//		
//				}
//	
//		}
//	}
	




void  PhysicsEngine::SortandSweep()
{

	
#if WINDOWS_BUILD
	std::sort(narrowlist.begin(),narrowlist.end(), [](PhysicsNode* xleft, PhysicsNode* xright){return xleft->GetPosition().x < xright->GetPosition().x;});

	for( vector<PhysicsNode*>::iterator i=narrowlist.begin(); i <narrowlist.end() - 1; ++i) 
	{
		for( vector<PhysicsNode*>::iterator j= i +1; j < narrowlist.end(); ++j) 
		{
			
			if((*i)->GetXend() > (*j)->GetXstart())
			{
				    PhysicsNode& first =*(*i);
				    PhysicsNode& second =*(*j);
					//cout << "car x start = " << first.GetXstart() << " x end = " << first.GetXend() << endl;
						//cout << "box x start = " << second.GetXstart() << " x end = " << second.GetXend() << endl;
					
					if(CollisionDetection(first, second))
			    {
				//cout << "GJK passed" << endl;
				if(first.GetIsCollide()==false || second.GetIsCollide ()==false)
				{
					//cout << "Collision" << endl;
					first.SetLinearVelocity(T3Vector3(0,0,0));
					first.SetForce(T3Vector3(0,0,0));
                    second.SetLinearVelocity(T3Vector3(0,0,0));
					second.SetForce(T3Vector3(0,0,0));
				}
				}

						}
				}
		}
#endif
	}
	




void	PhysicsEngine::BroadPhaseCollisions() {
	SortandSweep();

}

void	PhysicsEngine::NarrowPhaseCollisions() {

	
	for (unsigned int i = 0; i < allNodes.size(); i++) {

		if (allNodes[i]->GetPhysicsMesh() == NULL) continue;
		PhysicsNode& first = *allNodes[i];
		CollisionVolume* fv = first.GetCollisionVolume();
		if (!fv) continue;
		for (unsigned int j = i + 1; j < allNodes.size(); j++) {
			if (allNodes[j]->GetPhysicsMesh() == NULL) continue;

			PhysicsNode& second = *allNodes[j];
			CollisionVolume* sv = second.GetCollisionVolume();
			if (!sv) continue;

			
			if(CollisionDetection(first, second))
			{
						
				
				//cout << "GJK passed" << endl;
				if(first.GetIsCollide()==false || second.GetIsCollide ()==false)
				{
					if(check==true)
					{
                    cout << "Collision" << endl;

					   if(first.GetType()=='c'||second.GetType()=='c')
					   {
							cout<<"c is call";
							if(first.GetGameEntity())
							{
								cout<<"call if"<<endl;
#if WINDOWS_BUILD
								//RacerGame::update=1;}
#endif
							}
							else
							{
							
								if(second.GetGameEntity())
								{
								

						cout<<"call else"<<endl;
#if WINDOWS_BUILD
					//RacerGame::update=1;
#endif
								}
							}
							check=false;
						}
					/*first.SetLinearVelocity(T3Vector3(0,0,0));
					first.SetForce(T3Vector3(0,0,0));
                    second.SetLinearVelocity(T3Vector3(0,0,0));
					second.SetForce(T3Vector3(0,0,0));*/
					}
			
			
				}
				
			}

	


		//	switch(fv->GetType()) {
		//	/*case COLLISION_VOL_SPHERE:
		//		switch(sv->GetType()) {
		//		case COLLISION_VOL_SPHERE:
		//			CollisionData data;
		//			if (CollisionHelper::SphereSphereCollision(first, second, &data)) {
		//				CollisionHelper::AddCollisionImpulse(first, second, data);
		//			}
		//			continue;
		//		}
		//	case COLLISION_VOL_PLANE:
		//		switch(sv->GetType()) {
		//		case COLLISION_VOL_SPHERE:
		//			CollisionData data;
		//			if (CollisionHelper::PlaneSphereCollision(first, second, &data)) {
		//				CollisionHelper::AddCollisionImpulse(first, second, data);
		//			}
		//			continue;
		//		}*/
		//   	case COLLISION_AABB:
		//						switch(sv->GetType()) 
		//						{
		//							 case COLLISION_AABB:
		//		    					
		//							  if (CollisionHelper::AABBCollision(first, second )) 
		//							  {
		//								 // if(count==false){
		//									//  MyGame::newt=1 ;
		//									//	  count=true;}


		//							  //count=count+1;
		//							  // cout<<count<<endl;
		//							  }
		//							  continue;

		//							  case COLLISION_VOL_BBAA:
		//		    					
		//							  if (CollisionHelper::BBAACollision(second,first )) 
		//							  {
		//								 


		//								 // if(count==true){
		//									//  MyGame::dt=1 ;
		//							//	  count=false;}
		//							 // count=count+1;
		//							 //  cout<<count<<endl;
		//							  }
		//							 
		//								continue;
		//						  }


		//case COLLISION_VOL_BBAA:
		//						switch(sv->GetType()) 
		//						{
		//							 case COLLISION_AABB:
		//		    					
		//							  if (CollisionHelper::BBAACollision(first, second )) 
		//							  {
		//								 


		//								//  if(count==true){
		//									//  MyGame::dt=1 ;
		//								//  count=false;}
		//							 // count=count+1;
		//							  // cout<<count<<endl;
		//							  }
		//							 
		//								continue;
		//						  }
		//	}
		//
		}
	}
}

void	PhysicsEngine::AddNode(PhysicsNode* n) {
	allNodes.push_back(n);
}

void	PhysicsEngine::RemoveNode(PhysicsNode* n) {
	for(vector<PhysicsNode*>::iterator i = allNodes.begin(); i != allNodes.end(); ++i) {
		if((*i) == n) {
			i = allNodes.erase(i);
			return;
		}
	}
}

void	PhysicsEngine::AddConstraint(Constraint* s) {
	allSprings.push_back(s);
}

void	PhysicsEngine::RemoveConstraint(Constraint* c) {
	for(vector<Constraint*>::iterator i = allSprings.begin(); i != allSprings.end(); ++i) {
		if((*i) == c) {
			i = allSprings.erase(i);
			return;
		}
	}
}

void	PhysicsEngine::AddDebugDraw(DebugDrawer* d) {
	allDebug.push_back(d);
}

void	PhysicsEngine::RemoveDebugDraw(DebugDrawer* d) {
	for(vector<DebugDrawer*>::iterator i = allDebug.begin(); i != allDebug.end(); ++i) {
		if((*i) == d) {
			i = allDebug.erase(i);
			return;
		}
	}
}

void    PhysicsEngine::DrawDebug() {
	for(vector<DebugDrawer*>::iterator i = allDebug.begin(); i != allDebug.end(); ++i) {
		(*i)->DebugDraw();
	}
}