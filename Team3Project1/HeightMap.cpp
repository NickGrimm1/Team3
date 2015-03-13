#include "HeightMap.h"
#include "GameStateManager.h"

HeightMap::HeightMap(string name, bool useTextureWeights, unsigned char minHeight, unsigned char maxHeight, HeightMap* tileTo, unsigned int tileSide) 
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
			vertices[offset].SetPosition(T3Vector3(x * HEIGHTMAP_X, data[offset] , z * HEIGHTMAP_Z));
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

	if (tileTo != NULL)
		Tile(tileTo, tileSide, ((maxHeight - minHeight) / 20) * HEIGHTMAP_Y);

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
void HeightMap::Tile(HeightMap* tileTo, unsigned int tileSide, unsigned char maxDifference)
{
	if (tileSide & Side::LEFT)
	{
		// Match the height of the front of tileTo with the height of the back of this.
		for(int z = 0; z < RAW_HEIGHT; z++)
		{
			vertices[z].y = tileTo->vertices[(RAW_HEIGHT - 1) * RAW_WIDTH + z].y;
		}
		
		// Now each vertex touches, but we may have a bit of a big height difference... we need to smooth it...
		bool columnChange[RAW_HEIGHT];
		for (int i = 0; i < RAW_HEIGHT; ++i) // This will keep track of whether we are continuing to smooth in this z;
			columnChange[i] = true;

		for (int x = 1; x < RAW_WIDTH; ++x)
		{
			bool rowChange = false; // This allows us to track if we have made any changes this run through and call an early end to the loop.
			for (int z = 0; z < RAW_HEIGHT; ++z)
			{
				if (columnChange[z])
				{
					if (abs(vertices[x * RAW_WIDTH + z].y - vertices[(x - 1) * RAW_WIDTH + z].y) > maxDifference) // Is the height difference big between this row and the previous row?
					{
						rowChange = true;
						vertices[x * RAW_WIDTH + z].y < vertices[(x - 1) * RAW_WIDTH + z].y ?
							vertices[x * RAW_WIDTH + z].y = vertices[(x - 1) * RAW_WIDTH + z].y - maxDifference : // current row much lower, bring it up to the maximum difference.
							vertices[x * RAW_WIDTH + z].y = vertices[(x - 1) * RAW_WIDTH + z].y + maxDifference; // current row much higher, bring it down to the maximum difference.
					}
					else
					{
						columnChange[z] = false;
					}
				}
			}
			if (!rowChange)
				break;
		}
	}

	if (tileSide & Side::RIGHT)
	{
		// Match the height of the back of tileTo with the height of the front of this.
		for(int z = 0; z < RAW_HEIGHT; ++z)
		{
			vertices[(RAW_HEIGHT - 1) * RAW_WIDTH + z].y = tileTo->vertices[z].y;
		}
		
		// Now each vertex touches, but we may have a bit of a big height difference... we need to smooth it...
		bool columnChange[RAW_HEIGHT];
		for (int i = 0; i < RAW_HEIGHT; ++i) // This will keep track of whether we are continuing to smooth in this z;
			columnChange[i] = true;

		for (int x = RAW_WIDTH - 2; x > 0; --x)
		{
			bool rowChange = false; // This allows us to track if we have made any changes this run through and call an early end to the loop.
			for (int z = 0; z < RAW_HEIGHT; ++z)
			{
				if (columnChange[z])
				{
					if (abs(vertices[x * RAW_WIDTH + z].y - vertices[(x + 1) * RAW_WIDTH + z].y) > maxDifference) // Is the height difference big between this row and the previous row?
					{
						rowChange = true;
						vertices[x * RAW_WIDTH + z].y < vertices[(x + 1) * RAW_WIDTH + z].y ?
							vertices[x * RAW_WIDTH + z].y = vertices[(x + 1) * RAW_WIDTH + z].y - maxDifference : // current row much lower, bring it up to the maximum difference.
							vertices[x * RAW_WIDTH + z].y = vertices[(x + 1) * RAW_WIDTH + z].y + maxDifference; // current row much higher, bring it down to the maximum difference.
					}
					else
					{
						columnChange[z] = false;
					}
				}
			}
			if (!rowChange)
				break;
		}
	}

	if (tileSide & Side::FRONT)
	{
		// Match the height of the left of tileTo with the height of the right of this.
		for(int x = 0; x < RAW_WIDTH; ++x)
		{
			vertices[x * RAW_WIDTH].y = tileTo->vertices[x * RAW_WIDTH + RAW_HEIGHT - 1].y;
		}
		
		// Now each vertex touches, but we may have a bit of a big height difference... we need to smooth it...
		bool columnChange[RAW_WIDTH];
		for (int i = 0; i < RAW_WIDTH; ++i) // This will keep track of whether we are continuing to smooth in this z;
			columnChange[i] = true;

		for (int z = 1; z < RAW_HEIGHT; ++z)
		{
			bool rowChange = false; // This allows us to track if we have made any changes this run through and call an early end to the loop.
			for (int x = 0; x < RAW_WIDTH; ++x)
			{
				if (columnChange[x])
				{
					if (abs(vertices[x * RAW_WIDTH + z].y - vertices[x * RAW_WIDTH + z - 1].y) > maxDifference) // Is the height difference big between this row and the previous row?
					{
						rowChange = true;
						vertices[x * RAW_WIDTH + z].y < vertices[x * RAW_WIDTH + z - 1].y ?
							vertices[x * RAW_WIDTH + z].y = vertices[x * RAW_WIDTH + z - 1].y - maxDifference : // current row much lower, bring it up to the maximum difference.
							vertices[x * RAW_WIDTH + z].y = vertices[x * RAW_WIDTH + z - 1].y + maxDifference; // current row much higher, bring it down to the maximum difference.
					}
					else
					{
						columnChange[x] = false;
					}
				}
			}
			if (!rowChange)
				break;
		}
	}

	if (tileSide & Side::BACK)
	{
		// Match the height of the left of tileTo with the height of the right of this.
		for(int x = 0; x < RAW_WIDTH; ++x)
		{
			vertices[x * RAW_WIDTH + RAW_HEIGHT - 1].y = tileTo->vertices[x * RAW_WIDTH].y;
		}
		
		// Now each vertex touches, but we may have a bit of a big height difference... we need to smooth it...
		bool columnChange[RAW_WIDTH];
		for (int i = 0; i < RAW_WIDTH; ++i) // This will keep track of whether we are continuing to smooth in this z;
			columnChange[i] = true;

		for (int z = RAW_HEIGHT - 2; z > 0; --z)
		{
			bool rowChange = false; // This allows us to track if we have made any changes this run through and call an early end to the loop.
			for (int x = 0; x < RAW_WIDTH; ++x)
			{
				if (columnChange[x])
				{
					if (abs(vertices[x * RAW_WIDTH + z].y - vertices[x * RAW_WIDTH + z + 1].y) > maxDifference) // Is the height difference big between this row and the previous row?
					{
						rowChange = true;
						vertices[x * RAW_WIDTH + z].y < vertices[x * RAW_WIDTH + z + 1].y ?
							vertices[x * RAW_WIDTH + z].y = vertices[x * RAW_WIDTH + z + 1].y - maxDifference : // current row much lower, bring it up to the maximum difference.
							vertices[x * RAW_WIDTH + z].y = vertices[x * RAW_WIDTH + z + 1].y + maxDifference; // current row much higher, bring it down to the maximum difference.
					}
					else
					{
						columnChange[x] = false;
					}
				}
			}
			if (!rowChange)
				break;
		}
	}
}