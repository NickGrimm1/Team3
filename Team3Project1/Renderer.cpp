#include "Renderer.h"


Renderer::Renderer(Window &parent) : OGLRenderer(parent)
{

}

Renderer::~Renderer(void)
{
}

unsigned int AddLight(Light &arg)
{

}

Light& GetLight(int arg)
{

}

void RemoveLight(int arg)
{

}

unsigned int AddTexture(Texture &arg)
{

}

Texture& GetTexture(int arg)
{

}

void RemoveTexture(int arg)
{

}

unsigned int AddMesh(Mesh &arg)
{

}

Mesh& GetMesh(int arg)
{

}

void RemoveMesh(int arg)
{

}

void ToggleDebug(int arg, bool onOff)
{

}

void RenderScene(float msec)
{

}

void ShadowPass()
{

}

void DefferedLightPass()
{

}

void MotionBlurPass()
{

}

void BloomPass()
{

}