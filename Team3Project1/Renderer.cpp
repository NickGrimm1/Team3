#include "Renderer.h"
#include "GraphicsCommon.h"
#include "DrawableEntity3D.h"
#include "GameStateManager.h"

Renderer::Renderer(Window &parent, vector<Light*>& lightsVec, vector<SceneNode*>& sceneNodesVec, vector<DrawableEntity2D*>& overlayVec) : 
	OGLRenderer(parent), 
	lights(lightsVec), 
	sceneNodes(sceneNodesVec), 
	overlayElements(overlayVec)
{
	// Check OGLRenderer initialised ok
	if (!init) return;
	init = false;

	camera	= new Camera();
	quad	= Mesh::GenerateQuad();
	screenMesh = quad;

	//Shader initialisations go here.
	basicShader = GameStateManager::Assets()->LoadShader(this, SHADERDIR"TexturedVertex.glsl", SHADERDIR"TexturedFragment.glsl");
	sceneShader	= GameStateManager::Assets()->LoadShader(this, SHADERDIR"MainVertShader.glsl", SHADERDIR"MainFragShader.glsl");
	shadowShader = GameStateManager::Assets()->LoadShader(this, SHADERDIR"ShadowVertex.glsl", SHADERDIR"ShadowFragment.glsl");
	lightingShader = GameStateManager::Assets()->LoadShader(this, SHADERDIR"DeferredPassVertex.glsl", SHADERDIR"DeferredPassFragment.glsl");
	skyBoxShader = GameStateManager::Assets()->LoadShader(this, SHADERDIR"SkyBoxVertex.glsl", SHADERDIR"SkyBoxFragment.glsl");
	combineShader = GameStateManager::Assets()->LoadShader(this, SHADERDIR"CombineVertex.glsl", SHADERDIR"CombineFragment.glsl");
	particleShader = GameStateManager::Assets()->LoadShader(this, SHADERDIR"ParticleVertex.glsl", SHADERDIR"ParticleFragment.glsl", SHADERDIR"ParticleGeometry.glsl");

	if (!LoadCheck())
		return;

	// Setup projection matrices - gonna just keep copies of the matrices rather than keep recreating them
	perspectiveMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float) width / (float) height, 45.0f);
	orthographicMatrix = Matrix4::Orthographic(-1.0f,1.0f,(float)width, 0.0f,(float)height, 0.0f); // For HUD Elements only
	
	//Creation of buffers.
	GenerateScreenTexture(gbufferNormalTex);
	GenerateScreenTexture(lightEmissiveTex);
	GenerateScreenTexture(lightSpecularTex);

	//Generate scene depth texture buffer.
	glGenTextures(1, &gbufferDepthTex);
	glBindTexture(GL_TEXTURE_2D, gbufferDepthTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	//And colour texture buffer.
	glGenTextures(1, &gbufferColourTex);
	glBindTexture(GL_TEXTURE_2D, gbufferColourTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &gbufferFBO);			//Render the scene into this.
	glGenFramebuffers(1, &postProcessingFBO);	//PP in this.
	glGenFramebuffers(1, &shadowFBO);			//Shadow pre-render in this one.
	glGenFramebuffers(1, &deferredLightingFBO);	//Deferred lighting in this FBO.

	GLenum buffers[2];
	buffers[0] = GL_COLOR_ATTACHMENT0;
	buffers[1] = GL_COLOR_ATTACHMENT1;

	glBindFramebuffer(GL_FRAMEBUFFER, gbufferFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gbufferColourTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gbufferNormalTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, gbufferDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, gbufferDepthTex, 0);
	glDrawBuffers(2, buffers);

	//Check FBO attachment success with this command
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !gbufferDepthTex || !gbufferColourTex || !gbufferNormalTex) {
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, deferredLightingFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, lightEmissiveTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, lightSpecularTex, 0);
	glDrawBuffers(2, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !lightEmissiveTex || !lightSpecularTex)
		return;

	// Create shadow textures as requested
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowTex, 0);
	glDrawBuffer(GL_NONE);

	// TODO - Create Process FBO

	ambientLightColour = DEFAULT_AMBIENT_LIGHT_COLOUR;

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS); // Skybox sampling

	init = true;
}

