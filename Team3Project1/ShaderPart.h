#pragma once
#if WINDOWS_BUILD
#include "../Framework/OGLRenderer.h"
#endif
#include <string>

using namespace std;

class ShaderPart
{
public:
#if WINDOWS_BUILD
	ShaderPart(GLuint shader) : shader(shader)
	{
	}
	~ShaderPart()
	{
		glDeleteShader(shader);
	}
	GLuint shader;
#endif
	static ShaderPart* LoadShaderFile(string filename, ShaderType::Type type);
};