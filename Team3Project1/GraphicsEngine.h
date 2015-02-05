/**
<summary>Static Singleton.
The game's graphics engine.

Author: Nick Grimm
Version: 0.0.1 03/02/2015.</summary>
*/

#include "../Framework/Vector3.h"
#include "../Framework/Vector4.h"
#include "../Framework/Light.h"
#include "../Framework/Texture.h"
#include "../Framework/Mesh.h"
#include "../Framework/Keyboard.h"
#include "Renderer.h"

#define RENDER_HZ 60
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define GAME_TITLE "Team^3 - Endless Racer"

#define MAX_LIGHTS 10
#define MAX_MESHES 20
#define MAX_TEXTURES 20

class GraphicsEngine
{
	// TODO: Implement Graphics Engine.
public:
	static bool Initialize();
	static bool Destroy();
	static GraphicsEngine& GetGraphicsEngine() {return *engine;}

	bool HasInitialised() {return isInitialised;}

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

	bool StartGraphicsEngineThread();

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