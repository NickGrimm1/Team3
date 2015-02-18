#include "Shader.h"
#include "../Team3Project1/ShaderPart.h"

Shader::Shader()	
{
#if WINDOWS_BUILD
	program	= glCreateProgram();
#endif	
}

Shader::~Shader(void)	
{
#if WINDOWS_BUILD
	glDetachShader(program, vertexShader->shader);
	glDetachShader(program, fragmentShader->shader);
	glDetachShader(program, geometryShader->shader);
	glDeleteProgram(program);
#endif	
}

#if WINDOWS_BUILD
bool Shader::LinkProgram()	
{
	glAttachShader(program, vertexShader->shader);
	glAttachShader(program, fragmentShader->shader);
	if (geometryShader != NULL)
		glAttachShader(program, geometryShader->shader);
	SetDefaultAttributes();

	if(loadFailed) 
	{
		return false;
	}
	glLinkProgram(program); 

	GLint code;
	glGetProgramiv(program, GL_LINK_STATUS, &code);

	if (code == GL_FALSE)	
	{
#if DEBUG
		cout << "Linking failed!" << endl;
		char error[512];
		glGetInfoLogARB(program, sizeof(error), NULL, error);
		cout << error;
#endif
		loadFailed = true;
	}

	return code == GL_TRUE ?  true : false;
}

void	Shader::SetDefaultAttributes()	{
	glBindAttribLocation(program, VERTEX_BUFFER,  "position");
	glBindAttribLocation(program, COLOUR_BUFFER,  "colour");
	glBindAttribLocation(program, NORMAL_BUFFER,  "normal");
	glBindAttribLocation(program, TANGENT_BUFFER, "tangent");
	glBindAttribLocation(program, TEXTURE_BUFFER, "texCoord");

	glBindAttribLocation(program, MAX_BUFFER+1,  "transformIndex");
}
#endif