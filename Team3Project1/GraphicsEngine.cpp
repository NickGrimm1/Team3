#include "GraphicsEngine.h"
#include "GraphicsCommon.h"
#include "../Framework/T3Vector2.h"
#include "../Framework/T3Vector4.h"
#include <algorithm>
#include "GameStateManager.h"
#if PS3_BUILD
#include <sys/timer.h>
#endif

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

#if WINDOWS_BUILD
GraphicsEngine::GraphicsEngine() 
	: RENDER_TIME(1000.0f / RENDER_HZ)
#endif
#if PS3_BUILD
GraphicsEngine::GraphicsEngine()
	: Thread(&GraphicsEngine::threadExecution), RENDER_TIME(1000.0f / RENDER_HZ)
#endif
{
	//std::cout << "New Graphics Engine" << std::endl;
	isInitialised = false;
	width = SCREEN_WIDTH;
	height = SCREEN_HEIGHT;
	frameRate = 0;

#if WINDOWS_BUILD
	if (!Window::Initialise(GAME_TITLE, width, height, false)) return;
	
	Window::GetWindow().LockMouseToWindow(true);
	Window::GetWindow().ShowOSPointer(false);

	renderer = new Renderer(Window::GetWindow(), lights, gameEntityList, overlayElementsList);
	if (!renderer->HasInitialised()) return;
	
	GLuint loadingTex = renderer->CreateTexture(TEXTUREDIR"refresh.png", false, false, SOIL_FLAG_INVERT_Y);
	loadingTexture = new Texture(loadingTex);
#endif
#if PS3_BUILD
	
	renderer = new Renderer(lights, gameEntityList, overlayTexturesList, overlayTextsList);
	if(!renderer)
	{
		std::cout << "Renderer failed to load" << std::endl;
		return;
	}
	

	loadingTexture = new Texture("refresh", 0);
#endif

//	cout << "Loading Texture loaded" << endl;
	isLoading = true;
	isLoadingDrawing = false;
	
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

	renderer->SetLoadingIcon(loadingIcon);


	//cout << "Graphics Engine initialized" << endl;
	isInitialised = true; // Graphics Engine has initialised successfully
}

GraphicsEngine::~GraphicsEngine() {
	// TODO
	// Delete local array data - if not handled elsewhere

	// Fin
#if WINDOWS_BUILD
	renderer->DestroyTexture(loadingTexture->GetTextureName());
	Window::Destroy();
#endif
	delete loadingTexture;
	delete loadingIcon;
	delete renderer;
	delete sceneRoot;
	
}

#if PS3_BUILD
void GraphicsEngine::threadExecution(uint64_t arg)
{
	
	GraphicsEngine* myArg = (GraphicsEngine*)arg;
	myArg->Run();
}
#endif

