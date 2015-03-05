#include "MainMenu.h"
#include "GameStateManager.h"
#include "GraphicsTestScreen.h"

MainMenu::MainMenu(void)
{
#if WINDOWS_BUILD
	GameStateManager::Graphics()->EnableMousePointer(true);
#endif
	musicMuted = false;
	soundMuted = false;
	playerOne = GamepadEvents::PLAYERINDEX_MAX;
}

void MainMenu::Update(void) 
{
	if (playerOne == GamepadEvents::PLAYERINDEX_MAX)
	{
		playerOne = GameStateManager::Input()->GetActiveController();
		if (playerOne != GamepadEvents::PLAYERINDEX_MAX)
		{
			RemoveDrawable(pressStart, false);
#if WINDOWS_BUILD
			GameStateManager::Graphics()->EnableMousePointer(false);
#endif
			SetCurrentSelected(0);

		}
	}
}

MainMenu::~MainMenu(void)
{
	//delete newGame;
	//delete controls;
	//delete quitGame;
	//delete buttonTex;
	//delete buttonTexHover;
	//delete buttonTexClicked;

	// This may have been removed from entities if we have a controller...
	if (pressStart)
	{
		delete pressStart;
		pressStart = NULL;
	}
}


void MainMenu::LoadContent() {
	Font* font = GameStateManager::Assets()->LoadFont(this, TEXTUREDIR"quadrats.tga", 16, 16);

	buttonTex			= GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"Buttons/button.png", 0);
	buttonTexHover		= GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"Buttons/button_selected.png", 0);
	buttonTexClicked	= GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"Buttons/button_clicked.png", 0);
	scoreBoardTex		= GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"score_board.png", 0);

	musicNoMute			= GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"Buttons/music_nomute.png", 0);
	musicNoMuteHover	= GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"Buttons/music_nomute_selected.png", 0);
	musicMute			= GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"Buttons/music_mute.png", 0);
	musicMuteHover		= GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"Buttons/music_mute_selected.png", 0);

	soundNoMute			= GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"Buttons/sound_nomute.png", 0);
	soundNoMuteHover	= GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"Buttons/sound_nomute_selected.png", 0);
	soundMute			= GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"Buttons/sound_mute.png", 0);
	soundMuteHover		= GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"Buttons/sound_mute_selected.png", 0);

	float charWidth = 0.128f;
	float charHeight = 0.08f;

	//Offset for the text to keep it central in the button.
	float btnOffsetX = 0.02f;
	float btnOffsetY = 0.04f;

	//Button size
	float btnScaleX = 0.168f;
	float btnScaleY = 0.152f;

	//Button Position, determines the first button's postion, and Y is multiplied to place succesive buttons uniformly.
	float btnX = 0.25f;
	float btnY = 0.35f;

	newGame	= new UIButton(btnX, btnY, btnScaleX, btnScaleY, (void (GameScreen2D::*)()) &MainMenu::NewGameSelected, (void (GameScreen2D::*)(float, float)) &MainMenu::NewGameClicked, 
				new DrawableTexture2D(btnX, btnY, 1, btnScaleX, btnScaleY, buttonTex, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false),
				new DrawableText2D(btnX + btnOffsetX, btnY + btnOffsetY, 2.0f, charWidth, charHeight, "NEW GAME", font, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false), (void (GameScreen2D::*)()) &MainMenu::NewGameDeselcted, this);
	
	controls = new UIButton(btnX, btnY + 0.2f, btnScaleX, btnScaleY, (void (GameScreen2D::*)()) &MainMenu::ControlsSelcted, (void (GameScreen2D::*)(float, float)) &MainMenu::ControlsClicked, 
				new DrawableTexture2D(btnX, btnY + 0.17f, 1, btnScaleX, btnScaleY, buttonTex, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false),
				new DrawableText2D(btnX + btnOffsetX, btnY + 0.17f + btnOffsetY, 2.0f, charWidth, charHeight, "CONTROLS", font, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false), (void (GameScreen2D::*)()) &MainMenu::ControlsDeselected, this);
	
	quitGame = new UIButton(btnX, btnY + 0.4f, btnScaleX, btnScaleY, (void (GameScreen2D::*)()) &MainMenu::QuitSelected, (void (GameScreen2D::*)(float, float)) &MainMenu::QuitClicked, 
				new DrawableTexture2D(btnX, btnY + 0.34f, 1, btnScaleX, btnScaleY, buttonTex, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false),
				new DrawableText2D(btnX + btnOffsetX, btnY + 0.34f + btnOffsetY, 2.0f, charWidth, charHeight, "QUIT GAME", font, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false), (void (GameScreen2D::*)()) &MainMenu::QuitDeselected, this);
	
	float musicBtnWidth = 0.045f;
	float musicBtnHeight = 0.089f;

	float musicBtnX = 0.095f;
	float musicBtnY = 0.72f;

	music = new UIButton(musicBtnX, musicBtnY, musicBtnWidth, musicBtnHeight, (void(GameScreen2D::*)()) &MainMenu::MusicSelected, (void (GameScreen2D::*)(float, float)) &MainMenu::MusicClicked,
				new DrawableTexture2D(musicBtnX, musicBtnY, 1, musicBtnWidth, musicBtnHeight, musicNoMute, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false),
				new DrawableText2D(musicBtnX, musicBtnY, 0.0f, 0.0f, 0.0f, "", font, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false), (void (GameScreen2D::*)()) &MainMenu::MusicDeselected, this);

	sounds = new UIButton(musicBtnX, musicBtnY + musicBtnHeight, musicBtnWidth, musicBtnHeight, (void(GameScreen2D::*)()) &MainMenu::SoundSelected, (void (GameScreen2D::*)(float, float)) &MainMenu::SoundClicked,
				new DrawableTexture2D(musicBtnX, musicBtnY + musicBtnHeight, 1, musicBtnWidth, musicBtnHeight, soundNoMute, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false),
				new DrawableText2D(musicBtnX, musicBtnY + musicBtnHeight, 0.0f, 0.0f, 0.0f, "", font, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false), (void (GameScreen2D::*)()) &MainMenu::SoundDeselected, this);

	scoreBoard = new DrawableTexture2D(0.55f, 0.125f, 1, 0.35f, 0.75f, scoreBoardTex, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false);

	AddDrawable(scoreBoard);

	AddClickable(newGame);
	AddClickable(controls);
	AddClickable(quitGame);
	AddClickable(music);
	AddClickable(sounds);

	Font* startFont = GameStateManager::Assets()->LoadFont(this, TEXTUREDIR"tahoma.tga", 16, 16);
	pressStart = new DrawableText2D(0.3f, 0.1f, 1, 0.4f, 0.05f, "Player 1 Press Start or A", startFont);
	AddDrawable(pressStart);
}

