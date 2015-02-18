#pragma once
#if WINDOWS_BUILD
#include "GL/glew.h"
#endif
#include <string>

using namespace std;

class Texture
{
public:
	Texture(string filename);
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
};

