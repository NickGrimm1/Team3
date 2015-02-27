#pragma once
#if WINDOWS_BUILD
#include "../Framework/OGLRenderer.h"
#endif
#if PS3_BUILD
#include <cell\gcm.h>
#include <Cg\cg.h>
#include <Cg\cgBinary.h>
#include <cell\cgb.h>
#include <Cg\cgc.h>
#include <cell\sysmodule.h>
#include <sys\paths.h>
#include <vectormath\cpp\vectormath_aos.h>
#include <cell\cell_fs.h>
#include <map>
#endif
#include "../Framework/Shader.h"
#include "Vertex.h"
#include <string>

using namespace std;
#if PS3_BUILD
using namespace Vectormath::Aos;
#endif

class ShaderPart
{
public:
	ShaderPart(string raw, ShaderType::Type type);
	~ShaderPart()
	{
#if WINDOWS_BUILD
		glDeleteShader(shader);
#endif
	}
#if PS3_BUILD
	void* GetuCode();
	CGprogram GetProgram();
	unsigned int GetOffset();
	unsigned int GetAttributeIndex(VertexAttributes::Attributes attribute);
	CGparameter GetParameter(string target);
	void SetParameter(string name, Matrix4 &totranpose);
	void UpdateShaderMatrices(Matrix4 &model,Matrix4 &view, Matrix4 &proj);
#endif
#if WINDOWS_BUILD
	GLuint shader;
	static string LoadShaderFile(string filename);
#endif
#if PS3_BUILD
	static string LoadShaderFile(string filename){return filename;};
	void SetDefaultAttributes();
	unsigned int attributes[VertexAttributes::MAX];
	CGprogram program;
	void* ucode;
	//unsigned int offset;
	uint32_t offset;
	map<string, CGparameter> uniforms;
	virtual void SetParameter(string name, float* data);
	void UpdateShaderVariables();
	
#endif
private:
	ShaderPart(const ShaderPart& in);
	ShaderPart operator=(const ShaderPart& in);
	ShaderType::Type type;
};