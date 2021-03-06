/******************************************************************************
Class:GCMRenderer
Implements:
Author:Rich Davison
Description: As with the Graphics for Games OGLRenderer class, this class
wraps up all of the PS3 native GCM functions, and sets up a basic GCM rendered
scene. Derive your renderer class from this!

Much of this class is covered in the GCM tutorial PDF, but this year has been 
moved into separate classes a bit better


-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////

#if PS3_BUILD
#pragma once
#include <sysutil/sysutil_sysparam.h>			//And know about cellVideoOut etc
#include <vectormath/cpp/vectormath_aos.h>
#include <cell/gcm.h>
#include <Cg/cg.h>
#include <fstream>

#include "../Framework/Camera.h"
#include "../Framework/Shader.h"
#include "../Framework/SceneNode.h"
#include "Mesh.h"
#include "Texture.h"

//#include "../Framework/OBJMesh.h"
using namespace Vectormath::Aos;
using namespace cell::Gcm;

#define COMMAND_SIZE	(65536)			//64KB
#define BUFFER_SIZE		(1024*1024)		//1MB

class VertexShader;
class FragmentShader;
class SceneNode;
class Camera;

enum GCMResolution {
	GCM_RESOLUTION_480  = 4,
	GCM_RESOLUTION_576  = 5,
	GCM_RESOLUTION_720  = 2,
	GCM_RESOLUTION_1080 = 1,
	GCM_RESOLUTION_NONE = 0,

	GCM_RESOLUTION_MAX  = 6
};

enum Buffers
{
	GCM_FRONT_BUFFER	= 0,
	GCM_BACK_BUFFER		= 1,
	GCM_SKYBOX_BUFFER	= 2,
	GCM_TEXTURE_BUFFER	= 3, 
	GCM_SHADOW_BUFFER	= 4,
	GCM_LIGHT_BUFFER	= 5,
	GCM_MAX_BUFFERS		= 6
};


//typedef struct {
//	uint32_t Version;
//	uint32_t Size;
//	uint32_t NumTexture;
//} CellGtfFileHeader;
//
//typedef struct {
//	uint32_t Id;
//	uint32_t OffsetToTex;
//	uint32_t TextureSize;
//	CellGcmTexture tex;
//} CellGtfTextureAttribute;

class GCMRenderer	{
public:
	GCMRenderer(void);
	virtual ~GCMRenderer(void);

	/*
	Graphics memory allocation. A fully 'dynamic' engine is going to have to
	implement a proper memory management strategy, but you can probably get
	away with just filling up graphics memory for the project.
	*/
	static void*	localMemoryAlloc(const uint32_t size);
	static void*	localMemoryAlign(const uint32_t alignment, const uint32_t size);
	static void		setLocalMem(uint32_t to);

	virtual void RenderScene() = 0;

	void	SetCamera(Camera* n);
	void	SetRootNode(SceneNode * r);

	void	DrawNode(SceneNode*n);

	//static CellGcmTexture* LoadTGA(std::string name);
	//static CellGcmTexture* LoadGTF(std::string name);

	Shader* GetCurrentShader()const{return shader;}

protected:
	void	SetCurrentShader(Shader* shader);
	void	SetTextureSampler(CGparameter sampler, const CellGcmTexture *texture);

	void ClearBuffer(); //Clear the screen
	void SwapBuffers(); //Swap buffers
	void InitDisplay(); //Initialise the display and buffers
	void InitSurfaces();//Initialise the render surfaces
	void SetViewport(); //Set the viewport to default

	bool SetResolution(GCMResolution resolution);

	//You should REALLY know what these are, by now.
	Vectormath::Aos::Matrix4 modelMatrix;
	Vectormath::Aos::Matrix4 viewMatrix;
	Vectormath::Aos::Matrix4 projMatrix;
	Vectormath::Aos::Matrix4 textureMatrix;

	//Pointer to the start of graphics memory
	static uint32_t		localHeapStart;

	//Which of the two buffers is current
	bool				swapValue;

	//Screen width, height, and ratio, obviously.
	int			screenWidth;
	int			screenHeight;
	float		screenRatio;

	//Remember how in Graphics for Games, you all got confused by
	//currentShader, and kept setting it wrong and breaking
	//everything? Now there's two shaders to keep track of, have
	//fun!
	//FragmentShader*		currentFrag;	//currently assigned fragment shader
	//VertexShader*		currentVert;	//currently assigned vertex shader
	Shader*			shader;
	Camera*			camera;			//Current viewpoint origin
	SceneNode*		root;			//The scenegraph the renderer is drawing

	CellGcmSurface surfaces[2];			//Front and back buffers
	CellGcmSurface graphicsSurfaces[4]; // graphics buffers	

	unsigned int depthOffset;			//Pointer to our depth buffer
	unsigned int depthPitch;			//Pitch of our depth buffer

	Texture* texture;
};
#endif
