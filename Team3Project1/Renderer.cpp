#include "Renderer.h"


Renderer::Renderer(Window &parent) : OGLRenderer(parent)
{
	camera	= new Camera();
	root	= new SceneNode();
	

}

Renderer::~Renderer(void)
{
	delete[] texArray;
	delete[] meshArray;
	//delete[] lightArray;
	delete camera;
	delete root;
	/*
	delete snow;
	delete rain;
	delete sandstorm;
	delete basicShader;
	delete shadowShader;
	delete bloomShader;
	delete deferredShader;
	delete blurShader;
	delete combineShader;
	delete particleShader;
	*/
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

void DeferredLightPass()
{

}

void BloomPass()
{

}

void MotionBlurPass()
{

}

void GenerateScreenTexture(GLuint &into, bool depth = false)
{

}
