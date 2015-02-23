#include "Shader.h"
#include "../Team3Project1/ShaderPart.h"

#if WINDOWS_BUILD
Shader::Shader()	
{
	loadFailed = false;
	program	= glCreateProgram();
}

Shader::~Shader(void)	
{
	glDetachShader(program, vertexShader->shader);
	glDetachShader(program, fragmentShader->shader);
	if (geometryShader) {
		glDetachShader(program, geometryShader->shader);
	}
	glDeleteProgram(program);
}

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

void	Shader::SetDefaultAttributes()	
{
	glBindAttribLocation(program, 0, "position");
	glBindAttribLocation(program, 1, "normal");
	glBindAttribLocation(program, 2, "colour");
	glBindAttribLocation(program, 3, "texCoord");
	glBindAttribLocation(program, 4, "tangent");

	//glBindAttribLocation(program, MAX_BUFFER+1,  "transformIndex");
}
#endif