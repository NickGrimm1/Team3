#include "Renderer.h"

Renderer::Renderer(void)	{
	/*
	You're provided with a very basic vertex / fragment shader, to get you started
	with Cg, and drawing textured objects. 
	*/
	VertexShader* basicVert		= new VertexShader("/vertex.vpo");
	FragmentShader* basicFrag	= new FragmentShader("/fragment.fpo");

	this->SetCurrentShader(*basicVert,*basicFrag);

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
	this->SetCurrentShader(*currentVert,*currentFrag);

	cellGcmSetDepthTestEnable(CELL_GCM_TRUE);
	cellGcmSetDepthFunc(CELL_GCM_LESS);

	modelMatrix = Matrix4::identity(); 

	if(camera) {
		viewMatrix = camera->BuildViewMatrix();
	}
	else{
		viewMatrix = Matrix4::identity();
	}

	currentVert->UpdateShaderMatrices(modelMatrix, viewMatrix, projMatrix);

	if(root) {
		DrawNode(root);
	}

	SwapBuffers();
}