Renderer::~Renderer(void)
{
	delete camera;

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
	delete sceneShader;
	delete lightingShader;

	//Clear buffers
	//glDeleteTextures(1, &shadowTex); - TODO - handle deletion of shadow textures
	glDeleteTextures(1, &gbufferColourTex);
	glDeleteTextures(1, &gbufferDepthTex);
	glDeleteTextures(1, &gbufferNormalTex);
	glDeleteTextures(1, &lightEmissiveTex);
	glDeleteTextures(1, &lightSpecularTex);
	glDeleteTextures(2, postProcessingTex);
	glDeleteTextures(1, &skyBoxTex);
	glDeleteFramebuffers(1, &gbufferFBO);
	glDeleteFramebuffers(1, &postProcessingFBO);
	glDeleteFramebuffers(1, &deferredLightingFBO);
	glDeleteFramebuffers(1, &shadowFBO);
}

//Public method to initiate a draw to screen.
void Renderer::RenderScene() {

	// Main Render
//	ShadowPass();
	DrawScene();
//	DeferredLightPass();
//	CombineBuffers();

	// Post-Processing
	//TODO

	// Draw HUD/Menu overlay
//	Draw2DOverlay(); // TODO - Draw HUD first and use stencil to optimise main render pass

	SwapBuffers();
}

void Renderer::ToggleDebug(int arg, bool onOff)
{
	switch (arg)
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
		//Toggle bloom.
		break;
	case (6):
		//Toggle particle systems.
	default:
		break;
	}
}

/*----------Rendering pipeline-----------*/
void Renderer::UpdateScene(float msec)
{
	if (camera) 
	{
		camera->UpdateCamera();
	}

	//TODO - update particle systems
}

