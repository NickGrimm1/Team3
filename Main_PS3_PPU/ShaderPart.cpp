#if PS3_BUILD
#include "ShaderPart.h"
#include <fstream>
#include <iostream>
#include "../Framework/Shader.h"
#include "../Team3Project1/Mesh.h"

ShaderPart::ShaderPart(string raw, ShaderType::Type type) 
{
	name = SYS_APP_HOME + name;

	unsigned int dataSize = 0;
	char* data = NULL;

	//Open the file
	std::ifstream	file(name.c_str(),std::ios::in|std::ios::binary);
	//Oh no!
	if(!file) 
	{
#if DEBUG
		std::cout << "LoadBinaryShader: Can't find file: " << name << std::endl;
		return;
#endif
	}
	//This is the slightly messy default way to determine the size
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
}

/*
Sets a uniform value. In OpenGL we had loads of glUniformx type
functions, for the different types of data. In GCM we have a single
function, which takes a CGparameter and a pointer to some float data,
and the function works out the rest. Simpler, but more likely to go
wrong when incorrect data is sent to it. 
*/
void	ShaderPart::SetParameter(std::string name, float*data) {
	CGparameter p = GetParameter(name);

	//DON'T try to set a non-existent parameter. GCM will instantly
	//fall over.
	if(p) {	
		cellGcmSetVertexProgramParameter(p, data);
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
	CGparameter tangent_param	= cellGcmCgGetNamedParameter(program, "tangent");
	CGparameter colour_param	= cellGcmCgGetNamedParameter(program, "color");
	CGparameter tex_param		= cellGcmCgGetNamedParameter(program, "texCoord");

	//And now save out the actual resources (the name of the input registers they'll use)
	attributes[VertexAttributes::POSITION]	= cellGcmCgGetParameterResource(program, position_param) - CG_ATTR0;

	if(colour_param)	
		attributes[VertexAttributes::COLOUR] = cellGcmCgGetParameterResource(program, colour_param) - CG_ATTR0;

	if(tex_param) 
		attributes[VertexAttributes::TEXCOORD] = cellGcmCgGetParameterResource(program, tex_param) - CG_ATTR0;

	if(normal_param) 
		attributes[VertexAttributes::NORMAL] = cellGcmCgGetParameterResource(program, normal_param) - CG_ATTR0;

	if(tangent_param) 
		attributes[VertexAttributes::TANGENT] = cellGcmCgGetParameterResource(program, tangent_param) - CG_ATTR0;
}

void ShaderPart::SetParameter(std::string name, float*data) 
{
	CGparameter p = GetParameter(name);

	//DON'T try to set a non-existent parameter. GCM will instantly
	//fall over.
	if(p) {	
		cellGcmSetFragmentProgramParameter(program, p, data, offset);
	}
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
#endif