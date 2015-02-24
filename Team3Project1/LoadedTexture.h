#pragma once
#if WINDOWS_BUILD
#include "Texture.h"
#endif
#if PS3_BUILD
#include "../Main_PS3_PPU/TexturePS3.h"
#endif
#include <vector>

using namespace std;

class Texture;

class LoadedTexture
{
public:
	LoadedTexture(Texture* texture = NULL, void* callerID = NULL) : texture(texture)
	{
		callerIDs.push_back(callerID);
	}
	Texture* texture;
	vector<void*> callerIDs;
};