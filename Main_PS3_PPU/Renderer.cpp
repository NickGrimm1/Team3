#if PS3_BUILD

#include "Renderer.h"
#include "../Framework/Shader.h"
#include "../Team3Project1/ShaderPart.h"
#include "../Team3Project1/GameStateManager.h"
#include <cell/gcm.h>
int Renderer::count = 0;

Renderer::Renderer(vector<Light*>& lightsVec, vector<SceneNode*>& SceneNodesVec, vector<DrawableTexture2D*>& overlayTextureVec, vector<DrawableText2D*>& overlayTextVec)
	: lights(lightsVec),
	sceneNodes(SceneNodesVec),
	overlayTextures(overlayTextureVec),
	overlayTexts(overlayTextVec)
{
	camera	= NULL;

	// Setup projection matrices - gonna just keep copies of the matrices rather than keep recreating them
	perspectiveMatrix = Matrix4::perspective(0.7853982, screenRatio, 1.0f, 10000.0f);
	orthographicMatrix = Matrix4::orthographic(-1,1,1,-1,1,-1); // for drawing full screen quads
	hudMatrix = Matrix4::orthographic(0.0f, screenWidth, 1080.0f, 0.0f, -10.0f, 10.0f); // For HUD Elements only

	// Generate Sky Buffer & initial static map.
	cloudMap = new CellGcmTexture();
	cloudMap->width = 128;
	cloudMap->height = 128;
	cloudMap->pitch = 128*4;
	cloudMap->depth = 1;
	char *rsxdata = (char*)localMemoryAlign(128, 128*4*128);
	CreateStaticMap(rsxdata, 128, 0, 255); 
	unsigned int offset;
	cellGcmAddressToOffset(rsxdata, &offset);
	cloudMap->location = CELL_GCM_LOCATION_LOCAL;	//We want the texture in graphics memory
	cloudMap->offset = offset;						//at he following offset
	cloudMap->format = CELL_GCM_TEXTURE_A8R8G8B8;	//ARGB format - there doesn't seem to be a RGB format!
	cloudMap->format = CELL_GCM_TEXTURE_LN;		//Data is a 'linear' array of values
	cloudMap->mipmap = 1;		//How many mipmap levels are there (1 == 'largest level mipmap')
	cloudMap->cubemap = CELL_GCM_FALSE;	//No...it's not a cubemap
	cloudMap->dimension	= CELL_GCM_TEXTURE_DIMENSION_2;	//It's a 2D Texture...

	cellGcmSetStencilTestEnable(CELL_GCM_FALSE);

	shader = NULL;
	loadedTextMeshes.clear();
	//init = true;	
}

void Renderer::CreateStaticMap(char* data, const int resolution, unsigned char minValue, unsigned char maxValue)
{
	for (int x = 0; x < resolution; ++x)
		for (int y = 0; y < resolution; ++y)
			for (int i = 0; i < 4; ++i)
			{
				data[x * resolution * 4 + y * 4 + i] = (rand() % (maxValue - minValue) + minValue) / (float)maxValue;
			}
}

bool Renderer::LoadShaders()
{
	//Shader initialisations go here.
	basicShader		 = GameStateManager::Assets()->LoadShader(this, "/TexturedVertex.vpo", "/TexturedFragment.fpo");
	skyBoxShader	 = GameStateManager::Assets()->LoadShader(this, "/SkyDomeVertex.vpo", "/SkyDomeFragment.fpo");
	hudShader		 = GameStateManager::Assets()->LoadShader(this, "/TexturedVertex.vpo", "/BlendedFragment.fpo");
	return LoadCheck();
}

bool Renderer::LoadCheck()
{
	return (basicShader			!= NULL	&&
				basicShader->GetVertex() != NULL &&
				basicShader->GetFragment() != NULL &&
			skyBoxShader		!= NULL	&&
				skyBoxShader->GetVertex() != NULL &&
				skyBoxShader->GetFragment() != NULL &&
			hudShader			!= NULL &&
				hudShader->GetVertex() != NULL &&
				hudShader->GetFragment() != NULL);
}

