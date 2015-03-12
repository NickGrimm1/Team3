#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include "mesh.h"

#define RAW_WIDTH 256
#define RAW_HEIGHT 256

#define HEIGHTMAP_X 16.0f
#define HEIGHTMAP_Z 16.0f
#define HEIGHTMAP_Y 1.25f
#define HEIGHTMAP_TEX_X 1.0f / 16.0f
#define HEIGHTMAP_TEX_Z 1.0f / 16.0f

using namespace std;

class HeightMap : public Mesh 
{
public:
	HeightMap(string name = "", bool useTextureWeights = true, unsigned char minHeight = 0, unsigned char maxHeight = 255);
	~HeightMap(void) {}
	T3Vector3 GetVertexPosition(int x, int z);
private:
	unsigned char* data;
	float Clamp(float f, float min = 0.0f, float max = 1.0f);
};