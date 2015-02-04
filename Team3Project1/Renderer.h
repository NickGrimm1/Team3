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

	unsigned int	AddLight(Light &arg);
	Light&			GetLight(int arg);
	void			RemoveLight(int arg);

	unsigned int	AddTexture(Texture &arg);
	Texture&		GetTexture(int arg);
	void			RemoveTexture(int arg);

	unsigned int	AddMesh(Mesh &arg);
	Mesh&			GetMesh(int arg);
	void			RemoveMesh(int arg);

	void			ToggleDebug(int arg, bool onOff);

protected:
	//Rendering pipeline components.
	virtual void	RenderScene();
	virtual void	UpdateScene(float msec);
	void			ShadowPass();
	void			DeferredLightPass();
	void			MotionBlurPass();
	void			BloomPass();

	//SceneNode methods
	void			BuildNodeLists(SceneNode* from);
	void			SortNodeLists();
	void			ClearNodeLists();
	void			DrawNodes();
	void			DrawNode(SceneNode*n);

	//Member variables.
	bool			debugElem[10];

	Texture*		texArray[10];
	//Light*		lightArray[10];
	Mesh*			meshArray[10];

	Camera*			camera;

	SceneNode*		root;

	Frustum			frameFrustum;

	Weather*		snow, 
					rain, 
					sandstorm;

	Shader*			basicShader, 
					shadowShader, 
					bloomShader, 
					deferredShader, 
					blurShader, 
					combineShader,
					particleShader;

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

	vector<SceneNode*> transparentNodeList;
	vector<SceneNode*> nodeList;
};