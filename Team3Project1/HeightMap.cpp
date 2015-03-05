#include "HeightMap.h"
#include "GameStateManager.h"

HeightMap::HeightMap(string name, bool useTextureWeights, unsigned char minHeight, unsigned char maxHeight) 
{
	numVertices = RAW_WIDTH * RAW_HEIGHT;
	numIndices = (RAW_WIDTH - 1) * (RAW_HEIGHT - 1) * 6;
	
	AssignVertexMemory();
	AssignIndexMemory(numIndices);
	data = new unsigned char[numVertices];

	if (name != "")
	{
		// We have a file, we can read it in...
		std::ifstream file(name.c_str(), ios::binary);
		if (!file)
		{
			return;
		}	
	
		file.read((char*)data, numVertices * sizeof(unsigned char));
		file.close();
	}
	else
	{
		// Generate one...
		data = GameStateManager::Graphics()->GeneratePerlinNoise(256, minHeight, maxHeight);
	}
	
	for(int x = 0; x < RAW_WIDTH; ++x)
	{
		for(int z = 0; z < RAW_HEIGHT; ++z)
		{
			int offset = (x * RAW_WIDTH) + z;
			vertices[offset].SetPosition(T3Vector3(x * HEIGHTMAP_X, data[offset] * HEIGHTMAP_Y, z * HEIGHTMAP_Z));
			vertices[offset].SetTexCoord(T3Vector2(x * HEIGHTMAP_TEX_X, z * HEIGHTMAP_TEX_Z));
			if (useTextureWeights)
			{

				vertices[offset].SetColor(T3Vector4(Clamp(1.0f - abs(data[offset] - 0.0f) / 72.0f),
					Clamp(1.0f - abs(data[offset] - 64.0f) / 72.0f),
					Clamp(1.0f - abs(data[offset] - 192.0f) / 72.0f),
					Clamp(1.0f - abs(data[offset] - 256.0f) / 72.0f)));
			}
			else
			{
				vertices[offset].SetColor(T3Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
	}

	numIndices = 0;
	for (int x = 0; x < RAW_WIDTH -1; ++x) 
	{
		for (int z = 0; z < RAW_HEIGHT -1; ++z) 
		{
			int a = (x * (RAW_WIDTH)) + z;
			int b = ((x+1) * (RAW_WIDTH)) + z;
			int c = ((x+1) * (RAW_WIDTH)) + (z + 1);
			int d = (x * (RAW_WIDTH)) + (z + 1);

			indices[numIndices++] = c;
			indices[numIndices++] = b;
			indices[numIndices++] = a;
			indices[numIndices++] = a;
			indices[numIndices++] = d;
			indices[numIndices++] = c;
		}
	}

	GenerateNormals();
	GenerateTangents();
#if WINDOWS_BUILD
	BufferData();
#endif
}
T3Vector3 HeightMap::GetVertexPosition(int x, int z)
{
	return T3Vector3(x * HEIGHTMAP_X, data[x * RAW_WIDTH + z] * HEIGHTMAP_Y, z * HEIGHTMAP_Z);
}
float HeightMap::Clamp(float f, float min, float max)
{
	if (f < min)
		return min;

	if (f > max)
		return max;

	return f;
}