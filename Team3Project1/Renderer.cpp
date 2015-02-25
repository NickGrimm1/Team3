#include "Renderer.h"
#include "GraphicsCommon.h"
#include "DrawableEntity3D.h"
#include "GameStateManager.h"
#include "TextMesh.h"

// Structures required for point light shadows
struct CameraDirection {
	GLenum cubeFace;
	T3Vector3 target;
	T3Vector3 up;
};

CameraDirection faces[6] = {
	{GL_TEXTURE_CUBE_MAP_POSITIVE_X, T3Vector3(1.0f, 0.0f, 0.0f), T3Vector3(0.0f, -1.0f, 0.0f)},
	{GL_TEXTURE_CUBE_MAP_NEGATIVE_X, T3Vector3(-1.0f, 0.0f, 0.0f), T3Vector3(0.0f, -1.0f, 0.0f)},
	{GL_TEXTURE_CUBE_MAP_POSITIVE_Y, T3Vector3(0.0f, 1.0f, 0.0f), T3Vector3(0.0f, 0.0f, 1.0f)},
	{GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, T3Vector3(0.0f, -1.0f, 0.0f), T3Vector3(0.0f, 0.0f, -1.0f)},
	{GL_TEXTURE_CUBE_MAP_POSITIVE_Z, T3Vector3(0.0f, 0.0f, 1.0f), T3Vector3(0.0f, -1.0f, 0.0f)},
	{GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, T3Vector3(0.0f, 0.0f, -1.0f), T3Vector3(0.0f, -1.0f, 0.0f)}
};

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

	// Setup projection matrices - gonna just keep copies of the matrices rather than keep recreating them
	perspectiveMatrix = T3Matrix4::Perspective(1.0f, 10000.0f, (float) width / (float) height, 45.0f);
	orthographicMatrix = T3Matrix4::Orthographic(-1,1,1,-1,1,-1); // for drawing full screen quads
	hudMatrix = T3Matrix4::Orthographic(-1.0f,1.0f,(float)width, 0.0f,(float)height, 0.0f); // For HUD Elements only
	
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

	//And velocity buffer
	glGenTextures(1, &gbufferVelocity);
	glBindTexture(GL_TEXTURE_2D, gbufferVelocity);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, width, height, 0, GL_RG, GL_HALF_FLOAT, 0);

	samples[0] = 1.5f;
	samples[1] = 2.0f;
	samples[2] = 3.0f;
	//samples[3] = 4.0f;

	int z = 0;
	for (int i = 0; i < SAMPLENUM * 2; i += 2) {
		glGenTextures(1, &downSampleTex[i]);
		glBindTexture(GL_TEXTURE_2D, downSampleTex[i]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (int)(width / samples[z]), (int)(height / samples[z]), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		glGenTextures(1, &downSampleTex[i + 1]);
		glBindTexture(GL_TEXTURE_2D, downSampleTex[i + 1]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (int)(width / samples[z]), (int)(height / samples[z]), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		
		++z;
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &gbufferFBO);			//Render the scene into this.
	glGenFramebuffers(1, &postProcessingFBO);	//PP in this.
	glGenFramebuffers(1, &shadowFBO);			//Shadow pre-render in this one.
	glGenFramebuffers(1, &deferredLightingFBO);	//Deferred lighting in this FBO.

	GLenum buffers[3];
	buffers[0] = GL_COLOR_ATTACHMENT0;
	buffers[1] = GL_COLOR_ATTACHMENT1;
	buffers[2] = GL_COLOR_ATTACHMENT2;

	glBindFramebuffer(GL_FRAMEBUFFER, gbufferFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gbufferColourTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gbufferNormalTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, gbufferDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, gbufferDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gbufferVelocity, 0);
	glDrawBuffers(3, buffers);

	//Check FBO attachment success with this command
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !gbufferDepthTex || !gbufferColourTex || !gbufferNormalTex || !gbufferVelocity) {
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, deferredLightingFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, lightEmissiveTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, lightSpecularTex, 0);
	glDrawBuffers(2, buffers);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE || !lightEmissiveTex || !lightSpecularTex)
		return;

	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	
	// Will only bind when required
	glGenTextures(1, &shadowDepthTex);
	glBindTexture(GL_TEXTURE_2D, shadowDepthTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, postProcessingFBO);
	glGenTextures(3, postProcessingTex);
	for (unsigned int i = 0; i < 3; i++) {
		glBindTexture(GL_TEXTURE_2D, postProcessingTex[i]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	}

	ambientLightColour = DEFAULT_AMBIENT_LIGHT_COLOUR;

	drawDeferredLights = false;

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS); // cube sampling

	glClearColor(0, 0, 0, 1);
	SwapBuffers();
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	wglMakeCurrent(deviceContext, NULL);
	init = true;
}

