#if WINDOWS_BUILD
#pragma once

#include "SceneNode.h"
#include "PhysicsNode.h"
#include "SoundEmitter.h"

class SoundEntity	{
public:
	friend class GameClass;

	SoundEntity(void);
	//SoundEntity(SceneNode* s, PhysicsNode* p);
	SoundEntity(Sound*snd,SceneNode*s,PhysicsNode*se);
	virtual ~SoundEntity(void);

	virtual void	Update(float msec);

	SceneNode&		GetRenderNode()		{ return *renderNode;}
	PhysicsNode&	GetPhysicsNode()	{ return *physicsNode;}
	SoundEmitter&   GetSoundEmitter()	{ return *soundEmitter;}

	void			ConnectToSystems();
	//void			ConnectToSystemss();
	void			DisconnectFromSystems();

protected:
	SceneNode*		renderNode;
	PhysicsNode*	physicsNode;
	SoundEmitter*	soundEmitter;
};
#endif