#include "HudTestScreen.h"


HudTestScreen::HudTestScreen(void)
{
	inputEnabled=false;
	time=30;
	score=0;
	speed=0.0f;
}


HudTestScreen::~HudTestScreen(void)
{
}

void HudTestScreen::Update(){

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

	std::stringstream ss;
		std::string str;
		ss<<time;
		ss>>str;

		std::stringstream ss2;
		std::string str2;
		ss2<<score;
		ss2>>str2;

		std::stringstream ss3;
		std::string str3;
		ss3<<speed;
		ss3>>str3;

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

	AddDrawable(Playtime);
	AddDrawable(Score);

	Speed=	new DrawableText2D(
		0.7f, 
		0.6f, 
		0, 
		0.2f, 
		0.1f, 
		"Speed = "+str3, 
		f,
		0,
		T3Vector2(0.5f, 0.5f),
		T3Vector4(1.0f,1.0f,1.0f,0.7f));

	AddDrawable(Speed);
	

	//string Stime="time";

}

void HudTestScreen::UnloadContent()
{
	RemoveDrawable(Playtime, false);
	RemoveDrawable(Score, false);
	RemoveDrawable(Speed, false);


	GameStateManager::Assets()->UnloadFont(this, "tahoma");
	GameStateManager::Assets()->UnloadTexture(this, "Grass_Color");
	GameStateManager::Assets()->UnloadTexture(this, "calvin");
}



void HudTestScreen::SetScreen(int point,int time,float speed){
	
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

		std::stringstream ss5;
		std::string str5;
		ss5<<speed;
		ss5>>str5;

	Playtime->SetText("Time = "+str3);
	Score->SetText("Score = "+str4);
	Speed->SetText("Speed = "+str5);
}