#include "MainMenu.h"
#include "GameStateManager.h"

MainMenu::MainMenu(void)
{
	GameStateManager::Graphics()->EnableMousePointer(true);
	
}

void MainMenu::Update(void) {

}

MainMenu::~MainMenu(void)
{
	//delete newGame;
	//delete controls;
	//delete quitGame;
	//delete buttonTex;
	//delete buttonTexHover;
	//delete buttonTexClicked;
}


void MainMenu::LoadContent() {
	Font* font = GameStateManager::Assets()->LoadFont(this, TEXTUREDIR"quadrats.tga", 16, 16);

	buttonTex = GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"Buttons/button.png", 0);
	buttonTexHover = GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"Buttons/button_hover.png", 0);
	buttonTexClicked = GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"Buttons/button_clicked.png", 0);
	scoreBoardTex = GameStateManager::Assets()->LoadTexture(this, TEXTUREDIR"Buttons/score_board.png", 0);

	float charWidth = 0.025f;
	float charHeight = 0.06f;

	//Offset for the text to keep it central in the button.
	float btnOffsetX = 0.025f;
	float btnOffsetY = 0.05f;

	//Button size
	float btnScaleX = 0.21f;
	float btnScaleY = 0.19f;

	//Button Position, determines the first button's postion, and Y is multiplied to place succesive buttons uniformly.
	float btnX = 0.15f;
	float btnY = 0.2f;

	newGame = new UIButton(btnX, btnY, btnScaleX, btnScaleY, (void (GameScreen2D::*)()) &MainMenu::NewGameSelected, (void (GameScreen2D::*)(float, float)) &MainMenu::NewGameClicked, 
				new DrawableTexture2D(btnX, btnY, 1, btnScaleX, btnScaleY, buttonTex, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false),
				new DrawableText2D(btnX + btnOffsetX, btnY + btnOffsetY, 2.0f, 0.16f, 0.1f, "NEW GAME", font, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false), (void (GameScreen2D::*)()) &MainMenu::NewGameDeselcted, this);
	
	controls = new UIButton(btnX, btnY * 2.0f, btnScaleX, btnScaleY, (void (GameScreen2D::*)()) &MainMenu::ControlsSelcted, (void (GameScreen2D::*)(float, float)) &MainMenu::ControlsClicked, 
				new DrawableTexture2D(btnX, btnY * 2.0f, 1, btnScaleX, btnScaleY, buttonTex, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false),
				new DrawableText2D(btnX + btnOffsetX, btnY * 2.0f + btnOffsetY, 2.0f, 0.16f, 0.1f, "CONTROLS", font, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false), (void (GameScreen2D::*)()) &MainMenu::ControlsDeselected, this);
	
	quitGame = new UIButton(btnX, btnY * 2.0f, btnScaleX, btnScaleY, (void (GameScreen2D::*)()) &MainMenu::QuitSelected, (void (GameScreen2D::*)(float, float)) &MainMenu::QuitClicked, 
				new DrawableTexture2D(btnX, btnY * 3.0f, 1, btnScaleX, btnScaleY, buttonTex, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false),
				new DrawableText2D(btnX + btnOffsetX, btnY * 3.0f + btnOffsetY, 2.0f, 0.16f, 0.1f, "QUIT GAME", font, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false), (void (GameScreen2D::*)()) &MainMenu::QuitDeselcted, this);
	
	scoreBoard = new DrawableTexture2D(0.55f, 0.05f, 1, 0.4f, 0.9f, scoreBoardTex, 0.0f, T3Vector2(0.5f, 0.5f), T3Vector4(1,1,1,1), false);

	AddDrawable(scoreBoard);

	AddSelectable(newGame);
	AddSelectable(controls);
	AddSelectable(quitGame);

	AddClickable(newGame);
	AddClickable(controls);
	AddClickable(quitGame);
}

void MainMenu::UnloadContent() {
	GameStateManager::Assets()->UnloadTexture(this, TEXTUREDIR"Buttons/button.png");
	GameStateManager::Assets()->UnloadTexture(this, TEXTUREDIR"Buttons/button_hover.png");
	GameStateManager::Assets()->UnloadTexture(this, TEXTUREDIR"Buttons/button_clicked.png");
	GameStateManager::Assets()->UnloadTexture(this, TEXTUREDIR"Buttons/score_board.png");
}

void MainMenu::NewGameClicked(float x, float y) {
	newGame->GetTexture()->SetTexture(buttonTexClicked);
}

void MainMenu::NewGameSelected() {
	newGame->GetTexture()->SetTexture(buttonTexHover);
}

void MainMenu::NewGameDeselcted() {
	newGame->GetTexture()->SetTexture(buttonTex);
}

void MainMenu::ControlsClicked(float x, float y) {
	controls->GetTexture()->SetTexture(buttonTexClicked);
}

void MainMenu::ControlsSelcted() {
	controls->GetTexture()->SetTexture(buttonTexHover);
}

void MainMenu::ControlsDeselected() {
	controls->GetTexture()->SetTexture(buttonTex);
}

void MainMenu::QuitClicked(float x, float y) {
	quitGame->GetTexture()->SetTexture(buttonTexClicked);
}

void MainMenu::QuitSelected() {
	quitGame->GetTexture()->SetTexture(buttonTexHover);
}

void MainMenu::QuitDeselcted() {
	quitGame->GetTexture()->SetTexture(buttonTex);
}
