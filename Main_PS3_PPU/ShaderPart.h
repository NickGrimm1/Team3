#pragma once
#if PS3_BUILD
#include <cell/gcm.h>
#include <Cg/cg.h>
#include <Cg/cgBinary.h>
#include <cell/cgb.h>
#include <Cg/cgc.h>

#include <cell/sysmodule.h>

#include <sys/paths.h>							//And know about SYS_APP_HOME
#include <vectormath/cpp/vectormath_aos.h>
#include <cell/cell_fs.h>

#include <string>
#include <map>

using namespace cell::Gcm;

#include "GCMRenderer.h"

using namespace std;

class ShaderPart
{
public:
	ShaderPart(string raw, ShaderType::Type type);
	~ShaderPart()
	{
	}
	void* GetUCode() { return ucode; }
	CGProgram GetProgram() { return program; }
	unsigned int GetOffset() { return offset; }
	unsigned int GetAttributeIndex(VertexAttributes::Attributes attribute) { return attributes[attribute]; }
private:
	ShaderPart(const ShaderPart& in);
	ShaderPart operator=(const ShaderPart& in);
	void SetDefaultAttributes();
	unsigned int attributes[VertexAttributes::MAX];
	CGprogram program;
	void * ucode;
	unsigned int offset;
	map<string, CGparameter> uniforms;
	virtual void SetParameter(std::string name, float*data);
	void UpdateShaderVariables();
};
#endif