 /**
Version History:
0.0.1 03/02/2015
0.0.2 05/02/2015
	Added 2D Methods.
0.0.3 06/02/2015
	Changed 2D text method to take DrawableText.
	Added Depth indexing for 2D.
0.0.4 12/02/2015
	This became a thread class :)
*/

/**
<summary>Static Singleton.
The game's graphics engine.

Author: Nick Grimm
Version: 0.0.3 06/02/2015.</summary>
*/

#pragma once
#include "../Framework/T3Vector3.h"
#include "../Framework/T3Vector4.h"
#include "../Framework/Light.h"
#include "Texture.h"
#include "Mesh.h"
#if WINDOWS_BUILD
#include "Renderer.h"
#endif
#if PS3_BUILD
#include "../Main_PS3_PPU/Renderer.h"
#endif
#include "DrawableTexture2D.h"
#include "DrawableText2D.h"
#include "DrawableEntity3D.h"
#include "Thread.h"
#include "MutexClass.h"
#include <vector>
#include <algorithm>

class Renderer;

class GraphicsEngine : public Thread
{
public:
	bool GetRenderContext() 
	{
#if WINDOWS_BUILD
		return renderer->GetRenderContextForThread();
#endif
#if PS3_BUILD
		return true;
#endif
	}
	bool DropRenderContext() 
	{
#if WINDOWS_BUILD
		return renderer->DropRenderContextForThread();
#endif
#if PS3_BUILD
		return true;
#endif
	}
	int GetFrameRate() { return frameRate; }
#pragma region Entry/Exit
	/**
	<summary>Initializes a graphics engine.</summary>
	<param name='graphics'>The pointer to the resulting graphics engine instance.</summary>
	<returns>true if a Graphics Engine is initialized and ready.</returns>
	*/
	static bool Initialize(GraphicsEngine*& out);
	
	static bool LoadContent();
	
	static void UnloadContent();
	
	/**
	<summary>Destroys the graphics engine. Allows the game to exit cleanly.</summary>
	<returns>true if the graphics engine has exited.</returns>
	*/
	static bool Destroy();
	/**
	<summary>Gets whether the graphics engine has initialized and is ready to render.</summary>
	*/
	bool HasInitialised() { return isInitialised; }
#if WINDOWS_BUILD
	void Run();
#endif
#if PS3_BUILD
	static void Run(uint64_t arg);
#endif
#pragma endregion
#pragma region TwoD
	/**
	<summary>Adds a drawable texture to the scene. Idempotent.</summary>
    <param name="drawableTexture">The drawable texture.</param>
	*/
	void AddTextureToScene(DrawableTexture2D* drawableTexture);
	/**
	<summary>Removes a drawable texture from the scene. Indempotent.</summary>
    <param name="drawableTexture">The drawable texture.</param>
	*/
	void RemoveTextureFromScene(DrawableTexture2D* drawableTexture);
	/**
	<summary>Adds a drawable text string to the scene in the specified font at the specified position, in the specified color. Indempotent.</summary>
    <param name="drawableTexture">The drawable text.</param>
	*/
    void AddDrawableTextToScene(DrawableText2D* drawableText);
	/**
	<summary>Removes a drawable text string from the scene in the specified font at the specified position, in the specified color. Indempotent.</summary>
    <param name="drawableTexture">The drawable text.</param>
	*/
	void RemoveDrawableTextFromScene(DrawableText2D* drawableText);
#pragma endregion
#pragma region ThreeD
	/**
	<summary>Adds a 3D drawable entity to the scene. Indempotent.</summary>
    <param name="drawable">The drawable object.</param>
	<param name="parent">A drawable object that this object uses as its parent in the scene. NULL attaches the entity to the scene root. Default is NULL.</param>
	*/
	void AddDrawable(DrawableEntity3D* drawable, DrawableEntity3D* parent = NULL);
	/**
	<summary>Removes a 3D drawable object from the scene. Indempotent.</summary>
    <param name="drawable">The drawable object.</param>
	<param name="removeChildren">if true will also remove all children of this object from the scene, false will reattach children to the parent node. Default is true.</param>
	*/
	void RemoveDrawable(DrawableEntity3D* drawable, bool removeChildren = true);
	/**
	<summary>Removes a light to the scene. Indempotent.</summary>
    <param name="light">The light.</param>
	*/
	void RemoveLight(Light* light);
	/**
	<summary>Sets the camera for the scene.</summary>
    <param name="camera">The camera.</param>
	*/
	void SetCamera(Camera* cam);
#pragma endregion	

	PointLight* AddPointLight(T3Vector3 lightPosition, float lightRadius, T3Vector4 diffuseColour, T3Vector4 specularColour, bool castsShadow);
	DirectionalLight* AddDirectionalLight(T3Vector3 lightDirection, T3Vector4 diffuseColour, T3Vector4 specularColour);
	SpotLight* AddSpotLight(T3Vector3 lightPosition, T3Vector3 lightTarget, T3Vector3 upVector, float lightRadius, float lightAngle, T3Vector4 diffuseColour, T3Vector4 specularColour, bool castsShadow);
		
	// We don't actually need this since the GSM already holds the reference :)
	//static GraphicsEngine& GetGraphicsEngine() {return *engine;}

	void EnableLoadingIcon(bool value) {isLoading = value;}
#if WINDOWS_BUILD
	void EnableMousePointer(bool value) { Window::GetWindow().ShowOSPointer(value); }
#endif
	// Debugging
	void DrawDeferredLights(bool on);

	unsigned char* GeneratePerlinNoise(const int resolution, unsigned char minValue, unsigned char maxValue);

private:
	static GraphicsEngine* engine;

	GraphicsEngine();
	~GraphicsEngine();

	bool isInitialised;

	float DayNightCycle();

	void BuildNodeLists(SceneNode* from);
	void SortNodeLists();
	void ClearNodeLists();
	void DrawNodes();

	/**
	<summary>Adds a light to the scene. Indempotent.</summary>
    <param name="light">The light.</param>
	*/
	void AddLight(Light* light);

	Renderer* renderer;
#if WINDOWS_BUILD
	Frustum frameFrustum;
#endif
	Camera* camera;

	// Scene Elements
	SceneNode* sceneRoot; // Scene heirarchy
	vector<Light*> lights; // Scene lighting
	vector<DrawableEntity2D*> overlayElementsList; // HUD/Menu overlay items
	vector<SceneNode*> transparentGameEntityList; // list of transparent game elements sorted by distance from camera
	vector<SceneNode*> gameEntityList; // list of opaque game elements sorted by distance from camera

	MutexClass contentGuard;
	vector<Light*> addLightsList; // lights to be added during next update
	vector<Light*> removeLightsList; // lights to be removed during next update
	vector<DrawableEntity2D*> addHudList; // HUD elements to be added at next update
	vector<DrawableEntity2D*> removeHudList; // HUD elements to be removed at next update
	vector<pair<DrawableEntity3D*, DrawableEntity3D*> > addGameList; // Game elements to be added at next update
	vector<pair<DrawableEntity3D*, bool> > removeGameList; // Game elements to be removed at next update

	T3Vector3 boundingMin, boundingMax; // Defines a bounding box for the VISIBLE scene, built each frame from the nodes that pass frustum culling.

	int width;
	int height;

	bool isLoading;
	bool isLoadingDrawing;
	DrawableTexture2D* loadingIcon;
	Texture* loadingTexture;
	Texture* skyboxDay;
	Texture* skyboxNight;

	const float RENDER_TIME;
	float lastFrameTimeStamp;
	int frameRate;
	int time;
	bool inc;
};