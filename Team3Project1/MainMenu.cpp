#include "MainMenu.h"


MainMenu::MainMenu(void)
{
#if WINDOWS_BUILD
	GameStateManager::Graphics()->EnableMousePointer(true);
#endif
	musicMuted = false;
	soundMuted = false;
	playerOne = GamepadEvents::PLAYERINDEX_MAX;

	connectionTime = 0.0;
	inputEnabled = true;
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
			connectionTime = 500;
		}
	}
	else
		connectionTime < 0 ? connectionTime = 0 : connectionTime -= 5.0; // Slightly hacky way of not triggering input as soon as we first get a controller.
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
	//	delete pressStart;
	//	pressStart = NULL;
	}
}

void MainMenu::LoadContent() {
	GameStateManager::Graphics()->EnableLoadingIcon(true);
	wallpaper = new MenuScreen3D();
	GameStateManager::AddGameScreen(wallpaper);

	Font* font = GameStateManager::Assets()->LoadFont(this, "quadrats", 16, 16);

	buttonTex			= GameStateManager::Assets()->LoadTexture(this, "Buttons/button", 0);
	buttonTexHover		= GameStateManager::Assets()->LoadTexture(this, "Buttons/button_selected", 0);
	buttonTexClicked	= GameStateManager::Assets()->LoadTexture(this, "Buttons/button_clicked", 0);
	scoreBoardTex		= GameStateManager::Assets()->LoadTexture(this, "score_board", 0);
	titleArtTex			= GameStateManager::Assets()->LoadTexture(this, "TitleArt", 0);

	musicNoMute			= GameStateManager::Assets()->LoadTexture(this, "Buttons/music_nomute", 0);
	musicNoMuteHover	= GameStateManager::Assets()->LoadTexture(this, "Buttons/music_nomute_selected", 0);
	musicMute			= GameStateManager::Assets()->LoadTexture(this, "Buttons/music_mute", 0);
	musicMuteHover		= GameStateManager::Assets()->LoadTexture(this, "Buttons/music_mute_selected", 0);

	soundNoMute			= GameStateManager::Assets()->LoadTexture(this, "Buttons/sound_nomute", 0);
	soundNoMuteHover	= GameStateManager::Assets()->LoadTexture(this, "Buttons/sound_nomute_selected", 0);
	soundMute			= GameStateManager::Assets()->LoadTexture(this, "Buttons/sound_mute", 0);
	soundMuteHover		= GameStateManager::Assets()->LoadTexture(this, "Buttons/sound_mute_selected", 0);

	// Load Scoreboard
#if WINDOWS_BUILD
	scoreBoardConn = new Scoreboard();
	scoreBoardConn->RetrieveScoreboard();
	for (unsigned int i = 0; i < 10; i++) {
		scoreBoardConn->GetName(i);
		scores.push_back(pair<DrawableText2D*, DrawableText2D*>(new DrawableText2D(0.57f, 0.180f + (0.065f * i), 2, 0.12f, 0.065f, scoreBoardConn->GetName(i), font, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1.0f,1.0f,1.0f,1.0f), false),
			new DrawableText2D(0.75f, 0.180f + (0.065f * i), 2, 0.12f, 0.065f, scoreBoardConn->GetScore(i), font, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1.0f,1.0f,1.0f,1.0f), false)));
		AddDrawable(scores.back().first);
		AddDrawable(scores.back().second);
	}
