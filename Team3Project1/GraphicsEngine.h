/**
Version History:
0.0.1 03/02/2015
0.0.2 05/02/2015
	Added 2D Methods.
*/

/**
<summary>Static Singleton.
The game's graphics engine.

Author: Nick Grimm
Version: 0.0.1 03/02/2015.</summary>
*/

#pragma once
#include "../Framework/Vector3.h"
#include "../Framework/Vector4.h"
#include "../Framework/Light.h"
#include "../Framework/Texture.h"
#include "../Framework/Mesh.h"
#include "../Framework/Keyboard.h"
#include "Renderer.h"
#include "DrawableTexture2D.h"

#define RENDER_HZ 60
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define GAME_TITLE "Team^3 - Endless Racer"

#define MAX_LIGHTS 10
#define MAX_MESHES 20
#define MAX_TEXTURES 20

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

class GraphicsEngine
{
	// TODO: Implement Graphics Engine.
public:
#pragma region Entry/Exit
	/**
	<summary>Initializes a graphics engine.</summary>
	<param name='graphics'>The pointer to the resulting graphics engine instance.</summary>
	<returns>true if a Graphics Engine is initialized and ready.</returns>
	*/
	static bool Initialize(GraphicsEngine* out);
	/**
	<summary>Destroys the graphics engine. Allows the game to exit cleanly.</summary>
	<returns>true if the graphics engine has exited.</returns>
	*/
	static bool Destroy();
	/**
	<summary>Gets whether the graphics engine has initialized and is ready to render.</summary>
	*/
	bool HasInitialised() { return isInitialised; }
	bool StartGraphicsEngineThread();
#pragma endregion
#pragma region TwoD
	/**
	<summary>Adds a drawable texture to the scene. Idempotent.</summary>
    <param name="drawableTexture">The drawable texture.</param>
	*/
	void AddTextureToScene(DrawableTexture2D* drawableTexture);
	/**
	<summary>Removes a drawable texture from the scene.</summary>
    <param name="drawableTexture">The drawable texture.</param>
	*/
	void RemoveTextureFromScene(DrawableTexture2D* drawableTexture);
	/**
	<summary>Draws a text string to the screen in the specified font at the specified position, in the specified color. </summary>
    <param name="text">The text to draw.</param>
    <param name="fontName">The assetName of the font to use.</param>
    <param name="position">The position.</param>
    <param name="color">The colour. Default is White (1,1,1,1).</param>
    <param name="rotation">The rotation from up in degrees. Default is 0.</param>
	*/
    void Draw(const string& text, const string& fontName, const T3Rectangle& position, const Vector4& color = Vector4(1.0f, 1.0f, 1.0f, 1.0f), const float rotation = 0); // TODO: Make a drawable text class for this!!
#pragma endregion
	
	static GraphicsEngine& GetGraphicsEngine() {return *engine;}

	

	Keyboard& GetKeyboard();

	// Light interface
	unsigned int AddPointLight(Vector3 lightPosition, float lightRadius, Vector4 diffuseColour, Vector4 specularColour, bool castsShadow);
	unsigned int AddDirectionalLight(Vector3 lightDirection, Vector4 diffuseColour, Vector4 specularColour, bool castsShadow);
	unsigned int AddSpotLight(Vector3 lightPosition, Vector3 lightTarget, Vector3 upVector, float lightRadius, float lightAngle, Vector4 diffuseColour, Vector4 specularColour);

	// Return values indicates success of update
	bool ChangeLightColour(unsigned int lightReference, Vector4 newDiffuseColour, Vector4 newSpecularColour);
	bool ChangeLightPosition(unsigned int lightReference, Vector3 newLightPosition);
	bool ChangeLightRadius(unsigned int lightReference, float newLightRadius);
	bool ChangeLightAngle(unsigned int lightReference, float newLightAngle);
	bool ChangeLightTarget(unsigned int lightReference, Vector3 newLightTarget, Vector3 newUpVector);
			
	// Return values indicates success of update
	bool RemoveLight(unsigned int lightReference);

	// Mesh interface
	unsigned int AddMesh(Mesh* mesh);
	bool RemoveMesh(unsigned int meshReference);

	// Textures interface
	unsigned int AddTexture(const char* filename, bool enableMipMaps = false, bool enableAnisotropicFiltering = false);
	bool RemoveTexture(unsigned int textureReference);

	

private:
	static GraphicsEngine* engine;

	GraphicsEngine();
	~GraphicsEngine();

	void GraphicsEngineThread();

	bool isInitialised;

	void BuildNodeLists(SceneNode* from);
	void SortNodeLists();
	void ClearNodeLists();
	void DrawNodes();

	Renderer* renderer;

	Texture* textureArray[MAX_TEXTURES];
	unsigned int numTextures;

	Mesh*	 meshArray[MAX_MESHES];
	unsigned int numMeshs;

	Light*	 lightArray[MAX_LIGHTS];
	unsigned int numLights;

	Frustum frameFrustum;
	Camera* camera;

	SceneNode* sceneRoot;
	vector<SceneNode*> transparentNodeList;
	vector<SceneNode*> nodeList;
};