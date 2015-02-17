#include "Shader.h"

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

ShaderPart* Shader::LoadShaderFile(string filename, ShaderType::Type type)
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