#include "SceneNode.h"
#if PS3_BUILD
#include "../Team3Project1/GCMRenderer.h"
#endif
#include "../Team3Project1/DrawableEntity3D.h"
#include "../Team3Project1/Mesh.h"

SceneNode::SceneNode(Mesh*mesh, T3Vector4 colour)	{
	awake				= true;
	this->mesh			= mesh;
	this->colour		= colour;
	parent				= NULL;
	sceneElement		= NULL;
	boundingRadius		= 100.0f;
	distanceFromCamera	= 0.0f;
	
	modelScale			= T3Vector3(1,1,1);
}

SceneNode::SceneNode(DrawableEntity3D* entity)	{
	sceneElement		= entity;
	awake				= true;
	parent				= NULL;
	distanceFromCamera	= 0.0f;
	modelScale			= T3Vector3(1,1,1);
}

SceneNode::~SceneNode(void)	{
	for(unsigned int i = 0; i < children.size(); ++i) {
		delete children[i];
	}
	children.clear();
}

float SceneNode::GetBoundingRadius()
{
	return sceneElement->GetBoundingRadius();
}

void SceneNode::AddChild( SceneNode* s )	{
	children.push_back(s);
	s->parent = this;
}

void SceneNode::AddChildToParent(DrawableEntity3D* child, DrawableEntity3D* parent) {
	if (sceneElement == parent) { // this scene node holds the parent draw entity
		AddChild(new SceneNode(child));
	}
	else {
		for(vector<SceneNode*>::iterator i = children.begin(); i != children.end(); ++i) {
			(*i)->AddChildToParent(child, parent);
		}
	}
}

bool SceneNode::CompareByCameraDistance(SceneNode* a, SceneNode* b)  {
	return (a->distanceFromCamera < b->distanceFromCamera) ? true : false;
}

bool SceneNode::CompareByZ(SceneNode* a,SceneNode* b)  {
	return (a->GetWorldTransform().GetPositionVector().z < b->GetWorldTransform().GetPositionVector().z) ? true : false;
}

void SceneNode::Update(float msec)	 {
	if (sceneElement) {
		transform = T3Matrix4::Translation(sceneElement->GetOriginPosition()) * sceneElement->GetRotation().ToMatrix();
	}
	else
	{
		transform.ToIdentity();
	}

	if(parent) {
		worldTransform = parent->worldTransform * transform;
	}
	else{
		worldTransform = transform;
	}

	for(vector<SceneNode*>::iterator i = children.begin(); i != children.end(); ++i) {
		(*i)->Update(msec);
	}
}

bool SceneNode::RemoveChild(DrawableEntity3D* toDelete, bool recursive, bool removeChildren) {

	for(vector<SceneNode*>::iterator i = children.begin(); i != children.end(); ++i) {
		//if((*i) == s) {
		if ((*i)->GetDrawableEntity() == toDelete) {
			SceneNode* childToDelete = *i;
			i = children.erase(i);
			
			if (!removeChildren) {
				for(vector<SceneNode*>::iterator j = childToDelete->children.begin(); j != childToDelete->children.end(); ++j) {
					// Add deleted nodes children to this node
					children.push_back(*j);
				}
				childToDelete->children.clear(); // Empty deleted nodes child array so it does not attempt to delete its old children when it itself is deleted
			}

			delete childToDelete;

			return true;
		}
	}

	if(recursive) {
		for(vector<SceneNode*>::iterator i = children.begin(); i != children.end(); ++i) {
			if((*i)->RemoveChild(toDelete,recursive,removeChildren)) {
				return true;
			}
		}
	}
	return false;
}
#if WINDOWS_BUILD
void SceneNode::Draw(const OGLRenderer & r) {
	if (mesh) { 
		mesh->Draw();
	}

}
#endif

#if PS3_BUILD
void SceneNode::Draw(const GCMRenderer& r)
{
	if(mesh)
	{
		mesh->Draw(r.GetCurrentShader());
	}
}
#endif