bool Renderer::LoadShaders()
{
	//Shader initialisations go here.
	basicShader		 = GameStateManager::Assets()->LoadShader(this, SHADERDIR"TexturedVertex.glsl", SHADERDIR"TexturedFragment.glsl");
	sceneShader		 = GameStateManager::Assets()->LoadShader(this, SHADERDIR"MainVertShader.glsl", SHADERDIR"MainFragShader.glsl");
	shadowShader	 = GameStateManager::Assets()->LoadShader(this, SHADERDIR"ShadowVertex.glsl", SHADERDIR"ShadowFragment.glsl");
	lightingShader	 = GameStateManager::Assets()->LoadShader(this, SHADERDIR"DeferredPassVertex.glsl", SHADERDIR"DeferredPassFragment.glsl");
	skyBoxShader	 = GameStateManager::Assets()->LoadShader(this, SHADERDIR"SkyBoxVertex.glsl", SHADERDIR"SkyBoxFragment.glsl");
	combineShader	 = GameStateManager::Assets()->LoadShader(this, SHADERDIR"CombineVertex.glsl", SHADERDIR"CombineFragment.glsl");
	particleShader	 = GameStateManager::Assets()->LoadShader(this, SHADERDIR"ParticleVertex.glsl", SHADERDIR"ParticleFragment.glsl", SHADERDIR"ParticleGeometry.glsl");
	brightPassShader = GameStateManager::Assets()->LoadShader(this, SHADERDIR"TexturedVertex.glsl", SHADERDIR"BrightPassFragment.glsl");
	bloomCombShader	 = GameStateManager::Assets()->LoadShader(this, SHADERDIR"TexturedVertex.glsl", SHADERDIR"BloomCombFragment.glsl");
	gaussianShader	 = GameStateManager::Assets()->LoadShader(this, SHADERDIR"TexturedVertex.glsl", SHADERDIR"GaussianFragment.glsl");
	downSampleShader = GameStateManager::Assets()->LoadShader(this, SHADERDIR"TexturedVertex.glsl", SHADERDIR"DownSampleFragment.glsl");
	bloomFinalShader = GameStateManager::Assets()->LoadShader(this, SHADERDIR"TexturedVertex.glsl", SHADERDIR"BloomFinalFragment.glsl");
	velocityShader	 = GameStateManager::Assets()->LoadShader(this, SHADERDIR"VelocityVertex.glsl", SHADERDIR"VelocityFragment.glsl");
	motionBlurShader = GameStateManager::Assets()->LoadShader(this, SHADERDIR"TexturedVertex.glsl", SHADERDIR"BlurFragment.glsl");
	hudShader		 = GameStateManager::Assets()->LoadShader(this, SHADERDIR"TexturedVertex.glsl", SHADERDIR"BlendedFragment.glsl");
	return LoadCheck();
}

bool Renderer::LoadCheck()
{
	return (basicShader			!= NULL	&&
			sceneShader			!= NULL	&&
			shadowShader		!= NULL	&&
			lightingShader		!= NULL	&&
			skyBoxShader		!= NULL	&&
			combineShader		!= NULL	&&
			particleShader		!= NULL	&&
			brightPassShader	!= NULL &&
			bloomCombShader		!= NULL &&
			gaussianShader		!= NULL &&
			downSampleShader	!= NULL &&
			bloomFinalShader	!= NULL &&
			velocityShader		!= NULL &&
			motionBlurShader	!= NULL &&
			hudShader			!= NULL);
}

void Renderer::UnloadShaders()
{
	GameStateManager::Assets()->UnloadShader(this, SHADERDIR"TexturedVertex.glsl", SHADERDIR"TexturedFragment.glsl");
	GameStateManager::Assets()->UnloadShader(this, SHADERDIR"MainVertShader.glsl", SHADERDIR"MainFragShader.glsl");
	GameStateManager::Assets()->UnloadShader(this, SHADERDIR"ShadowVertex.glsl", SHADERDIR"ShadowFragment.glsl");
	GameStateManager::Assets()->UnloadShader(this, SHADERDIR"DeferredPassVertex.glsl", SHADERDIR"DeferredPassFragment.glsl");
	GameStateManager::Assets()->UnloadShader(this, SHADERDIR"SkyBoxVertex.glsl", SHADERDIR"SkyBoxFragment.glsl");
	GameStateManager::Assets()->UnloadShader(this, SHADERDIR"CombineVertex.glsl", SHADERDIR"CombineFragment.glsl");
	GameStateManager::Assets()->UnloadShader(this, SHADERDIR"ParticleVertex.glsl", SHADERDIR"ParticleFragment.glsl", SHADERDIR"ParticleGeometry.glsl");
	GameStateManager::Assets()->UnloadShader(this, SHADERDIR"TexturedVertex.glsl", SHADERDIR"BrightPassFragment.glsl");//Bright-pass Shader
	GameStateManager::Assets()->UnloadShader(this, SHADERDIR"TexturedVertex.glsl", SHADERDIR"BloomCombFragment.glsl");//Bloom combine Shader
	GameStateManager::Assets()->UnloadShader(this, SHADERDIR"TexturedVertex.glsl", SHADERDIR"GaussianFragment.glsl");//Gaussian Blur Shader
	GameStateManager::Assets()->UnloadShader(this, SHADERDIR"TexturedVertex.glsl", SHADERDIR"DownSampleFragment.glsl");//Downsample Shader
	GameStateManager::Assets()->UnloadShader(this, SHADERDIR"TexturedVertex.glsl", SHADERDIR"BloomFinalShader.glsl");//Final bloom Shader
	GameStateManager::Assets()->UnloadShader(this, SHADERDIR"VelocityVertex.glsl", SHADERDIR"VelocityFragment.glsl");//VBuffer Shader
	GameStateManager::Assets()->UnloadShader(this, SHADERDIR"TexturedVertex.glsl", SHADERDIR"BlurFragment.glsl");//Motion blur Shader
	GameStateManager::Assets()->UnloadShader(this, SHADERDIR"TexturedVertex.glsl", SHADERDIR"BlendedFragment.glsl");
}

