#include "GraphicsEngine.h"
#include "GraphicsCommon.h"
#include "../Framework/T3Vector2.h"
#include "../Framework/T3Vector4.h"
#include <algorithm>
#if WINDOWS_BUILD
GraphicsEngine* GraphicsEngine::engine = NULL;

bool GraphicsEngine::Initialize(GraphicsEngine*& out) {
	if (engine != NULL) 
		return engine->HasInitialised(); // an engine instance already exists, can't create another, return where we have a success yet.
	
	// Otherwise, create new engine
	engine = new GraphicsEngine();
	engine->sceneRoot = new SceneNode();
	engine->camera = NULL;

	out = engine;
	return engine->HasInitialised();
}

bool GraphicsEngine::Destroy() {

	if (engine == NULL) 
		return true; // no engine instance to destroy, this must be true by default.

	delete engine;
	engine = NULL;

	return true;
}

GraphicsEngine::GraphicsEngine() 
	: RENDER_TIME(1000.0f / 60)
{
	isInitialised = false;
	width = SCREEN_WIDTH;
	height = SCREEN_HEIGHT;
	frameRate = 0;

	if (!Window::Initialise(GAME_TITLE, width, height, false)) return;
	
	Window::GetWindow().LockMouseToWindow(true);
	Window::GetWindow().ShowOSPointer(false);

	renderer = new Renderer(Window::GetWindow(), lights, gameEntityList, overlayElementsList);
	if (!renderer->HasInitialised()) return;
	
	GLuint loadingTex = renderer->CreateTexture(TEXTUREDIR"refresh.png", false, false, SOIL_FLAG_INVERT_Y);
	isLoading = true;
	isLoadingDrawing = false;
	loadingTexture = new Texture(loadingTex);
	float aspect = (float) width / (float) height;
	loadingIcon = new DrawableTexture2D(
		0.95f,
		1.0f - 0.05f * aspect,
		1,
		0.05f,
		0.05f * aspect,
		loadingTexture,
		0,
		T3Vector2(0.5f, 0.5f),
		T3Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	isInitialised = true; // Graphics Engine has initialised successfully
}

GraphicsEngine::~GraphicsEngine() {
	// TODO
	// Delete local array data - if not handled elsewhere

	// Fin
	renderer->DestroyTexture(loadingTexture->GetTextureName());
	delete loadingTexture;
	delete loadingIcon;
	delete renderer;
	Window::Destroy();
}

void GraphicsEngine::Run() {
	isRunning = true;
	
	while (isRunning) {

		while (Window::GetWindow().GetTimer()->GetMS() - lastFrameTimeStamp < RENDER_TIME) { ; } // Fix the timestep
		float msec = Window::GetWindow().GetTimer()->GetMS() - lastFrameTimeStamp;
		lastFrameTimeStamp = Window::GetWindow().GetTimer()->GetMS();
		frameRate = (int)(1000.0f / msec);

		// add/remove requested items from scene lists
		contentGuard.lock_mutex();

		// Add/Remove Game objects
		for (unsigned int i = 0; i < addGameList.size(); i++) {
			if (addGameList[i].second == NULL) {
				// if no parent - add drawable as child of sceneRoot
				sceneRoot->AddChild(new SceneNode(addGameList[i].first));
			}
			else
			{
				sceneRoot->AddChildToParent(addGameList[i].first, addGameList[i].second);
			}
		}
		addGameList.clear();

		for (unsigned int i = 0; i < removeGameList.size(); i++) {
			sceneRoot->RemoveChild(removeGameList[i].first, removeGameList[i].second);
		}
		removeGameList.clear();

		// Add/Remove HUD elements
		for (unsigned int i = 0; i < addHudList.size(); i++) {
			overlayElementsList.push_back(addHudList[i]);
		}
		addHudList.clear();

		for (unsigned int i = 0; i < removeHudList.size(); i++) {
			for (vector<DrawableEntity2D*>::iterator j = overlayElementsList.begin(); j != overlayElementsList.end(); ++j) {
				if ((*j) == removeHudList[i]) {
					overlayElementsList.erase(j);
				}
			}
		}
		removeHudList.clear();

		for (unsigned int i = 0; i < addLightsList.size(); i++) {
			lights.push_back(addLightsList[i]);
		}
		addLightsList.clear();

		// Add/Remove lights
		for (unsigned int l = 0; l < removeLightsList.size(); l++) {
			for (vector<Light*>::iterator i = lights.begin(); i != lights.end(); ++i) {
				if ((*i) == removeLightsList[l]) {
					unsigned int shadowTex = (*i)->GetShadowTexture();
					if (shadowTex > 0) 
						renderer->DestroyTexture(shadowTex);
					lights.erase(i);
				}
			}
		}
		removeLightsList.clear();

		contentGuard.unlock_mutex();
			
		// Update data in scene nodes
		sceneRoot->Update(msec); // TODO - sort out proper timestep value - or remove timestep if not needed
		
		// Reset scene bounding box volume
		boundingMax = T3Vector3(0,0,0);
		boundingMin = T3Vector3(0,0,0);		

		// Update camera
		if (camera != NULL)
		{
			camera->UpdateCamera(); // may need to remove
			T3Matrix4 viewMatrix = camera->BuildViewMatrix();
			T3Matrix4 projMatrix = T3Matrix4::Perspective(1.0f, 10000.0f, (float) width / (float) height, 45.0f);
			frameFrustum.FromMatrix(projMatrix * viewMatrix);
		}

		// Build Node lists
		BuildNodeLists(sceneRoot);
		SortNodeLists();
		
		// Update directional lights with scene bounding box
		// Transform bounding volume by camera transform
		DirectionalLight::UpdateLightVolume(boundingMin, boundingMax);

		// Sort HUD elements
		std::sort(overlayElementsList.begin(), overlayElementsList.end(), 
			[] (const void* a, const void* b) {
				return (((DrawableEntity2D*) a)->GetDepth() < ((DrawableEntity2D*) b)->GetDepth());
		});
	
		// Sort Point Lights to the front of the lights list
		std::sort(lights.begin(), lights.end(), 
			[] (const void* a, const void* b) {
				return (((Light*) a)->GetType() < ((Light*) b)->GetType());
		});

		if (isLoading && !isLoadingDrawing) {
			overlayElementsList.push_back(loadingIcon);
			isLoadingDrawing = true;
		}
//		else if (!isLoading && isLoadingDrawing) {
//			overlayElementsList
//		}

		if (isLoadingDrawing) {
			loadingIcon->SetRotation(loadingIcon->GetRotation() + 1.0f);
		}

		// Render data
		renderer->RenderScene();

		// Clear node lists in preparation for next render cycle
		ClearNodeLists();
	}
}

PointLight* GraphicsEngine::AddPointLight(T3Vector3 lightPosition, float lightRadius, T3Vector4 diffuseColour, T3Vector4 specularColour, bool castsShadow) {
	unsigned int shadowTex = castsShadow ? renderer->CreateShadowCube() : 0;
	PointLight* l = new PointLight(lightPosition, diffuseColour, specularColour, lightRadius, shadowTex);
	lights.push_back(l);
	return l;
}

DirectionalLight* GraphicsEngine::AddDirectionalLight(T3Vector3 lightDirection, T3Vector4 diffuseColour, T3Vector4 specularColour) {
	//unsigned int shadowTex = castsShadow ? renderer->CreateShadowTexture() : 0;
	DirectionalLight* l = new DirectionalLight(lightDirection, diffuseColour, specularColour, 0);
	lights.push_back(l);
	return l;
}

SpotLight* GraphicsEngine::AddSpotLight(T3Vector3 lightPosition, T3Vector3 lightTarget, T3Vector3 upVector, float lightRadius, float lightAngle, T3Vector4 diffuseColour, T3Vector4 specularColour, bool castsShadow) {
	unsigned int shadowTex = castsShadow ? renderer->CreateShadowTexture() : 0;
	SpotLight* l = new SpotLight(lightPosition, lightTarget, upVector, diffuseColour, specularColour, lightRadius, lightAngle, shadowTex);
	lights.push_back(l);
	return l;
}

void GraphicsEngine::BuildNodeLists(SceneNode* from) {
	if (from->GetDrawableEntity() != NULL) {
	
		if (frameFrustum.InsideFrustum(*from)) {
			// Get distance from camera
			T3Vector3 dir = from->GetWorldTransform().GetPositionVector() - camera->GetPosition();
			from->SetCameraDistance(T3Vector3::Dot(dir, dir)); // gonna save ourselves a sqrt and compare distance^2

			T3Vector3 pos = from->GetWorldTransform().GetPositionVector();
			float boundingRadius = from->GetDrawableEntity()->GetBoundingRadius();
			if (pos.x - boundingRadius < boundingMin.x) boundingMin.x = pos.x - boundingRadius; 
			if (pos.y - boundingRadius < boundingMin.y) boundingMin.y = pos.y - boundingRadius; 
			if (pos.z - boundingRadius < boundingMin.z) boundingMin.z = pos.z - boundingRadius;
			if (pos.x + boundingRadius > boundingMax.x) boundingMax.x = pos.x + boundingRadius; 
			if (pos.y + boundingRadius > boundingMax.y) boundingMax.y = pos.y + boundingRadius; 
			if (pos.z + boundingRadius > boundingMax.z) boundingMax.z = pos.z + boundingRadius;

			if (from->GetColour().w < 1.0f)
				//transparent - add to transparent list
				transparentGameEntityList.push_back(from);
			else
				gameEntityList.push_back(from);
		}
	}

	// Note - gonna check children regardless - cause an arm might be inside the plane
	for (vector<SceneNode*>::const_iterator i = from->GetChildIteratorStart(); i < from->GetChildIteratorEnd(); ++i)
		BuildNodeLists(*i);
}

void GraphicsEngine::SortNodeLists() {
	// Sort both lists by order of distance from camera
	std::sort(transparentGameEntityList.begin(), transparentGameEntityList.end(), SceneNode::CompareByCameraDistance);
	std::sort(gameEntityList.begin(), gameEntityList.end(), SceneNode::CompareByCameraDistance);
}

void GraphicsEngine::DrawNodes() {
	// Draw opaque objects - closest to camera first
//	for (vector<SceneNode*>::const_iterator i = nodeList.begin(); i < nodeList.end(); ++i) renderer->Render(*i);

	// Draw transparent objects - furthest from camera first
//	for (vector<SceneNode*>::const_reverse_iterator i = transparentNodeList.rbegin(); i < transparentNodeList.rend(); ++i) renderer->Render(*i);
}

void GraphicsEngine::ClearNodeLists() {
	transparentGameEntityList.clear();
	gameEntityList.clear();
}

void GraphicsEngine::AddDrawable(DrawableEntity3D* drawable, DrawableEntity3D* parent) {
	
	contentGuard.lock_mutex();
	// TODO - is it worth checking node already in scene graph
	addGameList.push_back(pair<DrawableEntity3D*, DrawableEntity3D*>(drawable, parent));
	contentGuard.unlock_mutex();
}

void GraphicsEngine::RemoveDrawable(DrawableEntity3D* drawable, bool removeChildren) {
	contentGuard.lock_mutex();
	removeGameList.push_back(pair<DrawableEntity3D*, bool>(drawable, removeChildren));
	contentGuard.unlock_mutex();
}

void GraphicsEngine::AddTextureToScene(DrawableTexture2D* drawableTexture) {
	// TODO - is it worth checking already in list
	contentGuard.lock_mutex();
	addHudList.push_back(drawableTexture);
	contentGuard.unlock_mutex();
}

void GraphicsEngine::RemoveTextureFromScene(DrawableTexture2D* drawableTexture) {
	contentGuard.lock_mutex();
	removeHudList.push_back(drawableTexture);
	contentGuard.unlock_mutex();
}

void GraphicsEngine::AddDrawableTextToScene(DrawableText2D* drawableText) {
	// TODO - is it worth checking already in list
	contentGuard.lock_mutex();
	addHudList.push_back(drawableText);
	contentGuard.unlock_mutex();
}

void GraphicsEngine::RemoveDrawableTextFromScene(DrawableText2D* drawableText) {
	contentGuard.lock_mutex();
	removeHudList.push_back(drawableText);
	contentGuard.unlock_mutex();
}

void GraphicsEngine::AddLight(Light* light) {
	contentGuard.lock_mutex();
	addLightsList.push_back(light);
	contentGuard.unlock_mutex();
}

void GraphicsEngine::RemoveLight(Light* light) {
	contentGuard.lock_mutex();
	removeLightsList.push_back(light);
	contentGuard.unlock_mutex();
}

void GraphicsEngine::SetCamera(Camera* cam)
{
	camera = cam;
	renderer->SetCamera(cam);
}

bool GraphicsEngine::LoadContent()
{
	return (engine->renderer->LoadShaders() &&
				engine->renderer->LoadAssets());
}

void GraphicsEngine::UnloadContent()
{
	engine->renderer->UnloadShaders();
	engine->renderer->UnloadAssets();
}

void GraphicsEngine::DrawDeferredLights(bool on) {renderer->DrawDeferredLights(on);}

unsigned char* GraphicsEngine::GeneratePerlinNoise(const int resolution, unsigned char minValue, unsigned char maxValue)
{
	return renderer->GeneratePerlinNoise(resolution, minValue, maxValue);
}
#endif