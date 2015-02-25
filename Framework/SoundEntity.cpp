#if WINDOWS_BUILD
#include "SoundEntity.h"
#include "../Team3Project1/Renderer.h"
#include "../Team3Project1/PhysicsEngine.h"
#include "../Team3Project1/GameStateManager.h"

SoundEntity::SoundEntity(void)	{
	renderNode	= NULL;
	physicsNode = NULL;
	soundEmitter =NULL;

}

//SoundEntity::SoundEntity(Sound*snd,SceneNode*s,SoundEmitter*se){
//	soundEmitter = new SoundEmitter(snd);
//	//soundEmitter->SetSound(snd);
//	renderNode = s;
//	physicsNode= NULL;
//	//soundEmitter=se;
//}

SoundEntity::SoundEntity(Sound*snd,SceneNode*s,PhysicsNode*se){
	soundEmitter = new SoundEmitter(snd);
	renderNode = s;
	physicsNode= se;
	//soundEmitter=se;
}

SoundEntity::~SoundEntity(void)	{
	DisconnectFromSystems();

	delete renderNode;
	delete physicsNode;
	delete soundEmitter;
	
}

void	SoundEntity::Update(float msec) {

}


void	SoundEntity::ConnectToSystems() {
	//if(renderNode) {
	//	Renderer::GetRenderer().AddNode(renderNode);
	//}

	if(physicsNode) {
		GameStateManager::Physics()->AddNode(physicsNode);
		//PhysicsEngine::GetPhysicsEngine().AddNode(physicsNode);
	}

	if(soundEmitter) {
		SoundSystem::GetSoundSystem()->AddSoundEmitter(soundEmitter);
	}

	if(soundEmitter) {
		soundEmitter->SetTarget(renderNode);
	}
}

void	SoundEntity::DisconnectFromSystems() {
	if(renderNode) {
//		Renderer::GetRenderer().RemoveNode(renderNode);
	}

	if(physicsNode) {
		GameStateManager::Physics()->AddNode(physicsNode);
		//PhysicsEngine::GetPhysicsEngine().RemoveNode(physicsNode);
	}
	//if(soundEmitter) {
	//	SoundSystem::GetSoundSystem()->RemoveSoundEmitter(soundEmitter);
	//}

}
#endif