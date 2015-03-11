#if PS3_BUILD

#include "GCMRenderer.h"

//Static member variables need initialising!
uint32_t		GCMRenderer::localHeapStart	= 0;

GCMRenderer::GCMRenderer(void)	{
	if(cellGcmInit(COMMAND_SIZE, BUFFER_SIZE, memalign(1024*1024, BUFFER_SIZE))!= CELL_OK) 
		return;

	camera	= NULL;
	root	= NULL;
	shader = NULL;
	InitDisplay();
	InitSurfaces();
}

/*
This function is as it was in the GCM tutorial text. It sets the
display up, and also grabs the pointer to the start of graphics
memory. It'll also reserve enough memory for the front, back, and depth
buffers. 
*/
void GCMRenderer::InitDisplay()
{
	if(SetResolution(GCM_RESOLUTION_1080))
		;
	else if(SetResolution(GCM_RESOLUTION_720))
		;
	else if(SetResolution(GCM_RESOLUTION_576))
		;
	else if(SetResolution(GCM_RESOLUTION_480))
		;
	else 
		return;

	CellVideoOutState			vid_state;
	CellVideoOutResolution		resolution;

	cellVideoOutGetState(CELL_VIDEO_OUT_PRIMARY, 0, &vid_state);				//Query the current state of the video output
	cellVideoOutGetResolution(vid_state.displayMode.resolutionId, &resolution);	//Query the current state of the video resolution

	screenWidth			= resolution.width;
	screenHeight		= resolution.height;
	screenRatio			= (float)screenWidth / (float)screenHeight;



	//Configure the video output...
	CellVideoOutConfiguration	video_cfg;
	memset(&video_cfg, 0, sizeof(CellVideoOutConfiguration));
	video_cfg.resolutionId	= vid_state.displayMode.resolutionId;
	video_cfg.format		= CELL_VIDEO_OUT_BUFFER_COLOR_FORMAT_X8R8G8B8;
	video_cfg.pitch			= screenWidth*4;

	cellVideoOutConfigure(CELL_VIDEO_OUT_PRIMARY,	&video_cfg, NULL, 0);
	cellGcmSetFlipMode(CELL_GCM_DISPLAY_VSYNC);	//V-sync, aka swapinterval

	//Get the pointer to the start of graphics memory, as we need it to 
	//allocate the front, back, and depth buffers. 
	CellGcmConfig config;
	cellGcmGetConfiguration(&config);
	setLocalMem((uint32_t)config.localAddress);

	//gCellGcmDebugCheckCallback = my_check_callback;
	//cellGcmDebugCheckEnable(CELL_GCM_TRUE);
}

