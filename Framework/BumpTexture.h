#pragma once

/**
<summary>Bump Texture
Extends the Texture class to enable Bump Textures

Author: Derek Kelly
Version: 0.0.1 04/02/2015</summary>
*/

#include "Texture.h"

class BumpTexture : public Texture {
public:
	BumpTexture(const char* filename, unsigned int flags, GLenum textUnit, std::string shaderMapping) : Texture(filename, flags, textUnit, shaderMapping) {}
	~BumpTexture() {}

	virtual void BindTexture(bool set_texture_unit) {
		GLint prog = 0;
		glGetIntegerv(GL_CURRENT_PROGRAM, &prog);
		glUniform1i(glGetUniformLocation(prog, shaderMapping.c_str()), textureUnit);
		glUniform1i(glGetUniformLocation(prog, "useNormalTex"), textureUnit);
		if (set_texture_unit) {
			glActiveTexture(GL_TEXTURE0 + textureUnit);
			glBindTexture(GL_TEXTURE_2D, textureObject);
		}
	}
};