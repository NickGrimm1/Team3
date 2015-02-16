#include "AssetManager.h"

AssetManager* AssetManager::instance = NULL;

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