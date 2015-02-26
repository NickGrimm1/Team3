/******************************************************************************
Class:SceneNode
Implements:
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>
Description:A basic, but functional SceneNode class. SceneNodes have a
transformation matrix, and any number of children. They also have a parent, 
forminimumg a tree structure, with their parent above them, and children below.

They also have a seperate scale for their Mesh, for no other reason than
it made the CubeRobot class easier ;) You might find it useful, though...

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////


#pragma once
#include "T3Matrix4.h"
#include "T3Vector3.h"
#include "T3Vector4.h"
#include <vector>

#if WINDOWS_BUILD
#include "OGLRenderer.h"
#endif
#if PS3_BUILD
class GCMRenderer;
#endif
class Mesh;
class DrawableEntity3D;

class SceneNode{
public:
	 SceneNode(Mesh*m = NULL, T3Vector4 colour = T3Vector4(1,1,1,1));
	 SceneNode(DrawableEntity3D *entity);

	~SceneNode(void);

	void			SetTransform(const T3Matrix4 &matrix) { transform = matrix;}
	const T3Matrix4&	GetTransform() const				{ return transform;}
	T3Matrix4			GetWorldTransform() const			{ return worldTransform;}

	void			SetPrevMVP(const T3Matrix4 mvp) 		{ prevFrameMVP = mvp; }
	T3Matrix4			GetPrevMVP() const					{ return prevFrameMVP; }

	virtual void	Update(float msec);
#if WINDOWS_BUILD
	virtual void	Draw(const OGLRenderer& r);
#endif
#if PS3_BUILD
	virtual void	Draw(const GCMRenderer &r);
#endif
	T3Vector4			GetColour()		const			{return colour;}
	void			SetColour(const T3Vector4 &c)		{colour = c;}

	T3Vector3			GetModelScale()		const			{return modelScale;}
	void			SetModelScale(const T3Vector3 &s)		{modelScale = s;}

	void			AddChild(SceneNode* s);
	void			AddChildToParent(DrawableEntity3D* child, DrawableEntity3D* parent);
	bool			RemoveChild(DrawableEntity3D* toDelete, bool recursive = true, bool removeChildren = true);

	float			GetBoundingRadius(); const
	void			SetBoundingRadius(float f)	{boundingRadius = f;}

	float			GetCameraDistance() const	{return distanceFromCamera;}
	void			SetCameraDistance(float f)	{distanceFromCamera = f;}

	void			SetMesh(Mesh*m)				{mesh = m;}
	Mesh*			GetMesh()					{return mesh;}

	void				SetDrawableEntity(DrawableEntity3D* entity) {sceneElement = entity;}
	DrawableEntity3D*	GetDrawableEntity() {return sceneElement;}

	bool	IsAwake()	{return awake;}
	void	Wake()		{awake = true;}
	void	Sleep()		{awake = false;} 

	std::vector<SceneNode*>::const_iterator GetChildIteratorStart()	{return children.begin();}
	std::vector<SceneNode*>::const_iterator GetChildIteratorEnd()	{return children.end();}

	static bool		CompareByCameraDistance(SceneNode*a,SceneNode*b) ;
	static bool		CompareByZ(SceneNode*a,SceneNode*b) ;

protected:
	DrawableEntity3D* sceneElement;
	T3Matrix4		worldTransform;
	T3Matrix4		transform;
	T3Matrix4		prevFrameMVP;
	SceneNode*	parent;
	float		distanceFromCamera;
	float		boundingRadius;
	T3Vector4		colour;
	T3Vector3		modelScale;
	bool		awake;
	Mesh*		mesh;
	std::vector<SceneNode*>		children;
};

