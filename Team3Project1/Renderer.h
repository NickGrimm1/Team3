/**
<summary>Renderer pipline class,
contains all the methods and variables needed to output
grapical elements to screen.

Author: Sherif Salem
Version: 0.0.1 03/02/2015.</summary>
*/
#if WINDOWS_BUILD
#pragma once

#include "../Framework/Frustum.h"
#include "../Framework/SceneNode.h"
#include "../Framework/Light.h"
#include "Texture.h"
#include "../Framework/OGLRenderer.h"
#include "../Framework/Weather.h"
#include "../Framework/Camera.h"
#include "Mesh.h"
#include "DrawableEntity2D.h"
#include "DrawableText2D.h"
#include "DrawableTexture2D.h"
#include "MutexClass.h"

#include <vector>

#define SAMPLENUM 3
#define SHADOWSIZE 2048 //* 8 ?

/*
struct LightData {
	T3Vector4*	lightColour;
	T3Vector4*	lightSpecColour;



	T3Vector3*	lightPos;
	T3Vector3*	lightDir;
	T3Vector3*	cameraPos;

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
	void			ToggleDebug(int arg, bool onOff);

	GLuint			CreateTexture(const char* filename, bool enableMipMaps = false, bool enableAnisotropicFiltering = false);
	GLuint			CreateCubeTexture(const char* filename);
	GLuint			CreateShadowTexture();
	GLuint			CreateShadowCube();
	bool			DestroyTexture(GLuint textureReference);

	void			SetSkyBoxTexture(GLuint tex) {skyBoxTex = tex;}

	T3Vector4			GetAmbientColour() const {return ambientLightColour;}
	void			SetAmbientColour(T3Vector4& colour) {ambientLightColour = colour;}

	bool			GetRenderContextForThread();
	bool			DropRenderContextForThread();

	bool LoadShaders();
	bool LoadAssets();
	void UnloadShaders();
	void UnloadAssets();

	void DrawDeferredLights(bool on) {drawDeferredLights = on;}

protected:

	T3Matrix4			cameraMatrix; // Get camera matrix once at start of scene


	//Rendering pipeline components.
	void			DrawScene();
	void			ShadowPass();
	void			DrawNodes(bool enableTextures);

	void			DeferredLightPass();
	void			DrawDeferredPointLight(Light* l);
	void			DrawDeferredSpotLight(Light* l);
	void			DrawDeferredDirectionalLight(Light* l);

	void			CombineBuffers();
	void			DrawSkybox();
	void			BloomPass();
	void			MotionBlurPass();
	void			DrawFrameBufferTex(GLuint fboTex); // Draw the texture passed to it to screen
	
	void			Draw2DOverlay();
	void			Draw2DText(DrawableText2D& text);
	void			Draw2DTexture(DrawableTexture2D& texture);

	void			GenerateScreenTexture(GLuint &into, bool depth = false);
	bool			LoadCheck();
	bool			ActiveTex();

	void			CreateStaticMap(const int resolution); // Generate a static noise map that can be turned into Perlin noise.

	//Member variables.
	bool			activeTex;
	unsigned int	nextTextureUnit;

	// Debugging
	bool			drawDeferredLights;
	bool			debugElem[10];

	vector<Light*>&	lights;
	vector<SceneNode*>& sceneNodes; // game elements to draw (opaque), sorted by distance from camera, closest first.
	vector<DrawableEntity2D*>& overlayElements; // HUD/Menu elements, sorted by "distance" from camera (overlay level). Closest first

	T3Matrix4 orthographicMatrix;	// Gonna be constantly switching between orthographic (for HUD) and perspective (for scene) projection
	T3Matrix4 perspectiveMatrix;	// for drawing full screen quads (post-processing)
	T3Matrix4 hudMatrix; // For drawing HUD Elements only

	Mesh*			screenMesh;			// A quad mesh for drawing screen filling textures
	Mesh*			sphereMesh;			// A sphere mesh for drawing deferred point lights
	Mesh*			coneMesh;			// A cone mesh for drawing deferred spot lights
	Mesh*			circleMesh;			// A circle mesh for drawing deferred spot lights
	Mesh*			skyDome;			// The top of a sphere - used as a 'hat' to wear a skybox :)

	SceneNode*		root;
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
	Shader*			cloudShader; // Generates clouds
	Shader*			skyBoxShader; // Draws the cloud texture on the skydome.
	Shader*			combineShader;
	Shader*			particleShader;
	Shader*			brightPassShader;
	Shader*			bloomCombShader;
	Shader*			gaussianShader;
	Shader*			downSampleShader;
	Shader*			bloomFinalShader;
	Shader*			velocityShader;
	Shader*			motionBlurShader;
	Shader*			hudShader;
	
	GLuint			gbufferFBO; //Geometry buffer
	GLuint			postProcessingFBO;
	GLuint			deferredLightingFBO;
	GLuint			shadowFBO;
	GLuint			skyColourBuffer; //The buffer for holding the clous texture.
	GLuint			skyBufferFBO;
	GLuint			cloudMap;	// The texture for holding the static map the clouds are generated from.

	GLuint			gbufferColourTex;
	GLuint			gbufferDepthTex;
	GLuint			gbufferNormalTex;
	GLuint			shadowDepthTex; // unfortunately required for omni-directional shadows
	GLuint			gbufferVelocity;

	GLuint			skyBoxTex;
	GLuint			lightEmissiveTex;
	GLuint			lightSpecularTex;
	GLuint			postProcessingTex[3]; // At start of post-processing, postProcessingTex[0] holds the rendered scene
	GLuint			downSampleTex[SAMPLENUM * 2];

	T3Vector4			ambientLightColour; // The scenes ambient light settings

	MutexClass		openglMutex;		// Prevents different threads for using OpenGL at same time	

	float			samples[3];
};
#endif
