#pragma once
#include "../Framework/Mesh.h"
#include <vector>

using namespace std;

class LoadedMesh
{
public:
	LoadedMesh(Mesh* mesh = NULL, void* callerID = NULL) : mesh(mesh)
	{
		callerIDs.push_back(callerID);
	}
	Mesh* mesh;
	vector<void*> callerIDs;
};