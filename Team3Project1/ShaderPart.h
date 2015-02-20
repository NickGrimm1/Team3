#if WINDOWS_BUILD
#pragma once
#include "../Framework/OGLRenderer.h"
#include <string>

using namespace std;

class ShaderPart
{
public:
	ShaderPart(string raw, ShaderType::Type type);
	~ShaderPart()
	{
		glDeleteShader(shader);
	}
	GLuint shader;
	static string LoadShaderFile(string filename);
private:
	ShaderPart(const ShaderPart& in);
	ShaderPart operator=(const ShaderPart& in);
};
#endif