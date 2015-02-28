#include "Renderer.h"
#include "../Framework/Shader.h"
#include "../Team3Project1/ShaderPart.h"

Renderer::Renderer(void)	{
	/*
	You're provided with a very basic vertex / fragment shader, to get you started
	with Cg, and drawing textured objects. 
	*/
	std::cout << "begin renderer constructor"<<std::endl;
	//get to shader part and fall over, gg, next map pls ps3.
	ShaderPart* basicVert		= new ShaderPart("/vertex.vpo", ShaderType::VERTEX);
	
	std::cout <<"basic vert did something" << std::endl;
	ShaderPart* basicFrag		= new ShaderPart("/fragment.fpo", ShaderType::FRAGMENT);
	std::cout <<"basic frag did something" << std::endl;
	//Shader* s = new Shader();
	s = new Shader();
	std::cout <<"shader did something" << std::endl;
	shader->SetVertex(basicVert);
	shader->SetFragment(basicFrag);
	this->SetCurrentShader(shader);

	/*
	Projection matrix...0.7853982 is 45 degrees in radians.
	*/
	projMatrix	= Matrix4::perspective(0.7853982, screenRatio, 1.0f, 10000.0f);	//CHANGED TO THIS!!
}

Renderer::~Renderer(void)	{
}

/*
Main rendering function. Note how it's essentially the same as the
ones you were writing in OpenGL! We start by clearing the buffer,
render some stuff, then swap the buffers. All that's different is
some slightly different matrix access.

*/
void Renderer::RenderScene() {
	std::cout << "RenderScene!" << std::endl;
	SetViewport();
	ClearBuffer();
	//this->SetCurrentShader(shader);
	this->SetCurrentShader(shader);
	cellGcmSetDepthTestEnable(CELL_GCM_TRUE);
	cellGcmSetDepthFunc(CELL_GCM_LESS);

	modelMatrix = Matrix4::identity(); 

	if(camera) {
		T3Matrix4 m = camera->BuildViewMatrix();
		viewMatrix = Matrix4::identity();
		for (int x = 0; x < 4; ++x)
			for (int y = 0; y < 4; ++y)
				viewMatrix.setElem(x, y, m.values[y + x * 4]);
	}
	else{
		viewMatrix = Matrix4::identity();
	}

	//shader->GetVertex()->UpdateShaderMatrices(modelMatrix, viewMatrix, projMatrix);
	shader->GetVertex()->UpdateShaderMatrices(modelMatrix, viewMatrix, projMatrix);
	if(root) {
		DrawNode(root);
	}

	SwapBuffers();
	//cout << "Frame Drawn"
}
