#if WINDOWS_BUILD

#include "Texture.h"
#include "SOIL.h"

Texture::Texture(string filename, unsigned int flags)
{
	textureObject = SOIL_load_OGL_texture(filename.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, flags);
	if (!textureObject) {
		textureObject = 0; // make sure GetTexture will return an error
		return;
	}

	GLint width = 0;
	GLint height = 0;
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureObject);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
	memory = (width * height * 4) / 1024.0f / 1024.0f; // Assume all textures are 32 bit for the moment.
}

Texture::Texture(GLuint textureObj)
{
	textureObject = textureObj;
	if (!textureObject) {
		textureObject = 0; // make sure GetTexture will return an error
		return;
	}
}

Texture::~Texture()
{
	glDeleteTextures(1, &textureObject);
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
		GLfloat maximumAnisotropy = 0.0f;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maximumAnisotropy);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maximumAnisotropy);
	}
	else glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 0.0f);

	glBindTexture(GL_TEXTURE_2D, 0);
}
#endif