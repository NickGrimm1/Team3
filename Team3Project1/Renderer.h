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
#include "../Framework/OGLRenderer.h"
#include "../Framework/Weather.h"
#include "../Framework/Camera.h"
#include "../Framework/ParticleEmitter.h"
#include "Texture.h"
#include "Mesh.h"
#include "DrawableEntity2D.h"
#include "DrawableText2D.h"
#include "DrawableTexture2D.h"
#include "MutexClass.h"
#include "TextMesh.h"

#include <vector>
#include <map>

#define SAMPLENUM 3
#define SHADOWSIZE 2048

class Renderer : public OGLRenderer
{
public:
	static int count;
	Renderer(Window &parent, vector<Light*>& lightsVec, vector<SceneNode*>& sceneNodesVec, vector<DrawableEntity2D*>& overlayVec);
	~Renderer(void);

	void			SetCamera(Camera* cam) {camera = cam;};

	void			SetLoadingIcon(DrawableTexture2D* icon) {loadingIcon = icon;}

	void			RenderScene();
	void			ToggleDebug(int arg, bool onOff);

	GLuint			CreateTexture(const char* filename, bool enableMipMaps = false, bool enableAnisotropicFiltering = false, unsigned int flags = 0);
	GLuint			CreateCubeTexture(const char* filename);
	GLuint			CreateShadowTexture();
	GLuint			CreateShadowCube();
	bool			DestroyTexture(GLuint textureReference);

	void			SetSkyBoxTexture(GLuint tex) {skyBoxTex = tex;}

	T3Vector4		GetAmbientColour() const {return ambientLightColour;}
	void			SetAmbientColour(T3Vector4& colour) {ambientLightColour = colour;}

	void			SetDayNight(float arg) { dayNight = arg; }

	bool			GetRenderContextForThread();
	bool			DropRenderContextForThread();

	bool			LoadAssets();
	void			UnloadShaders();
	void			UnloadAssets();

	void			DrawDeferredLights(bool on) {drawDeferredLights = on;}

	unsigned char*	GeneratePerlinNoise(const int resolution, unsigned char minValue, unsigned char maxValue);

	unsigned int	GetTextMeshMemory() {return textMeshMemory / 1024;} //KB
protected:

	T3Matrix4		cameraMatrix; // Get camera matrix once at start of scene

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
	void			EdgeDetectPass();
	void			MotionBlurPass();
	void			DrawFrameBufferTex(GLuint fboTex); // Draw the texture passed to it to screen
	
	void			Draw2DOverlay();
	void			Draw2DText(DrawableText2D& text);
	void			Draw2DTexture(DrawableTexture2D& texture);

	void			GenerateScreenTexture(GLuint &into, bool depth = false);
	bool			LoadCheck();
	bool			ActiveTex();

	void			CreateStaticMap(GLuint* target, const int resolution, unsigned char minValue, unsigned char maxValue); // Generate a static noise map that can be turned into Perlin noise.

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
	Mesh*			quadMesh;			// A quad mesh (0-1) for drawing 2D textures.

	SceneNode*		root;
	Camera*			camera;

	Frustum			frameFrustum;

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
	Shader*			edgeDetectShader;
	Shader*			downSampleShader;
	Shader*			bloomFinalShader;
	Shader*			velocityShader;
	Shader*			motionBlurShader;
	Shader*			hudShader;
	
	GLuint			gbufferFBO; //Geometry buffer
	GLuint			postProcessingFBO;
	GLuint			deferredLightingFBO;
	GLuint			shadowFBO;
	GLuint			skyColourBuffer[2]; //The buffer for holding the cloud texture.
	GLuint			skyBufferFBO;
	
	GLuint			daySkyTex;
	GLuint			nightSkyTex;
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

	T3Vector4		ambientLightColour; // The scenes ambient light settings

	MutexClass		openglMutex;		// Prevents different threads for using OpenGL at same time	

	float			samples[3];
	int				currentFPS;
	float			dayNight;

	map<string, TextMesh*> loadedTextMeshes;
	unsigned int textMeshMemory;

	DrawableTexture2D* loadingIcon;
};
#endif
