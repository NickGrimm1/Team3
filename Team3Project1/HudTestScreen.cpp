#include "HudTestScreen.h"


HudTestScreen::HudTestScreen(void)
{
	f = GameStateManager::Assets()->LoadFont(this, TEXTUREDIR"tahoma.tga", 16, 16);
	
	time=30;
	score=0;
	    std::stringstream ss;
		std::string str;
		ss<<time;
		ss>>str;

		std::stringstream ss2;
		std::string str2;
		ss2<<score;
		ss2>>str2;

		Playtime=	new DrawableText2D(
		0.7f, 
		0.7f, 
		0, 
		0.2f, 
		0.1f, 
		"Time = "+str, 
		f,
		0,
		T3Vector2(0.5f, 0.5f),
		T3Vector4(1.0f,1.0f,1.0f,0.7f));
	
	AddDrawable(Playtime);

		Score=	new DrawableText2D(
		0.7f, 
		0.8f, 
		0, 
		0.2f, 
		0.1f, 
		"Score = "+str2, 
		f,
		0,
		T3Vector2(0.5f, 0.5f),
		T3Vector4(1.0f,1.0f,1.0f,0.7f));

	 AddDrawable(Score);
}


HudTestScreen::~HudTestScreen(void)
{
}

void HudTestScreen::Update(){
	//cout<<"2d update"<<endl;
	
	
}

void HudTestScreen::LoadContent() {
	Font* f = GameStateManager::Assets()->LoadFont(this, "tahoma", 16, 16);
#if WINDOWS_BUILD
	Texture* grassTex = GameStateManager::Assets()->LoadTexture(this, "Grass_Color", SOIL_FLAG_MIPMAPS);
#endif
#if PS3_BUILD
	Texture* grassTex = GameStateManager::Assets()->LoadTexture(this, "Grass_Color", 0);
#endif
	Texture* calvinTex = GameStateManager::Assets()->LoadTexture(this, "calvin", 0);


	//string Stime="time";


	//AddDrawable(new DrawableText2D(
	//	0.3f, 
	//	0.7f, 
	//	0, 
	//	0.4f, 
	//	0.1f, 
	//	"Rotated", 
	//	f,
	//	0,
	//	T3Vector2(0.5f, 0.5f),
	//	T3Vector4(1.0f,1.0f,1.0f,1.0f)));

	//AddDrawable(new DrawableText2D(
	//	0.3f, 
	//	0.7f, 
	//	0, 
	//	0.4f, 
	//	0.1f, 
	//	"Rotated", 
	//	f,
	//	15,
	//	T3Vector2(0.5f, 0.5f),
	//	T3Vector4(1.0f,1.0f,1.0f,0.4f)));

	//AddDrawable(new DrawableText2D(
	//	0.3f, 
	//	0.7f, 
	//	0, 
	//	0.4f, 
	//	0.1f, 
	//	"Rotated", 
	//	f,
	//	30,
	//	T3Vector2(0.5f, 0.5f),
	//	T3Vector4(1.0f,1.0f,1.0f,0.4f)));

	//AddDrawable(new DrawableText2D(
	//	0.3f, 
	//	0.7f, 
	//	0, 
	//	0.4f, 
	//	0.1f, 
	//	"Rotated", 
	//	f,
	//	45,
	//	T3Vector2(1.0f, 0.5f),
	//	T3Vector4(1.0f,1.0f,1.0f,0.4f)));

	//AddDrawable(new DrawableText2D(
	//	0.3f, 
	//	0.7f, 
	//	0, 
	//	0.4f, 
	//	0.1f, 
	//	"Rotated", 
	//	f,
	//	90,
	//	T3Vector2(1.0f, 0.5f),
	//	T3Vector4(1.0f,1.0f,1.0f,0.4f)));

	//AddDrawable(
	//	new DrawableTexture2D(
	//	0.05f, 
	//	0.05f, 
	//	1, 
	//	0.05f, 
	//	0.05f, 
	//	grassTex,
	//	0,
	//	T3Vector2(),
	//	T3Vector4(1.0f,1.0f,1.0f,0.5f)));

	//AddDrawable(
	//	new DrawableTexture2D(
	//	0.25f, 
	//	0.05f, 
	//	-1, 
	//	0.05f, 
	//	0.05f, 
	//	calvinTex,
	//	30.0,
	//	T3Vector2(0.5f, 0.5f),
	//	T3Vector4(1.0f,1.0f,0.0f,0.1f)));
}

void HudTestScreen::UnloadContent()
{
	GameStateManager::Assets()->UnloadFont(this, "tahoma");
	GameStateManager::Assets()->UnloadTexture(this, "Grass_Color");
	GameStateManager::Assets()->UnloadTexture(this, "calvin");
}



void HudTestScreen::SetScreen(int point,int time){
	
	//SetTime(time);
	//SetTime(time);

	    std::stringstream ss3;
		std::string str3;
		ss3<<time;
		ss3>>str3;

		std::stringstream ss4;
		std::string str4;
		ss4<<point;
		ss4>>str4;

	Playtime->SetText("Time = "+str3);
	Score->SetText("Score = "+str4);
	
}