/*
Surfaces are GCMs implementation of Frame Buffers. In fact, in GCM, even the
front and back buffers are 'FBOs'. We could probably have a single surface,
and swap the colour target for the back and front buffer, but it's easier to
just have a surface for the two buffers, and simply swap between them. 

Surfaces can have 4 colour attachments; attachments are turned off by giving
them a memory offset of 0.

Note: We set the memory offset for the first colour attachment on line 63, in
the init_display function.
*/
void GCMRenderer::InitSurfaces()	
{
	//Memory for these buffers has to be 64-byte aligned!
	depthPitch = screenWidth*4;
	uint32_t depthSize	= depthPitch*screenHeight;
	void* depthBuffer	= localMemoryAlign(64, depthSize);
	cellGcmAddressToOffset(depthBuffer, &depthOffset);

	uint32_t colourPitch	=	screenWidth*4;	//Pitch is defined as (width * bytes per pixel)
	uint32_t colourSize	=	colourPitch*screenHeight;

	for(int i = 0; i < 2; ++i) {
		surfaces[i].colorPitch[0] = colourPitch;
		void*buffer = localMemoryAlign(64, colourSize);	//Allocate enough memory for the buffer
		cellGcmAddressToOffset(buffer, &surfaces[i].colorOffset[0]);	//and calculate the offset
		//Attach the memory to a surface
		cellGcmSetDisplayBuffer(i, surfaces[i].colorOffset[0], colourPitch, screenWidth, screenHeight);

		//Disable the other colour attachments for this surface
		for(int j = 1; j < 4; ++j) {
			surfaces[i].colorLocation[j]	=	CELL_GCM_LOCATION_LOCAL;
			surfaces[i].colorOffset[j]		=	0;
			surfaces[i].colorPitch[j]		=	64;
		}

		surfaces[i].colorFormat 		= CELL_GCM_SURFACE_A8R8G8B8;
		surfaces[i].colorTarget			= CELL_GCM_SURFACE_TARGET_0;
		surfaces[i].colorLocation[0]	= CELL_GCM_LOCATION_LOCAL;

		surfaces[i].depthFormat 		= CELL_GCM_SURFACE_Z24S8;
		surfaces[i].depthLocation		= CELL_GCM_LOCATION_LOCAL;

		//Both surfaces are sharing the same depth buffer. 
		surfaces[i].depthOffset			= depthOffset;
		surfaces[i].depthPitch 			= depthPitch;

		surfaces[i].type				= CELL_GCM_SURFACE_PITCH;
		surfaces[i].antialias			= CELL_GCM_SURFACE_CENTER_1;

		surfaces[i].width 				= screenWidth;
		surfaces[i].height 				= screenHeight;
		surfaces[i].x 					= 0;
		surfaces[i].y 					= 0;
	}
	/**Initialise Graphics Buffers**/
	//for(int i = 0; i < 4; ++i) {
	//	graphicsSurfaces[i].colorPitch[0] = colourPitch;
	//	void*buffer = localMemoryAlign(64, colourSize);	//Allocate enough memory for the buffer
	//	cellGcmAddressToOffset(buffer, &graphicsSurfaces[i].colorOffset[0]);	//and calculate the offset
	//	//Attach the memory to a surface
	//	cellGcmSetDisplayBuffer(i, graphicsSurfaces[i].colorOffset[0], colourPitch, screenWidth, screenHeight);

	//	//Disable the other colour attachments for this surface
	//	for(int j = 1; j < 4; ++j) {
	//		graphicsSurfaces[i].colorLocation[j]	=	CELL_GCM_LOCATION_LOCAL;
	//		graphicsSurfaces[i].colorOffset[j]		=	0;
	//		graphicsSurfaces[i].colorPitch[j]		=	64;
	//	}

	//	graphicsSurfaces[i].colorFormat 		= CELL_GCM_SURFACE_A8R8G8B8;
	//	graphicsSurfaces[i].colorTarget			= CELL_GCM_SURFACE_TARGET_0;
	//	graphicsSurfaces[i].colorLocation[0]	= CELL_GCM_LOCATION_LOCAL;

	//	graphicsSurfaces[i].depthFormat 		= CELL_GCM_SURFACE_Z24S8;
	//	graphicsSurfaces[i].depthLocation		= CELL_GCM_LOCATION_LOCAL;

	//	//Both surfaces are sharing the same depth buffer. 
	//	//TODO - fix this before it causes ps3 drama
	//	graphicsSurfaces[i].depthOffset			= depthOffset;
	//	graphicsSurfaces[i].depthPitch 			= depthPitch;

	//	graphicsSurfaces[i].type				= CELL_GCM_SURFACE_PITCH;
	//	graphicsSurfaces[i].antialias			= CELL_GCM_SURFACE_CENTER_1;

	//	graphicsSurfaces[i].width 				= screenWidth;
	//	graphicsSurfaces[i].height 				= screenHeight;
	//	graphicsSurfaces[i].x 					= 0;
	//	graphicsSurfaces[i].y 					= 0;
	//}

	//This is how we tell GCM to render into a surface, like we do in OpenGL
	//using the function glBindFramebuffer.
	cellGcmSetSurface(&surfaces[0]);
}

/*
Render out the final frame, and quit GCM.
*/
GCMRenderer::~GCMRenderer(void)	{
	cellGcmSetWaitFlip();
	cellGcmFinish(1);	// Let PPU wait for all commands done (include waitFlip)
}

/*
In GCM, all you are provided with is a pointer to graphics memory - all we can do
is advance that pointer along as we allocate new memory. We could write a fancy
memory manager, but that's left up to you, if you need it! (One such system could
be implemented with a doubly linked list of a simple data structure to keep track
of start pointer and size....enough help!)
*/