void Renderer::UnloadShaders()
{
	GameStateManager::Assets()->UnloadShader(this, SHADERDIR"TexturedVertex.vpo", SHADERDIR"TexturedFragment.fpo");
	GameStateManager::Assets()->UnloadShader(this, SHADERDIR"SkyDomeVertex.glsl", SHADERDIR"SkyDomeFragment.glsl");
	GameStateManager::Assets()->UnloadShader(this, SHADERDIR"TexturedVertex.glsl", SHADERDIR"BlendedFragment.glsl");
}

bool Renderer::LoadAssets() {
	// Load Meshes required for rendering operations
	if (!LoadShaders())
		return false;

	circleMesh = GameStateManager::Assets()->LoadCircle(this, 20); // Circle for spotlight rendering
	screenMesh = GameStateManager::Assets()->LoadQuad(this); // Quad for rendering textures to screen
	sphereMesh = GameStateManager::Assets()->LoadMesh(this, MESHDIR"sphere.obj"); // Sphere for point light rendering
	coneMesh = GameStateManager::Assets()->LoadCone(this, 20); // Cone for spotlight rendering
	skyDome = GameStateManager::Assets()->LoadMesh(this, MESHDIR"dome.obj"); // Skydome
	quadMesh = GameStateManager::Assets()->LoadQuadAlt(this);
	nightSkyTex = GameStateManager::Assets()->LoadTexture(this, "night_sky", 0);
	daySkyTex = GameStateManager::Assets()->LoadTexture(this, "day_sky", 0);

	if (!sphereMesh || !coneMesh || !circleMesh || !screenMesh) 
	{
		return false;
	}
	
	return true;
}

void Renderer::UnloadAssets() 
{
	GameStateManager::Assets()->UnloadCircle(this, 20); // Circle for spotlight rendering
	GameStateManager::Assets()->UnloadQuad(this); // Quad for rendering textures to screen
	GameStateManager::Assets()->UnloadMesh(this, MESHDIR"sphere.obj"); // Sphere for point light rendering
	GameStateManager::Assets()->UnloadCone(this, 20); // Cone for spotlight rendering
	GameStateManager::Assets()->UnloadMesh(this, MESHDIR"dome.obj"); // Skydome
	GameStateManager::Assets()->UnloadQuadAlt(this);
	GameStateManager::Assets()->UnloadTexture(this, "night_sky");
	GameStateManager::Assets()->UnloadTexture(this, "day_sky");
}

Renderer::~Renderer(void)
{
	shader = NULL;
}

void Renderer::RenderScene() 
{
	renderMutex.lock_mutex(); // prevent other threads from accessing OpenGL during rendering
	SetViewport();
	ClearBuffer();

	if (camera) 
	{
		T3Matrix4 m = camera->BuildViewMatrix();
		
		cameraMatrix = Matrix4::identity();
		for (int x = 0; x < 4; ++x)
			for (int y = 0; y < 4; ++y)
				cameraMatrix.setElem(x, y, m.values[y + x * 4]);

		count = 0;
		DrawScene();

		DrawSkybox();
	}

	//Draw HUD/Menu overlay
	loadingIcon->SetRotation(loadingIcon->GetRotation() + 1.0f);
	if (hudShader != NULL && quadMesh != NULL) 
	{
	Draw2DOverlay();
	}

	SwapBuffers();
	//wglMakeCurrent(deviceContext, NULL);
	renderMutex.unlock_mutex();
}

void Renderer::ToggleDebug(int arg, bool on)
{
	
}

void Renderer::DrawScene()
{
	if (basicShader)
	{
		// Use stencil buffer to track unaltered pixels. Use to draw skybox later
		cellGcmSetStencilTestEnable(CELL_GCM_TRUE);
		cellGcmSetStencilFunc(CELL_GCM_ALWAYS, 0, 0); // Always passes
		cellGcmSetStencilOp(CELL_GCM_KEEP, CELL_GCM_KEEP, CELL_GCM_REPLACE);

		ClearBuffer();
	
		//SetCurrentShader(sceneShader);
		SetCurrentShader(basicShader);

		// Bind Shader variables
		viewMatrix = cameraMatrix;
		projMatrix = perspectiveMatrix;
		modelMatrix = Matrix4::identity();
		shader->GetVertex()->UpdateShaderMatrices(modelMatrix, viewMatrix, projMatrix);
		
		DrawNodes(true);
	
		cellGcmSetStencilTestEnable(CELL_GCM_FALSE);
	}
}

