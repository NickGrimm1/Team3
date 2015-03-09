#if PS3_BUILD
#pragma once
#include "GraphicsEngine.h"

/*
*Ps3 Graphics Engine implementation
*Initial Author: Matthew Johnson
*
*
*/
GraphicsEngine* GraphicsEngine::engine = NULL;
bool GraphicsEngine::Initialize(GraphicsEngine*& out)
{
	if(engine != NULL)
	{
		return engine->HasInitialised(); // an engine instance already exists
	}
	//else create a new engine
	engine = new GraphicsEngine();
	engine->sceneRoot = new SceneNode();
	engine->camera = NULL;

	out = engine;
	return engine->HasInitialised();
}

bool GraphicsEngine::LoadContent()
{
	return (engine->renderer->LoadShaders() && engine->renderer->LoadAssets()); 
}

void GraphicsEngine::UnloadContent()
{
	engine->renderer->UnloadShaders();
	engine->renderer->UnloadAssets();
}

bool GraphicsEngine::Destroy()
{
	if(engine == NULL)
	{
		return true;
	}

	delete engine;
	engine = NULL;

	return true;
}

void GraphicsEngine::Run()
{
	std::cout << "Graphics Thread Started! " << std::endl;
	engine->isRunning = true;
	while (engine->isRunning)
	{
		/*if(addGameList.empty())
		{
			cout << " NOTHING IS ANYWHERE D: D: D:" << endl;
		}*/
		cout << "addGameList.size() = " << addGameList.size() << endl;
		for (unsigned int i = 0; i < addGameList.size(); ++i)
		{
			cout << "adding game entities" << endl;
			if(addGameList[i].second == NULL)
			{
				cout << "adding game entities1" << endl;
				sceneRoot->AddChild(new SceneNode(addGameList[i].first));
			}
			else
			{
				cout << "adding game entities2" << endl;
				sceneRoot->AddChildToParent(addGameList[i].first, addGameList[i].second);
			}
		}
		GameTimer* msec = new GameTimer();
		addGameList.clear();
		sceneRoot->Update(msec->GetMS());
		BuildNodeLists(sceneRoot);
		SortNodeLists();
		std::cout << "Graphics Thread Rendering! " << std::endl;
		engine->renderer->RenderScene();
	}
	std::cout << "Graphics Thread Ended! " << std::endl;
	sys_ppu_thread_exit (0);
}

 void GraphicsEngine::threadExecution(uint64_t arg)
{
	GraphicsEngine* myArg = (GraphicsEngine*)arg;
	myArg->Run();
}

/*may need to make these methods thread safe for PS3*/
void GraphicsEngine::AddTextureToScene(DrawableTexture2D* drawableTexture)
{
	addHudList.push_back(drawableTexture);
}

void GraphicsEngine::RemoveTextureFromScene(DrawableTexture2D* drawableTexture)
{
	removeHudList.push_back(drawableTexture);
}

void GraphicsEngine::AddDrawableTextToScene(DrawableText2D* drawableText)
{
	addHudList.push_back(drawableText);
}

void GraphicsEngine::RemoveDrawableTextFromScene(DrawableText2D* drawableText)
{
	removeHudList.push_back(drawableText);
}

void GraphicsEngine::AddDrawable(DrawableEntity3D* drawable, DrawableEntity3D* parent )
{
	addGameList.push_back(pair<DrawableEntity3D*, DrawableEntity3D*>(drawable, parent));
}

void GraphicsEngine::RemoveDrawable(DrawableEntity3D* drawable, bool removeChildren)
{
	removeGameList.push_back(pair<DrawableEntity3D*, bool>(drawable, removeChildren));
}

void GraphicsEngine::RemoveLight(Light* light)
{
	removeLightsList.push_back(light);
}

void GraphicsEngine::SetCamera(Camera* cam)
{
	camera = cam;
	renderer->SetCamera(cam);
}

PointLight* GraphicsEngine::AddPointLight(T3Vector3 lightPosition, float lightRadius, T3Vector4 diffuseColour, T3Vector4 specularColour, bool castsShadow)
{
	//if cast shadow, create shadow cube
	/*unsigned int shadowTex = castsShadow ? renderer->CreateShadowCube() : 0;
	PointLight* l = new PointLight(lightPosition, diffuseColour, specularColour, lightRadius, shadowTex);
	lights.push_back(l);
	return l;*/
}

DirectionalLight* GraphicsEngine::AddDirectionalLight(T3Vector3 lightDirection, T3Vector4 diffuseColour, T3Vector4 specularColour)
{
	/*DirectionalLight* l = new DirectionalLight(lightDirection, diffuseColour, specularColour, 0);
	lights.push_back(l);
	return l;*/
}