bool Renderer::LoadAssets() {
	// Load Meshes required for rendering operations
	
	circleMesh = GameStateManager::Assets()->LoadCircle(this, 20); // Circle for spotlight rendering
	screenMesh = GameStateManager::Assets()->LoadQuad(this); // Quad for rendering textures to screen
	sphereMesh = GameStateManager::Assets()->LoadMesh(this, MESHDIR"sphere.obj"); // Sphere for point light rendering
	coneMesh = GameStateManager::Assets()->LoadCone(this, 20); // Cone for spotlight rendering
	
	if (!sphereMesh || !coneMesh || !circleMesh || !screenMesh) {
		cout << "Renderer::LoadAssets() - unable to load rendering assets";
		return false;
	}
	
	return true;
}

void Renderer::UnloadAssets() {
	GameStateManager::Assets()->LoadCircle(this, 20); // Circle for spotlight rendering
	GameStateManager::Assets()->LoadQuad(this); // Quad for rendering textures to screen
	GameStateManager::Assets()->LoadMesh(this, MESHDIR"sphere.obj"); // Sphere for point light rendering
	GameStateManager::Assets()->LoadCone(this, 20); // Cone for spotlight rendering
}

Renderer::~Renderer(void)
{
	/*
	delete snow;
	delete rain;
	delete sandstorm;
	*/

	currentShader = NULL;

	//Clear buffers
	glDeleteTextures(1, &gbufferColourTex);
	glDeleteTextures(1, &gbufferDepthTex);
	glDeleteTextures(1, &gbufferNormalTex);
	glDeleteTextures(1, &shadowDepthTex);
	glDeleteTextures(1, &gbufferVelocity);
	glDeleteTextures(1, &lightEmissiveTex);
	glDeleteTextures(1, &lightSpecularTex);
	glDeleteTextures(3, postProcessingTex);
	glDeleteTextures(SAMPLENUM * 2, downSampleTex);
	glDeleteTextures(1, &skyBoxTex);
	glDeleteFramebuffers(1, &gbufferFBO);
	glDeleteFramebuffers(1, &postProcessingFBO);
	glDeleteFramebuffers(1, &deferredLightingFBO);
	glDeleteFramebuffers(1, &shadowFBO);
}

//Public method to initiate a draw to screen.
void Renderer::RenderScene() {

	openglMutex.lock_mutex(); // prevent other threads from accessing OpenGL during rendering
	if (!wglMakeCurrent(deviceContext, renderContext)) {
		cout << "Renderer::RenderScene() - unable to obtain rendering context!!!" << endl;
	}
	glClear(GL_COLOR_BUFFER_BIT);
	cameraMatrix = camera->BuildViewMatrix();

	// Main Render
	ShadowPass();
	DrawScene();
	DeferredLightPass();
	CombineBuffers();

	// Post-Processing
	//BloomPass();
	MotionBlurPass();

	DrawFrameBufferTex(postProcessingTex[1]);

	// Draw HUD/Menu overlay
	Draw2DOverlay();

	SwapBuffers();
	wglMakeCurrent(deviceContext, NULL);
	openglMutex.unlock_mutex();
}

