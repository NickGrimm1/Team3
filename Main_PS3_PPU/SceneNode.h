/******************************************************************************
Class:SceneNode
Implements:
Author:Rich Davison
Description: A subset of the SceneNode class you saw in the Graphics for Games
module. Extend this as you wish! Nothing here you haven't seen before...

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////


#pragma once

#include <vector>

class Mesh; 
#include <vectormath/cpp/vectormath_aos.h>

using namespace Vectormath::Aos;

class SceneNode	{
public:
	SceneNode(void);
	virtual ~SceneNode(void);

	void			SetTransform(const Matrix4 &matrix) { transform = matrix; } 
	const Matrix4&	GetTransform() const				{ return transform; }
	Matrix4			GetWorldTransform() const			{ return worldTransform; }

	void			SetMesh(Mesh*m)						{ mesh = m; }
	Mesh*			GetMesh()							{ return mesh; }

	virtual void	Update(float msec);


	void			AddChild(SceneNode& s);

	std::vector<SceneNode*>::const_iterator GetChildIteratorStart()	{return children.begin();}
	std::vector<SceneNode*>::const_iterator GetChildIteratorEnd()	{return children.end();}

protected:
	Mesh*		mesh;
	SceneNode*	parent;
	Matrix4		worldTransform;
	Matrix4		transform;

	std::vector<SceneNode*>		children;
};
