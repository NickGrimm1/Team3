#include "GameOver.h"


GameOver::GameOver(GameScreen3D* mainGame, GameScreen2D* hud) {
#if WINDOWS_BUILD
	GameStateManager::Graphics()->EnableMousePointer(true);
#endif
	musicMuted = false;
	soundMuted = false;

	racerScreen = mainGame;
	hudScreen = hud;
}

GameOver::~GameOver(void) {

}

void GameOver::LoadContent() {
	Font* font = GameStateManager::Assets()->LoadFont(this, "quadrats", 16, 16);

	buttonTex			= GameStateManager::Assets()->LoadTexture(this, "Buttons/button", 0);
	buttonTexHover		= GameStateManager::Assets()->LoadTexture(this, "Buttons/button_selected", 0);
	buttonTexClicked	= GameStateManager::Assets()->LoadTexture(this, "Buttons/button_clicked", 0);

	musicNoMute			= GameStateManager::Assets()->LoadTexture(this, "Buttons/music_nomute", 0);
	musicNoMuteHover	= GameStateManager::Assets()->LoadTexture(this, "Buttons/music_nomute_selected", 0);
	musicMute			= GameStateManager::Assets()->LoadTexture(this, "Buttons/music_mute", 0);
	musicMuteHover		= GameStateManager::Assets()->LoadTexture(this, "Buttons/music_mute_selected", 0);

	soundNoMute			= GameStateManager::Assets()->LoadTexture(this, "Buttons/sound_nomute", 0);
	soundNoMuteHover	= GameStateManager::Assets()->LoadTexture(this, "Buttons/sound_nomute_selected", 0);
	soundMute			= GameStateManager::Assets()->LoadTexture(this, "Buttons/sound_mute", 0);
	soundMuteHover		= GameStateManager::Assets()->LoadTexture(this, "Buttons/sound_mute_selected", 0);

	gameOverTex			= GameStateManager::Assets()->LoadTexture(this, "gameOver", 0);

	float charWidth = 0.16f;
	float charHeight = 0.08f;

	float btnOffsetX = 0.02f;
	float btnOffsetY = 0.04f;

	//Button size
	float btnScaleX = 0.2f;
	float btnScaleY = 0.152f;

	float btnX = 0.4f;
	float btnY = 0.5f;

	mainMenu = new UIButton(btnX, btnY, btnScaleX, btnScaleY, (void (GameScreen2D::*)()) &GameOver::MainMenuSelected, (void (GameScreen2D::*)(float, float)) &GameOver::MainMenuClicked, 
				new DrawableTexture2D(btnX, btnY, 1, btnScaleX, btnScaleY, buttonTex, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false),
				new DrawableText2D(btnX + btnOffsetX, btnY + btnOffsetY, 2.0f, charWidth, charHeight, "MAIN MENU", font, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false), (void (GameScreen2D::*)()) &GameOver::MainMenuDeselected, this);
	
	quitGame = new UIButton(btnX, btnY + 0.17f, btnScaleX, btnScaleY, (void (GameScreen2D::*)()) &GameOver::QuitGameSelected, (void (GameScreen2D::*)(float, float)) &GameOver::QuitGameClicked, 
				new DrawableTexture2D(btnX, btnY + 0.17f, 1, btnScaleX, btnScaleY, buttonTex, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false),
				new DrawableText2D(btnX + btnOffsetX, btnY + 0.17f + btnOffsetY, 2.0f, charWidth, charHeight, "QUIT GAME", font, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false), (void (GameScreen2D::*)()) &GameOver::QuitGameDeselected, this);
	
	float musicBtnWidth = 0.045f;
	float musicBtnHeight = 0.089f;

	float musicBtnX = 0.095f;
	float musicBtnY = 0.70f;

	music = new UIButton(musicBtnX, musicBtnY, musicBtnWidth, musicBtnHeight, (void (GameScreen2D::*)()) &GameOver::MusicSelected, (void (GameScreen2D::*)(float, float)) &GameOver::MusicClicked, 
				new DrawableTexture2D(musicBtnX, musicBtnY, 1, musicBtnWidth, musicBtnHeight, musicNoMute, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false),
				new DrawableText2D(musicBtnX, musicBtnY, 0.0f, 0.0f, 0.0f, "", font, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false), (void (GameScreen2D::*)()) &GameOver::MusicDeselected, this);
	
	sounds = new UIButton(musicBtnX + musicBtnWidth, musicBtnY, musicBtnWidth, musicBtnHeight, (void (GameScreen2D::*)()) &GameOver::SoundsSelected, (void (GameScreen2D::*)(float, float)) &GameOver::SoundsClicked, 
				new DrawableTexture2D(musicBtnX + musicBtnWidth, musicBtnY, 1, musicBtnWidth, musicBtnHeight, soundNoMute, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false),
				new DrawableText2D(musicBtnX + musicBtnWidth, musicBtnY, 0.0f, 0.0f, 0.0f, "", font, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false), (void (GameScreen2D::*)()) &GameOver::SoundsDeselected, this);

	gameOver = new DrawableTexture2D(0.25f, 0.1f, 1, 0.5f, 0.3f, gameOverTex, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false);

	AddClickable(mainMenu);
	AddClickable(quitGame);
	AddClickable(music);
	AddClickable(sounds);

	AddDrawable(gameOver);
}