void Renderer::ToggleDebug(int arg, bool on)
{
	switch (arg)
	{
	case DEBUG_DRAW_DEFERRED_LIGHTS:
		drawDeferredLights = on;
		break;
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

//Draws Scene to buffer object
void Renderer::DrawScene()
{
	glBindFramebuffer(GL_FRAMEBUFFER, gbufferFBO);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Use stencil buffer to track unaltered pixels. Use to draw skybox later
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_ALWAYS, 1, 1); // Always passes
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	glClearColor(0.2f, 0.2f, 0.2f, 0.2f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	SetCurrentShader(sceneShader);
//	SetCurrentShader(basicShader);

	// Bind Shader variables
	viewMatrix = cameraMatrix;
	projMatrix = perspectiveMatrix;
	UpdateShaderMatrices();
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*) &cameraMatrix.GetPositionVector());
	glUniform4fv(glGetUniformLocation(currentShader->GetProgram(), "nodeColour"), 1, (float*) &T3Vector4(1,1,1,1));

	// Pass any light/shadow data for any lights which generate shadows into the renderer
	unsigned int shadowCount = 0;
	char buffer[20];
	for (unsigned int i = 0; i < lights.size(); i++) {
		if (lights[i]->GetShadowTexture() > 0) { // Shadow depth texture exists for light, so use
			// Calculate the view projection matrix for the light so can sample shadow map (binding to textureMatrix for the minimumute
			T3Matrix4 shadowMatrix = biasMatrix * lights[i]->GetProjectionMatrix() * lights[i]->GetViewMatrix(T3Vector3(cameraMatrix.GetPositionVector()));
			
			sprintf_s(buffer, 20, "shadowProjMatrix[%d]", i);
			glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), buffer), 1, false, (float*) &shadowMatrix);
			
			// Bind shadow texture
			if (lights[i]->GetType() != POINT_LIGHT_TYPE) {
				sprintf_s(buffer, 20, "shadowTex[%d]", i);
				glUniform1i(glGetUniformLocation(currentShader->GetProgram(), buffer), SHADOW_TEXTURE_UNIT + shadowCount);
				glActiveTexture(GL_TEXTURE0 + SHADOW_TEXTURE_UNIT + shadowCount);
				glBindTexture(GL_TEXTURE_2D, lights[i]->GetShadowTexture());
			}
			else {
				sprintf_s(buffer, 20, "shadowCube[%d]", i);
				glUniform1i(glGetUniformLocation(currentShader->GetProgram(), buffer), SHADOW_TEXTURE_UNIT + shadowCount);
				glActiveTexture(GL_TEXTURE0 + SHADOW_TEXTURE_UNIT + shadowCount);
				glBindTexture(GL_TEXTURE_CUBE_MAP, lights[i]->GetShadowTexture());
			}

			// Bind light data
			lights[i]->BindLight(shadowCount);

			shadowCount++;
		}
	}

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "numShadows"), shadowCount);
			
	DrawNodes(true);

	if (drawDeferredLights) {
		// Draw Deferred lights
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "useNormalTex"), 0);
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "useDiffuseTex"), 0);
		for (unsigned int i = 0; i < lights.size(); i++) {
			modelMatrix = lights[i]->GetModelMatrix();
			UpdateShaderMatrices();
			switch (lights[i]->GetType()) {
			case POINT_LIGHT_TYPE:
				sphereMesh->Draw();
				break;
			case SPOTLIGHT_LIGHT_TYPE:
				coneMesh->Draw();
				break;
			}
		}
	}

	glUseProgram(0);

	// TODO - handle particle systems
	//if (weatherOn)
	//	DrawParticles();
	
	glDisable(GL_STENCIL_TEST); // Believe can disable here
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//	DrawFrameBufferTex(gbufferColourTex);
}

void Renderer::ShadowPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glViewport(0, 0, SHADOWSIZE, SHADOWSIZE);
	glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);	
	
	SetCurrentShader(shadowShader);
	for (unsigned int i = 0; i < lights.size(); i++) {

		// Does light cast shadows (does it have a depth texture attached)?
		if (lights[i]->GetShadowTexture() <= 0) continue; // only process shadow data if light set to cast shadows

		if (lights[i]->GetType() != POINT_LIGHT_TYPE) {
			// Attach depth texture to FBO
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, lights[i]->GetShadowTexture(), 0);
			glClear(GL_DEPTH_BUFFER_BIT);
			
			projMatrix = lights[i]->GetProjectionMatrix();
			viewMatrix = lights[i]->GetViewMatrix(cameraMatrix.GetPositionVector());
			UpdateShaderMatrices();
			
			DrawNodes(false);
			
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
			
		}
		else {
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			GLenum c = GL_COLOR_ATTACHMENT0;
			glDrawBuffers(1, &c);
			for (unsigned int f = 0; f < 6; f++)  {
				viewMatrix = T3Matrix4::BuildViewMatrix(lights[i]->GetPosition(), lights[i]->GetPosition() + faces[f].target, faces[f].up);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowDepthTex, 0);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, faces[f].cubeFace, lights[i]->GetShadowTexture(), 0); 
				glClear(GL_COLOR_BUFFER_BIT);
				glClear(GL_DEPTH_BUFFER_BIT);

				projMatrix = lights[i]->GetProjectionMatrix();
				UpdateShaderMatrices();

				DrawNodes(false);

				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, faces[f].cubeFace, 0, 0); 
			}
		}
	}

	glUseProgram(0);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	glClearColor(0.2f,0.2f,0.2f,1.0f);
	glViewport(0, 0, width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::DrawNodes(bool enableTextures) {
	// Draw Scene
	for (unsigned int i = 0; i < sceneNodes.size(); i++) {
		DrawableEntity3D& entity = *sceneNodes[i]->GetDrawableEntity();

		if (enableTextures) {
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
		}
		else {
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "useDiffuseTex"), 0);	
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "useNormalTex"), 0);
		}

		// ignore shader for the minimumute
		
		modelMatrix = sceneNodes[i]->GetWorldTransform() * T3Matrix4::Scale(entity.GetScale());
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"),	1,false, (float*)&modelMatrix);
		
		textureMatrix.ToIdentity(); // add to texture/drawableentity class

		entity.GetMesh()->Draw();
	}
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

	viewMatrix = cameraMatrix;
	glUniform3fv(glGetUniformLocation(currentShader->GetProgram(), "cameraPos"), 1, (float*) &cameraMatrix.GetPositionVector());
	glUniform2f(glGetUniformLocation(currentShader->GetProgram(), "pixelSize"), 1.0f / (float) width, 1.0f / (float) height);

	// Draw deferred scene lights
	for (unsigned int i = 0; i < lights.size(); ++i) {
		switch (lights[i]->GetType()) {
		case POINT_LIGHT_TYPE:
			DrawDeferredPointLight(lights[i]);
			break;
		case DIRECTIONAL_LIGHT_TYPE:
			DrawDeferredDirectionalLight(lights[i]);
			break;
		case SPOTLIGHT_LIGHT_TYPE:
			DrawDeferredSpotLight(lights[i]);
			break;
		default:
			cout << "Renderer::DeferredLightPass() - Unknown Deferred Light type" << endl;
		}
	}

	glDisable(GL_BLEND);

	glClearColor(0.2f, 0.2f, 0.2f, 1); // Reset clear colour to grey

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
}

