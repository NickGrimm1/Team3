#pragma once
#if PS3_BUILD
#include "../Team3Project1/TextureCommon.h"

#include <cell/gcm.h>
#include <string>

using namespace std;

typedef struct {
	uint32_t Version;
	uint32_t Size;
	uint32_t NumTexture;
} CellGtfFileHeader;

typedef struct {
	uint32_t Id;
	uint32_t OffsetToTex;
	uint32_t TextureSize;
	CellGcmTexture tex;
} CellGtfTextureAttribute;

class Texture : public TextureCommon
{
public:
	Texture(string filename, unsigned int flags);
	CellGcmTexture* GetTexture() { return texture; }
private:
	CellGcmTexture* texture;
	CellGcmTexture* LoadTGA(std::string name);
	CellGcmTexture* LoadGTF(std::string name);
};
#endif