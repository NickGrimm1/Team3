#include "GraphicsEngine.h"

#include <algorithm>

bool GraphicsEngine::Initialize() {
	if (engine != NULL) return false; // an engine instance already exists, can't create another
	
	// Otherwise, create new engine
	engine = new GraphicsEngine();
	return engine->HasInitialised();
}

bool GraphicsEngine::Destroy() {

	if (engine == NULL) return false; // no engine instance to destroy

	delete engine;
	return true;
}

GraphicsEngine::GraphicsEngine() {
	isInitialised = false;
	
	if (!Window::Initialise(GAME_TITLE, SCREEN_WIDTH, SCREEN_HEIGHT, false)) return;
	
	renderer = new Renderer(Window::GetWindow());
	if (!renderer->HasInitialised()) return;

	isInitialised = true; // Graphics Engine has initialised successfully
}

bool GraphicsEngine::StartGraphicsEngineThread() {


}

void GraphicsEngine::GraphicsEngineThread() {
	while (true) {
		// Update data in scene nodes
		BuildNodeLists(sceneRoot);
		SortNodeLists();
		
		// Render data
		DrawNodes();

		// Clear node lists in preparation for next render cycle
		ClearNodeLists();
	}
}

Keyboard& GraphicsEngine::GetKeyboard() {
	return *(Window::GetKeyboard());
}

// TODO - deal with castsShadows
unsigned int GraphicsEngine::AddPointLight(Vector3 lightPosition, float lightRadius, Vector4 diffuseColour, Vector4 specularColour, bool castsShadow) {
	if (numLights >= MAX_LIGHTS) return -1; // no more space for lights
	PointLight* l = new PointLight(lightPosition, diffuseColour, specularColour, lightRadius);
	lightArray[numLights] = l;
	return numLights++;
}

unsigned int GraphicsEngine::AddDirectionalLight(Vector3 lightDirection, Vector4 diffuseColour, Vector4 specularColour, bool castsShadow) {
	if (numLights >= MAX_LIGHTS) return -1; // no more space for lights
	DirectionalLight* l = new DirectionalLight(lightDirection, diffuseColour, specularColour);
	lightArray[numLights] = l;
	return numLights++;
}

unsigned int GraphicsEngine::AddSpotLight(Vector3 lightPosition, Vector3 lightTarget, Vector3 upVector, float lightRadius, float lightAngle, Vector4 diffuseColour, Vector4 specularColour) {
	if (numLights >= MAX_LIGHTS) return -1; // no more space for lights
	SpotLight* l = new SpotLight(lightPosition, lightTarget, upVector, diffuseColour, specularColour, lightRadius, lightAngle);
	lightArray[numLights] = l;
	return numLights++;
}

bool GraphicsEngine::RemoveLight(unsigned int lightReference) {
	
}

void GraphicsEngine::BuildNodeLists(SceneNode* from) {

	if (frameFrustum.InsideFrustum(*from)) {
		// Get distance from camera
		Vector3 dir = from->GetWorldTransform().GetPositionVector() - camera->GetPosition();
		from->SetCameraDistance(Vector3::Dot(dir, dir)); // gonna save ourselves a sqrt and compare distance^2

		if (from->GetColour().w < 1.0f) 
			//transparent - add to transparent list
			transparentNodeList.push_back(from);
		else
			nodeList.push_back(from);
	}

	// Note - gonna check children regardless - cos an arm might be inside the plane
	for (vector<SceneNode*>::const_iterator i = from->GetChildIteratorStart(); i < from->GetChildIteratorEnd(); ++i)
		BuildNodeLists(*i);
}

void GraphicsEngine::SortNodeLists() {
	// Sort both lists by order of distance from camera
	std::sort(transparentNodeList.begin(), transparentNodeList.end(), SceneNode::CompareByCameraDistance);
	std::sort(nodeList.begin(), nodeList.end(), SceneNode::CompareByCameraDistance);
}

void GraphicsEngine::DrawNodes() {
	// Draw opaque objects - closest to camera first
	for (vector<SceneNode*>::const_iterator i = nodeList.begin(); i < nodeList.end(); ++i) renderer->Render(*i);

	// Draw transparent objects - furthest from camera first
	for (vector<SceneNode*>::const_reverse_iterator i = transparentNodeList.rbegin(); i < transparentNodeList.rend(); ++i) renderer->Render(*i);
}

void GraphicsEngine::ClearNodeLists() {
	transparentNodeList.clear();
	nodeList.clear();
}