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
#if WINDOWS_BUILD
	shader = glCreateShader(type);
	const char *chars = raw.c_str();
	glShaderSource(shader, 1, &chars, NULL);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (status == GL_FALSE)	
	{
		return;
	}
#endif
#if PS3_BUILD
	string name = SYS_APP_HOME + raw;
	unsigned int dataSize = 0;
	char* data = NULL;
	//Open the file
	std::ifstream	file(name.c_str(),std::ios::in|std::ios::binary);
	//Oh no!
	if(!file) 
	{
		return;
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
	file.open(filename.c_str());
	if(!file.is_open())
	{
		return NULL;
	}

	while(!file.eof()){
		getline(file,temp);
		into += temp + "\n";
	}
	file.close();

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
	CGparameter p = GetParameter(name);

	//DON'T try to set a non-existent parameter. GCM will instantly
	//fall over.
	if(p) {	
		if(type == ShaderType::VERTEX)
		{
			cellGcmSetVertexProgramParameter(p, data);
		}
		else
		{
			cellGcmSetFragmentProgramParameter(program, p, data, offset);
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
	Matrix4 tempMatrix = transpose(totranpose);	
	SetParameter(name, (float*)&tempMatrix);
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
	CGparameter colour_param	= cellGcmCgGetNamedParameter(program, "color");
	CGparameter tex_param		= cellGcmCgGetNamedParameter(program, "texCoord");
	CGparameter tangent_param	= cellGcmCgGetNamedParameter(program, "tangent");
	
	//And now save out the actual resources (the name of the input registers they'll use)
	attributes[VertexAttributes::POSITION]	= (position_param == 0) ? -1 : cellGcmCgGetParameterResource(program, position_param) - CG_ATTR0;
	attributes[VertexAttributes::NORMAL]	= (normal_param	  == 0) ? -1 : cellGcmCgGetParameterResource(program, normal_param)	  - CG_ATTR0;
	attributes[VertexAttributes::COLOUR]	= (colour_param	  == 0) ? -1 : cellGcmCgGetParameterResource(program, colour_param)	  - CG_ATTR0;
	attributes[VertexAttributes::TEXCOORD]  = (tex_param	  == 0) ? -1 : cellGcmCgGetParameterResource(program, tex_param)	  - CG_ATTR0;
	attributes[VertexAttributes::TANGENT]	= (tangent_param  == 0) ? -1 : cellGcmCgGetParameterResource(program, tangent_param)  - CG_ATTR0;
}

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

void	ShaderPart::UpdateShaderMatrices(Matrix4 &model,Matrix4 &view, Matrix4 &proj)
{
	SetParameter("modelMatrix", model);
	SetParameter("viewMatrix", view);
	SetParameter("projMatrix", proj);
}
#endif
#if PS3_BUILD
CGprogram ShaderPart::GetProgram(){ return program;}

unsigned int ShaderPart::GetOffset(){return offset;}

int ShaderPart::GetAttributeIndex(VertexAttributes::Attributes attribute){return attributes[attribute];}

CGparameter ShaderPart::GetParameter(string target)
{
	std::map<std::string, CGparameter>::iterator i = uniforms.find(target);
	if(i != uniforms.end()) 
	{ 
		// if its in the map , return it!
		return i->second;
	}

	CGparameter p = cellGcmCgGetNamedParameter(program, target.c_str());
	uniforms.insert (std::make_pair(target, p));
	return p;
}

void* ShaderPart::GetuCode()
{
	return ucode;
}
#endif