void Renderer::ShadowPass()
{
	
}

void Renderer::DrawNodes(bool enableTextures)
{
	// Draw Scene
	for (unsigned int i = 0; i < sceneNodes.size(); i++)
	{
		DrawableEntity3D& entity = *sceneNodes[i]->GetDrawableEntity();

		if (enableTextures)
		{
			// Handle colour and bump textures
			if (entity.GetTexture() && entity.GetTexture()->GetTexture()) 
			{
				CellGcmTexture* t = entity.GetTexture()->GetTexture(); //texture->GetTexture();
				SetTextureSampler(shader->GetFragment()->GetParameter("texture"), t);
				//SetTextureSampler(shader->GetFragment()->GetParameter("texture"), entity.GetTexture()->GetTexture());
				
			}else if(entity.GetMesh()->GetNormTex() && entity.GetMesh()->GetNormTex()->GetTexture())
			{
				CellGcmTexture* t = entity.GetMesh()->GetNormTex()->GetTexture(); //texture->GetTexture();
				SetTextureSampler(shader->GetFragment()->GetParameter("texture"), t);
			}
		}

		T3Matrix4 m = sceneNodes[i]->GetWorldTransform() * T3Matrix4::Scale(entity.GetScale());
		for (int x = 0; x < 4; ++x)
			for (int y = 0; y < 4; ++y)
				modelMatrix.setElem(x, y, m.values[y + x * 4]);
		textureMatrix = Matrix4::identity(); // add to texture/drawableentity class
		shader->GetVertex()->UpdateShaderMatrices(modelMatrix,viewMatrix,projMatrix);
		entity.GetMesh()->Draw(shader);
	}
}

void Renderer::DeferredLightPass()
{
}

void Renderer::DrawDeferredPointLight(Light* l) 
{
}

void Renderer::DrawDeferredSpotLight(Light* l) 
{
}

void Renderer::DrawDeferredDirectionalLight(Light* l) 
{
}

void Renderer::CombineBuffers() 
{
}

void Renderer::DrawSkybox() 
{ 
	cellGcmSetCullFaceEnable(CELL_GCM_FALSE);
	cellGcmSetDepthTestEnable(CELL_GCM_FALSE);
	SetCurrentShader(skyBoxShader);
	SetTextureSampler(shader->GetFragment()->GetParameter("nightSkyTex"), nightSkyTex->GetTexture());
	SetTextureSampler(shader->GetFragment()->GetParameter("daySkyTex"), daySkyTex->GetTexture());
	shader->GetFragment()->SetParameter("dayNightMix", &dayNight);
	shader->GetVertex()->SetParameter("worldPos", (float*)T3Vector3(0,0,0));

	viewMatrix = cameraMatrix;
	projMatrix = perspectiveMatrix;
	T3Matrix4 m = T3Matrix4::Translation(T3Vector3(camera->GetPosition().x, camera->GetPosition().y - 25, camera->GetPosition().z)) * T3Matrix4::Scale(T3Vector3(100, 100, 100));
	for (int x = 0; x < 4; ++x)
		for (int y = 0; y < 4; ++y)
			modelMatrix.setElem(x, y, m.values[y + x * 4]);
	shader->GetVertex()->UpdateShaderMatrices(modelMatrix,viewMatrix,projMatrix);
	skyDome->Draw(shader);
}

/*--------Post-Processing methods--------*/

void Renderer::BloomPass()
{
}

void Renderer::MotionBlurPass()
{
}

