/**
<summary>Static Singleton.
The game's asset manager. Loads & Unloads assets.

This class offers the following guarantees:
1. All assets loaded through the class will be destroyed on exit.
2. No asset will be deleted more than once (no heap corruption).
3. No asset will loaded more than once at any time (no memory leak).
4. At any time after one object has called Load*(), the asset shall be available for use for that object,
until such time that object calls Unload*().

Author: Nick Grimm
Version: 0.0.1 03/02/2015.</summary>
*/
#pragma once
#include <vector>
#include <map>
//#include "Font.h"
#include "../Framework/Shader.h"
#include "LoadedTexture.h"
#include "LoadedMesh.h"

using namespace std;

class AssetManager
{
	// TODO: Implement Asset Manager.
public:
	static bool Initialize(AssetManager*& out) 
	{ 
		if (instance == NULL)
			instance = new AssetManager();

		out = instance;
		return true; 
	}
	static void Destroy();

	Texture* LoadTexture(void* callerID, string filePath);
	void UnloadTexture(void* callerID, string filePath);

	//Font* LoadFont(void* callerID, string filePath);
	//void UnloadFont(void* callerID, string filePath);

	Mesh* LoadMesh(void* callerID, string filePath);
	void UnloadMesh(void* callerID, string filePath);

	//Shader* LoadShader(void* callerID, string vertexShaderFilePath, string fragmentShaderFilePath, string geometryShaderFilePath = "");
	//void UnloadShader(void* callerID, string vertexShaderFilePath, string fragmentShaderFilePath, string geometryShaderFilePath = "");

	// Generate Heightmap
	// Load audio

private:
	AssetManager()
	{ }
	~AssetManager()
	{ }

	static AssetManager* instance;
	
	map<string, LoadedTexture> loadedTextures;
	//map<string, map<Font*, vector<void*>>> loadedFonts;
	map<string, LoadedMesh> loadedMeshes;

	map<string, map<unsigned int, vector<void*>>> loadedVertexShaders;
	map<string, map<unsigned int, vector<void*>>> loadedFragmentShaders;
	map<string, map<unsigned int, vector<void*>>> loadedGeometryShaders;
	map<string, map<Shader*, vector<void*>>> loadedShaders;
};