#include "Renderer.h"
#include "../Framework/Shader.h"
#include "../Team3Project1/ShaderPart.h"

Renderer::Renderer(void)	{
	/*
	You're provided with a very basic vertex / fragment shader, to get you started
	with Cg, and drawing textured objects. 
	*/
	ShaderPart* basicVert		= new ShaderPart("/vertex.vpo", ShaderType::VERTEX);
	ShaderPart* basicFrag		= new ShaderPart("/fragment.fpo", ShaderType::FRAGMENT);
	Shader* s = new Shader();
	s->SetVertex(basicVert);
	s->SetFragment(basicFrag);
	this->SetCurrentShader(s);

	/*
	Projection matrix...0.7853982 is 45 degrees in radians.
	*/
	projMatrix	= Matrix4::perspective(0.7853982, screenRatio, 1.0f, 20000.0f);	//CHANGED TO THIS!!
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
	this->SetCurrentShader(shader);

	cellGcmSetDepthTestEnable(CELL_GCM_TRUE);
	cellGcmSetDepthFunc(CELL_GCM_LESS);

	modelMatrix = Matrix4::identity(); 

	if(camera) {
		T3Matrix4 m = camera->BuildViewMatrix();
		viewMatrix = Matrix4::identity();
		for (int x = 0; x < 4; ++x)
			for (int y = 0; y < 4; ++y)
				viewMatrix.setElem(y, x, m.values[y + x * 4]);
	}
	else{
		viewMatrix = Matrix4::identity();
	}

	shader->GetVertex()->UpdateShaderMatrices(modelMatrix, viewMatrix, projMatrix);
	
	if(root) {
		DrawNode(root);
	}

	SwapBuffers();
}