void Renderer::Draw2DOverlay() 
{

	if (hudShader)
	{
	
		cellGcmSetDepthTestEnable(CELL_GCM_TRUE);
		cellGcmSetCullFace(CELL_GCM_FRONT);
		cellGcmSetAlphaTestEnable(CELL_GCM_TRUE);

		SetCurrentShader(hudShader);
		projMatrix = hudMatrix;
		viewMatrix = Matrix4::identity();

		for (unsigned int i = 0; i < overlayTexts.size(); ++i) 
		{
			shader->GetFragment()->SetParameter("blendColour", (float*)(overlayTexts[i]->GetColor()));
			Draw2DText(*overlayTexts[i]);
		}

		for (unsigned int i = 0; i < overlayTextures.size(); ++i) 
		{
			shader->GetFragment()->SetParameter("blendColour", (float*)(overlayTextures[i]->GetColor()));
			Draw2DTexture(*overlayTextures[i]);
		}
		
		cellGcmSetAlphaTestEnable(CELL_GCM_FALSE);
		cellGcmSetDepthTestEnable(CELL_GCM_FALSE);
		cellGcmSetCullFace(CELL_GCM_BACK);
	}
}

void Renderer::Draw2DText(DrawableText2D& text) 
{
	if (text.GetText() != "")
	{
		TextMesh* textMesh = NULL;
		map<string, TextMesh*>::iterator i = loadedTextMeshes.find(text.GetText());
		if (i == loadedTextMeshes.end()) 
		{
			// Create a text mesh of appropriate length to display text and store
			textMesh = new TextMesh(text.GetText(), *text.GetFont());
			if (textMesh == NULL)
			{
				return;
			}
			string t = text.GetText();
			loadedTextMeshes.insert(std::pair<std::string, TextMesh*>(t, textMesh));
		}
		else 
		{
			textMesh = (*i).second;
		}

		T3Vector3 origin = T3Vector3(text.GetOrigin().x * text.width * screenWidth, text.GetOrigin().y * text.height * screenHeight, 0);
		T3Matrix4 rotation = T3Matrix4::Translation(origin) * T3Matrix4::Rotation(text.GetRotation(), T3Vector3(0, 0, 1)) * T3Matrix4::Translation(origin * -1.0f);
		T3Matrix4 m = T3Matrix4::Translation(T3Vector3(text.x * screenWidth, text.y * screenHeight, 1)) * rotation * T3Matrix4::Scale(T3Vector3(screenWidth * text.width / (float) text.GetText().length(), screenHeight * text.height, 1));
		for (int x = 0; x < 4; ++x)
				for (int y = 0; y < 4; ++y)
					modelMatrix.setElem(x, y, m.values[y + x * 4]);
		textureMatrix = Matrix4::identity();
		shader->GetVertex()->UpdateShaderMatrices(modelMatrix, viewMatrix, projMatrix);
		SetTextureSampler(shader->GetFragment()->GetParameter("diffuseTex"), text.GetFont()->GetTexture()->GetTexture());
		textMesh->Draw(shader);
	}
}

void Renderer::Draw2DTexture(DrawableTexture2D& texture) 
{
	if (texture.GetTexture())
	{
		T3Vector3 origin = T3Vector3(texture.GetOrigin().x * texture.width * screenWidth, texture.GetOrigin().y * texture.height * screenHeight, 0);
		T3Matrix4 rotation = T3Matrix4::Translation(origin) * T3Matrix4::Rotation(texture.GetRotation(), T3Vector3(0,0,1)) * T3Matrix4::Translation(-origin);
		T3Matrix4 m = T3Matrix4::Translation(T3Vector3(texture.x * screenWidth, texture.y * screenHeight, 0)) * rotation * T3Matrix4::Scale(T3Vector3(texture.width * screenWidth, texture.height * screenHeight, 1));
		for (int x = 0; x < 4; ++x)
			for (int y = 0; y < 4; ++y)
				modelMatrix.setElem(x, y, m.values[y + x * 4]);

		textureMatrix = Matrix4::identity();	
		shader->GetVertex()->UpdateShaderMatrices(modelMatrix, viewMatrix, projMatrix);

		SetTextureSampler(shader->GetFragment()->GetParameter("diffuseTex"), texture.GetTexture()->GetTexture());

		quadMesh->Draw(shader);
	}
}

bool Renderer::ActiveTex()
{
	activeTex = !activeTex;
	return activeTex;
}

unsigned char* Renderer::GeneratePerlinNoise(const int resolution, unsigned char minValue, unsigned char maxValue)
{
}

unsigned int Renderer::GetTextMeshMemory(){}
#endif