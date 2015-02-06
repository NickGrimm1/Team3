#include "Renderer.h"



Renderer::Renderer(Window &parent) : OGLRenderer(parent)
{
	camera	= new Camera();
	root	= new SceneNode();
	quad	= Mesh::GenerateQuad();

	//Shader initialisations go here.
	basicShader		= new Shader(SHADERDIR"TexturedVertex.glsl", SHADERDIR"TexturedFragment.glsl");
	shadowShader	= new Shader(SHADERDIR"ShadowVertex.glsl", SHADERDIR"ShadowFragment.glsl");
	skyBoxShader	= new Shader(SHADERDIR"SkyBoxVertex.glsl", SHADERDIR"SkyBoxFragment.glsl");
	combineShader	= new Shader(SHADERDIR"CombineVertex.glsl", SHADERDIR"CombineFragment.glsl");
	particleShader	= new Shader(SHADERDIR"ParticleVertex.glsl", SHADERDIR"ParticleFragment.glsl", SHADERDIR"ParticleGeometry.glsl");

	if (!LoadCheck())
		return;

	//Creation of buffers.
	GenerateScreenTexture(bufferNormalTex);
	GenerateScreenTexture(lightEmissiveTex);
	GenerateScreenTexture(lightSpecularTex);

	//Generate scene depth texture buffer.
	glGenTextures(1, &bufferDepthTex);
	glBindTexture(GL_TEXTURE_2D, bufferDepthTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	//And colour texture buffer.
	for (int i = 0; i < 2; ++i) {
		glGenTextures(1, &bufferColourTex[i]);
		glBindTexture(GL_TEXTURE_2D, bufferColourTex[i]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	}

	//And shadow texture buffer.
	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &bufferFBO);		//Render the scene into this.
	glGenFramebuffers(1, &processFBO);		//PP in this.
	glGenFramebuffers(1, &shadowFBO);		//Shadow pre-render in this one.
	glGenFramebuffers(1, &pointLightFBO);	

	GLenum buffers[2];
	buffers[0] = GL_COLOR_ATTACHMENT0;
	buffers[1] = GL_COLOR_ATTACHMENT1;

	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[0], 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, bufferNormalTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, bufferDepthTex, 0);
	glDrawBuffers(2, buffers);

	//Check FBO attachment success with this command
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !bufferDepthTex || !bufferColourTex[0]) {
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, pointLightFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, lightEmissiveTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, lightSpecularTex, 0);
	glDrawBuffers(2, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !lightEmissiveTex || !lightSpecularTex)
		return;

	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTex, 0);
	glDrawBuffer(GL_NONE);

	init = true;
}

Renderer::~Renderer(void)
{
	delete camera;
	delete root;

	/*
	delete snow;
	delete rain;
	delete sandstorm;
	*/

	delete basicShader;
	delete shadowShader;
	delete bloomShader;
	delete deferredShader;
	delete blurShader;
	delete combineShader;
	delete particleShader;

	//Clear buffers
	glDeleteTextures(1, &shadowTex);
	glDeleteTextures(2, bufferColourTex);
	glDeleteTextures(1, &bufferDepthTex);
	glDeleteTextures(1, &bufferNormalTex);
	glDeleteTextures(1, &lightEmissiveTex);
	glDeleteTextures(1, &lightSpecularTex);
	glDeleteFramebuffers(1, &bufferFBO);
	glDeleteFramebuffers(1, &processFBO);
	glDeleteFramebuffers(1, &shadowFBO);
}

//Public method to initiate a draw to screen.
void Renderer::Render(float msec, LightData arg_lights)
{
	lights = arg_lights;
}

void Renderer::ToggleDebug(int arg, bool onOff)
{
	switch (onOff)
	{
	case (1):
		//Toggle wireframe
		break;
	case (2):
		//Toggle shadows.
		break;
	case (3):
		//Toggle blur.
		break;
	case (4):
		//Toggle deffered lighting.
		break;
	case (5):
		//Toggle something else.
		break;
	case (6):
		//Toggle another thing.
	default:
		break;
	}
}