SpotLight* GraphicsEngine::AddSpotLight(T3Vector3 lightPosition, T3Vector3 lightTarget, T3Vector3 upVector, float lightRadius, float lightAngle, T3Vector4 diffuseColour, T3Vector4 specularColour, bool castsShadow)
{
	//unsigned int shadowTex = castsShadow ? renderer->CreateShadowTexture() : 0;
	//SpotLight* l = new SpotLight(lightPosition, lightTarget, upVector, diffuseColour, specularColour, lightRadius, lightAngle, shadowTex);
	//lights.push_back(l);
	//return l;
}

void GraphicsEngine::DrawDeferredLights(bool on)
{
	renderer->DrawDeferredLights(on);
}

GraphicsEngine::GraphicsEngine()
	: Thread(&GraphicsEngine::threadExecution), RENDER_TIME(1000.0f / 60.0f)
{
	//initialise the renderer
	isInitialised = false;
	if(gameEntityList.empty()){cout << "gameEntityList is empty for reals" << endl;}
	renderer = new Renderer(lights,gameEntityList,overlayElementsList);
	if(!renderer)
	{
		return;
	}
	isInitialised = true;
	
}

GraphicsEngine::~GraphicsEngine()
{
	//clean up engine
	delete renderer;
}

void GraphicsEngine::BuildNodeLists(SceneNode* from)
{
	cout << "Graphics Engine: attempting to build node list mf" << endl;
	if(from->GetDrawableEntity() != NULL)
	{
		cout << "Graphics Engine: Have a drawable entity" << endl;
			T3Vector3 dir = from->GetWorldTransform().GetPositionVector() - camera->GetPosition();
			from->SetCameraDistance(T3Vector3::Dot(dir,dir));
			cout << "Graphics Engine: got world transform and set camera" << endl;
			T3Vector3 pos = from->GetWorldTransform().GetPositionVector() - camera->GetPosition();
			float boundingRadius = from->GetDrawableEntity()->GetBoundingRadius();
			cout << "Graphics Engine: got world transform and bounding radius" << endl;
			/*if (pos.x - boundingRadius < boundingMin.x) 
			{
				boundingMin.x = pos.x - boundingRadius;
			}
			if (pos.y - boundingRadius < boundingMin.y)
			{
				boundingMin.y = pos.y - boundingRadius; 
			}
			if (pos.z - boundingRadius < boundingMin.z) 
			{
				boundingMin.z = pos.z - boundingRadius;
			}
			if (pos.x + boundingRadius > boundingMax.x) 
			{
				boundingMax.x = pos.x + boundingRadius;
			}
			if (pos.y + boundingRadius > boundingMax.y) 
			{
				boundingMax.y = pos.y + boundingRadius; 
			}
			if (pos.z + boundingRadius > boundingMax.z)
			{
				boundingMax.z = pos.z + boundingRadius;
			}
*/
			if(from->GetColour().w < 1.0f)
			{
				//if transparent add to transparent entity list
				transparentGameEntityList.push_back(from);
			}
			else
			{
				gameEntityList.push_back(from);
			}
	}
	cout << "scene node Children: " << from->GetChildren() << endl;
	for(vector<SceneNode*>::const_iterator i = from->GetChildIteratorStart(); i < from->GetChildIteratorEnd(); ++i)
	{
			BuildNodeLists(*i);
	}
}

void GraphicsEngine::SortNodeLists()
{
	std::sort(transparentGameEntityList.begin(), transparentGameEntityList.end(), SceneNode::CompareByCameraDistance);
	std::sort(gameEntityList.begin(), gameEntityList.end(), SceneNode::CompareByCameraDistance);
}

void GraphicsEngine::ClearNodeLists()
{
	transparentGameEntityList.clear();
	gameEntityList.clear();
}
void GraphicsEngine::DrawNodes()
{
		// Draw opaque objects - closest to camera first
//	for (vector<SceneNode*>::const_iterator i = nodeList.begin(); i < nodeList.end(); ++i) renderer->Render(*i);

	// Draw transparent objects - furthest from camera first
//	for (vector<SceneNode*>::const_reverse_iterator i = transparentNodeList.rbegin(); i < transparentNodeList.rend(); ++i) renderer->Render(*i);
}

void GraphicsEngine::AddLight(Light* light)
{
	addLightsList.push_back(light);
}

unsigned char* GraphicsEngine::GeneratePerlinNoise(const int resolution, unsigned char minValue, unsigned char maxValue)
{
	return NULL;
}
#endif
