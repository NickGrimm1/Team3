#pragma once
#include <vector>
#include "ShaderPart.h"

using namespace std;

class LoadedShaderPart
{
public:
	LoadedShaderPart(ShaderPart* shaderPart = NULL, void* callerID = NULL) : shaderPart(shaderPart)
	{
		callerIDs.push_back(callerID);
	}
	ShaderPart* shaderPart;
	vector<void*> callerIDs;
};