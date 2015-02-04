#pragma once

/**
<summary>Texture
Implements a Texture class that manages Shader bindings

Author: Derek Kelly
Version: 0.0.1 04/02/2015</summary>
*/


#include "GL/glew.h"
#include <string>

class Texture
{
public:
	Texture(const char* filename, unsigned int flags, GLenum textUnit, std::string mapping);
	Texture(GLuint textName, GLenum textUnit, std::string mapping);
	~Texture(void);

	GLuint GetTextureName() const {return textureObject;}
	GLuint GetTextureUnit() const {return textureUnit;}
	virtual void BindTexture(bool set_texture_unit);

	// Texture settings
	void SetRepeating(bool repeat);
	void SetMinMagFiltering(GLint min, GLint mag);
	void SetAnistropicFiltering(bool enable);

protected:
	Texture() {};

	// Texture state
	GLuint textureObject;		// the texture's OpenGL object name
	GLuint textureUnit;			// the texture unit the texture is loaded in
	std::string shaderMapping;  // The name of the sampler in the shader the texture unit should be linked to

	// Filter setting
	bool repeating; // determines whether textures is repeated or clamped
	GLint minFilter; // GL_TEXTURE_MIN_FILTER setting
	GLint magFilter; // GL_TEXTURE_MAG_FILTER setting
	bool anisotropic; // enables anisotropic filtering for the texture
};

