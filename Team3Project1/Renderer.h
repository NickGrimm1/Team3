#pragma once
#include "../Framework/Light.h"
#include "../Framework/Texture.h"
#include "../Framework/OGLRenderer.h"

class Renderer : public OGLRenderer
{
public:
	Renderer(void);
	~Renderer(void);

	void		Render(float msec);

	unsigned int AddLight(Light &arg);
	Light&		GetLight(int arg);
	void		RemoveLight(int arg);

	unsigned int AddTexture(Texture &arg);
	Texture&	GetTexture(int arg);
	void		RemoveTexture(int arg);

	unsigned int AddMesh(Mesh &arg);
	Mesh&		GetMesh(int arg);
	void		RemoveMesh(int arg);

private:
	//Start of render pipeline
	void		RenderScene(float msec);
	
	//Rendering pipeline components.
	void		ShadowPass();
	void		DeferredLightPass();
	void		MotionBlurPass();
	void		BloomPass();

	//Member variables.

	Texture*	texArray[10];
	//Light		lightArray[10];
	Mesh*		meshArray[10];

	Shader*		basicShader, shadowShader, bloomShader, deferredShader, blurShader, combineShader;



};