#endif

	float charWidth = 0.200f;
	float charHeight = 0.08f;

	//Offset for the text to keep it central in the button.
	float btnOffsetX = 0.02f;
	float btnOffsetY = 0.04f;

	//Button size
	float btnScaleX = 0.240f;
	float btnScaleY = 0.152f;

	//Button Position, determines the first button's postion, and Y is multiplied to place succesive buttons uniformly.
	float btnX = 0.21f;
	float btnY = 0.38f;

	newGame	= new UIButton(btnX, btnY, btnScaleX, btnScaleY, (void (GameScreen2D::*)()) &MainMenu::NewGameSelected, (void (GameScreen2D::*)(float, float)) &MainMenu::NewGameClicked, 
				new DrawableTexture2D(btnX, btnY, 1.0f, btnScaleX, btnScaleY, buttonTex, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1.0f,1.0f,1.0f,1.0f), false),
				new DrawableText2D(btnX + btnOffsetX, btnY + btnOffsetY, 2, charWidth, charHeight, "NEW GAME", font, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1.0f,1.0f,1.0f,1.0f), false), (void (GameScreen2D::*)()) &MainMenu::NewGameDeselcted, this);
	
	controls = new UIButton(btnX, btnY + 0.2f, btnScaleX, btnScaleY, (void (GameScreen2D::*)()) &MainMenu::ControlsSelcted, (void (GameScreen2D::*)(float, float)) &MainMenu::ControlsClicked, 
				new DrawableTexture2D(btnX, btnY + 0.17f, 1.0f, btnScaleX, btnScaleY, buttonTex, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1.0f,1.0f,1.0f,1.0f), false),
				new DrawableText2D(btnX + btnOffsetX, btnY + 0.17f + btnOffsetY, 2, charWidth, charHeight, "CONTROLS", font, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1.0f,1.0f,1.0f,1.0f), false), (void (GameScreen2D::*)()) &MainMenu::ControlsDeselected, this);
	
	quitGame = new UIButton(btnX, btnY + 0.4f, btnScaleX, btnScaleY, (void (GameScreen2D::*)()) &MainMenu::QuitSelected, (void (GameScreen2D::*)(float, float)) &MainMenu::QuitClicked, 
				new DrawableTexture2D(btnX, btnY + 0.34f, 1.0f, btnScaleX, btnScaleY, buttonTex, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1.0f,1.0f,1.0f,1.0f), false),
				new DrawableText2D(btnX + btnOffsetX, btnY + 0.34f + btnOffsetY, 2, charWidth, charHeight, "QUIT GAME", font, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1.0f,1.0f,1.0f,1.0f), false), (void (GameScreen2D::*)()) &MainMenu::QuitDeselected, this);
	
	float musicBtnWidth = 0.045f;
	float musicBtnHeight = 0.089f;

	float musicBtnX = 0.095f;
	float musicBtnY = 0.70f;

	music = new UIButton(musicBtnX, musicBtnY, musicBtnWidth, musicBtnHeight, (void(GameScreen2D::*)()) &MainMenu::MusicSelected, (void (GameScreen2D::*)(float, float)) &MainMenu::MusicClicked,
				new DrawableTexture2D(musicBtnX, musicBtnY, 1.0f, musicBtnWidth, musicBtnHeight, musicNoMute, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1.0f,1.0f,1.0f,1.0f), false),
				new DrawableText2D(musicBtnX, musicBtnY, 0, 0.0f, 0.0f, "", font, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1.0f,1.0f,1.0f,1.0f), false), (void (GameScreen2D::*)()) &MainMenu::MusicDeselected, this);

	sounds = new UIButton(musicBtnX, musicBtnY + musicBtnHeight, musicBtnWidth, musicBtnHeight, (void(GameScreen2D::*)()) &MainMenu::SoundSelected, (void (GameScreen2D::*)(float, float)) &MainMenu::SoundClicked,
				new DrawableTexture2D(musicBtnX, musicBtnY + musicBtnHeight, 1.0f, musicBtnWidth, musicBtnHeight, soundNoMute, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1.0f,1.0f,1.0f,1.0f), false),
				new DrawableText2D(musicBtnX, musicBtnY + musicBtnHeight, 0, 0.0f, 0.0f, "", font, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1.0f,1.0f,1.0f,1.0f), false), (void (GameScreen2D::*)()) &MainMenu::SoundDeselected, this);

	scoreBoard = new DrawableTexture2D(0.55f, 0.145f, 1.0f, 0.35f, 0.75f, scoreBoardTex, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1.0f,1.0f,1.0f,1.0f), false);
	titleArt = new DrawableTexture2D(0.13f, 0.15f, 1.0f, 0.4f, 0.2f, titleArtTex, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1.0f,1.0f,1.0f,1.0f), false);

	AddDrawable(scoreBoard);
	AddDrawable(titleArt);

	AddClickable(newGame);
	AddClickable(controls);
	AddClickable(quitGame);
	AddClickable(music);
	AddClickable(sounds);

	Font* startFont = GameStateManager::Assets()->LoadFont(this, "tahoma", 16, 16);
	pressStart = new DrawableText2D(0.3f, 0.1f, 1.0f, 0.4f, 0.05f, "Player 1 Press Start or A", startFont);
	AddDrawable(pressStart);

	//finished loading screen
	GameStateManager::Graphics()->EnableLoadingIcon(false);
}

