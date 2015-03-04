#include "Renderer.h"
#include "../Framework/Shader.h"
#include "../Team3Project1/ShaderPart.h"
#include "../Team3Project1/GameStateManager.h"
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
	//s = new Shader();
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
	cellGcmSetDepthTestEnable(CELL_GCM_FALSE);
	cellGcmSetDepthFunc(CELL_GCM_LESS);
	cellGcmSetCullFaceEnable(CELL_GCM_FALSE);
	cellGcmSetFrontFace(CELL_GCM_CW);

	modelMatrix = Matrix4::identity(); 

	if(camera) {
		T3Matrix4 m = camera->BuildViewMatrix();
		std::cout << "Renderer: View Matrix (T3): " << m << std::endl;

		viewMatrix = Matrix4::identity();
		for (int x = 0; x < 4; ++x)
			for (int y = 0; y < 4; ++y)
				viewMatrix.setElem(x, y, m.values[y + x * 4]);


		/*std::cout << "Renderer: View Matrix (SCE): " << std::endl;
		for (int x = 0; x < 4; ++x)
		{
			for (int y = 0; y < 4; ++y)
			{
				std::cout << viewMatrix.getElem(x,y) << ",";
			}
			std::cout << std::endl;
		}*/
	}
	else{
		viewMatrix = Matrix4::identity();
	}

	shader->GetVertex()->UpdateShaderMatrices(modelMatrix, viewMatrix, projMatrix);
	if(root) {
		DrawNode(root);
	}

	SwapBuffers();
	//cout << "Frame Drawn"
}

bool Renderer::LoadShaders()
{
	basicShader = GameStateManager::Assets()->LoadShader(this, "../Shaders/vertex.vpo","../Shaders/fragment.fpo");
	return true;
}
void Renderer::UnloadShaders()
{
	GameStateManager::Assets()->UnloadShader(this,  "../Shaders/vertex.vpo","../Shaders/fragment.fpo");
}
bool Renderer::LoadAssets()
{
	circleMesh = GameStateManager::Assets()->LoadCircle(this, 20);				  // Circle for spotlight rendering
	quadMesh   = GameStateManager::Assets()->LoadQuad(this); 
	sphereMesh = GameStateManager::Assets()->LoadMesh(this, MESHDIR"sphere.obj"); // Sphere for point light rendering
	coneMesh   = GameStateManager::Assets()->LoadCone(this, 20);				  // Cone for spotlight rendering
	
	if (!sphereMesh || !coneMesh || !circleMesh || !quadMesh) {
		cout << "Renderer::LoadAssets() - unable to load rendering assets";
		return false;
	}
	
	return true;
}
void Renderer::UnloadAssets()
{
	GameStateManager::Assets()->UnloadCircle(this, 20);				   // Circle for spotlight rendering
	GameStateManager::Assets()->UnloadQuad(this);					   // Quad for rendering textures to screen
	GameStateManager::Assets()->UnloadMesh(this, MESHDIR"sphere.obj"); // Sphere for point light rendering
	GameStateManager::Assets()->UnloadCone(this, 20);				   // Cone for spotlight rendering
	
}

unsigned int Renderer::CreateShadowCube()
{
	return 0;//look into this later
}
unsigned int Renderer::CreateShadowTexture()
{
	return 0;//look into this later
}