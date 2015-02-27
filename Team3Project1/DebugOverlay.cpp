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
	
	AddDrawable(new DrawableTexture2D(0, 0, -1, 1, 1, GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"TitleSafeArea.png", SOIL_FLAG_INVERT_Y), 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1.0f, 0.0f, 0.0f, 0.25f)));
	
	AddDrawable(new DrawableText2D(120, 72, 1, 200, 20, "Frame Rates: ", font)); 
	AddDrawable(new DrawableText2D(140, 92, 1, 180, 20, "  Render: ", font)); 
	AddDrawable(new DrawableText2D(140, 112, 1, 180, 20, "  Physics: ", font)); 
	AddDrawable(new DrawableText2D(140, 132, 1, 180, 20, "  Input: ", font));
	AddDrawable(new DrawableText2D(140, 152, 1, 180, 20, "Asset Management: ", font));
	AddDrawable(new DrawableText2D(140, 172, 1, 180, 20, "  Total Memory Usage: ", font));
	AddDrawable(new DrawableText2D(140, 192, 1, 180, 20, "  Textures: ", font));
	AddDrawable(new DrawableText2D(140, 212, 1, 180, 20, "  Meshes: ", font));

	renderFrameRate = new DrawableText2D(320, 92, 1, 20, 20, "0", font);
	physicsFrameRate = new DrawableText2D(320, 112, 1, 30, 20, "0", font);
	inputFrameRate = new DrawableText2D(320, 132, 1, 20, 20, "0", font);
	AddDrawable(renderFrameRate);
	AddDrawable(physicsFrameRate);
	AddDrawable(inputFrameRate);

	totalMemory = new DrawableText2D(320, 172, 1, 80, 20, "0", font);
	textureMemory = new DrawableText2D(320, 192, 1, 80, 20, "0", font);
	meshMemory = new DrawableText2D(320, 212, 1, 80, 20, "0", font);
	AddDrawable(totalMemory);
	AddDrawable(textureMemory);
	AddDrawable(meshMemory);

	AddDrawable(new DrawableText2D(400, 172, 1, 20, 20, "MB", font));
	AddDrawable(new DrawableText2D(400, 192, 1, 20, 20, "MB", font));
	AddDrawable(new DrawableText2D(400, 212, 1, 20, 20, "MB", font));
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