//Draws Scene to buffer object
void Renderer::DrawScene()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	glBindFramebuffer(GL_FRAMEBUFFER, gbufferFBO);

	// Use stencil buffer to track unaltered pixels. Use to draw skybox later
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 1); // Always passes
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glClearColor(0.2f, 0.2f, 0.2f, 0.2f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	SetCurrentShader(sceneShader);
	
	// Bind Shader variables
	viewMatrix = camera->BuildViewMatrix();
	projMatrix = perspectiveMatrix;
	UpdateShaderMatrices();
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*) &camera->GetPosition());

	// Pass any light/shadow data for any lights which generate shadows into the renderer
	unsigned int shadowCount = 0;
	char buffer[20];
	for (unsigned int i = 0; i < lights.size(); i++) {
		if (lights[i]->GetShadowTexture() > 0) { // Shadow depth texture exists for light, so use
			// Calculate the view projection matrix for the light so can sample shadow map (binding to textureMatrix for the minute
			Matrix4 shadowMatrix = biasMatrix * lights[i]->GetProjectionMatrix() * lights[i]->GetViewMatrix(Vector3(0,0,0)); // TODO - handle point light shadows properly
			
			sprintf_s(buffer, 20, "shadowProjMatrix[%d]", i);
			glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), buffer), 1, false, (float*) &shadowMatrix);
			
			// Bind shadow texture
			sprintf_s(buffer, 20, "shadowTex[%d]", i);
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), buffer), SHADOW_TEXTURE_UNIT + shadowCount);
			glActiveTexture(GL_TEXTURE0 + SHADOW_TEXTURE_UNIT + shadowCount);
			glBindTexture(GL_TEXTURE_2D, lights[i]->GetShadowTexture());
			
			// Bind light data
			lights[i]->BindLight(shadowCount);

			shadowCount++;
		}
	}

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "numShadows"), shadowCount);
			
	// Draw Scene
	for (unsigned int i = 0; i < sceneNodes.size(); i++) {
		DrawableEntity3D& entity = *sceneNodes[i]->GetDrawableEntity();

		// Handle colour and bump textures
		if (entity.GetTexture() && entity.GetTexture()->GetTextureName() > 0) {
			glActiveTexture(GL_TEXTURE0 + MESH_OBJECT_COLOUR_TEXTURE_UNIT);
			glBindTexture(GL_TEXTURE_2D, entity.GetTexture()->GetTextureName());
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), MESH_OBJECT_COLOUR_TEXTURE_UNIT);	
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "useDiffuseTex"), 1);	
		}
		else
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "useDiffuseTex"), 0);	
		
		if (entity.GetBumpTexture() && entity.GetBumpTexture()->GetTextureName() > 0) {
			glActiveTexture(GL_TEXTURE0 + MESH_OBJECT_NORMAL_TEXTURE_UNIT);
			glBindTexture(GL_TEXTURE_2D, entity.GetBumpTexture()->GetTextureName());
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "normalTex"), MESH_OBJECT_NORMAL_TEXTURE_UNIT);
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "useNormalTex"), 1);	
		}
		else
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "useNormalTex"), 0);
			
		// ignore shader for the minute
		
		modelMatrix = sceneNodes[i]->GetWorldTransform() * Matrix4::Scale(entity.GetScale());
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"),	1,false, (float*)&modelMatrix);
		
		textureMatrix.ToIdentity(); // add to texture/drawableentity class

		entity.GetMesh()->Draw();
	}
	
	glUseProgram(0);

	// TODO - handle particle systems
	//if (weatherOn)
	//	DrawParticles();
	
	glDisable(GL_STENCIL_TEST); // Believe can disable here
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::ShadowPass()
{
	if (shadowFBO == 0) return; // no shadows

	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	
	SetCurrentShader(shadowShader);
	for (unsigned int i = 0; i < lights.size(); i++) {

		// Does light cast shadows (does it have a depth texture attached)?
		if (lights[i]->GetShadowTexture() <= 0) continue; // only process shadow data if light set to cast shadows

		// Attach depth texture to FBO
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, lights[i]->GetShadowTexture(), 0);
		glClear(GL_DEPTH_BUFFER_BIT);
		
		projMatrix		= lights[i]->GetProjectionMatrix();
		viewMatrix		= lights[i]->GetViewMatrix(Vector3(0,0,0)); // TODO - handle point light shadows properly
		UpdateShaderMatrices();

		// Draw Scene
		for (unsigned int j = 0; j < sceneNodes.size(); j++) {
			DrawableEntity3D& entity = *sceneNodes[j]->GetDrawableEntity();
			modelMatrix = sceneNodes[j]->GetWorldTransform() * Matrix4::Scale(entity.GetScale());
			glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"),	1,false, (float*)&modelMatrix);
			entity.GetMesh()->Draw();
		}
	
	}

	glUseProgram(0);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glViewport(0, 0, width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DeferredLightPass()
{
	SetCurrentShader(lightingShader);
	glBindFramebuffer(GL_FRAMEBUFFER, deferredLightingFBO);

	glClearColor(0,0,0,1); // Want black background for light textures
	glClear(GL_COLOR_BUFFER_BIT);

	// additive blending - pixels covered by multiple lights have the values added together
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE); 

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "depthTex"), GBUFFER_DEPTH_TEXTURE_UNIT);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "normTex"), GBUFFER_NORMALS_TEXTURE_UNIT);

	// Bind textures from G-buffer pass
	glActiveTexture(GL_TEXTURE0 + GBUFFER_DEPTH_TEXTURE_UNIT);
	glBindTexture(GL_TEXTURE_2D, gbufferDepthTex);
	glActiveTexture(GL_TEXTURE0 + GBUFFER_NORMALS_TEXTURE_UNIT);
	glBindTexture(GL_TEXTURE_2D, gbufferNormalTex);

	viewMatrix = camera->BuildViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "viewMatrix"),	1, false, (float*) &viewMatrix);
	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "projMatrix"),	1, false, (float*) &perspectiveMatrix);

	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*) &camera->GetPosition());
	glUniform2f(glGetUniformLocation(currentShader->GetProgram(), "pixelSize"), 1.0f / (float) width, 1.0f / (float) height);

	glEnable(GL_CULL_FACE);
	
	// Draw deferred scene lights
	for (unsigned int i = 0; i < lights.size(); ++i) {
		lights[i]->DrawLightDeferred(camera->GetPosition());
	}

	glDisable(GL_BLEND);

	glClearColor(0.2f, 0.2f, 0.2f, 1); // Reset clear colour to grey

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
}