void GraphicsEngine::Run() 
{
	isRunning = true;
	
	time = 0;
	inc = true;

	while (isRunning)
	{
#if WINDOWS_BUILD
		while (Window::GetWindow().GetTimer()->GetMS() - lastFrameTimeStamp < RENDER_TIME) { ; } // Fix the timestep
		float msec = (float) Window::GetWindow().GetTimer()->GetMS() - lastFrameTimeStamp;
		lastFrameTimeStamp = (float) Window::GetWindow().GetTimer()->GetMS();
		frameRate = (int)(1000.0f / msec);
#endif
#if PS3_BUILD
		while (GameStateManager::GetTimer()->GetMS() - lastFrameTimeStamp < RENDER_TIME)
			sys_timer_usleep(100);
 		float msec = (float)GameStateManager::GetTimer()->GetMS() - lastFrameTimeStamp;
		lastFrameTimeStamp = (float)GameStateManager::GetTimer()->GetMS();
		frameRate = (int)(1000.0f / msec);
#endif

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

#if WINDOWS_BUILD
		// Add/Remove HUD elements
		for (unsigned int i = 0; i < addHudList.size(); i++) {
			overlayElementsList.push_back(addHudList[i]);
		}
		addHudList.clear();

		for (unsigned int i = 0; i < removeHudList.size(); i++) {
			for (unsigned int j = 0; j < overlayElementsList.size(); ++j) {
				if (overlayElementsList[j] == removeHudList[i]) {
					overlayElementsList.erase(overlayElementsList.begin() + j);
				}
			}
		}
		removeHudList.clear();
#endif
#if PS3_BUILD
	// Add/Remove HUD elements
	for (unsigned int i = 0; i < addHudTextureList.size(); i++) 
	{
		//addHudTextureList[i]->SetDepth(-addHudTextureList[i]->GetDepth());
		overlayTexturesList.push_back(addHudTextureList[i]);
	}
	addHudTextureList.clear();
	// Add/Remove HUD elements
	for (unsigned int i = 0; i < addHudTextList.size(); i++) 
	{
		//addHudTextList[i]->SetDepth(-addHudTextList[i]->GetDepth());
		overlayTextsList.push_back(addHudTextList[i]);
	}
	addHudTextList.clear();

	for (unsigned int i = 0; i < removeHudTextureList.size(); i++) 
	{
		for (unsigned int j = 0; j < overlayTexturesList.size(); ++j) 
		{
			if (overlayTexturesList[j] == removeHudTextureList[i]) 
			{
				overlayTexturesList.erase(overlayTexturesList.begin() + j);
			}
		}
	}
	removeHudTextureList.clear();
	for (unsigned int i = 0; i < removeHudTextList.size(); i++) 
	{
		for (unsigned int j = 0; j < overlayTextsList.size(); ++j) 
		{
			if (overlayTextsList[j] == removeHudTextList[i]) 
			{
				overlayTextsList.erase(overlayTextsList.begin() + j);
			}
		}
	}
	removeHudTextList.clear();
#endif

	

		for (unsigned int i = 0; i < addLightsList.size(); i++) {
			lights.push_back(addLightsList[i]);
		}
		addLightsList.clear();

		// Add/Remove lights
		for (unsigned int l = 0; l < removeLightsList.size(); l++) {
			for (unsigned int i = 0; i < lights.size(); ++i) {
				if (lights[i] == removeLightsList[l]) {
					unsigned int shadowTex = lights[i]->GetShadowTexture();
#if WINDOWS_BUILD
					if (shadowTex > 0) 
						renderer->DestroyTexture(shadowTex);
#endif
					lights.erase(lights.begin() + i);
				}
			}
		}
		removeLightsList.clear();

		
			
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
#if WINDOWS_BUILD
			frameFrustum.FromMatrix(projMatrix * viewMatrix);
#endif
		

			// Build Node lists
			BuildNodeLists(sceneRoot);
			SortNodeLists();
		}
		// Update directional lights with scene bounding box - NOT WORKING
		// Transform bounding volume by camera transform
//#if WINDOWS_BUILD
//		DirectionalLight::UpdateLightVolume(boundingMin, boundingMax);
//#endif

		// Sort HUD elements
#if WINDOWS_BUILD
	std::sort(overlayElementsList.begin(), overlayElementsList.end(), SortDrawableEntity2D);
#endif
#if PS3_BUILD
	std::sort(overlayTexturesList.begin(), overlayTexturesList.end(), SortDrawableEntity2D);
	std::sort(overlayTextsList.begin(), overlayTextsList.end(), SortDrawableEntity2D);
#endif
	
#if WINDOWS_BUILD
		// Sort Point Lights to the front of the lights list
	std::sort(lights.begin(), lights.end(), SortLights);
#endif

	if (isLoading && !isLoadingDrawing) 
	{
#if WINDOWS_BUILD
			overlayElementsList.push_back(loadingIcon);
#endif
#if PS3_BUILD
		overlayTexturesList.push_back(loadingIcon);
#endif
			isLoadingDrawing = true;
		}
		else if (!isLoading && isLoadingDrawing) 
		{
#if WINDOWS_BUILD
			for (unsigned int i = 0; i < overlayElementsList.size(); i++) 
			{
				if (overlayElementsList[i] == loadingIcon) 
				{
					overlayElementsList.erase(overlayElementsList.begin() + i);
				}
			}
#endif
#if PS3_BUILD
			for (unsigned int i = 0; i < overlayTexturesList.size(); i++) 
			{
				if (overlayTexturesList[i] == loadingIcon) 
				{
					overlayTexturesList.erase(overlayTexturesList.begin() + i);
				}
			}
#endif
		}

		//Update the day/night float
		renderer->SetDayNight(DayNightCycle());

		// Render data
		renderer->RenderScene();

		// Clear node lists in preparation for next render cycle
		ClearNodeLists();

		contentGuard.unlock_mutex();
	}

#if PS3_BUILD
	sys_ppu_thread_exit (0);
#endif
}

PointLight* GraphicsEngine::AddPointLight(T3Vector3 lightPosition, float lightRadius, T3Vector4 diffuseColour, T3Vector4 specularColour, bool castsShadow) 
{
#if WINDOWS_BUILD
	unsigned int shadowTex = castsShadow ? renderer->CreateShadowCube() : 0;
	PointLight* l = new PointLight(lightPosition, diffuseColour, specularColour, lightRadius, shadowTex);
	lights.push_back(l);
	return l;
#endif
}

DirectionalLight* GraphicsEngine::AddDirectionalLight(T3Vector3 lightDirection, T3Vector4 diffuseColour, T3Vector4 specularColour)
{
#if WINDOWS_BUILD
	//unsigned int shadowTex = castsShadow ? renderer->CreateShadowTexture() : 0;
	DirectionalLight* l = new DirectionalLight(lightDirection, diffuseColour, specularColour, 0);
	lights.push_back(l);
	return l;
#endif
}

