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

#include <vector>

#define SHADOWSIZE 2048 //* 8 ?

/*
struct LightData {
	Vector4*	lightColour;
	Vector4*	lightSpecColour;

	Vector3*	lightPos;
	Vector3*	lightDir;
	Vector3*	cameraPos;

	float		lightRadius;
	float		lightAngle;
	int			lightType;
};
*/

class Renderer : public OGLRenderer
{
public:
	Renderer(Window &parent, vector<Light*> lightsVec, vector<SceneNode*> sceneNodesVec);
	~Renderer(void);

	//void			Render(SceneNode* sn, vector<Light*> arg_lights);
	void			RenderScene();
	virtual void	UpdateScene(float msec);
	void			ToggleDebug(int arg, bool onOff);

	GLuint			CreateTexture(const char* filename, bool enableMipMaps = false, bool enableAnisotropicFiltering = false);
	GLuint			CreateShadowTexture();
	bool			DestroyTexture(GLuint textureReference);

protected:
	//Rendering pipeline components.
	void			DrawScene();
	void			ShadowPass();
	void			DeferredLightPass();
	void			CombineBuffers();
	void			BloomPass();
	void			MotionBlurPass();

	void			GenerateScreenTexture(GLuint &into, bool depth = false);
	bool			LoadCheck();
	bool			ActiveTex();

	//Member variables.
	bool			activeTex;
	unsigned int	nextTextureUnit;

	bool			debugElem[10];

	vector<Light*>&	lights;
	vector<SceneNode*>& sceneNodes;

	Matrix4 orthographicMatrix;	// Gonna be constantly switching between orthographic (for HUD) and perspective (for scene) projection
	Matrix4 perspectiveMatrix;	// Rather than constantly regenerating matrices - just keep a copy of each

	Mesh*			quad;

	Camera*			camera;

	SceneNode*		root;

	Frustum			frameFrustum;

	/*
	Weather*		snow; 
	Weather*		rain; 
	Weather*		sandstorm;
	*/

	Shader*			basicShader;
	Shader*			shadowShader;
	Shader*			sceneShader;
	Shader*			lightingShader;
	Shader*			skyBoxShader;
	Shader*			combineShader;
	Shader*			particleShader;
	Shader*			bloomShader;
	Shader*			deferredShader;
	Shader*			blurShader;
	
	GLuint			gbufferFBO;
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