void Renderer::DrawDeferredPointLight(Light* l) {
	projMatrix = perspectiveMatrix;
	modelMatrix = l->GetModelMatrix();
	UpdateShaderMatrices();
	
	l->BindLight(); // Set up light details in shader

	glEnable(GL_CULL_FACE);
	float dist = (l->GetPosition() - cameraMatrix.GetPositionVector()).Length();
	if (dist < l->GetRadius()) {
		// camera is inside the light volume!
		glCullFace(GL_FRONT);
	}
	else {
		glCullFace(GL_BACK);
	}
	sphereMesh->Draw();

	glCullFace(GL_BACK);
}

void Renderer::DrawDeferredSpotLight(Light* l) {
	projMatrix = perspectiveMatrix;
	modelMatrix = l->GetModelMatrix();
	UpdateShaderMatrices();

	l->BindLight();


	glEnable(GL_CULL_FACE);
	//glDisable(GL_CULL_FACE);
	bool inSpotlight = SpotLight::IsInSpotlight(cameraMatrix.GetPositionVector(), l);
	if (inSpotlight) {
		// camera is inside the light volume!
		glCullFace(GL_FRONT);
	}
	else {
		glCullFace(GL_BACK);
	}
	coneMesh->Draw();

	// If inside the cone, looking down the axis, need to draw a circular base to prevent base being discarded
	glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, (float*) &((SpotLight*) l)->GetBaseModelMatrix());

	if (inSpotlight) { 
	// camera is inside the light volume!
		glCullFace(GL_FRONT);
		circleMesh->Draw();
	}

	glCullFace(GL_BACK);

}

void Renderer::DrawDeferredDirectionalLight(Light* l) {
	projMatrix = orthographicMatrix;
	viewMatrix.ToIdentity();
	modelMatrix = l->GetModelMatrix();
	UpdateShaderMatrices();

	l->BindLight();
	glDisable(GL_CULL_FACE);
	screenMesh->Draw();
	glEnable(GL_CULL_FACE);
}

void Renderer::CombineBuffers() {// merge scene render with lighting pass
	glDisable(GL_DEPTH_TEST);
	
	SetCurrentShader(combineShader);
	glBindFramebuffer(GL_FRAMEBUFFER, postProcessingFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, postProcessingTex[0], 0); // the final picture
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, gbufferDepthTex, 0); // Stencil buffer from the first pass render
	
	// Setup matrices
	projMatrix = orthographicMatrix;
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

//	DrawSkybox(); // Finally, draw the skybox
	
	glEnable(GL_DEPTH_TEST);
	glUseProgram(0);
}

void Renderer::DrawSkybox() { // Draw skybox only where screen has not previously been written to
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 1); // Stencil test should only pass if stencil buffer not already set to 1 (from scene render)
	glDepthMask(GL_FALSE); // Disable writes to the depth buffer
	
	SetCurrentShader(skyBoxShader);
	
	// Bind shader variables
	viewMatrix = cameraMatrix;
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

