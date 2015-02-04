#pragma once

/**
<summary>Empty Texture
Extends the Texture class to enable objects with NO Textures to be used with texture based shaders

Author: Derek Kelly
Version: 0.0.1 04/02/2015</summary>
*/

#include "Texture.h"

class EmptyTexture : public Texture {
public:
	EmptyTexture() {};

	virtual void BindTexture(bool set_texture_unit) {
		GLint prog = 0;
		glGetIntegerv(GL_CURRENT_PROGRAM, &prog);
		glUniform1i(glGetUniformLocation(prog, "useDiffuseTex"), 0);
		glUniform1i(glGetUniformLocation(prog, "useNormalTex"), 0);
	}
};