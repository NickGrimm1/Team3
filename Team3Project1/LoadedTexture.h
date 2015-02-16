#pragma once
#include "../Framework/Texture.h"
#include <vector>

using namespace std;

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