#include "DebugOverlay.h"
#include "Font.h"
#include "GameStateManager.h"
#include "DrawableTexture2D.h"

DebugOverlay::DebugOverlay()
{
}
DebugOverlay::~DebugOverlay()
{
}
void DebugOverlay::LoadContent()
{
	Font* font = GameStateManager::Assets()->LoadFont(this, TEXTUREDIR"tahoma.tga", 16, 16);
	
	AddDrawable(new DrawableTexture2D(0, 0, -1, 1, 1, GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"TitleSafeArea.png", 0), 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1.0f, 0.0f, 0.0f, 0.25f)));
	
	float charWidth = 0.0125f;
	float charHeight = 0.03f;
	AddDrawable(new DrawableText2D(0, 0, 1,		13 * charWidth, charHeight, "Frame Rates: ", font)); 
	AddDrawable(new DrawableText2D(0, 0.03f, 1,	11 * charWidth, charHeight, "  Render: ", font)); 
	AddDrawable(new DrawableText2D(0, 0.06f, 1,	12 * charWidth, charHeight, "  Physics: ", font)); 
	AddDrawable(new DrawableText2D(0, 0.09f, 1,	10 * charWidth, charHeight, "  Input: ", font));
	AddDrawable(new DrawableText2D(0, 0.12f, 1,	18 * charWidth, charHeight, "Asset Management: ", font));
	AddDrawable(new DrawableText2D(0, 0.15f, 1,	23 * charWidth, charHeight, "  Total Memory Usage: ", font));
	AddDrawable(new DrawableText2D(0, 0.18f, 1,  12 * charWidth, charHeight, "  Textures: ", font));
	AddDrawable(new DrawableText2D(0, 0.21f, 1,  10 * charWidth, charHeight, "  Meshes: ", font));
	AddDrawable(new DrawableText2D(0, 0.24f, 1,  10 * charWidth, charHeight, "Mouse X: ", font));
	AddDrawable(new DrawableText2D(0, 0.27f, 1,  10 * charWidth, charHeight, "Mouse Y: ", font));

	renderFrameRate = new DrawableText2D(11 * charWidth, 0.03f, 1, 2 * charWidth, charHeight, "0", font);
	physicsFrameRate = new DrawableText2D(12 * charWidth, 0.06f, 1, 3 * charWidth, charHeight, "0", font);
	inputFrameRate = new DrawableText2D(10 * charWidth, 0.09f, 1, 2 * charWidth, charHeight, "0", font);
	AddDrawable(renderFrameRate);
	AddDrawable(physicsFrameRate);
	AddDrawable(inputFrameRate);

	totalMemory = new DrawableText2D(23 * charWidth, 0.15f, 1, 3 * charWidth, charHeight, "0", font);
	textureMemory = new DrawableText2D(12 * charWidth, 0.18f, 1, 3 * charWidth, charHeight, "0", font);
	meshMemory = new DrawableText2D(10 * charWidth, 0.21f, 1, 3 * charWidth, charHeight, "0", font);
	AddDrawable(totalMemory);
	AddDrawable(textureMemory);
	AddDrawable(meshMemory);

	AddDrawable(new DrawableText2D(26 * charWidth, 0.16f, 1, 2 * charWidth, 0.02f, "MB", font));
	AddDrawable(new DrawableText2D(15 * charWidth, 0.19f, 1, 2 * charWidth, 0.02f, "MB", font));
	AddDrawable(new DrawableText2D(13 * charWidth, 0.22f, 1, 2 * charWidth, 0.02f, "MB", font));

	mouseX = new DrawableText2D(9 * charWidth, 0.24f, 1, 8 * charWidth, charHeight, "0", font);
	mouseY = new DrawableText2D(9 * charWidth, 0.27f, 1, 8 * charWidth, charHeight, "0", font);
	AddDrawable(mouseX);
	AddDrawable(mouseY);
}
void DebugOverlay::Update()
{
	renderFrameRate->SetText(to_string(GameStateManager::Graphics()->GetFrameRate()));
	physicsFrameRate->SetText(to_string(GameStateManager::Physics()->GetFrameRate()));
	inputFrameRate->SetText(to_string(GameStateManager::Input()->GetFrameRate()));
	totalMemory->SetText(to_string(GameStateManager::Assets()->GetTotalMemory()));
	textureMemory->SetText(to_string(GameStateManager::Assets()->GetTextureMemory()));
	meshMemory->SetText(to_string(GameStateManager::Assets()->GetMeshMemory()));
}
void DebugOverlay::UnloadContent()
{
	GameStateManager::Assets()->UnloadFont(this, TEXTUREDIR"tahoma.tga");
	GameStateManager::Assets()->UnloadTexture(this, TEXTUREDIR"TitleSafeArea.png");
}
void DebugOverlay::MouseMoved(T3Vector2& start, T3Vector2& finish)
{
	mouseX->SetText(to_string(finish.x));
	mouseY->SetText(to_string(finish.y));
}