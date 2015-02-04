#pragma once
#include "../Framework/OGLRenderer.h"

class Renderer : public OGLRenderer
{
public:
	Renderer(void);
	~Renderer(void);

	void Render(float msec);
private:
	void RenderScene(float msec);
	//void AddLight(Light arg);
	//void RemoveLight(Light arg);
	void ShadowPass();
	void MotionBlurPass();
	void BloomPass();
};