void GameOver::Update() {

}

void GameOver::UnloadContent() {
	RemoveClickable(mainMenu, true, false);
	RemoveClickable(quitGame, true, false);
	RemoveClickable(music, true, false);
	RemoveClickable(sounds, true, false);

	RemoveDrawable(gameOver, false);

	GameStateManager::Assets()->UnloadFont(this, "quadrats");

	GameStateManager::Assets()->UnloadTexture(this, "Buttons/button");
	GameStateManager::Assets()->UnloadTexture(this, "Buttons/button_selected");
	GameStateManager::Assets()->UnloadTexture(this, "Buttons/button_clicked");
								
	GameStateManager::Assets()->UnloadTexture(this, "Buttons/music_nomute");
	GameStateManager::Assets()->UnloadTexture(this, "Buttons/music_nomute_selected");
	GameStateManager::Assets()->UnloadTexture(this, "Buttons/music_mute");
	GameStateManager::Assets()->UnloadTexture(this, "Buttons/music_mute_selected");
								
	GameStateManager::Assets()->UnloadTexture(this, "Buttons/sound_nomute");
	GameStateManager::Assets()->UnloadTexture(this, "Buttons/sound_nomute_selected");
	GameStateManager::Assets()->UnloadTexture(this, "Buttons/sound_mute");
	GameStateManager::Assets()->UnloadTexture(this, "Buttons/sound_mute_selected");
	GameStateManager::Assets()->UnloadTexture(this, "gameOver");
}

void GameOver::MainMenuClicked(float x, float y) {
	mainMenu->GetTexture()->SetTexture(buttonTexClicked);

	MainMenu* menu = new MainMenu();
	GameStateManager::RemoveGameScreen(hudScreen);
	GameStateManager::RemoveGameScreen(racerScreen);
	GameStateManager::RemoveGameScreen(this);
	GameStateManager::AddGameScreen(menu);
}

void GameOver::QuitGameClicked(float x, float y) {
	quitGame->GetTexture()->SetTexture(buttonTexClicked);
	GameStateManager::Instance()->Exit();
}

void GameOver::SoundsClicked(float x, float y) {
	if (soundMuted)
		sounds->GetTexture()->SetTexture(soundNoMuteHover);
	else
		sounds->GetTexture()->SetTexture(soundMuteHover);

	soundMuted = !soundMuted;
}

void GameOver::MusicClicked(float x, float y) {
	if (musicMuted) 
		music->GetTexture()->SetTexture(musicNoMuteHover);
	else
		music->GetTexture()->SetTexture(musicMuteHover);

	musicMuted = !musicMuted;
}

void GameOver::MainMenuSelected() {
	mainMenu->GetTexture()->SetTexture(buttonTexHover);
}

void GameOver::MainMenuDeselected() {
	mainMenu->GetTexture()->SetTexture(buttonTex);
}

void GameOver::QuitGameSelected() {
	quitGame->GetTexture()->SetTexture(buttonTexHover);
}

void GameOver::QuitGameDeselected() {
	quitGame->GetTexture()->SetTexture(buttonTex);
}

void GameOver::SoundsSelected() {
	if (soundMuted)
		sounds->GetTexture()->SetTexture(soundMuteHover);
	else
		sounds->GetTexture()->SetTexture(soundNoMuteHover);
}

void GameOver::SoundsDeselected() {
	if (soundMuted) 
		sounds->GetTexture()->SetTexture(soundMute);
	else
		sounds->GetTexture()->SetTexture(soundNoMute);
}

void GameOver::MusicSelected() {
	if (musicMuted)
		music->GetTexture()->SetTexture(musicMuteHover);
	else
		music->GetTexture()->SetTexture(musicNoMuteHover);
}

void GameOver::MusicDeselected() {
	if (musicMuted)
		music->GetTexture()->SetTexture(musicMute);
	else
		music->GetTexture()->SetTexture(musicNoMute);
}