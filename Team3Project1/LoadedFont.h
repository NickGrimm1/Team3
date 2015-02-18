#pragma once
#include "Font.h"
#include <vector>

using namespace std;

class LoadedFont
{
public:
	LoadedFont(Font* font = NULL, void* callerID = NULL) : font(font)
	{
		callerIDs.push_back(callerID);
	}
	Font* font;
	vector<void*> callerIDs;
};