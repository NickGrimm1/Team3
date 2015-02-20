#pragma once
#if WINDOWS_BUILD
#include "../Framework/OGLRenderer.h"
#endif
#include <string>
#if PS3_BUILD
#include <map>
#endif

using namespace std;

class ShaderPart
{
public:
	ShaderPart(string raw, ShaderType::Type type);
#if WINDOWS_BUILD
	~ShaderPart()
	{
		glDeleteShader(shader);
	}
	GLuint shader;
	static string LoadShaderFile(string filename);
#endif
#if PS3_BUILD
	void* GetUCode() { return ucode; }
	CGProgram GetProgram() { return program; }
	unsigned int GetAttributeIndex(VertexAttributes attribute) { return attributes[attribute]; }
#endif
private:
	ShaderPart(const ShaderPart& in);
	ShaderPart operator=(const ShaderPart& in);
#if PS3_BUILD
	void SetDefaultAttributes();
	unsigned int attributes[VertexAttributes::MAX];
	CGprogram program;
	void * ucode;
	unsigned int offset;
	map<string, CGparameter> uniforms;
	virtual void SetParameter(std::string name, float*data);
	void UpdateShaderVariables();
#endif
};