/******************************************************************************
Class:Shader
Implements:
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>
Description:VERY simple class to encapsulate GLSL shader loading, linking,
and binding. Useful additions to this class would be overloaded functions to
replace the glUniformxx functions in external code, and possibly a map to store
uniform names and their resulting bindings. 

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////

#pragma once
#if WINDOWS_BUILD
#include "OGLRenderer.h"
#endif
#include "../Team3Project1/ShaderPart.h"

namespace ShaderType
{
	enum Type
	{
		VERTEX = 0,
		FRAGMENT = 1,
		GEOMETRY = 2,
	};
}

using namespace std;
class Shader	
{
public:
	Shader();
	~Shader(void);

#if WINDOWS_BUILD
	GLuint  GetProgram() { return program; }
	bool	LinkProgram();
	void SetGeometry(ShaderPart* value) { geometryShader = value; }
	ShaderPart* GetGeometry() { return geometryShader; }
#endif
	void SetVertex(ShaderPart* value) { vertexShader = value; }
	ShaderPart* GetVertex() { return vertexShader; }
	void SetFragment(ShaderPart* value) { fragmentShader = value; }
	ShaderPart* GetFragment() { return fragmentShader; }
	static ShaderPart* LoadShaderFile(string filename, ShaderType::Type type);
protected:
#if WINDOWS_BUILD
	GLuint program;
	ShaderPart* geometryShader;
	void SetDefaultAttributes();
#endif
	ShaderPart* vertexShader;
	ShaderPart* fragmentShader;

	bool loadFailed;
};