void Renderer::CombineBuffers() {// merge scene render with lighting pass
	glDisable(GL_DEPTH_TEST);

	SetCurrentShader(combineShader);
	glBindFramebuffer(GL_FRAMEBUFFER, postProcessingFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, postProcessingTex[0], 0); // the final picture
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, gbufferDepthTex, 0); // Stencil buffer from the first pass render
	
	// Setup matrices
	projMatrix = Matrix4::Orthographic(-1,1,1,-1,1,-1);
	modelMatrix.ToIdentity();
	viewMatrix.ToIdentity();
	textureMatrix.ToIdentity();
	UpdateShaderMatrices();

	// Bind textures in shaders
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), GBUFFER_COLOUR_TEXTURE_UNIT);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "normalTex"), GBUFFER_NORMALS_TEXTURE_UNIT);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "emissiveTex"), DEFERRED_LIGHTS_EMISSIVE_TEXTURE_UNIT);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "specularTex"), DEFERRED_LIGHTS_SPECULAR_TEXTURE_UNIT);
	glUniform4fv(glGetUniformLocation(currentShader->GetProgram(), "ambientLight"), 1, (float*) &ambientLightColour);

	// Setup texture units
	glActiveTexture(GL_TEXTURE0 + GBUFFER_COLOUR_TEXTURE_UNIT);
	glBindTexture(GL_TEXTURE_2D , gbufferColourTex);

	glActiveTexture(GL_TEXTURE0 + GBUFFER_NORMALS_TEXTURE_UNIT);
	glBindTexture(GL_TEXTURE_2D, gbufferNormalTex); // attaching normal texture from g-buffer pass as have stored shadow map data in the w value of the normals

	glActiveTexture(GL_TEXTURE0 + DEFERRED_LIGHTS_EMISSIVE_TEXTURE_UNIT);
	glBindTexture(GL_TEXTURE_2D, lightEmissiveTex);

	glActiveTexture(GL_TEXTURE0 + DEFERRED_LIGHTS_SPECULAR_TEXTURE_UNIT);
	glBindTexture(GL_TEXTURE_2D, lightSpecularTex);

	glStencilMask(GL_FALSE); // Keep the stencil intact for drawing the skybox
	screenMesh->Draw(); // Render scene
	glStencilMask(GL_TRUE);

	DrawSkybox(); // Finally, draw the skybox
	
	glEnable(GL_DEPTH_TEST);
	glUseProgram(0);
}

void Renderer::DrawSkybox() { // Draw skybox only where screen has not previously been written to
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 1); // Stencil test should only pass if stencil buffer not already set to 1 (from scene render)
	glDepthMask(GL_FALSE); // Disable writes to the depth buffer
	
	SetCurrentShader(skyBoxShader);
	
	// Bind shader variables
	viewMatrix = camera->BuildViewMatrix();
	projMatrix = perspectiveMatrix;
	UpdateShaderMatrices();

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "cubeTex"), SKYBOX_TEXTURE_UNIT);
	glActiveTexture(GL_TEXTURE0 + SKYBOX_TEXTURE_UNIT);
	glBindTexture(GL_TEXTURE_2D , skyBoxTex);

	screenMesh->Draw();

	glUseProgram(0);
	glDisable(GL_STENCIL_TEST); // finished with the stencil for now
	glDepthMask(GL_TRUE); // Enables writes to the depth buffer
}

//TODO: finish this
void Renderer::BloomPass()
{

}
//TODO: finish this
void Renderer::MotionBlurPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, postProcessingFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, postProcessingTex[ActiveTex()], 0);//1
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

void Renderer::Draw2DOverlay() {



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
	return (basicShader != NULL		&&
			shadowShader != NULL	&&
			skyBoxShader != NULL	&&
			combineShader != NULL	&&
			particleShader != NULL	&&
			sceneShader != NULL		&&
			lightingShader != NULL);
}

bool Renderer::ActiveTex()
{
	activeTex = !activeTex;
	return activeTex;
}

GLuint Renderer::CreateTexture(const char* filename, bool enableMipMaps, bool enableAnisotropicFiltering) {
	
	unsigned int flags = false;
	if (enableMipMaps) flags |= SOIL_FLAG_MIPMAPS;
	GLuint textureObject = SOIL_load_OGL_texture(filename, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, flags);
	if (!textureObject)
		textureObject = 0; // make sure GetTexture will return an error
	return textureObject;
}

GLuint Renderer::CreateShadowTexture() {
	//Create a shadow texture buffer
	GLuint shadowTex;
	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	return shadowTex;
}

bool Renderer::DestroyTexture(GLuint textureReference) {
	glDeleteTextures(1, &textureReference);
	return true;
}
