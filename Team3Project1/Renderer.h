/**
<summary>Renderer pipline class,
contains all the methods and variables needed to output
grapical elements to screen.

Author: Sherif Salem
Version: 0.0.1 03/02/2015.</summary>
*/

#pragma once

#include "../Framework/Frustum.h"
#include "../Framework/SceneNode.h"
#include "../Framework/Light.h"
#include "../Framework/Texture.h"
#include "../Framework/OGLRenderer.h"
#include "../Framework/Weather.h"
#include "../Framework/Camera.h"

class Renderer : public OGLRenderer
{
public:
	Renderer(Window &parent);
	~Renderer(void);

	void			Render(float msec);

	void			ToggleDebug(int arg, bool onOff);

protected:
	//Rendering pipeline components.
	virtual void	RenderScene();
	virtual void	UpdateScene(float msec);
	void			ShadowPass();
	void			DeferredLightPass();
	void			BloomPass();
	void			MotionBlurPass();

	void			GenerateScreenTexture(GLuint &into, bool depth = false);

	//Member variables.
	bool			debugElem[10];

	Camera*			camera;

	SceneNode*		root;

	Frustum			frameFrustum;

	/*Weather*		snow, 
					rain, 
					sandstorm;
					*/
	/*Shader*			basicShader, 
					shadowShader, 
					bloomShader, 
					deferredShader, 
					blurShader, 
					combineShader,
					particleShader;
					*/

	GLuint			bufferFBO, 
					processFBO, 
					pointLightFBO, 
					shadowFBO, 
					bufferColourTex[2],
					bufferDepthTex, 
					bufferNormalTex, 
					cubeMap, 
					shadowTex, 
					lightEmissiveTex, 
					lightSpecularTex;
};