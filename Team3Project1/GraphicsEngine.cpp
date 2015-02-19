#include "GraphicsEngine.h"
#include <algorithm>

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

GraphicsEngine::GraphicsEngine() {
	isInitialised = false;
	width = SCREEN_WIDTH;
	height = SCREEN_HEIGHT;

	if (!Window::Initialise(GAME_TITLE, width, height, false)) return;
	
	renderer = new Renderer(Window::GetWindow(), lights, gameEntityList, overlayElementsList);
	if (!renderer->HasInitialised()) return;

	isInitialised = true; // Graphics Engine has initialised successfully
}

GraphicsEngine::~GraphicsEngine() {
	// TODO
	// Delete remaining Texture data from OpenGL
	// Delete remaining Mesh data from OpenGL

	// Delete local array data

	delete renderer;
	Window::Destroy();
}

void GraphicsEngine::Run() {
	isRunning = true;
	
	while (isRunning) {
		//std::cout << "Graphics is Running";

		// TODO - add/remove requested items from scene lists

		renderer->UpdateScene(1.0f / RENDER_HZ); // TODO - use proper frame time

		// Update data in scene nodes
		sceneRoot->Update(1.0f / RENDER_HZ); // TODO - sort out proper timestep value - or remove timestep if not needed
		
		// Reset scene bounding box volume
		boundingMax = Vector3(0,0,0);
		boundingMin = Vector3(0,0,0);		

		if (camera != NULL)
		{
			Matrix4 viewMatrix = camera->BuildViewMatrix();
			Matrix4 projMatrix = Matrix4::Perspective(1.0f, 10000.0f, (float) width / (float) height, 45.0f);
			frameFrustum.FromMatrix(projMatrix * viewMatrix);
		}

		// Build Node lists
		BuildNodeLists(sceneRoot);
		SortNodeLists();
		
		// Update directional lights with scene bounding box
		// Transform bounding volume by camera transform
		if (camera != NULL) // Check if we have a camera - the game may not have initialised yet!!!
		{
			Matrix4 viewMatrix = camera->BuildViewMatrix();
			Vector3 camMin = viewMatrix * boundingMin;
			Vector3 camMax = viewMatrix * boundingMax;
			DirectionalLight::UpdateLightVolume(camMin.z, camMax.z, camMax.x, camMin.x, camMax.y, camMin.y);
		}

		// Sort HUD elements
		
		std::sort(overlayElementsList.begin(), overlayElementsList.end(), 
			[] (const void* a, const void* b) {
				return (((DrawableEntity2D*) a)->GetDepth() < ((DrawableEntity2D*) b)->GetDepth());
		});
	
		// Render data
		renderer->RenderScene();

		// Clear node lists in preparation for next render cycle
		ClearNodeLists();
	}
}

// TODO - deal with castsShadows
PointLight* GraphicsEngine::AddPointLight(Vector3 lightPosition, float lightRadius, Vector4 diffuseColour, Vector4 specularColour, bool castsShadow) {
	unsigned int shadowTex = castsShadow ? renderer->CreateShadowTexture() : 0;
	PointLight* l = new PointLight(lightPosition, diffuseColour, specularColour, lightRadius, shadowTex);
	lights.push_back(l);
	return l;
}

DirectionalLight* GraphicsEngine::AddDirectionalLight(Vector3 lightDirection, Vector4 diffuseColour, Vector4 specularColour, bool castsShadow) {
	unsigned int shadowTex = castsShadow ? renderer->CreateShadowTexture() : 0;
	DirectionalLight* l = new DirectionalLight(lightDirection, diffuseColour, specularColour, shadowTex);
	lights.push_back(l);
	return l;
}

SpotLight* GraphicsEngine::AddSpotLight(Vector3 lightPosition, Vector3 lightTarget, Vector3 upVector, float lightRadius, float lightAngle, Vector4 diffuseColour, Vector4 specularColour, bool castsShadow) {
	unsigned int shadowTex = castsShadow ? renderer->CreateShadowTexture() : 0;
	SpotLight* l = new SpotLight(lightPosition, lightTarget, upVector, diffuseColour, specularColour, lightRadius, lightAngle, shadowTex);
	lights.push_back(l);
	return l;
}

void GraphicsEngine::BuildNodeLists(SceneNode* from) {
	if (from->GetDrawableEntity() != NULL) {
	
		if (frameFrustum.InsideFrustum(*from)) {
			// Get distance from camera
			Vector3 dir = from->GetWorldTransform().GetPositionVector() - camera->GetPosition();
			from->SetCameraDistance(Vector3::Dot(dir, dir)); // gonna save ourselves a sqrt and compare distance^2

			Vector3 pos = from->GetWorldTransform().GetPositionVector();
			if (pos.x < boundingMin.x) boundingMin.x = pos.x; 
			if (pos.y < boundingMin.y) boundingMin.y = pos.y; 
			if (pos.z < boundingMin.z) boundingMin.z = pos.z;
			if (pos.x > boundingMax.x) boundingMax.x = pos.x; 
			if (pos.y > boundingMax.y) boundingMax.y = pos.y; 
			if (pos.z > boundingMax.z) boundingMax.z = pos.z;

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
	// TODO - is it worth checking node already in scene graph
	
	// if no parent - add drawable as child of sceneRoot
	if (parent == NULL) {
		sceneRoot->AddChild(new SceneNode(drawable));
	}
	else
	{
		sceneRoot->AddChildToParent(drawable, parent);
	}
}

void GraphicsEngine::RemoveDrawable(DrawableEntity3D* drawable, bool removeChildren) {
	sceneRoot->RemoveChild(drawable, true, removeChildren);
}

void GraphicsEngine::AddTextureToScene(DrawableTexture2D* drawableTexture) {
	// TODO - is it worth checking already in list
	overlayElementsList.push_back(drawableTexture);
}

void GraphicsEngine::RemoveTextureFromScene(DrawableTexture2D* drawableTexture) {
	for (auto i = overlayElementsList.begin(); i != overlayElementsList.end(); ++i) {
		if ((*i) == drawableTexture) {
			overlayElementsList.erase(i);
		}
	}
}

void GraphicsEngine::AddDrawableTextToScene(DrawableText2D* drawableText) {
	// TODO - is it worth checking already in list
	overlayElementsList.push_back(drawableText);
}

void GraphicsEngine::RemoveDrawableTextFromScene(DrawableText2D* drawableText) {
	for (auto i = overlayElementsList.begin(); i != overlayElementsList.end(); ++i) {
		if ((*i) == drawableText) {
			overlayElementsList.erase(i);
		}
	}
}

void GraphicsEngine::AddLight(Light* light) {
	lights.push_back(light);
}

void GraphicsEngine::RemoveLight(Light* light) {
	for (auto i = lights.begin(); i != lights.end(); ++i) {
		if ((*i) == light) {
			unsigned int shadowTex = light->GetShadowTexture();
			if (shadowTex > 0) 
				renderer->DestroyTexture(shadowTex);
			lights.erase(i);
		}
	}
}

void GraphicsEngine::SetCamera(Camera* cam)
{
	camera = cam;
	renderer->SetCamera(cam);
}