/*--------Post-Processing methods--------*/
//TODO: finish this
void Renderer::BloomPass()
{
/*	
	- First a bright-pass filter is used to accentuate light and turn everything else black.
	- Then it is downsampled into three different sizes.
	- A gaussian blur is then applied to each downsample.
	- The downsamples are then combined with the original scene image to produce the final output.
*/
	/*-----------------------------Bright pass filter-----------------------------*/

	glBindFramebuffer(GL_FRAMEBUFFER, postProcessingFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, postProcessingTex[1], 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	SetCurrentShader(brightPassShader);
	projMatrix = T3Matrix4::Orthographic(-1, 1, 1, -1, -1, 1);
	viewMatrix.ToIdentity();
	UpdateShaderMatrices();

	glActiveTexture(GL_TEXTURE0 + MESH_OBJECT_COLOUR_TEXTURE_UNIT);
	glBindTexture(GL_TEXTURE_2D, postProcessingTex[0]);

	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "luminimumance"), 0.09f);
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "middleGrey"), 0.18f);
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "whiteCutOff"), 0.8f);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), MESH_OBJECT_COLOUR_TEXTURE_UNIT);

	screenMesh->Draw();

	/*---------------------------Downsample the images----------------------------*/

	glClear(GL_COLOR_BUFFER_BIT);
	SetCurrentShader(downSampleShader);
	UpdateShaderMatrices();

	glActiveTexture(GL_TEXTURE0 + MESH_OBJECT_COLOUR_TEXTURE_UNIT);
	glBindTexture(GL_TEXTURE_2D, postProcessingTex[1]);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), MESH_OBJECT_COLOUR_TEXTURE_UNIT);

	int z = 0;
	for (int i = 0; i < SAMPLENUM * 2; i += 2) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, downSampleTex[i], 0);
		glClear(GL_COLOR_BUFFER_BIT);

		glViewport(0,0, (int)(width/samples[z]), (int)(height/samples[z]));

		glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "sampleLevel"), samples[z]);

		screenMesh->Draw();
		++z;
}

	/*--------------------Gaussian Blur the downsampled images--------------------*/

	SetCurrentShader(gaussianShader);
	UpdateShaderMatrices();
	for (int o = 0; o < 4; o++){
		z = 0;
		for (int i = 0; i < SAMPLENUM * 2; i += 2) {
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, downSampleTex[i + 1], 0);
			glClear(GL_COLOR_BUFFER_BIT);
			glViewport(0,0, (int)(width/samples[z]), (int)(height/samples[z]));
			
			glUniform2f(glGetUniformLocation(currentShader->GetProgram(), "pixelSize"), 1.0f / (float)(width/samples[z]), 1.0f / (float)(height/samples[z]));
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "isVertical"), 0);
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), MESH_OBJECT_COLOUR_TEXTURE_UNIT);

			glActiveTexture(GL_TEXTURE0 + MESH_OBJECT_COLOUR_TEXTURE_UNIT);
			glBindTexture(GL_TEXTURE_2D, downSampleTex[i]);

			screenMesh->Draw();
			
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, downSampleTex[i], 0);
			glClear(GL_COLOR_BUFFER_BIT);

			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "isVertical"), 1);
			glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), MESH_OBJECT_COLOUR_TEXTURE_UNIT);

			glActiveTexture(GL_TEXTURE0 + MESH_OBJECT_COLOUR_TEXTURE_UNIT);
			glBindTexture(GL_TEXTURE_2D, downSampleTex[i + 1]);

			screenMesh->Draw();
			++z;
}
	}
	glViewport(0, 0, width, height);

	/*-------------------Combine the brightpass images together-------------------*/
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, downSampleTex[2], 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	
	SetCurrentShader(bloomCombShader);
	UpdateShaderMatrices();
	
	glActiveTexture(GL_TEXTURE0 + GBUFFER_DSAMPLE_TEXTURE_UNIT_0);
	glBindTexture(GL_TEXTURE_2D, downSampleTex[0]);
	glActiveTexture(GL_TEXTURE0 + GBUFFER_DSAMPLE_TEXTURE_UNIT_1);
	glBindTexture(GL_TEXTURE_2D, downSampleTex[2]);
	glActiveTexture(GL_TEXTURE0 + GBUFFER_DSAMPLE_TEXTURE_UNIT_2);
	glBindTexture(GL_TEXTURE_2D, downSampleTex[4]);

	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),"downSample0"), GBUFFER_DSAMPLE_TEXTURE_UNIT_0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),"downSample1"), GBUFFER_DSAMPLE_TEXTURE_UNIT_1);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(),"downSample2"), GBUFFER_DSAMPLE_TEXTURE_UNIT_2);

	screenMesh->Draw();

	/*------------------One final blurring to reduce artifacting------------------*/

	SetCurrentShader(gaussianShader);
	UpdateShaderMatrices();

	for (int i = 0; i < 2; ++i) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, postProcessingTex[1], 0);
		glClear(GL_COLOR_BUFFER_BIT);
		
		glActiveTexture(GL_TEXTURE0 + MESH_OBJECT_COLOUR_TEXTURE_UNIT);
		glBindTexture(GL_TEXTURE_2D, postProcessingTex[2]);

		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "isVertical"), 0);
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), MESH_OBJECT_COLOUR_TEXTURE_UNIT);

		screenMesh->Draw();

		//now to swap the colour buffers, and do the second blur Pass
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, postProcessingTex[2], 0);
		glClear(GL_COLOR_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0 + MESH_OBJECT_COLOUR_TEXTURE_UNIT);
		glBindTexture(GL_TEXTURE_2D, postProcessingTex[1]);

		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "isVertical"), 1);
		glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), MESH_OBJECT_COLOUR_TEXTURE_UNIT);

		screenMesh->Draw();
	}

	/*----------------Combine the bloom image with the scene image----------------*/

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, postProcessingTex[1], 0);
	glClear(GL_COLOR_BUFFER_BIT);
	SetCurrentShader(bloomFinalShader);
	UpdateShaderMatrices();

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, postProcessingTex[2]);
	glActiveTexture(GL_TEXTURE0 + MESH_OBJECT_COLOUR_TEXTURE_UNIT);
	glBindTexture(GL_TEXTURE_2D, postProcessingTex[0]);
	
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "darkTex"), 5);
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "mixVal"), 0.5);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), MESH_OBJECT_COLOUR_TEXTURE_UNIT);
	
	screenMesh->Draw();
	
	glUseProgram(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
//TODO: just need to work out how to write the scene texture to the quad.
void Renderer::MotionBlurPass()
{
	glClearColor(0,0,0,0);
	glBindFramebuffer(GL_FRAMEBUFFER, postProcessingFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gbufferVelocity, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	SetCurrentShader(velocityShader);
	
	viewMatrix = cameraMatrix;
	projMatrix = perspectiveMatrix;
	UpdateShaderMatrices();

	//Modified draw call that will set the previous MVP after drawing.
	for (unsigned int i = 0; i < sceneNodes.size(); i++) {
		DrawableEntity3D& entity = *sceneNodes[i]->GetDrawableEntity();

		modelMatrix = sceneNodes[i]->GetWorldTransform() * T3Matrix4::Scale(entity.GetScale());
		T3Matrix4 previousMVP = sceneNodes[i]->GetPrevMVP();
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "modelMatrix"), 1, false, (float*)&modelMatrix);
		glUniformMatrix4fv(glGetUniformLocation(currentShader->GetProgram(), "prevMVP"), 1, false, (float*)&previousMVP);

		textureMatrix.ToIdentity();

		entity.GetMesh()->Draw();
		sceneNodes[i]->SetPrevMVP(projMatrix * viewMatrix * modelMatrix);
	}
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, postProcessingTex[1], 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	SetCurrentShader(motionBlurShader);
	projMatrix = orthographicMatrix;
	modelMatrix.ToIdentity();
	viewMatrix.ToIdentity();
	textureMatrix.ToIdentity();
	UpdateShaderMatrices();

	glDisable(GL_DEPTH_TEST);

	glActiveTexture(GL_TEXTURE0 + GBUFFER_VELOCITY_UNIT);
	glBindTexture(GL_TEXTURE_2D, gbufferVelocity);
	glActiveTexture(GL_TEXTURE0 + MESH_OBJECT_COLOUR_TEXTURE_UNIT);
	glBindTexture(GL_TEXTURE_2D, postProcessingTex[0]);

	//TODO: Create a method that does the currFPS/TargetFPS calc
	glUniform1f(glGetUniformLocation(currentShader->GetProgram(), "velocityScale"), 1.0);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "velocityTex"), GBUFFER_VELOCITY_UNIT);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), MESH_OBJECT_COLOUR_TEXTURE_UNIT);

	screenMesh->Draw();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
	glEnable(GL_DEPTH_TEST);
}

