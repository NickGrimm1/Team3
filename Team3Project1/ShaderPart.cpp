#include "ShaderPart.h"
#include <fstream>
#include "../Framework/Shader.h"
#include <stdio.h>
#include "Vertex.h"
#if PS3_BUILD
#include <iostream>
#include "../Team3Project1/Mesh.h"
#endif

ShaderPart::ShaderPart(std::string raw, ShaderType::Type type) : type(type)
{
	std::cout <<"shader part started" << std::endl;
#if WINDOWS_BUILD
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
#endif
#if PS3_BUILD
	std::cout <<"ShaderPart: got to first ps3 block" << std::endl;
	
	string name = SYS_APP_HOME + raw;
	std::cout <<"ShaderPart:made a string" << std::endl;
	unsigned int dataSize = 0;
	std::cout <<"ShaderPart: made an int" << std::endl; 
	char* data = NULL;
	std::cout <<"ShaderPart: initialised some variables" << std::endl;
	//Open the file
	std::ifstream	file(name.c_str(),std::ios::in|std::ios::binary);
	std::cout <<"ShaderPart: opened the shader file" << std::endl;
	//Oh no!
	if(!file) 
	{
//#if DEBUG
		std::cout << "LoadBinaryShader: Can't find file: " << name << std::endl;
		return;
//#endif
	}
	//This is the slightly messy default way to determinimume the size
	//of a file (seek to the end of it, grab the read position, seek
	//back to the start)
	file.seekg(0, std::ios::end);
	dataSize = file.tellg();
	file.seekg(0, std::ios::beg);

	//Allocate some graphics memory for the shader.
	//data = (char*)GCMRenderer::localMemoryAlign(64,dataSize);

	data = (char*)malloc(dataSize);
	//Load the shader straight into graphics memory. What type of data transfer
	//is this? How fast will it be?
	file.read(data,dataSize);
	file.close();	//Done with the data, close the file.

	//Now we have the binary data, we can cast it to a CGprogram
	//data structure. 
	program		= (CGprogram)(void*)data;

	//And initialise it. This will make sure the binary data is in the correct
	//format etc. 
	cellGcmCgInitProgram(program);

	//Lastly, we need to figure out where the ucode of the shader is located
	//and save the offset of it. It's really only fragment shaders that need
	//to know this, but just in case there's some obscure GCM function that
	//uses it, we'll save it even for vertex shaders.
	unsigned int ucodeSize;
	void *ucodePtr;

	cellGcmCgGetUCode(program, &ucodePtr, &ucodeSize);
	ucode = GCMRenderer::localMemoryAlign(64,ucodeSize);
	memcpy(ucode, ucodePtr, ucodeSize);
	cellGcmAddressToOffset(ucode, &offset);

	if (type == ShaderType::VERTEX)
	{
		for(int i = 0; i < VertexAttributes::MAX; ++i)
			attributes[i] = 0;
		
		SetDefaultAttributes();
	}
	else if (type == ShaderType::FRAGMENT)
		UpdateShaderVariables();
#endif
}
#if WINDOWS_BUILD
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
#if PS3_BUILD
/*
Sets a uniform value. In OpenGL we had loads of glUniformx type
functions, for the different types of data. In GCM we have a single
function, which takes a CGparameter and a pointer to some float data,
and the function works out the rest. Simpler, but more likely to go
wrong when incorrect data is sent to it. 
*/
void	ShaderPart::SetParameter(std::string name, float*data) 
{
	//std::cout << "Shader Part: Getting parameter: " << name << std::endl;
	CGparameter p = GetParameter(name);

	//DON'T try to set a non-existent parameter. GCM will instantly
	//fall over.
	if(p) {	
		if(type == ShaderType::VERTEX)
		{
			cellGcmSetVertexProgramParameter(p, data);
			std::cout << "ShaderPart(Vertex): Set Parameter: " <<  name << std::endl;
		}
		else
		{
			cellGcmSetFragmentProgramParameter(program, p, data, offset);
			std::cout << "ShaderPart(Fragment): Set Parameter: " <<  name << std::endl;
		}
	}
}

/*
Sets a matrix. While there's only one uniform setting function, we still
need a special case. In GCM, matrices are transposed to how they are in
Sony's own matrix code. So, we wrap matrix uniform setting around a 
transpose function, so it's less likely you'll accidentally set your 
matrix wrong
*/
void ShaderPart::SetParameter(std::string name, Matrix4 &totranpose) {
	std::cout << "Transposing: " << name << std::endl;
	Matrix4 tempMatrix = transpose(totranpose);
	//std::cout << "Shader Part: Transposed Matrix (SCE): " << std::endl;
	//	for (int x = 0; x < 4; ++x)
	//	{
	//		for (int y = 0; y < 4; ++y)
	//		{
	//			std::cout << tempMatrix.getElem(x,y) << ",";
	//		}
	//		std::cout << std::endl;
	//	}
	//	float* m = (float*)&tempMatrix;
	//	std::cout << "Shader Part: Setting " << name << " " << &tempMatrix << std::endl;
	SetParameter(name, (float*)&tempMatrix);
	std::cout << "Shader Part: " << name << " Set" << std::endl;
}

