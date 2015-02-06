/**
<summary>Texture
Implements a Texture class that manages Shader bindings

Author: Derek Kelly
Version: 0.0.1 04/02/2015</summary>
*/

#include "Texture.h"
#include "SOIL.h"
#include "common.h"

// DK - trying to remove OpenGL specific stuff, just want as wrapper class
/*
Texture::Texture(const char* filename, unsigned int flags, GLenum textUnit, std::string mapping) :
	shaderMapping(mapping)
{
	textureObject = SOIL_load_OGL_texture(filename, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, flags);
	if (!textureObject) {
		textureObject = 0; // make sure GetTexture will return an error
		return;
	}
	textureUnit = textUnit;
}
*/
Texture::Texture(GLuint textName, GLenum textUnit, std::string mapping) :
	shaderMapping(mapping) 
{
	textureObject = textName;
	if (!textureObject) {
		textureObject = 0; // make sure GetTexture will return an error
		return;
	}
	textureUnit = textUnit;
}

Texture::~Texture(void)
{
	//glDeleteTextures(1, &textureObject);
}

void Texture::BindTexture(bool set_texture_unit) {
	GLint prog = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &prog);
	glUniform1i(glGetUniformLocation(prog, shaderMapping.c_str()), textureUnit);
	glUniform1i(glGetUniformLocation(prog, "useDiffuseTex"), textureObject);

	if (set_texture_unit) {
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GL_TEXTURE_2D, textureObject);
	}
}

void Texture::SetRepeating(bool repeat) {
	repeating = repeat;
	glBindTexture(GL_TEXTURE_2D, textureObject);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, // x-axis
					repeating ? GL_REPEAT : GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, // y-axis
					repeating ? GL_REPEAT : GL_CLAMP);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::SetMinMagFiltering(GLint min, GLint mag) {
	minFilter = min;
	magFilter = mag;

	glBindTexture(GL_TEXTURE_2D, textureObject);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);// minifying function - pixel being textured maps to greater than one texture element
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);// magnification function - pixel being textured maps to area less than or equal to one texture element (texel)
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::SetAnistropicFiltering(bool enable) {
	anisotropic = enable;
	
	glBindTexture(GL_TEXTURE_2D, textureObject);
	if (anisotropic) {
		GLfloat maxAnisotropy = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
	}
	else glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 0.0f);

	glBindTexture(GL_TEXTURE_2D, 0);
}