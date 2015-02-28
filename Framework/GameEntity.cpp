#include "GameEntity.h"
#include "../Team3Project1/Renderer.h"
#include "PhysicsSystem.h"
#include "../Team3Project1/GameStateManager.h"

GameEntity::GameEntity(void)	{
//	renderNode	= NULL;
	physicsNode = NULL;
}

GameEntity::GameEntity(PhysicsNode* p) {
//	renderNode	= s;
	physicsNode = p;
}

GameEntity::~GameEntity(void)	{
	DisconnectFromSystems();

//	delete renderNode;
	delete physicsNode;
}

void	GameEntity::Update(float msec) {

}

void	GameEntity::ConnectToSystems() {
	if(physicsNode) {
		GameStateManager::Physics()->AddNode(physicsNode);
		physicsNode->SetTarget(this);
	}

//	if(renderNode && physicsNode) {
//		physicsNode->SetTarget(renderNode);
//	}
}

void	GameEntity::DisconnectFromSystems() {
	//if(renderNode) {
//		Renderer::GetRenderer().RemoveNode(renderNode);
//	}

	if(physicsNode) {
		GameStateManager::Physics()->RemoveNode(physicsNode);
	}
}