/*
Allocate some amount of graphics memory. Returns a pointer to the start of the 
allocated area.
*/
void* GCMRenderer::localMemoryAlloc(const uint32_t size)	{
	uint32_t currentHeap = localHeapStart;		//store current memory pointer
	localHeapStart += (size + 1023) & (~1023);	//advance the memory pointer
	return (void*)currentHeap;					//return the temp 'old' memory pointer
}

/*
Allocate an ALIGNED area of graphics memory. Certain GCM data structures (fragment shaders, textures)
must be at a certain alignment, or they'll not work correctly. This function uses localMemoryAlloc,
but 'twiddles' the graphics pointer to advance it to the next valid alignment position first. 
*/
void* GCMRenderer::localMemoryAlign(const uint32_t alignment, const uint32_t size)	{
	localHeapStart = (localHeapStart + alignment-1) & (~(alignment-1));
	return (void*)localMemoryAlloc(size);
}

/*
Explicitly set the location in graphics memory the pointer is pointing at. I can't remember
why I added this function, or made it public...it's pretty dangerous.
*/
void GCMRenderer::setLocalMem(uint32_t to) {
	localHeapStart = to;
}

/*
Sets up the current viewport
*/
void GCMRenderer::SetViewport() 
{
	uint16_t x;
	uint16_t y;
	uint16_t w;
	uint16_t h;
	float minimum;
	float maximum;
	float scale[4];
	float offset[4];

	/*
	I kinda gloss over what this function does in the GCM tutorial text,
	but if you can't be bothered to look up the cellGcmSetViewport SDK
	documentation, here's basically what it does. 

	Remember how in the Introduction to Graphics tutorial, it was explained
	that at the 'end' of the pipeline, your graphics end up in 'Native Device'
	space, which has an axis ranging from -1 to 1? The viewport function must
	then map those -1 to 1 axis coordinates to your screen space. Internally 
	to GCM, this will be done with another matrix.

	To map an x axis range of -1 to 1 to screen coordinates, we can multiply
	the x axis by half the width (assuminimumg a default width of 720, that gives
	us a new range of -360 to 360), and then translating / offsetting it also
	by half of the width (moving us to 0 to 720). The same is done with the Y
	axis, while the Z axis goes from 0.0 to 1.0 (remember the depth buffer
	values?)

	Note also the y axis scaling is inverted - NDC has <MAX AXIS VALUE> at the 
	'top', whereas our screenspace coordinates have <MIN AXIS VALUE> at the 
	'top', so we can 'flip' the image the right way up with a negative scale.
	*/

	x = 0;	//starting position of the viewport (left of screen)
	y = 0;  //starting position of the viewport (top of screen)
	w = screenWidth;
	h = screenHeight;
	minimum = 0.0f;
	maximum = 1.0f;
	//Scale our NDC coordinates to the size of the screen
	scale[0] = w * 0.5f;
	scale[1] = h * -0.5f;
	scale[2] = (maximum - minimum) * 0.5f;
	scale[3] = 0.0f;

	//Translate from a range starting from -1 to a range starting at 0
	offset[0] = x + scale[0];
	offset[1] = y + h * 0.5f;
	offset[2] = (maximum + minimum) * 0.5f;
	offset[3] = 0.0f;

	//analogous to the glViewport function...but with extra values!
	cellGcmSetViewport(x, y, w, h, minimum, maximum, scale, offset);
}

void GCMRenderer::SwapBuffers() 
{
	// wait until FlipStatus = 0 so that PPU does not run too ahead of RSX
	// FlipStatus turns to 0 when the previous flip is finished
	while (cellGcmGetFlipStatus() != 0)
		sys_timer_usleep(200);

	// reset FlipStatus = 1
	cellGcmResetFlipStatus();

	// queue Flip command
	cellGcmSetFlip((uint8_t)swapValue);
	//Force all commands up to and including the flip command to be processed
	cellGcmFlush();

	// wait the flip is done before start rendering to the other buffer
	cellGcmSetWaitFlip();

	//Invert the boolean 
	swapValue = !swapValue;
	//Use the bool as an index into our array of surfaces (a bit naughty!)
	cellGcmSetSurface(&surfaces[swapValue]);
}

