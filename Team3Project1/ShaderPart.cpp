#include "ShaderPart.h"
#include <fstream>
#include "../Framework/Shader.h"

ShaderPart* ShaderPart::LoadShaderFile(string filename, ShaderType::Type type)
{
#if WINDOWS_BUILD
	ifstream file;
	string temp;
	string into;
#if DEBUG
	printf << "Loading shader text from " << filename << endl << endl;
#endif
	file.open(filename.c_str());
	if(!file.is_open())
	{
#if DEBUG
		printf << "File does not exist!" << endl;
#endif
		return NULL;
	}

	while(!file.eof()){
		getline(file,temp);
		into += temp + "\n";
	}
	file.close();
#if DEBUG
	cout << "Loaded shader text!" << endl << endl;

	cout << "Compiling Shader..." << endl;
#endif

	GLuint shader = glCreateShader(type);
	const char *chars = into.c_str();
	glShaderSource(shader, 1, &chars, NULL);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)	
	{
#if DEBUG
		cout << "Compiling failed!" << endl;
		char error[512];
		glGetInfoLogARB(shader, sizeof(error), NULL, error);
		cout << error;
#endif
		return NULL;
	}
#if DEBUG
	cout << "Compiling success!" << endl << endl;
#endif
	return new ShaderPart(shader);
#endif
}