#pragma once
#if WINDOWS_BUILD
#include "GL/glew.h"
#endif
#include <string>

using namespace std;

#if PS3_BUILD
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
#endif

class Texture
{
public:
	Texture(string filename, unsigned int flags);
	~Texture();
	void SetRepeating(bool repeat);
	void SetAnistropicFiltering(bool enable);
#if WINDOWS_BUILD
	GLuint GetTextureName() const { return textureObject; }
	void SetMinMagFiltering(GLint min, GLint mag);
#endif
protected:
	#if WINDOWS_BUILD
	GLuint textureObject;
	GLint minFilter; // GL_TEXTURE_MIN_FILTER setting
	GLint magFilter; // GL_TEXTURE_MAG_FILTER setting
#endif
	bool repeating; // determines whether textures is repeated or clamped
	bool anisotropic; // enables anisotropic filtering for the texture
private:
	Texture(const Texture& in);
	Texture& operator=(const Texture& in);
#if PS3_BUILD
	CellGcmTexture* LoadTGA(std::string name);
	CellGcmTexture* LoadGTF(std::string name);
#endif
};
