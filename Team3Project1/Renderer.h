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
#include "../Framework/Mesh.h"
#include "DrawableEntity2D.h"
#include "MutexClass.h"

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
	Renderer(Window &parent, vector<Light*>& lightsVec, vector<SceneNode*>& sceneNodesVec, vector<DrawableEntity2D*>& overlayVec);
	~Renderer(void);

	void			SetCamera(Camera* cam) {camera = cam;};

	void			RenderScene();
	virtual void	UpdateScene(float msec);
	void			ToggleDebug(int arg, bool onOff);

	GLuint			CreateTexture(const char* filename, bool enableMipMaps = false, bool enableAnisotropicFiltering = false);
	GLuint			CreateShadowTexture();
	GLuint			CreateCubeTexture(const char* filename);
	bool			DestroyTexture(GLuint textureReference);

	void			SetSkyBoxTexture(GLuint tex) {skyBoxTex = tex;}

	Vector4			GetAmbientColour() const {return ambientLightColour;}
	void			SetAmbientColour(Vector4& colour) {ambientLightColour = colour;}

	bool			GetRenderContextForThread();
	bool			DropRenderContextForThread();

protected:
	//Rendering pipeline components.
	void			DrawScene();
	void			ShadowPass();
	void			DeferredLightPass();
	void			CombineBuffers();
	void			DrawSkybox();
	void			BloomPass();
	void			MotionBlurPass();
	void			DrawFrameBufferTex(GLuint fboTex);
	void			Draw2DOverlay();

	void			GenerateScreenTexture(GLuint &into, bool depth = false);
	bool			LoadCheck();
	bool			ActiveTex();

	//Member variables.
	bool			activeTex;
	unsigned int	nextTextureUnit;

	bool			debugElem[10];

	vector<Light*>&	lights;
	vector<SceneNode*>& sceneNodes; // game elements to draw (opaque), sorted by distance from camera, closest first.
	vector<DrawableEntity2D*>& overlayElements; // HUD/Menu elements, sorted by "distance" from camera (overlay level). Closest first

	Matrix4 orthographicMatrix;	// Gonna be constantly switching between orthographic (for HUD) and perspective (for scene) projection
	Matrix4 perspectiveMatrix;	// Rather than constantly regenerating matrices - just keep a copy of each

	Mesh*			quad;

	Camera*			camera;

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
	GLuint			postProcessingFBO;
	GLuint			deferredLightingFBO;
	GLuint			shadowFBO;
	GLuint			gbufferColourTex;
	GLuint			gbufferDepthTex;
	GLuint			gbufferNormalTex;
	GLuint			skyBoxTex;
	GLuint			lightEmissiveTex;
	GLuint			lightSpecularTex;
	GLuint			postProcessingTex[2]; // At start of post-processing, postProcessingTex[0] holds the rendered scene

	Vector4			ambientLightColour; // The scenes ambient light settings

	Mesh*			screenMesh;			// A quad mesh for drawing screen filling textures

	MutexClass		openglMutex;		// Prevents different threads for using OpenGL at same time	
};