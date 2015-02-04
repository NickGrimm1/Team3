/**
<summary>Static Singleton.
The game's graphics engine.

Author: Nick Grimm
Version: 0.0.1 03/02/2015.</summary>
*/

#include "Vector3.h"
#include "../Framework/Vector4.h"
#include "../Framework/Light.h"
#include "../Framework/Texture.h"
#include "../Framework/Mesh.h"
#include "../Framework/Keyboard.h"
#include "Renderer.h"

class GraphicsEngine
{
	// TODO: Implement Graphics Engine.
public:
	static bool Initialize(GraphicsEngine& out);
	static bool Destroy(GraphicsEngine& engine);

	bool CreateNewGameWindow(const char* title, unsigned int width, unsigned int height, bool fullScreen); // Presumably this will be different for PS3

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
	GraphicsEngine();
	~GraphicsEngine();

	Renderer	renderer;
	Window		window;
};