void MainMenu::UnloadContent() {
	RemoveClickable(newGame);
	RemoveClickable(controls);
	RemoveClickable(quitGame);
	RemoveClickable(music);
	RemoveClickable(sounds);
	RemoveDrawable(scoreBoard);

	GameStateManager::Assets()->UnloadTexture(this, TEXTUREDIR"Buttons/button.png");
	GameStateManager::Assets()->UnloadTexture(this, TEXTUREDIR"Buttons/button_selected.png");
	GameStateManager::Assets()->UnloadTexture(this, TEXTUREDIR"Buttons/button_clicked.png");
	GameStateManager::Assets()->UnloadTexture(this, TEXTUREDIR"score_board.png");

	GameStateManager::Assets()->UnloadTexture(this, TEXTUREDIR"Buttons/music_nomute.png");
	GameStateManager::Assets()->UnloadTexture(this, TEXTUREDIR"Buttons/music_nomute_selected.png");
	GameStateManager::Assets()->UnloadTexture(this, TEXTUREDIR"Buttons/music_mute.png");
	GameStateManager::Assets()->UnloadTexture(this, TEXTUREDIR"Buttons/music_mute_selected.png");
								
	GameStateManager::Assets()->UnloadTexture(this, TEXTUREDIR"Buttons/sound_nomute.png");
	GameStateManager::Assets()->UnloadTexture(this, TEXTUREDIR"Buttons/sound_nomute_selected.png");
	GameStateManager::Assets()->UnloadTexture(this, TEXTUREDIR"Buttons/sound_mute.png");
	GameStateManager::Assets()->UnloadTexture(this, TEXTUREDIR"Buttons/sound_mute_selected.png");

	GameStateManager::Assets()->UnloadFont(this, TEXTUREDIR"quadrats.tga");
	GameStateManager::Assets()->UnloadFont(this, TEXTUREDIR"tahoma.tga");
}