void Renderer::DrawFrameBufferTex(GLuint fboTex) {
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // draw the result to screen
	
	SetCurrentShader(basicShader);
		
	glActiveTexture(GL_TEXTURE26);
	glBindTexture(GL_TEXTURE_2D, fboTex);
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), 26);
	
	projMatrix = orthographicMatrix;
	modelMatrix.ToIdentity();
	viewMatrix.ToIdentity();
	textureMatrix.ToIdentity();
	UpdateShaderMatrices();
	screenMesh->Draw();

	glEnable(GL_DEPTH_TEST);

	glUseProgram(0);
}

void Renderer::Draw2DOverlay() {
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	
	SetCurrentShader(hudShader);
	projMatrix = hudMatrix;
	viewMatrix.ToIdentity();

	for (unsigned int i = 0; i < overlayElements.size(); i++) {

		glUniform4fv(glGetUniformLocation(currentShader->GetProgram(), "blendColour"), 1, (float*) &overlayElements[i]->GetColor());
		
		switch (overlayElements[i]->GetType()) {
		case DrawableType::Text:
			Draw2DText((DrawableText2D&) *overlayElements[i]);
			break;
		case DrawableType::Texture:
			Draw2DTexture((DrawableTexture2D&) *overlayElements[i]);
			break;
		default:
			cout << "Renderer::Draw2DOverlay() - Unidentified 2D element type, " << overlayElements.size() << endl;
		}
	}

	glUseProgram(0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}

void Renderer::Draw2DText(DrawableText2D& text) {
	TextMesh* textMesh = new TextMesh(text.GetText(), *text.GetFont());
	
	T3Vector3 origin = T3Vector3(text.GetOrigin().x * text.width, text.GetOrigin().y * text.height, 0);
	T3Matrix4 rotation = T3Matrix4::Translation(origin) * T3Matrix4::Rotation(text.GetRotation(), T3Vector3(0,0,-1)) * T3Matrix4::Translation(origin * -1.0f);
	modelMatrix = T3Matrix4::Translation(T3Vector3(text.x, height - text.y, 0)) * rotation * T3Matrix4::Scale(T3Vector3(text.width / (float) text.GetText().length(), text.height, 1));
	textureMatrix.ToIdentity();	
	UpdateShaderMatrices();

	glActiveTexture(GL_TEXTURE0 + MESH_OBJECT_COLOUR_TEXTURE_UNIT);
	glBindTexture(GL_TEXTURE_2D, text.GetFont()->GetTexture()->GetTextureName());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), MESH_OBJECT_COLOUR_TEXTURE_UNIT);
	
	textMesh->Draw();
	delete textMesh;
}

