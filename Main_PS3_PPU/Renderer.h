/******************************************************************************
Class:Renderer
Implements:
Author:Rich Davison
Description: For consistency with the Graphics for Games module, we're deriving
a 'renderer' from the actual API specific parent class. This is the one you 
should be fiddling with! Add whatever you like, here.

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////
/*
*Extended by: Matt J
*Implemented functionality missing from Richards renderer as
*his renderer only had RenderScene, Renderer & ~Renderer
*/
#pragma once
#include "../Team3Project1/GCMRenderer.h"
#include "../Team3Project1/Mesh.h"
#include "../Framework/SceneNode.h"
#include "../Team3Project1/DrawableEntity2D.h"
#include "../Team3Project1/DrawableEntity3D.h"
#include "../Team3Project1/DrawableText2D.h"
#include "../Team3Project1/DrawableTexture2D.h"
#include "../Framework/Light.h"

#include <vector>


class Renderer : public GCMRenderer	{
public:
	Renderer(vector<Light*>& lightsVec, vector<SceneNode*>& SceneNodesVec, vector<DrawableEntity2D*>& overlayVec);
	~Renderer(void);

	virtual void RenderScene();

	bool LoadShaders();
	void UnloadShaders();
	bool LoadAssets();
	void UnloadAssets();

	unsigned int CreateShadowCube();
	unsigned int CreateShadowTexture();

	void DrawDeferredLights(bool on) {drawDeferredLights = on;}

protected:
	bool	drawDeferredLights;

	Mesh* circleMesh;
	Mesh* quadMesh;
	Mesh* sphereMesh;
	Mesh* coneMesh;

	Shader* basicShader;

	vector<Light*>& lights;
	vector<SceneNode*>& sceneNodes;
	vector<DrawableEntity2D*>& overlayElements;
};