SpotLight* GraphicsEngine::AddSpotLight(T3Vector3 lightPosition, T3Vector3 lightTarget, T3Vector3 upVector, float lightRadius, float lightAngle, T3Vector4 diffuseColour, T3Vector4 specularColour, bool castsShadow)
{
#if WINDOWS_BUILD
	unsigned int shadowTex = castsShadow ? renderer->CreateShadowTexture() : 0;
	SpotLight* l = new SpotLight(lightPosition, lightTarget, upVector, diffuseColour, specularColour, lightRadius, lightAngle, shadowTex);
	lights.push_back(l);
	return l;
#endif
}

void GraphicsEngine::BuildNodeLists(SceneNode* from) {
	if (from->GetDrawableEntity() != NULL) {
	
#if WINDOWS_BUILD
		if (frameFrustum.InsideFrustum(*from)) 
		{
#endif
			// Get distance from camera
			T3Vector3 dir = from->GetWorldTransform().GetPositionVector() - camera->GetPosition();
			from->SetCameraDistance(T3Vector3::Dot(dir, dir)); // gonna save ourselves a sqrt and compare distance^2

			// Update scene AABB for directional lighting - NOT WORKING
			/*T3Vector3 pos = from->GetWorldTransform().GetPositionVector();
			float boundingRadius = from->GetDrawableEntity()->GetBoundingRadius();
			if (pos.x - boundingRadius < boundingMin.x) boundingMin.x = pos.x - boundingRadius; 
			if (pos.y - boundingRadius < boundingMin.y) boundingMin.y = pos.y - boundingRadius; 
			if (pos.z - boundingRadius < boundingMin.z) boundingMin.z = pos.z - boundingRadius;
			if (pos.x + boundingRadius > boundingMax.x) boundingMax.x = pos.x + boundingRadius; 
			if (pos.y + boundingRadius > boundingMax.y) boundingMax.y = pos.y + boundingRadius; 
			if (pos.z + boundingRadius > boundingMax.z) boundingMax.z = pos.z + boundingRadius;*/

//			if (from->GetColour().w < 1.0f)	 - 3D TRANSPARENCIES NOT IMPLEMENTED
				//transparent - add to transparent list
//				transparentGameEntityList.push_back(from);
//			else
				gameEntityList.push_back(from);
#if WINDOWS_BUILD
		}
#endif
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
#if WINDOWS_BUILD
	addHudList.push_back(drawableTexture);
#endif
#if PS3_BUILD
	addHudTextureList.push_back(drawableTexture);
#endif
	contentGuard.unlock_mutex();
}

void GraphicsEngine::RemoveTextureFromScene(DrawableTexture2D* drawableTexture) {
	contentGuard.lock_mutex();
#if WINDOWS_BUILD
	removeHudList.push_back(drawableTexture);
#endif
#if PS3_BUILD
	removeHudTextureList.push_back(drawableTexture);
#endif
	contentGuard.unlock_mutex();
}

void GraphicsEngine::AddDrawableTextToScene(DrawableText2D* drawableText) {
	// TODO - is it worth checking already in list
	contentGuard.lock_mutex();
#if WINDOWS_BUILD
	addHudList.push_back(drawableText);
#endif
#if PS3_BUILD
	addHudTextList.push_back(drawableText);
#endif
	contentGuard.unlock_mutex();
}

void GraphicsEngine::RemoveDrawableTextFromScene(DrawableText2D* drawableText) {
	contentGuard.lock_mutex();
#if WINDOWS_BUILD
	removeHudList.push_back(drawableText);
#endif
#if PS3_BUILD
	removeHudTextList.push_back(drawableText);
#endif
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
	contentGuard.lock_mutex();
	camera = cam;
	renderer->SetCamera(cam);
	contentGuard.unlock_mutex();
}

bool GraphicsEngine::LoadContent()
{
	return engine->renderer->LoadAssets();
}

void GraphicsEngine::UnloadContent()
{
	engine->renderer->UnloadAssets();
}

void GraphicsEngine::DrawDeferredLights(bool on) {renderer->DrawDeferredLights(on);}

unsigned char* GraphicsEngine::GeneratePerlinNoise(const int resolution, unsigned char minValue, unsigned char maxValue)
{
	return renderer->GeneratePerlinNoise(resolution, minValue, maxValue);
}

float GraphicsEngine::DayNightCycle() {
	float out;

	//Decide if day/night is transitioning
	if (time > 4500 && time < 5500) {//Halfway through the cycle, it is changing.
		out = time - 4500.0f;
		out /= 1000.0f;
	}
	else if (time < 4500)	//day
		out = 0.0f;
	else					//Night
		out = 1.0f;

	if (inc) {
		if (time >= 10000) {
			inc = false;
			time--;
		}
		else
			time++;
	}
	else {
		if (time <= 0) {
			inc = true;
			time++;
		}
		else
			time--;
	}

	return out;	
}