/*--------------------Clicked Methods--------------------*/

void MainMenu::NewGameClicked(float x, float y) {
	newGame->GetTexture()->SetTexture(buttonTexClicked);
#if WINDOWS_BUILD
	GameStateManager::Graphics()->EnableMousePointer(false);
#endif
	GraphicsTestScreen* game = new GraphicsTestScreen();
	GameStateManager::Instance()->ChangeScreen(game);
}

void MainMenu::ControlsClicked(float x, float y) {
	controls->GetTexture()->SetTexture(buttonTexClicked);
}

void MainMenu::QuitClicked(float x, float y) {
	quitGame->GetTexture()->SetTexture(buttonTexClicked);
	GameStateManager::Instance()->Exit();
}

void MainMenu::MusicClicked(float x, float y) {
	if (musicMuted) {
		music->GetTexture()->SetTexture(musicNoMuteHover);
#if WINDOWS_BUILD
		GameStateManager::Audio()->SetMasterVolume(1.0f);
#endif
	}
	else {
		music->GetTexture()->SetTexture(musicMuteHover);
#if WINDOWS_BUILD
		GameStateManager::Audio()->SetMasterVolume(0.0f);
#endif
	}
	musicMuted = !musicMuted;
}

void MainMenu::SoundClicked(float x, float y) {
	if (soundMuted)
		sounds->GetTexture()->SetTexture(soundNoMuteHover);
	else
		sounds->GetTexture()->SetTexture(soundMuteHover);

	soundMuted = !soundMuted;
}

/*--------------------Selected Methods--------------------*/

void MainMenu::NewGameSelected() {
	newGame->GetTexture()->SetTexture(buttonTexHover);
}

void MainMenu::ControlsSelcted() {
	controls->GetTexture()->SetTexture(buttonTexHover);
}

void MainMenu::QuitSelected() {
	quitGame->GetTexture()->SetTexture(buttonTexHover);
}

void MainMenu::MusicSelected() {
	if (musicMuted)
		music->GetTexture()->SetTexture(musicMuteHover);
	else
		music->GetTexture()->SetTexture(musicNoMuteHover);
}

void MainMenu::SoundSelected() {
	if (soundMuted)
		sounds->GetTexture()->SetTexture(soundMuteHover);
	else
		sounds->GetTexture()->SetTexture(soundNoMuteHover);
}

/*-------------------Deselected Methods-------------------*/

void MainMenu::NewGameDeselcted() {
	newGame->GetTexture()->SetTexture(buttonTex);
}

void MainMenu::ControlsDeselected() {
	controls->GetTexture()->SetTexture(buttonTex);
}

void MainMenu::QuitDeselected() {
	quitGame->GetTexture()->SetTexture(buttonTex);
}

void MainMenu::MusicDeselected() {
	if (musicMuted)
		music->GetTexture()->SetTexture(musicMute);
	else
		music->GetTexture()->SetTexture(musicNoMute);
}

void MainMenu::SoundDeselected() {
	if (soundMuted)
		sounds->GetTexture()->SetTexture(soundMute);
	else
		sounds->GetTexture()->SetTexture(soundNoMute);
}

void MainMenu::GamepadDisconnect(GamepadEvents::PlayerIndex playerID)
{
	playerOne = GamepadEvents::PLAYERINDEX_MAX;
	AddDrawable(pressStart);
#if WINDOWS_BUILD
		GameStateManager::Graphics()->EnableMousePointer(true);
#endif
		ClearSelection();
}
#if WINDOWS_BUILD
void MainMenu::MouseEvent(MouseEvents::EventType type, MouseEvents::MouseButtons button, T3Vector2& position)
{
	if (playerOne == GamepadEvents::PLAYERINDEX_MAX)
		GameScreen2D::MouseEvent(type, button, position);
}
void MainMenu::MouseMoved(T3Vector2& start, T3Vector2& finish)
{
	if (playerOne == GamepadEvents::PLAYERINDEX_MAX)
		GameScreen2D::MouseMoved(start, finish);
}
#endif