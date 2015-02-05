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


#define SHADOWSIZE 2048 * 8

class Renderer : public OGLRenderer
{
public:
	Renderer(Window &parent);
	~Renderer(void);

	void			Render(float msec, SceneNode* root);

	void			ToggleDebug(int arg, bool onOff);

protected:
	//Rendering pipeline components.
	void			DrawScene();
	virtual void	UpdateScene(float msec);
	void			ShadowPass();
	void			DeferredLightPass();
	void			BloomPass();
	void			MotionBlurPass();

	void			GenerateScreenTexture(GLuint &into, bool depth = false);
	bool			LoadCheck();
	//Member variables.
	bool			debugElem[10];

	Camera*			camera;

	SceneNode*		root;

	Frustum			frameFrustum;

	/*Weather*		snow; 
	Weather*		rain; 
	Weather*		sandstorm;
					*/

	Shader*			basicShader; 
	Shader*			shadowShader;
	Shader*			skyBoxShader;
	Shader*			combineShader;
	Shader*			particleShader;
	Shader*			bloomShader;
	Shader*			deferredShader;
	Shader*			blurShader;
	
	GLuint			bufferFBO;
	GLuint			processFBO;
	GLuint			pointLightFBO;
	GLuint			shadowFBO;
	GLuint			bufferColourTex[2];
	GLuint			bufferDepthTex;
	GLuint			bufferNormalTex;
	GLuint			cubeMap;
	GLuint			shadowTex;
	GLuint			lightEmissiveTex;
	GLuint			lightSpecularTex;
};