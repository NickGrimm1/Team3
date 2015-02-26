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
#include "LoadedFont.h"
#include "LoadedShader.h"
#include "LoadedTexture.h"
#include "LoadedMesh.h"
#include "LoadedShaderPart.h"

using namespace std;

class AssetManager
{
	// TODO: Implement Asset Manager.
public:
	static bool Initialize(AssetManager*& out);
	static void Destroy();

	Texture* LoadTexture(void* callerID, string filePath, unsigned int flags);
	void UnloadTexture(void* callerID, string filePath);

	Font* LoadFont(void* callerID, string filePath, unsigned int xCount, unsigned int yCount);
	void UnloadFont(void* callerID, string filePath);

	Mesh* LoadMesh(void* callerID, string filePath);
	void UnloadMesh(void* callerID, string filePath);

	//Generates a single triangle, with RGB colours
	Mesh* LoadTriangle(void* callerID);
	//Generates a single white quad, going from -1 to 1 on the x and y axis.
	Mesh* LoadQuad(void* callerID);
	//Generates a single white quad, going from 0 to 1 on the x and y axis.
	Mesh* LoadQuadAlt(void* callerID);
	//Generates a single white quad, going from -0.5 to 0.5 on the x and y axis.
	Mesh* LoadQuadCentral(void* callerID);
	//Generates a coloured quad, going from -1 to 1 on the x and z axis, with adjustable texture coords.
	Mesh* LoadQuadTexCoordCol(void* callerID, T3Vector2 scale, T3Vector2 texCoord, T3Vector4 colour); //NX 01/11/2012
	// Generates Circle/Cone/Cylinder meshes with subdivs segments to approximate curvature
	Mesh* LoadCircle(void* callerID, unsigned int subdivs);
	Mesh* LoadCone(void* callerID, unsigned int subdivs);
	Mesh* LoadCylinder(void* callerID, unsigned int subdivs);
	//Generates a single triangle, with RGB colours
	void UnloadTriangle(void* callerID);
	//Generates a single white quad, going from -1 to 1 on the x and y axis.
	void UnloadQuad(void* callerID);
	//Generates a single white quad, going from 0 to 1 on the x and y axis.
	void UnloadQuadAlt(void* callerID);
	//Generates a single white quad, going from -0.5 to 0.5 on the x and y axis.
	void UnloadQuadCentral(void* callerID);
	//Generates a coloured quad, going from -1 to 1 on the x and z axis, with adjustable texture coords.
	void UnloadQuadTexCoordCol(void* callerID, T3Vector2 scale, T3Vector2 texCoord, T3Vector4 colour); //NX 01/11/2012
	// Generates Circle/Cone/Cylinder meshes with subdivs segments to approximate curvature
	void UnloadCircle(void* callerID, unsigned int subdivs);
	void UnloadCone(void* callerID, unsigned int subdivs);
	void UnloadCylinder(void* callerID, unsigned int subdivs);

	Shader* LoadShader(void* callerID, string vertexShaderFilePath, string fragmentShaderFilePath, string geometryShaderFilePath = "");
	void UnloadShader(void* callerID, string vertexShaderFilePath, string fragmentShaderFilePath, string geometryShaderFilePath = "");

	// Generate Heightmap
	// Load audio

	int GetTotalMemory() { return (int)(textureMemory + meshMemory); }
	int GetTextureMemory() { return (int)textureMemory; }
	int GetMeshMemory() { return (int)meshMemory; }
private:
	AssetManager()
	{
		meshMemory = 0;
		textureMemory = 0;
	}
	~AssetManager()
	{ }

	static AssetManager* instance;
	
	map<string, LoadedTexture> loadedTextures;
	map<string, LoadedFont> loadedFonts;
	map<string, LoadedMesh> loadedMeshes;

	map<string, LoadedShaderPart> loadedShaderParts;
	map<string, LoadedShader> loadedShaders;

	Mesh* triangle;
	Mesh* quad;
	Mesh* quadAlt;
	Mesh* quadCentral;
	Mesh* quadTexCoordCol;
	Mesh* circle;
	Mesh* cone;
	Mesh* cylinder;

	vector<void*> triangleUsers;
	vector<void*> quadUsers;
	vector<void*> quadAltUsers;
	vector<void*> quadCentralUsers;
	vector<void*> quadTexCoordColUsers;
	
	map<unsigned int, LoadedMesh> circleUsers;
	map<unsigned int, LoadedMesh> coneUsers;
	map<unsigned int, LoadedMesh> cylinderUsers;

	float meshMemory;
	float textureMemory;
};