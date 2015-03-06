#include "PauseScreen.h"


PauseScreen::PauseScreen(void) {
#if WINDOWS_BUILD
	GameStateManager::Graphics()->EnableMousePointer(true);
#endif
	//Set these to reflect the current state of the audio engine.
	musicMuted = false;
	soundMuted = false;
}

PauseScreen::~PauseScreen(void) {

}

void PauseScreen::Update() {

}

void PauseScreen::LoadContent() {
	Font* font = GameStateManager::Assets()->LoadFont(this, TEXTUREDIR"quadrats.tga", 16, 16);

	buttonTex			= GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"Buttons/button.png", 0);
	buttonTexHover		= GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"Buttons/button_selected.png", 0);
	buttonTexClicked	= GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"Buttons/button_clicked.png", 0);

	musicNoMute			= GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"Buttons/music_nomute.png", 0);
	musicNoMuteHover	= GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"Buttons/music_nomute_selected.png", 0);
	musicMute			= GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"Buttons/music_mute.png", 0);
	musicMuteHover		= GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"Buttons/music_mute_selected.png", 0);

	soundNoMute			= GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"Buttons/sound_nomute.png", 0);
	soundNoMuteHover	= GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"Buttons/sound_nomute_selected.png", 0);
	soundMute			= GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"Buttons/sound_mute.png", 0);
	soundMuteHover		= GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"Buttons/sound_mute_selected.png", 0);

	pauseTex			= GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"pause_screen.png", 0);

	float charWidth = 0.16f;
	float charHeight = 0.08f;

	float btnOffsetX = 0.02f;
	float btnOffsetY = 0.04f;

	//Button size
	float btnScaleX = 0.2f;
	float btnScaleY = 0.152f;

	float btnX = 0.4f;
	float btnY = 0.5f;

	resumeGame	= new UIButton(btnX, btnY, btnScaleX, btnScaleY, (void (GameScreen2D::*)()) &PauseScreen::ResumeSelected, (void (GameScreen2D::*)(float, float)) &PauseScreen::ResumeClicked, 
				new DrawableTexture2D(btnX, btnY, 1, btnScaleX, btnScaleY, buttonTex, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false),
				new DrawableText2D(btnX + btnOffsetX, btnY + btnOffsetY, 2.0f, charWidth, charHeight, "RESUME GAME", font, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false), (void (GameScreen2D::*)()) &PauseScreen::ResumeDeselected, this);
	
	mainMenu = new UIButton(btnX, btnY + 0.17f, btnScaleX, btnScaleY, (void (GameScreen2D::*)()) &PauseScreen::MainMenuSelected, (void (GameScreen2D::*)(float, float)) &PauseScreen::MainMenuClicked, 
				new DrawableTexture2D(btnX, btnY + 0.17f, 1, btnScaleX, btnScaleY, buttonTex, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false),
				new DrawableText2D(btnX + btnOffsetX, btnY + 0.17f + btnOffsetY, 2.0f, charWidth, charHeight, "MAIN MENU", font, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false), (void (GameScreen2D::*)()) &PauseScreen::MainMenuDeselected, this);
	
	float musicBtnWidth = 0.045f;
	float musicBtnHeight = 0.089f;

	float musicBtnX = 0.095f;
	float musicBtnY = 0.70f;

	music = new UIButton(musicBtnX, musicBtnY, musicBtnWidth, musicBtnHeight, (void (GameScreen2D::*)()) &PauseScreen::MusicSelected, (void (GameScreen2D::*)(float, float)) &PauseScreen::MusicClicked, 
				new DrawableTexture2D(musicBtnX, musicBtnY, 1, musicBtnWidth, musicBtnHeight, musicNoMute, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false),
				new DrawableText2D(musicBtnX, musicBtnY, 0.0f, 0.0f, 0.0f, "", font, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false), (void (GameScreen2D::*)()) &PauseScreen::MusicDeselected, this);
	
	sounds = new UIButton(musicBtnX + musicBtnWidth, musicBtnY, musicBtnWidth, musicBtnHeight, (void (GameScreen2D::*)()) &PauseScreen::SoundsSelected, (void (GameScreen2D::*)(float, float)) &PauseScreen::SoundsClicked, 
				new DrawableTexture2D(musicBtnX + musicBtnWidth, musicBtnY, 1, musicBtnWidth, musicBtnHeight, soundNoMute, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false),
				new DrawableText2D(musicBtnX + musicBtnWidth, musicBtnY, 0.0f, 0.0f, 0.0f, "", font, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false), (void (GameScreen2D::*)()) &PauseScreen::SoundsDeselected, this);

	pauseLogo = new DrawableTexture2D(0.25f, 0.1f, 1, 0.5f, 0.3f, pauseTex, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false);

	AddClickable(resumeGame);
	AddClickable(mainMenu);
	AddClickable(music);
	AddClickable(sounds);

	AddDrawable(pauseLogo);
}