void MainMenu::UnloadContent() {
	RemoveClickable(newGame, true, false);
	RemoveClickable(controls, true, false);
	RemoveClickable(quitGame, true, false);
	RemoveClickable(music, true, false);
	RemoveClickable(sounds, true, false);

	RemoveDrawable(scoreBoard, false);
	RemoveDrawable(pressStart, false);
	RemoveDrawable(titleArt, false);

#ifdef WINDOWS_BUILD
	for (unsigned int i = 0; i < 10; i++) {
		RemoveDrawable(scores[i].first, false);
		RemoveDrawable(scores[i].second, false);
	}
	scores.clear();
#endif

	GameStateManager::Assets()->UnloadTexture(this, "Buttons/button");
	GameStateManager::Assets()->UnloadTexture(this, "Buttons/button_selected");
	GameStateManager::Assets()->UnloadTexture(this, "Buttons/button_clicked");
	GameStateManager::Assets()->UnloadTexture(this, "score_board");
	GameStateManager::Assets()->UnloadTexture(this, "TitleArt");
								
	GameStateManager::Assets()->UnloadTexture(this, "Buttons/music_nomute");
	GameStateManager::Assets()->UnloadTexture(this, "Buttons/music_nomute_selected");
	GameStateManager::Assets()->UnloadTexture(this, "Buttons/music_mute");
	GameStateManager::Assets()->UnloadTexture(this, "Buttons/music_mute_selected");

	GameStateManager::Assets()->UnloadTexture(this, "Buttons/sound_nomute");
	GameStateManager::Assets()->UnloadTexture(this, "Buttons/sound_nomute_selected");
	GameStateManager::Assets()->UnloadTexture(this, "Buttons/sound_mute");
	GameStateManager::Assets()->UnloadTexture(this, "Buttons/sound_mute_selected");

	GameStateManager::Assets()->UnloadFont(this, "quadrats");
	GameStateManager::Assets()->UnloadFont(this, "tahoma");
#if WINDOWS_BUILD
	delete scoreBoardConn;
#endif
}



/*--------------------Clicked Methods--------------------*/

void MainMenu::NewGameClicked(float x, float y) 
{
	std::cout << "New Game Clicked" << std::endl;
	GameStateManager::Graphics()->EnableLoadingIcon(true);
	newGame->GetTexture()->SetTexture(buttonTexClicked);
#if WINDOWS_BUILD
	GameStateManager::Graphics()->EnableMousePointer(false);
#endif
	GameStateManager::Instance()->RemoveGameScreen(wallpaper);
	GameStateManager::Instance()->RemoveGameScreen(this);
#if WINDOWS_BUILD
	RacerGame* game = new RacerGame();
//	GraphicsTestScreen* game = new GraphicsTestScreen();
	GameStateManager::Physics()->SetGame(game);
	GameStateManager::Instance()->ChangeScreen(game);
	//GameStateManager::RemoveGameScreen(this);
#endif
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
		GameStateManager::Audio()->EnableGlobalSounds(true);
		GameStateManager::Audio()->EnableLocalSounds(true);
#endif
	}
	else {
		music->GetTexture()->SetTexture(musicMuteHover);
#if WINDOWS_BUILD
		GameStateManager::Audio()->EnableGlobalSounds(false);
		GameStateManager::Audio()->EnableLocalSounds(true);
#endif
	}
	musicMuted = !musicMuted;
}

void MainMenu::SoundClicked(float x, float y) {
	if (soundMuted){
		sounds->GetTexture()->SetTexture(soundNoMuteHover);
#if WINDOWS_BUILD
		GameStateManager::Audio()->SetMasterVolume(1.0f);
#endif		
	}
	else{
		sounds->GetTexture()->SetTexture(soundMuteHover);
#if WINDOWS_BUILD
		GameStateManager::Audio()->SetMasterVolume(0.0f);
#endif	
	}
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
void MainMenu::GamepadEvent(GamepadEvents::PlayerIndex playerID, GamepadEvents::EventType type, GamepadEvents::Button button)
{
	std::cout <<"MainMenu:GPEvent"<<std::endl;
	if (connectionTime < 200.0)
		GameScreen2D::GamepadEvent(playerID, type, button);
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