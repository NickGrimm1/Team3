#include "GraphicsEngine.h"

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

Keyboard& GraphicsEngine::GetKeyboard() {
	return *(Window::GetKeyboard());
}

// TODO - deal with castsShadows
unsigned int GraphicsEngine::AddPointLight(Vector3 lightPosition, float lightRadius, Vector4 diffuseColour, Vector4 specularColour, bool castsShadow) {
	PointLight* l = new PointLight(lightPosition, diffuseColour, specularColour, lightRadius);
	return renderer->AddLight(*l); // TODO - get Sherif to accept pointers
}

unsigned int GraphicsEngine::AddDirectionalLight(Vector3 lightDirection, Vector4 diffuseColour, Vector4 specularColour, bool castsShadow) {
	DirectionalLight* l = new DirectionalLight(lightDirection, diffuseColour, specularColour);
	return renderer->AddLight(*l);
}

unsigned int GraphicsEngine::AddSpotLight(Vector3 lightPosition, Vector3 lightTarget, Vector3 upVector, float lightRadius, float lightAngle, Vector4 diffuseColour, Vector4 specularColour) {
	SpotLight* l = new SpotLight(lightPosition, lightTarget, upVector, diffuseColour, specularColour, lightRadius, lightAngle);
	return renderer->AddLight(*l);
}

bool GraphicsEngine::RemoveLight(unsigned int lightReference) {
	renderer->RemoveLight(lightReference);
}