void PauseScreen::UnloadContent() {
	RemoveClickable(resumeGame);
	RemoveClickable(mainMenu);
	RemoveClickable(music);
	RemoveClickable(sounds);

	RemoveDrawable(pauseLogo);

	GameStateManager::Assets()->UnloadFont(this, TEXTUREDIR"quadrats.tga");

	GameStateManager::Assets()->UnloadTexture(this, TEXTUREDIR"Buttons/button.png");
	GameStateManager::Assets()->UnloadTexture(this, TEXTUREDIR"Buttons/button_selected.png");
	GameStateManager::Assets()->UnloadTexture(this, TEXTUREDIR"Buttons/button_clicked.png");
								
	GameStateManager::Assets()->UnloadTexture(this, TEXTUREDIR"Buttons/music_nomute.png");
	GameStateManager::Assets()->UnloadTexture(this, TEXTUREDIR"Buttons/music_nomute_selected.png");
	GameStateManager::Assets()->UnloadTexture(this, TEXTUREDIR"Buttons/music_mute.png");
	GameStateManager::Assets()->UnloadTexture(this, TEXTUREDIR"Buttons/music_mute_selected.png");
								
	GameStateManager::Assets()->UnloadTexture(this, TEXTUREDIR"Buttons/sound_nomute.png");
	GameStateManager::Assets()->UnloadTexture(this, TEXTUREDIR"Buttons/sound_nomute_selected.png");
	GameStateManager::Assets()->UnloadTexture(this, TEXTUREDIR"Buttons/sound_mute.png");
	GameStateManager::Assets()->UnloadTexture(this, TEXTUREDIR"Buttons/sound_mute_selected.png");

	GameStateManager::Assets()->UnloadTexture(this, TEXTUREDIR"pause_screen.png");
}

void PauseScreen::ResumeClicked(float x, float y) {
	resumeGame->GetTexture()->SetTexture(buttonTexClicked);
}

void PauseScreen::MainMenuClicked(float x, float y) {
	mainMenu->GetTexture()->SetTexture(buttonTexClicked);
	
	MainMenu* menu = new MainMenu();
	GameStateManager::ChangeScreen(menu);
}

void PauseScreen::SoundsClicked(float x, float y) {
	if (soundMuted)
		sounds->GetTexture()->SetTexture(soundNoMuteHover);
	else
		sounds->GetTexture()->SetTexture(soundMuteHover);

	soundMuted = !soundMuted;
}

void PauseScreen::MusicClicked(float x, float y) {
	if (musicMuted) 
		music->GetTexture()->SetTexture(musicNoMuteHover);
	else
		music->GetTexture()->SetTexture(musicMuteHover);

	musicMuted = !musicMuted;
}

void PauseScreen::ResumeSelected() {
	resumeGame->GetTexture()->SetTexture(buttonTexHover);
}

void PauseScreen::ResumeDeselected() {
	resumeGame->GetTexture()->SetTexture(buttonTex);
}

void PauseScreen::MainMenuSelected() {
	mainMenu->GetTexture()->SetTexture(buttonTexHover);
}

void PauseScreen::MainMenuDeselected() {
	mainMenu->GetTexture()->SetTexture(buttonTex);
}

void PauseScreen::SoundsSelected() {
	if (soundMuted)
		sounds->GetTexture()->SetTexture(soundMuteHover);
	else
		sounds->GetTexture()->SetTexture(soundNoMuteHover);
}

void PauseScreen::SoundsDeselected() {
	if (soundMuted) 
		sounds->GetTexture()->SetTexture(soundMute);
	else
		sounds->GetTexture()->SetTexture(soundNoMute);
}

void PauseScreen::MusicSelected() {
	if (musicMuted)
		music->GetTexture()->SetTexture(musicMuteHover);
	else
		music->GetTexture()->SetTexture(musicNoMuteHover);
}

void PauseScreen::MusicDeselected() {
	if (musicMuted)
		music->GetTexture()->SetTexture(musicMute);
	else
		music->GetTexture()->SetTexture(musicNoMute);
}