void Renderer::Draw2DTexture(DrawableTexture2D& texture) {
	T3Vector3 origin = T3Vector3(texture.GetOrigin().x * texture.width, texture.GetOrigin().y * texture.height, 0);
	T3Matrix4 rotation = T3Matrix4::Translation(origin) * T3Matrix4::Rotation(texture.GetRotation(), T3Vector3(0,0,-1)) * T3Matrix4::Translation(origin * -1.0f);
	modelMatrix = T3Matrix4::Translation(T3Vector3(texture.x, height - texture.y, 0)) * rotation * T3Matrix4::Scale(T3Vector3(texture.width, texture.height, 1));
	textureMatrix.ToIdentity();	
	UpdateShaderMatrices();

	glActiveTexture(GL_TEXTURE0 + MESH_OBJECT_COLOUR_TEXTURE_UNIT);
	glBindTexture(GL_TEXTURE_2D, texture.GetTexture()->GetTextureName());
	glUniform1i(glGetUniformLocation(currentShader->GetProgram(), "diffuseTex"), MESH_OBJECT_COLOUR_TEXTURE_UNIT);

	screenMesh->Draw();
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

bool Renderer::ActiveTex()
{
	activeTex = !activeTex;
	return activeTex;
}

GLuint Renderer::CreateTexture(const char* filename, bool enableMipMaps, bool enableAnisotropicFiltering) {
	openglMutex.lock_mutex();
	wglMakeCurrent(deviceContext, renderContext);

	unsigned int flags = false;
	if (enableMipMaps) flags |= SOIL_FLAG_MIPMAPS;
	GLuint textureObject = SOIL_load_OGL_texture(filename, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, flags);
	if (!textureObject)
		textureObject = 0; // make sure GetTexture will return an error

	wglMakeCurrent(deviceContext, NULL);
	openglMutex.unlock_mutex();

	return textureObject;
}

GLuint Renderer::CreateShadowTexture() {
	openglMutex.lock_mutex();
	wglMakeCurrent(deviceContext, renderContext);
	
	//Create a shadow texture buffer
	GLuint shadowTex;
	glGenTextures(1, &shadowTex);
	glBindTexture(GL_TEXTURE_2D, shadowTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	
	wglMakeCurrent(deviceContext, NULL);
	openglMutex.unlock_mutex();

	return shadowTex;
}

unsigned int Renderer::CreateShadowCube() {

	openglMutex.lock_mutex();
	wglMakeCurrent(deviceContext, renderContext);

	GLuint shadowTex;
	// Create cube map
	glGenTextures(1, &shadowTex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, shadowTex);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	
	/*
	for (unsigned int i = 0 ; i < 6 ; i++) {//
		// Set Tex parameters for each cube face (+x, -x, +y, -y, +z, -z)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOWSIZE, SHADOWSIZE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    }
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	*/

	for (unsigned int i = 0 ; i < 6 ; i++) {//
		// Set Tex parameters for each cube face (+x, -x, +y, -y, +z, -z)
	    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_R32F, SHADOWSIZE, SHADOWSIZE, 0, GL_RED, GL_FLOAT, NULL);
   }
	
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	wglMakeCurrent(deviceContext, NULL);
	openglMutex.unlock_mutex();

	return shadowTex;
}

bool Renderer::DestroyTexture(GLuint textureReference) {
	openglMutex.lock_mutex();
	wglMakeCurrent(deviceContext, renderContext);

	glDeleteTextures(1, &textureReference);
	
	wglMakeCurrent(deviceContext, NULL);
	openglMutex.unlock_mutex();
	return true;
}

bool Renderer::GetRenderContextForThread() {
	openglMutex.lock_mutex();
	return (0 != wglMakeCurrent(deviceContext, renderContext));
}

bool Renderer::DropRenderContextForThread() {
	bool result = (0 != wglMakeCurrent(deviceContext, NULL));
	openglMutex.unlock_mutex(); 
	return result;
}