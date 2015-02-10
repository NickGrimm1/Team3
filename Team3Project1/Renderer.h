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

#include <algorithm>

#define SHADOWSIZE 2048 * 8

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
	Renderer(Window &parent);
	~Renderer(void);
	void			Render(SceneNode* sn, Light arg_lights[]);
	void			RenderScene();
	virtual void	UpdateScene(float msec);
	void			ToggleDebug(int arg, bool onOff);

	void	SetCamera(Camera*c);

	void	AddNode(SceneNode* n);

	void	RemoveNode(SceneNode* n);

	//Statics
	static bool Initialise() {
		instance = new Renderer(Window::GetWindow());
		return instance->HasInitialised();
	}

	static void Destroy() {
		delete instance;
	}
	
	static Renderer&GetRenderer() { return *instance;}

protected:
	//Rendering pipeline components.
	void			DrawScene();
	void			ShadowPass();
	void			DeferredLightPass();
	void			BloomPass();
	void			MotionBlurPass();

	void			GenerateScreenTexture(GLuint &into, bool depth = false);
	bool			LoadCheck();
	bool			ActiveTex();

	//Member variables.
	bool			activeTex;

	bool			debugElem[10];

	vector<Light>	lights;
	vector<SceneNode*> sceneNodes;

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

	//Renderer(Window &parent);
	//virtual ~Renderer(void);

	void	BuildNodeLists(SceneNode* from);
	void	SortNodeLists();
	void	ClearNodeLists();
	void	DrawNodes();
	void	DrawNode(SceneNode*n);



	Shader*		simpleShader;



	vector<SceneNode*> transparentNodeList;
	vector<SceneNode*> nodeList;

	static Renderer*	instance;
};