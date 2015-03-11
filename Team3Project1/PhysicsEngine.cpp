#include "PhysicsEngine.h"
#include "RacerGame.h"
#include "../Framework/CollisionHelper.h"
#if WINDOWS_BUILD
#include <algorithm>
#include <list>
#endif

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
	//instance->ThreadRun();
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

#if WINDOWS_BUILD
		NarrowPhaseCollisions();

		narrowlist.clear();

		for(vector<Constraint*>::iterator i = allSprings.begin(); i != allSprings.end(); ++i) {
			(*i)->Update(msec);
		}

		for(vector<PhysicsNode*>::iterator i = allNodes.begin(); i != allNodes.end(); ++i) {
			(*i)->Update(msec);
			narrowlist.push_back((*i));
		}
		//BroadPhaseCollisions();
#endif
	}
}
#if WINDOWS_BUILD
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
	
	Vertex * vertex1=shape1.GetPhysicsVertex();
	int number1=shape1.GetNunmberVertex();
	for(int i=0;i<number1;i++)
	{
		worldpoints1.push_back(T3Matrix4::Translation(shape1.GetTarget()->GetOriginPosition()) * shape1.GetTarget()->GetRotation().ToMatrix() * T3Matrix4::Scale(shape1.GetTarget()->GetScale()) * vertex1[i].GetPosition());
	}
	shape1.SetWorldPoints(worldpoints1);

	//Vertex * vertex2=shape2.GetTarget()->GetMesh()->GetVertices();
	Vertex * vertex2=shape2.GetPhysicsVertex();
	int number2=shape2.GetNunmberVertex();
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

	for( vector<PhysicsNode*>::iterator i=narrowlist.begin(); i <narrowlist.end(); ++i) 
	{
		for( vector<PhysicsNode*>::iterator j= i +1; j !=narrowlist.end(); ++j) 
		{
			
			if((*i)->GetXend() > (*j)->GetXstart())
			{
				    PhysicsNode& first =*(*i);
				    PhysicsNode& second =*(*j);

					if (first.GetPhysicsVertex() == NULL) continue;
					if (second.GetPhysicsVertex() == NULL) continue;
					if(CollisionDetection(first, second))
			    {
				
				if(first.GetIsCollide()==false && second.GetIsCollide ()==true)
				{
					//if(second.GetType()=='f'){
					OnCollision(first,second);
					//}
				}
				if(first.GetIsCollide()==false || second.GetIsCollide ()==false)
				{
					
					first.SetLinearVelocity(T3Vector3(0,0,0));
					first.SetForce(T3Vector3(0,0,0));
                    second.SetLinearVelocity(T3Vector3(0,0,0));
					second.SetForce(T3Vector3(0,0,0));
				}
			
				if ((first.GetIsCollide()==true && second.GetIsCollide ()==true) && ((first.Getcar_wheel()==true && second.Getcar_wheel()==true)))
				{
			
					/*first.SetLinearVelocity(T3Vector3(0,0,0));
					first.SetForce(T3Vector3(0,0,0));
                    second.SetLinearVelocity(T3Vector3(0,0,0));
					second.SetForce(T3Vector3(0,0,0));*/

					CollisionData* data = new CollisionData();
					bool succeeded = EPA(first, second, data);
 					if (succeeded)
					{
						CollisionHelper::AddCollisionImpulse( first, second, *data);

			}
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

		if (allNodes[i]->GetPhysicsVertex() == NULL) continue;
		PhysicsNode& first = *allNodes[i];
	//	CollisionVolume* fv = first.GetCollisionVolume();
	//	if (!fv) continue;
		for (unsigned int j = i + 1; j < allNodes.size(); j++) {
			if (allNodes[j]->GetPhysicsVertex() == NULL) continue;

			PhysicsNode& second = *allNodes[j];
		//	CollisionVolume* sv = second.GetCollisionVolume();
			//if (!sv) continue;

			
			if(CollisionDetection(first, second))
			{
				//OnCollision(first,second);
				if(first.GetIsCollide()==true && second.GetIsCollide ()==true)
				{
					OnCollision(first,second);
					//if(second.GetType()=='f'){
					//OnCollision(first,second);
					//}
				}
				if(first.GetIsCollide()==false || second.GetIsCollide ()==false)
				{
					//OnCollision(first,second);
					//OnCollision(first,second);
						/*if(check==true)
					{

					   if(first.GetType()=='c'||second.GetType()=='c')
					   {
						if(first.GetGameEntity()){
						RacerGame::update=1;}
					   }

					   else{
						if(second.GetGameEntity())
					      {
					RacerGame::update=1;
						  }
					    }
					
						check=false;
					}*/
					/*first.SetLinearVelocity(T3Vector3(0,0,0));
					first.SetForce(T3Vector3(0,0,0));
                    second.SetLinearVelocity(T3Vector3(0,0,0));
					second.SetForce(T3Vector3(0,0,0));*/
				}
			
				if ((first.GetIsCollide()==true && second.GetIsCollide ()==true) && ((first.Getcar_wheel()==true && second.Getcar_wheel()==true)))
				{
			
					/*first.SetLinearVelocity(T3Vector3(0,0,0));
					first.SetForce(T3Vector3(0,0,0));
                    second.SetLinearVelocity(T3Vector3(0,0,0));
					second.SetForce(T3Vector3(0,0,0));*/

					CollisionData* data = new CollisionData();
					bool succeeded = EPA(first, second, data);
 					if (succeeded)
					{
						CollisionHelper::AddCollisionImpulse(first, second, *data);


						if(first.Getplanecollision()==true && second.Getplanecollision()==false)

						{
							first.SetUseGravity(false);
							second.SetUseGravity(false);
							T3Vector3 temp1,temp2;
							temp1 =  first.GetLinearVelocity();
							temp1.y = 0;
							first.SetLinearVelocity(temp1);
							
							temp2 =  second.GetLinearVelocity();
							temp2.y = 0;
							second.SetLinearVelocity(temp2);
													
			                second.SetPosition(second.GetPosition() + T3Vector3(0, 2.0f, 0));

			}
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
		//							  }
		//							 
		//								continue;
		//						  }
		//	}
		//
		}
	}
}

float Dist(const T3Vector3& a, const T3Vector3& b)
{
	return (a-b).Length();
}

	const unsigned _EXIT_ITERATION_LIMIT =50;
	const float _EXIT_THRESHOLD = 0.0001f;
	struct EPA_Point
	{
		T3Vector3 v;
		BOOL operator==(const EPA_Point &a) const { return v == a.v; }
	};

	//the edge to store points of triangle
	struct EPA_Edge
	{
		EPA_Point Point[2];
		EPA_Edge(const T3Vector3 &a, const T3Vector3 &b)
		{
			Point[0].v = a;
			Point[1].v = b;
		}
	};

	struct EPA_Triangle
	{
		EPA_Point Point[3];
		T3Vector3 Triangle_normal;

		EPA_Triangle(const T3Vector3 &a, const T3Vector3 &b, const T3Vector3 &c)
		{
			Point[0].v = a;
			Point[1].v = b;
			Point[2].v = c;

			Triangle_normal= T3Vector3::Cross((a-b),(a-c));
			Triangle_normal.Normalise();
	}
	};

	std::vector<EPA_Triangle> lst_EPA_Triangle;
	std::vector<EPA_Edge> lst_EPA_Edge;
	auto addEdge = [&](const T3Vector3 &a, const T3Vector3 &b) -> void
	{
		for(auto it = lst_EPA_Edge.begin(); it != lst_EPA_Edge.end();it++)
		{
			//if the adge already exsists, delete it and do not add the new one
			if((it->Point[0].v - b).Length() < _EXIT_THRESHOLD && (it->Point[1].v - a).Length() < _EXIT_THRESHOLD)
			{
				it = lst_EPA_Edge.erase(it);
				break;
			}
			//it++;
		}
		lst_EPA_Edge.push_back(EPA_Edge(a, b));
	};

	
bool PhysicsEngine::EPA(PhysicsNode& shape1,PhysicsNode& shape2, CollisionData* data)
{


	lst_EPA_Edge.clear();
	lst_EPA_Triangle.clear();


	//Avoid Degenerate Triangles from GJK
	if (Dist(a,b) < _EXIT_THRESHOLD 
		|| Dist(a,c) < _EXIT_THRESHOLD 
		|| Dist(a,d) < _EXIT_THRESHOLD 
		|| Dist(b,c) < _EXIT_THRESHOLD
		|| Dist(b,d) < _EXIT_THRESHOLD
		|| Dist(c,d) < _EXIT_THRESHOLD)
	return false;

	// add the triangles from GJK to the list
	lst_EPA_Triangle.emplace_back(a,b,c);
	lst_EPA_Triangle.emplace_back(a,c,d);
	lst_EPA_Triangle.emplace_back(b,c,d);
	lst_EPA_Triangle.emplace_back(a,b,d);


	// Fix Triangle winding order
	T3Vector3 centre = (a + b + c + d) / 4.0f;
	for (unsigned int i = 0, len = lst_EPA_Triangle.size(); i < len; ++i)
	{
		EPA_Triangle& t =  lst_EPA_Triangle[i];
		if (T3Vector3::Dot(t.Triangle_normal, centre) > 0.0f)
		{
			EPA_Triangle replace = EPA_Triangle(t.Point[0].v, t.Point[2].v, t.Point[1].v);
			t = replace;
		}
	}


	// add the adge to the list when finding the triangle which can be seen by the new point
	unsigned _EXIT_ITERATION_NUM = 0;
	// loop to find the final triangle which is cloest to the origin
	while (true)
	{
		if(_EXIT_ITERATION_NUM++ >= _EXIT_ITERATION_LIMIT) 
			break;
	
		if (lst_EPA_Triangle.size() > 500)
			break;

		float closest_distance = 10000.0f;
		T3Vector3 contactpoint;
		std::vector<EPA_Triangle>::iterator closest_triangle = lst_EPA_Triangle.begin();

		// find the initial closest triangle to origin
		for(auto it = lst_EPA_Triangle.begin(); it != lst_EPA_Triangle.end(); it++)
		{
			float distance = fabs(T3Vector3::Dot (it->Triangle_normal, it->Point[0].v));

			if(distance < closest_distance)
			{
				closest_distance = distance;
				closest_triangle = it;
			}
		}

		//new point which is in the direction of closest triangle's normal
		T3Vector3 new_point = support(shape1, shape2, closest_triangle->Triangle_normal);
	
		//distance from new point to the closest triangle
		float distance_new_point = abs(T3Vector3::Dot(closest_triangle->Triangle_normal, new_point));
	
		if(distance_new_point - closest_distance < _EXIT_THRESHOLD)
		{
			float bary_x,bary_y,bary_z;

			//calculate the barycentric coordinates of the closest point

			barycentric(closest_triangle->Triangle_normal * closest_distance,
				closest_triangle->Point[0].v, closest_triangle->Point[1].v, closest_triangle->Point[2].v, 
				&bary_x, &bary_y, &bary_z);

			// contact point
			contactpoint = ((closest_triangle->Point[0].v*bary_x)+
						  (closest_triangle->Point[1].v*bary_y)+
						  (closest_triangle->Point[2].v*bary_z));		

			//OGLRenderer::DrawDebugLine(DEBUGDRAW_PERSPECTIVE, closest_triangle->Point[0].v, closest_triangle->Point[1].v);
		//	OGLRenderer::DrawDebugLine(DEBUGDRAW_PERSPECTIVE, closest_triangle->Point[0].v, closest_triangle->Point[2].v);
		//	OGLRenderer::DrawDebugLine(DEBUGDRAW_PERSPECTIVE, closest_triangle->Point[1].v, closest_triangle->Point[2].v);

			//collision data
			T3Matrix4 transform_1 = T3Matrix4::Translation(shape1.GetTarget()->GetOriginPosition()) * shape1.GetTarget()->GetRotation().ToMatrix() * T3Matrix4::Scale(shape1.GetTarget()->GetScale());
			T3Matrix4 transform_2 = T3Matrix4::Translation(shape2.GetTarget()->GetOriginPosition()) * shape2.GetTarget()->GetRotation().ToMatrix() * T3Matrix4::Scale(shape2.GetTarget()->GetScale());


			//contactpoint = shape2.GetPosition() - contactpoint;
			OGLRenderer::DrawDebugCross(DEBUGDRAW_PERSPECTIVE, transform_1 * -contactpoint, T3Vector3(2, 2, 2), T3Vector3(1, 0, 1));
			OGLRenderer::DrawDebugCross(DEBUGDRAW_PERSPECTIVE, transform_2 * contactpoint, T3Vector3(2, 2, 2), T3Vector3(0, 1, 1));

			data-> m_point1 = transform_1 * -contactpoint;
			data-> m_point2 = transform_2 * -contactpoint;
			data-> m_normal =  -closest_triangle->Triangle_normal;
			data-> m_penetration = closest_distance * 2.f;

			return (closest_triangle->Triangle_normal.Length() > 0.9f);
				
		}

		// remove the triangles which can be seen by the new point
		for(auto it= lst_EPA_Triangle.begin(); it!= lst_EPA_Triangle.end();)
		{
			//can 'it' be seen from the new point?
			if(T3Vector3::Dot(it->Triangle_normal, (new_point - it->Point[0].v)) > _EXIT_THRESHOLD)
			{
				if (new_point == it->Point[0].v || new_point == it->Point[1].v)
			{
					printf("INVALID TRIANGLE!!");
					return false;
				}

				addEdge(it->Point[0].v, it->Point[1].v);
				addEdge(it->Point[1].v, it->Point[2].v);
				addEdge(it->Point[2].v, it->Point[0].v);

				it = lst_EPA_Triangle.erase(it);
				continue;
			}
			it++;
		}

		// add new triangles which contains new point to the list 
		for(auto it= lst_EPA_Edge.begin(); it!= lst_EPA_Edge.end();it++)
		{
			lst_EPA_Triangle.emplace_back(new_point, it->Point[0].v, it->Point[1].v);
		}
		lst_EPA_Edge.clear();
				
	}

	return false;
}

void PhysicsEngine::barycentric(const T3Vector3 &p, const T3Vector3 &a, const T3Vector3 &b, const T3Vector3 &c, float *x, float *y, float *z) {
	// code from Crister Erickson's Real-Time Collision Detection      
	T3Vector3 v0 = b - a,v1 = c - a,v2 = p - a; 
	float d00 = T3Vector3::Dot(v0,v0); 
	float d01 = T3Vector3::Dot(v0,v1);
	float d11 = T3Vector3::Dot(v1,v1);
	float d20 = T3Vector3::Dot(v2,v0); 
	float d21 = T3Vector3::Dot(v2,v1); 
	float denom = d00 * d11 - d01 * d01; 
	*y = (d11 * d20 - d01 * d21) / denom;     
	*z = (d00 * d21 - d01 * d20) / denom;      
	*x = 1.0f - *y - *z; 
}

void	PhysicsEngine::AddNode(PhysicsNode* n) {
	allNodes.push_back(n);
}

void	PhysicsEngine::RemoveNode(PhysicsNode* n) {
	for(vector<PhysicsNode*>::iterator i = allNodes.begin(); i != allNodes.end(); ++i) {
		if((*i) == n) {
			allNodes.erase(i);
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
			allSprings.erase(i);
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
			allDebug.erase(i);
			return;
		}
	}
}

void    PhysicsEngine::DrawDebug() {
	for(vector<DebugDrawer*>::iterator i = allDebug.begin(); i != allDebug.end(); ++i) {
		(*i)->DebugDraw();
	}
}

void PhysicsEngine::OnCollision(PhysicsNode& p1, PhysicsNode& p2)
{
	gameClass->CollisionBetween(p1.GetGameEntity(),p2.GetGameEntity());
}
#endif