/*
Clears the screen. Technically 'clears the current buffer', but that'll always be
the 'back buffer' when everything is used properly. 
*/
void GCMRenderer::ClearBuffer() {

	cellGcmSetColorMask(
		CELL_GCM_COLOR_MASK_R|
		CELL_GCM_COLOR_MASK_G|
		CELL_GCM_COLOR_MASK_B|
		CELL_GCM_COLOR_MASK_A);

	//glClearColor, but taking a single 32 bit value, in ARGB byte format
	//Note how we're shifting bytes to byte aligned positions in the 32
	//bit value we're sending.
	cellGcmSetClearColor((51<<0)|(51<<8)|(51<<16)|(255<<24));

	//Tell GCM that we want to clear all of the colours of the current
	//surface
	cellGcmSetClearSurface(CELL_GCM_CLEAR_Z|CELL_GCM_CLEAR_S|
		CELL_GCM_CLEAR_R|CELL_GCM_CLEAR_G|CELL_GCM_CLEAR_B|CELL_GCM_CLEAR_A);
}

/*
Sets the current vertex and shader programs. It was never really discussed
during the graphics module, but you can have shader programs that are part
of multiple shader executables. GCM is /always/ like this, with vertex
and fragment shaders entirely separate. To keep consistency with the method
you were all introduced to in Graphics for Games, this function sets both
fragment and vertex shader.

Note how the vertex shader takes a pointer to the ucode, while fragment
shaders take an offset. This looks messy, and might initially seem
ridiculous, but it is because vertex shaders can be ran from system memory,
while fragment shaders MUST be ran in graphics memory, and so its address
must be 'offsettable'. 
*/
void	GCMRenderer::SetCurrentShader(Shader* s) 
{
	cellGcmSetFragmentProgram(s->GetFragment()->GetProgram(), s->GetFragment()->GetOffset());
	cellGcmSetVertexProgram(s->GetVertex()->GetProgram(), s->GetVertex()->GetuCode());
	shader = s;
}

//Sets the camera. Can be NULL
void	GCMRenderer::SetCamera(Camera* c) 
{
	camera = c;
}

//Sets the root node. Can be NULL
void	GCMRenderer::SetRootNode(SceneNode * r) {
	root = r;
}
/*
Draws a scene node. Based on the second scene graph tutorial code.

For those who are interested, the reason node drawing has become part of the
renderer, and not a function of the SceneNode class was to keep SceneNode
'agnostic' - it could be used for an OpenGL system, or GCM, or anything else,
without having to be changed. This makes it slightly less flexible in that
it means the renderer has to know what to do with every type of scene node
(think about things like a shader change node, or a uniform change node). It
also helps keep track of where API header files have to be included, libraries
added, etc. 

We can't really get around Mesh having to have some sort of API-specific code
in it, though (think about separate VBOs vs interleaves ones etc), so API code
still ends up leaking out of the Renderer class, so maybe you'll want to 
refactor SceneNode so it handles it's own drawing after all (via platform 
specific #ifdefs to encapsulate API code). You'll have to think up some way
of safely setting shaders and textures on a renderer...
*/
void	GCMRenderer::DrawNode(SceneNode*n)	
{
	if(n->GetDrawableEntity()) 
	{
		//GCC complains about function returns being used as parameters passed
		//around, or we'd just use GetWorldTransform as the function param
		DrawableEntity3D& entity = *n->GetDrawableEntity();
		T3Matrix4 transform = n->GetTransform();

		Matrix4 m;
		for (int x = 0; x < 4; ++x)
			for (int y = 0; y < 4; ++y)
				m.setElem(x, y, transform.values[y + x * 4]);

		//shader = entity.GetShader();
		//shader->GetVertex()->UpdateShaderMatrices(m, viewMatrix, projMatrix);

		/*
		If your SceneNodes can have their own texture, handle the setting of it
		here! In Graphics for Games, the Mesh::Draw function handled the setting
		of textures to units, but the slight changes in how GCM handles textures
		make it more intuitive to place it here, instead.
		*/
		
		CellGcmTexture* t = texture->GetTexture();
		SetTextureSampler(shader->GetFragment()->GetParameter("texture"), t);

		/*
		The GCM Mesh class needs the current vertex shader, fragment
		shader is just sent for convenience, in case it's needed in future...
		*/
		entity.GetMesh()->Draw(shader);
	}



	//Remember to draw our children!
	for(std::vector<SceneNode*>::const_iterator i = n->GetChildIteratorStart(); i != n->GetChildIteratorEnd(); ++i) {
		DrawNode((*i));
	}
	
}

