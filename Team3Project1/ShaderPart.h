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
	ShaderPart(string raw, ShaderType::Type type);
	~ShaderPart()
	{
		glDeleteShader(shader);
	}
	GLuint shader;
#endif
	static string LoadShaderFile(string filename);
private:
	ShaderPart(const ShaderPart& in);
	ShaderPart operator=(const ShaderPart& in);
};