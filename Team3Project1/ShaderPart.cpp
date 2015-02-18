#include "ShaderPart.h"
#include <fstream>
#include "../Framework/Shader.h"
#include <stdio.h>

#if WINDOWS_BUILD
ShaderPart::ShaderPart(string raw, ShaderType::Type type) 
{
	shader = glCreateShader(type);
	const char *chars = raw.c_str();
	glShaderSource(shader, 1, &chars, NULL);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)	
	{
#if DEBUG
		printf("Compiling failed!\n");
		char error[512];
		glGetInfoLogARB(shader, sizeof(error), NULL, error);
		cout << error;
#endif
		return;
	}
#if DEBUG
	printf("Compiling success!\n\n");
#endif
}

string ShaderPart::LoadShaderFile(string filename)
{

	ifstream file;
	string temp;
	string into;
#if DEBUG
	printf("Loading shader text from ");
	printf(filename.c_str());
	printf("\n\n");
#endif
	file.open(filename.c_str());
	if(!file.is_open())
	{
#if DEBUG
	printf("File does not exist!\n");
#endif
		return NULL;
	}

	while(!file.eof()){
		getline(file,temp);
		into += temp + "\n";
	}
	file.close();
#if DEBUG
	printf("Loaded shader text!\n");

	printf("Compiling Shader...\n");
#endif

	return into;
}
#endif