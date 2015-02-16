#include "AssetManager.h"
#include "../Framework/OBJMesh.h"
#include "../Framework/MD5Mesh.h"

AssetManager* AssetManager::instance = NULL;

void AssetManager::Destroy()
{
	for (map<string, LoadedTexture>::iterator i = instance->loadedTextures.begin(); i != instance->loadedTextures.end(); i++)
	{
		delete (*i).second.texture;
		i = instance->loadedTextures.erase(i);
	}
	for (map<string, LoadedMesh>::iterator i = instance->loadedMeshes.begin(); i != instance->loadedMeshes.end(); i++)
	{
		delete (*i).second.mesh;
		i = instance->loadedMeshes.erase(i);
	}

	if (instance != NULL)
		delete instance;
	instance = NULL;
}

Texture* AssetManager::LoadTexture(void* callerID, string filePath)
{
	// Check if this texture is already loaded
	map<string, LoadedTexture>::iterator i = loadedTextures.find(filePath);
	if (i != loadedTextures.end())
	{
		// Check if this caller already has this texture loaded
		for (int j = 0; j < loadedTextures[filePath].callerIDs.size(); j++)
		{
			if (loadedTextures[filePath].callerIDs[j] == callerID)
				return loadedTextures[filePath].texture; // It has, simply return

			loadedTextures[filePath].callerIDs.push_back(callerID); // It hasn't, add this caller, then return.
			return loadedTextures[filePath].texture;
		}
	}
	else
	{
		// Load this texture in...
		Texture* newTexture = new Texture(filePath.c_str(), 0, 0, "");
		loadedTextures.insert(pair<string, LoadedTexture>(filePath, LoadedTexture(newTexture, callerID)));
		return newTexture;
	}
}

void AssetManager::UnloadTexture(void* callerID, string filePath)
{
	// Check if this texture is actually loaded...
	map<string, LoadedTexture>::iterator i = loadedTextures.find(filePath);
	if (i == loadedTextures.end())
		return; // This texture does not exist, ignore.

	// This texture does exist. Remove the caller from the list of owners.
	for (vector<void*>::iterator i = loadedTextures[filePath].callerIDs.begin(); i != loadedTextures[filePath].callerIDs.end(); i++)
	{
		if (*i == callerID)
		{
			i = loadedTextures[filePath].callerIDs.erase(i);
			break;
		}
	}

	// No owners left? delete
	if (loadedTextures[filePath].callerIDs.size() == 0)
	{
		delete loadedTextures[filePath].texture;
		i = loadedTextures.erase(i);
	}
}

Mesh* AssetManager::LoadMesh(void* callerID, string filePath)
{
	// Check if this mesh is already loaded
	map<string, LoadedMesh>::iterator i = loadedMeshes.find(filePath);
	if (i != loadedMeshes.end())
	{
		// Check if this caller already has this texture loaded
		for (int j = 0; j < loadedMeshes[filePath].callerIDs.size(); j++)
		{
			if (loadedMeshes[filePath].callerIDs[j] == callerID)
				return loadedMeshes[filePath].mesh; // It has, simply return

			loadedMeshes[filePath].callerIDs.push_back(callerID); // It hasn't, add this caller, then return.
			return loadedMeshes[filePath].mesh;
		}
	}
	else
	{
		// Load this texture in...
		Mesh* newMesh;
		if (filePath.substr(filePath.length() - 4, 3) == "obj")
		{
			newMesh = new OBJMesh(filePath);
		}
		else if (filePath.substr(filePath.length() - 4, 3) == "md5")
		{
			newMesh = new MD5Mesh(MD5FileData(filePath));
		}
		else
			return NULL; // Unrecognised fileType. OOOPS!!

		loadedMeshes.insert(pair<string, LoadedMesh>(filePath, LoadedMesh(newMesh, callerID)));
		return newMesh;
	}
}

void AssetManager::UnloadMesh(void* callerID, string filePath)
{
	// Check if this texture is actually loaded...
	map<string, LoadedMesh>::iterator i = loadedMeshes.find(filePath);
	if (i == loadedMeshes.end())
		return; // This texture does not exist, ignore.

	// This texture does exist. Remove the caller from the list of owners.
	for (vector<void*>::iterator i = loadedMeshes[filePath].callerIDs.begin(); i != loadedMeshes[filePath].callerIDs.end(); i++)
	{
		if (*i == callerID)
		{
			i = loadedMeshes[filePath].callerIDs.erase(i);
			break;
		}
	}

	// No owners left? delete
	if (loadedMeshes[filePath].callerIDs.size() == 0)
	{
		delete loadedMeshes[filePath].mesh;
		i = loadedMeshes.erase(i);
	}
}