/*
Just as with OpenGL, we must bind our vertex shader inputs to some vertex
attributes. This function essentially does the same as the OpenGL framework
version, but is a little bit more involved
*/
void ShaderPart::SetDefaultAttributes() {
	/*
	Get the handles to our vertex attributes
	*/
	CGparameter position_param	= cellGcmCgGetNamedParameter(program, "position");
	CGparameter normal_param	= cellGcmCgGetNamedParameter(program, "normal");
	CGparameter tangent_param	= cellGcmCgGetNamedParameter(program, "tangent");
	CGparameter colour_param	= cellGcmCgGetNamedParameter(program, "color");
	CGparameter tex_param		= cellGcmCgGetNamedParameter(program, "texCoord");
	
	//And now save out the actual resources (the name of the input registers they'll use)
	attributes[VertexAttributes::POSITION]	= (position_param == 0) ? -1 : cellGcmCgGetParameterResource(program, position_param) - CG_ATTR0;
	attributes[VertexAttributes::COLOUR]	= (colour_param	  == 0) ? -1 : cellGcmCgGetParameterResource(program, colour_param)	  - CG_ATTR0;
	attributes[VertexAttributes::TEXCOORD]  = (tex_param	  == 0) ? -1 : cellGcmCgGetParameterResource(program, tex_param)	  - CG_ATTR0;
	attributes[VertexAttributes::NORMAL]	= (normal_param	  == 0) ? -1 : cellGcmCgGetParameterResource(program, normal_param)	  - CG_ATTR0;
	attributes[VertexAttributes::TANGENT]	= (tangent_param  == 0) ? -1 : cellGcmCgGetParameterResource(program, tangent_param)  - CG_ATTR0;
}

//void ShaderPart::SetParameter(std::string name, float*data) 
//{
//	CGparameter p = GetParameter(name);
//
//	//DON'T try to set a non-existent parameter. GCM will instantly
//	//fall over.
//	if(p) {	
//		cellGcmSetFragmentProgramParameter(program, p, data, offset);
//	}
//}

/*
When uniforms are changed in a fragment shader, we must tell GCM to reload
the instruction cache of the shader, so call this after you've set anything
on the fragment shader. You might want to overload SetParameter to always
call the gcm function, but it's more efficient to call it only once per 'set'
of uniform changes required.
*/
void ShaderPart::UpdateShaderVariables() 
{
	cellGcmSetUpdateFragmentProgramParameter(offset);
}
/*
Sets the shaders matrices to the passed in values. Handy to call at the start of
a frame, or when using a shader for the first time. It's more efficient to use
SetParameter directly for setting just a single matrix, though. 
*/
//std::ostream& operator<<(std::ostream& o, const Matrix4& m)
//{
//
//	o << "PS3_Mat4(";
//	for (int i = 0; i < 4; ++i)
//	{
//		o << "\t\t" << m[i][0] << "," << m[i][1] << "," << m[i][2] << "," << m[i][3] << "\n";
//	}
//	o << ");\n\n";
//	return o;
//}

void	ShaderPart::UpdateShaderMatrices(Matrix4 &model,Matrix4 &view, Matrix4 &proj) {

	//std::cout << "####MODEL MTX:" << model << "\n#####VIEW MTX: " << view << "\n####PROJ MTX: " << proj;

	SetParameter("modelMat", model);
	SetParameter("viewMat", view);
	SetParameter("projMat", proj);
}
#endif
#if PS3_BUILD
CGprogram ShaderPart::GetProgram(){std::cout<<"ShaderPart: Program: " << &program << program << std::endl; return program;}

unsigned int ShaderPart::GetOffset(){return offset;}

int ShaderPart::GetAttributeIndex(VertexAttributes::Attributes attribute){return attributes[attribute];}

CGparameter ShaderPart::GetParameter(string target)
{
	std::map<std::string, CGparameter>::iterator i = uniforms.find(target);
	if(i != uniforms.end()) 
	{ 
		// if its in the map , return it!
		std::cout << "ShaderPart::GetParameterL: Returning " << target << " from map." << std::endl;
		return i->second;
	}

	CGparameter p = cellGcmCgGetNamedParameter(program, target.c_str());
	if(!p) 
	{
		std::cout << "Can't find named parameter:" << target << std::endl;
	}

	uniforms.insert (std::make_pair(target, p));
	std::cout << "ShaderPart::GetParameterL: Returning " << target << " from named." << std::endl;
	return p;
}

void* ShaderPart::GetuCode()
{
	return ucode;
}
#endif
