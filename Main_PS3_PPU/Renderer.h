/******************************************************************************
Class:Renderer
Implements:
Author:Rich Davison
Description: For consistency with the Graphics for Games module, we're deriving
a 'renderer' from the actual API specific parent class. This is the one you 
should be fiddling with! Add whatever you like, here.

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////
/*
*Extended by: Matt J
*Implemented functionality missing from Richards renderer as
*his renderer only had RenderScene, Renderer & ~Renderer
*/
#if PS3_BUILD
#pragma once
#include "../Team3Project1/GCMRenderer.h"
#include "../Team3Project1/Mesh.h"
#include "../Framework/SceneNode.h"
#include "../Team3Project1/DrawableEntity2D.h"
#include "../Team3Project1/DrawableEntity3D.h"
#include "../Team3Project1/DrawableText2D.h"
#include "../Team3Project1/DrawableTexture2D.h"
#include "../Framework/Light.h"
#include "../Team3Project1/TextMesh.h"
#include "../Team3Project1/MutexClass.h"

#include <map>
#include <vector>

using namespace std;

#define SAMPLENUM 3
#define SHADOWSIZE 2048

class Renderer : public GCMRenderer	
{
public:
	Renderer::Renderer(vector<Light*>& lightsVec, vector<SceneNode*>& SceneNodesVec, vector<DrawableTexture2D*>& overlayTextureVec, vector<DrawableText2D*>& overlayTextVec);
	~Renderer(void);

	bool GetRenderContextForThread()
	{
		renderMutex.lock_mutex();
		return true;
	}
	bool DropRenderContextForThread()
	{
		renderMutex.unlock_mutex();
		return true;
	}

	void SetCamera(Camera* cam) {camera = cam;};

	void RenderScene();
	void ToggleDebug(int arg, bool onOff);
	void SetLoadingIcon(DrawableTexture2D* icon) {loadingIcon = icon;}

	T3Vector4 GetAmbientColour() const {return ambientLightColour;}
	void SetAmbientColour(T3Vector4& colour) {ambientLightColour = colour;}

	void SetDayNight(float arg) { dayNight = arg; }

	//bool GetRenderContextForThread();
	//bool DropRenderContextForThread();

	bool LoadShaders();
	bool LoadAssets();
	void UnloadShaders();
	void UnloadAssets();

	unsigned int GetTextMeshMemory();

	void DrawDeferredLights(bool on) {drawDeferredLights = on;}

	unsigned char* GeneratePerlinNoise(const int resolution, unsigned char minValue, unsigned char maxValue);

	/*GLuint			CreateTexture(const char* filename, bool enableMipMaps = false, bool enableAnisotropicFiltering = false, unsigned int flags = 0);
	GLuint			CreateCubeTexture(const char* filename);
	GLuint			CreateShadowTexture();
	GLuint			CreateShadowCube();
	bool			DestroyTexture(GLuint textureReference);*/

	//void			SetSkyBoxTexture(GLuint tex) {skyBoxTex = tex;}
protected:
	Matrix4 cameraMatrix; // Get camera matrix once at start of scene

	//Rendering pipeline components.
	void DrawScene();
	void ShadowPass();
	void DrawNodes(bool enableTextures);

	void DeferredLightPass();
	void DrawDeferredPointLight(Light* l);
	void DrawDeferredSpotLight(Light* l);
	void DrawDeferredDirectionalLight(Light* l);

	void CombineBuffers();
	void DrawSkybox();
	void BloomPass();
	void MotionBlurPass();
	//void DrawFrameBufferTex(GLuint fboTex); // Draw the texture passed to it to screen
	
	void Draw2DOverlay();
	void Draw2DText(DrawableText2D& text);
	void Draw2DTexture(DrawableTexture2D& texture);

	//void GenerateScreenTexture(GLuint &into, bool depth = false);
	bool LoadCheck();
	bool ActiveTex();

	//void CreateStaticMap(GLuint* target, const int resolution, unsigned char minValue, unsigned char maxValue); // Generate a static noise map that can be turned into Perlin noise.

	//Member variables.
	bool activeTex;
	unsigned int nextTextureUnit;

	// Debugging
	bool drawDeferredLights;
	bool debugElem[10];

	vector<Light*>&	lights;
	vector<SceneNode*>& sceneNodes; // game elements to draw (opaque), sorted by distance from camera, closest first.
	vector<DrawableTexture2D*>& overlayTextures; // HUD/Menu elements, sorted by "distance" from camera (overlay level). Closest first
	vector<DrawableText2D*>& overlayTexts; // HUD/Menu elements, sorted by "distance" from camera (overlay level). Closest first

	Matrix4 orthographicMatrix;	// Gonna be constantly switching between orthographic (for HUD) and perspective (for scene) projection
	Matrix4 perspectiveMatrix;	// for drawing full screen quads (post-processing)
	Matrix4 hudMatrix; // For drawing HUD Elements only

	Mesh* screenMesh;			// A quad mesh for drawing screen filling textures
	Mesh* sphereMesh;			// A sphere mesh for drawing deferred point lights
	Mesh* coneMesh;			// A cone mesh for drawing deferred spot lights
	Mesh* circleMesh;			// A circle mesh for drawing deferred spot lights
	Mesh* skyDome;			// The top of a sphere - used as a 'hat' to wear a skybox :)
	Mesh* quadMesh;			// A quad mesh (0-1) for drawing 2D textures.

	SceneNode* root;
	Camera*	camera;

	//Frustum	frameFrustum;

	/*
	Weather*		snow; 
	Weather*		rain; 
	Weather*		sandstorm;
	*/

	Shader* basicShader;
	Shader*	shadowShader;
	Shader*	sceneShader;
	Shader*	lightingShader;
	Shader*	cloudShader; // Generates clouds
	Shader*	skyBoxShader; // Draws the cloud texture on the skydome.
	Shader*	combineShader;
	Shader*	particleShader;
	Shader*	brightPassShader;
	Shader*	bloomCombShader;
	Shader*	gaussianShader;
	Shader*	downSampleShader;
	Shader*	bloomFinalShader;
	Shader*	velocityShader;
	Shader*	motionBlurShader;
	Shader* hudShader;
	
	//GLuint			gbufferFBO; //Geometry buffer
	//GLuint			postProcessingFBO;
	//GLuint			deferredLightingFBO;
	//GLuint			shadowFBO;
	//GLuint			skyColourBuffer[2]; //The buffer for holding the cloud texture.
	//GLuint			skyBufferFBO;
	//
	//GLuint			daySkyTex;
	//GLuint			nightSkyTex;
	//GLuint			cloudMap;	// The texture for holding the static map the clouds are generated from.
	//GLuint			gbufferColourTex;
	//GLuint			gbufferDepthTex;
	//GLuint			gbufferNormalTex;
	//GLuint			shadowDepthTex; // unfortunately required for omni-directional shadows
	//GLuint			gbufferVelocity;

	//GLuint			skyBoxTex;
	//GLuint			lightEmissiveTex;
	//GLuint			lightSpecularTex;
	//GLuint			postProcessingTex[3]; // At start of post-processing, postProcessingTex[0] holds the rendered scene
	//GLuint			downSampleTex[SAMPLENUM * 2];

	T3Vector4 ambientLightColour; // The scenes ambient light settings

	MutexClass renderMutex;		// Prevents different threads for using OpenGL at same time	

	float samples[3];
	int currentFPS;
	float dayNight;

	std::map<string, TextMesh*> loadedTextMeshes;
	DrawableTexture2D* loadingIcon;
};
#endif