/*----------Rendering pipeline-----------*/
//Draws Scene to buffer object
void Renderer::DrawScene()
{
	ShadowPass();
	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float)width / (float)height, 45.0f);
	
	//First the Skybox is drawn.
	glDepthMask(GL_FALSE);
	SetCurrentShader(skyBoxShader);
	viewMatrix = camera->BuildViewMatrix();
	UpdateShaderMatrices();
	
	quad->Draw();

	glUseProgram(0);
	glDepthMask(GL_TRUE);

	//Then the terrain and tree is drawn.
	//SetCurrentShader(sceneShader);
	//SetShaderLight(*light);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "shadowTex"), 9);
	glActiveTexture(GL_TEXTURE9);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	//viewMatrix = camera->BuildViewMatrix();
	
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "bumpTex"), 1);

	UpdateShaderMatrices();

	/*
	heightMap->Draw();

	if (lightSpheres) {
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		DrawNode(lightSphere);
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	}
	
	DrawNode(sceneNode);
	DrawWater();
	*/
	modelMatrix.ToIdentity();
	textureMatrix.ToIdentity();
	activeTex = false;
	
	glUseProgram(0);
	//if (weatherOn)
	//	DrawParticles();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Renderer::UpdateScene(float msec)
{
	if (camera) 
	{
		camera->UpdateCamera();
	}
	root->Update(msec);
}

void Renderer::ShadowPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glEnable(GL_DEPTH_TEST);

	SetCurrentShader(shadowShader);
	projMatrix		= Matrix4::Perspective(300.0f, 10000.0f, (float)width / (float)height, 45.0f);
	viewMatrix		= Matrix4::BuildViewMatrix(*lights.lightPos, Vector3(2056, 10, 2056));
	shadowMatrix	= biasMatrix*(projMatrix * viewMatrix);

	UpdateShaderMatrices();

	//heightMap->Draw();
	//DrawNode(sceneNode);
	//DrawWater();

	textureMatrix.ToIdentity();
	modelMatrix.ToIdentity();

	glUseProgram(0);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, width, height);

	//glDisable(GL_DEPTH_TEST);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
//TODO: finish this
void Renderer::DeferredLightPass()
{

}
//TODO: finish this
void Renderer::BloomPass()
{

}
//TODO: finish this
void Renderer::MotionBlurPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, processFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[ActiveTex()], 0);//1
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	SetCurrentShader(blurShader);
	projMatrix = Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
	viewMatrix.ToIdentity();
	UpdateShaderMatrices();

	glDisable(GL_DEPTH_TEST);

	glUniform2f(glGetUniformLocation(currentShader->GetProgram(), "pixelSize"), 1.0f / (float)width, 1.0f / (float)height);

	//Depending on how the motion blur works, this can be adjusted for one, two or more passes.

	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[activeTex], 0);//1
	//glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "isVertical"), 0);

	//quad->SetTexture(bufferColourTex[ActiveTex()]);//0
	//quad->Draw();

	//
	////now to swap the colour buffers, and do the second blur Pass
	//glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "isVertical"), 1);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferColourTex[activeTex], 0);//0

	//quad->SetTexture(bufferColourTex[ActiveTex()]);//1
	//quad->Draw();
	//

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
	glEnable(GL_DEPTH_TEST);
}

void Renderer::GenerateScreenTexture(GLuint &into, bool depth)
{
	glGenTextures(1, &into);
	glBindTexture(GL_TEXTURE_2D, into);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	glTexImage2D(GL_TEXTURE_2D, 0, 
		depth ? GL_DEPTH_COMPONENT24 : GL_RGBA8, 
		width , height, 0,
		depth ? GL_DEPTH_COMPONENT : GL_RGBA,
		GL_UNSIGNED_BYTE, NULL);
	
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool Renderer::LoadCheck()
{
	if (!basicShader->LinkProgram())
		return false;
	if (!shadowShader->LinkProgram())
		return false;
	if (!skyBoxShader->LinkProgram())
		return false;
	if (!combineShader->LinkProgram())
		return false;
	if (!particleShader->LinkProgram())
		return false;
}

bool Renderer::ActiveTex()
{
	activeTex = !activeTex;
}
