#include "SceneNode.h"

SceneNode::SceneNode(void)	{
	transform		= Matrix4::identity();
	worldTransform	= Matrix4::identity();
	parent			= NULL;
	mesh			= NULL;
}

SceneNode::~SceneNode(void)	{
}

void	SceneNode::Update(float msec) {
	if(parent) {
		worldTransform = parent->worldTransform * transform;
	}
	else{
		worldTransform = transform;
	}

	for(std::vector<SceneNode*>::iterator i = children.begin(); i != children.end(); ++i) {
		(*i)->Update(msec);
	}
}

void	SceneNode::AddChild(SceneNode& s) {
	children.push_back(&s);
	s.parent = this;
}