#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include "mesh.h"
#include "Spline.h"

#define RAW_WIDTH 256
#define RAW_HEIGHT 256

#define HEIGHTMAP_X 2.0f
#define HEIGHTMAP_Z 1.0f
#define HEIGHTMAP_Y 2.0f
#define HEIGHTMAP_TEX_X 1.0f / 16.0f
#define HEIGHTMAP_TEX_Z 1.0f / 16.0f

using namespace std;

namespace Side
{
	enum S
	{
		NONE = 0,
		FRONT = 1,
		BACK = 2,
		LEFT = 3,
		RIGHT = 4,
	};
}

class HeightMap : public Mesh 
{
public:
	HeightMap(string name = "", bool useTextureWeights = true, unsigned char minHeight = 0, unsigned char maxHeight = 255, HeightMap* tileTo = NULL, unsigned int tileSide = Side::NONE);
	~HeightMap(void) {}
	T3Vector3 GetVertexPosition(int x, int z);
	
	//void ShiftHeightMap(bool shiftLeft, Spline* spline);

private:
	unsigned char* data;
	float Clamp(float f, float min = 0.0f, float max = 1.0f);
	void Tile(HeightMap* tileTo, unsigned int tileSide, unsigned char maxDifference);
};