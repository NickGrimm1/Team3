#include "Renderer.h"


Renderer::Renderer(Window &parent) : OGLRenderer(parent)
{
	camera	= NULL;
	root	= new SceneNode();
	//TODO: Add in all the initialisation stuff.
}

Renderer::~Renderer(void)
{
}

unsigned int AddLight(Light &arg)
{

}

Light& GetLight(int arg)
{

}

void RemoveLight(int arg)
{

}

unsigned int AddTexture(Texture &arg)
{

}

Texture& GetTexture(int arg)
{

}

void RemoveTexture(int arg)
{

}

unsigned int AddMesh(Mesh &arg)
{

}

Mesh& GetMesh(int arg)
{

}

void RemoveMesh(int arg)
{

}

void ToggleDebug(int arg, bool onOff)
{

}


/*----------Rendering pipeline-----------*/
void RenderScene()
{

}

void UpdateScene(float msec)
{
	if (camera) 
	{
		camera->UpateCamera(msec);
	}
	root->Update(msec);
}

void ShadowPass()
{

}

void DefferedLightPass()
{

}

void MotionBlurPass()
{

}

void BloomPass()
{

}


/*-----------SceneNode Methods-----------*/
void BuildNodeLists(SceneNode* from)
{

}

void SortNodeLists()
{

}

void ClearNodeLists()
{

}

void DrawNodes()
{

}

void DrawNode(SceneNode* n)
{

}
