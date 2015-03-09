#include "PhysicsNode.h"

const T3Vector3 PhysicsNode::gravity = T3Vector3(0,-5,0);

PhysicsNode::PhysicsNode(void): vol(NULL) {
	target = NULL;
	useGravity = true;
	physicsMesh = NULL;
}

PhysicsNode::PhysicsNode(Quaternion orientation, T3Vector3 position): vol(NULL) {
	m_orientation	= orientation;
	m_position		= position;
	m_linearVelocity = T3Vector3(0,0,0);
	useGravity = true;
	physicsMesh = NULL;
}

PhysicsNode::~PhysicsNode(void)	{

}

//You will perform your per-object physics integration, here!
//I've added in a bit that will set the transform of the
//graphical representation of this object, too.
void	PhysicsNode::Update(float msec) {
	//FUN GOES HERE
	if (msec > 10000)
		msec = 80;

	float sec = msec / 1000.f;

	T3Vector3 acc = (m_force+m_friction)*m_invMass + (useGravity? gravity: T3Vector3(0,0,0));
	
	
	m_linearVelocity = m_linearVelocity + acc*sec;

	m_linearVelocity = m_linearVelocity*LINEAR_VELOCITY_DAMP;
	/*if (T3Vector3::Dot(m_linearVelocity, m_linearVelocity) < LINEAR_VELOCITY_MIN) {
		m_linearVelocity = T3Vector3(0,0,0);
	}*/

	m_position = m_position + m_linearVelocity*sec;

	


	
	
	/*switch(vol->GetType())
	{
	 case COLLISION_SPHERE:
		{ CollisionSphere& cSphere = *(CollisionSphere*)vol;  
			float r = cSphere.GetRadius();
			Xstart=m_position.x -r;
			Xend=m_position.x +r;
		}
			break;
	 
		case COLLISION_PLANE:
		{
			CollisionPlane& cPlane =  *(CollisionPlane*)vol;
			Xstart=m_position.x -1000;
			Xend=m_position.x +1000;
		}
			break;

			case COLLISION_AABB:
		{
			
			CollisionAABB& cAABB =  *(CollisionAABB*)vol;
			float l= cAABB.getHalfDimensions().x;
			Xstart=m_position.x -l;
			Xend=m_position.x +l;
		}
			break;
	
	}*/

	//F=u*T_engine*xg*xd*n/Rw;

	T3Vector3 angAcc = m_invInertia*m_torque;
	m_angularVelocity = m_angularVelocity + angAcc*sec;
	m_angularVelocity = m_angularVelocity*ANGULAR_VELOCITY_DAMP;
	m_orientation = m_orientation + m_orientation*(m_angularVelocity*(sec /2.0f));
	m_orientation.Normalise();

	m_force = T3Vector3(0,0,0);
	m_torque = T3Vector3(0,0,0);

	if(target) {
		target->SetRotation(m_orientation);
		target->SetOriginPosition(m_position);
	}
}

void PhysicsNode::AddForce(T3Vector3 point, T3Vector3 force) {
	m_force = m_force + force;
	m_torque = m_torque + T3Vector3::Cross(point - m_position, force);
}

/*
This function simply turns the orientation and position
of our physics node into a transformation matrix, suitable
for plugging into our Renderer!

It is cleaner to work with matrices when it comes to rendering,
as it is what shaders expect, and allow us to keep all of our
transforms together in a single construct. But when it comes to
physics processing and 'game-side' logic, it is much neater to
have seperate orientations and positions.

*/
T3Matrix4		PhysicsNode::BuildTransform() {
	T3Matrix4 m = m_orientation.ToMatrix();

	m.SetPositionVector(m_position);

	return m;
}

T3Vector3 PhysicsNode::GetFathestPointInDirection(PhysicsNode& shape1,T3Vector3 d){
	
	int index=0;

	Points=shape1.GetWorldPoints();



	float maximumDot= T3Vector3::Dot(Points[0],d);

		for(int i=1;i<Points.size();i++)
		{
			float dot=T3Vector3::Dot(Points[i],d);

			if(dot>maximumDot)
			{
			  maximumDot=dot;
			  index=i;
			
			}
		
		}
		return Points[index];

	//T3Vector3 returnVector;
	//
	//
	//	//T3Vector3 points = newWorldPoints[0]; 

 //     Points=GetWorldPoints();
	//
	//	float maximumDot=T3Vector3::Dot(Points[0],d);
	//	for(int i=1;i<Points.size();i++)
	//	{


	//		T3Vector3 m = Points[i] ; 
	//		float dot=T3Vector3::Dot(m,d);
	//		if(dot>maximumDot)
	//		{
	//			maximumDot=dot;
	//			returnVector=m;
	//		
	//		}

	//	}
	//	return returnVector;
		
}