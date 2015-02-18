#pragma once
#include "../Framework/Shader.h"
#include <vector>

using namespace std;

class LoadedShader
{
public:
	LoadedShader(Shader* shader = NULL, void* callerID = NULL) : shader(shader)
	{
		callerIDs.push_back(callerID);
	}
	Shader* shader;
	vector<void*> callerIDs;
};