/*
GCM handles texturing in a pretty similar way to OpenGL - texture samplers in 
the shader. Unlike OpenGL, where you could bind a texture to a texture unit,
and not necessarily know which texture sampler was sampling from it (and vice
versa, of course), in GCM you just grab the sampler, and set a texture on it -
the assignment between texture sampler and units is handled automatically.

Instead of spreading texture sampler and unit assignments out throughout the code,
it therefore makes sense to just have a function that takes a handle to a uniform
texture sampler and a texture, and binds one to the other. 

Example Usage:
Set the sampler called 'texture' to the meshes default texture:
	SetTextureSampler(currentFrag->GetParameter("texture"), n->GetMesh()->GetDefaultTexture())

Set the sampler called 'diffuse' to have no texture
	SetTextureSampler(currentFrag->GetParameter("diffuse"), NULL)

*/
void	GCMRenderer::SetTextureSampler(CGparameter sampler, const CellGcmTexture *texture) {
	if(!sampler) {
		return; //cellGcmCgGetParameterResource dies on an invalid parameter!
	}
	//Get a pointer to the actual bound texture unit for this sampler
	CGresource unitResource = (CGresource)(cellGcmCgGetParameterResource(shader->GetFragment()->program, sampler) - CG_TEXUNIT0);

	cellGcmSetTexture(unitResource, texture);		//Set texture unit to sample from the given texture
	
	//If we want to turn off a texture unit, for example
	if(!texture) {
		cellGcmSetTextureControl(unitResource, CELL_GCM_FALSE, 0, 0, 0); //Disable sampling on the TU
		return;
	}

	cellGcmSetTextureControl(unitResource, CELL_GCM_TRUE, 0, 0, 0); //Enable sampling on the TU

	//Set how the texture is accessed. This is analogous to the 
	//glTexParameteri function you should be used to, by now.
	cellGcmSetTextureAddress(unitResource,
		CELL_GCM_TEXTURE_CLAMP,
		CELL_GCM_TEXTURE_CLAMP,
		CELL_GCM_TEXTURE_CLAMP,
		CELL_GCM_TEXTURE_UNSIGNED_REMAP_NORMAL,
		CELL_GCM_TEXTURE_ZFUNC_LESS,
		0);

	//Set how the texture is filtered, just like glTexParameteri, again.
	//We get an extra function on top of OpenGL - getting to choose the
	//actual type of linear filter to be applied. We're going to use
	//quincunx filtering, which takes 5 samples per fragment.
	cellGcmSetTextureFilter(unitResource, 0,
		CELL_GCM_TEXTURE_LINEAR,
		CELL_GCM_TEXTURE_LINEAR,
		CELL_GCM_TEXTURE_CONVOLUTION_QUINCUNX);
}

bool GCMRenderer::SetResolution(GCMResolution resolution) {
	//First, query if resolution is viable
	CellVideoOutResolution resInfo;

	if(!cellVideoOutGetResolutionAvailability(CELL_VIDEO_OUT_PRIMARY,resolution, CELL_VIDEO_OUT_ASPECT_AUTO,0)) {
		return false;
	}

	if(cellVideoOutGetResolution( resolution, &resInfo ) != 0) {
		return false;
	}

	CellVideoOutConfiguration config = {	resolution, 
		CELL_VIDEO_OUT_BUFFER_COLOR_FORMAT_X8R8G8B8,
		CELL_VIDEO_OUT_ASPECT_AUTO,
		{0,0,0,0,0,0,0,0,0},
		cellGcmGetTiledPitchSize( resInfo.width * 4 )
	};


	if(cellVideoOutConfigure( CELL_VIDEO_OUT_PRIMARY, &config, NULL, 0 ) != 0) {
		return false;
	}

	return true;
}
#endif
