/**
<summary>Pause screen. Shows when player presses escape while playing,
allows the music/sound to be muted along with quitting the current instance
of the game to the main menu.

Author: Sherif Salem
Version: 0.0.1 02/03/2015.</summary>
*/

#pragma once

#include "GameScreen2D.h"
#include "GamePad.h"
#include "UIButton.h"
#include "GameStateManager.h"
#include "GraphicsTestScreen.h"
#include "MainMenu.h"
//class AudioTestClass;
class PauseScreen :	public GameScreen2D
{
public:

	PauseScreen(GameScreen3D* mainGame, GameScreen2D* hud);
	~PauseScreen(void);

	virtual void LoadContent();
	virtual void Update();
	virtual void UnloadContent();

	virtual void Resume() {GameStateManager::RemoveGameScreen(this);};

#if WINDOWS_BUILD
	virtual void MouseScrolled(T3Vector2& position, int amount) { }
	virtual void KeyboardEvent(KeyboardEvents::EventType type, KeyboardEvents::Key key) { }
	//virtual void MouseEvent(MouseEvents::EventType type, MouseEvents::MouseButtons button, T3Vector2& position);
	//virtual void MouseMoved(T3Vector2& start, T3Vector2& finish);
#endif
	virtual void GamepadDisconnect(GamepadEvents::PlayerIndex playerID) { }
private:

	void ResumeClicked(float x, float y);
	void MainMenuClicked(float x, float y);
	void SoundsClicked(float x, float y);
	void MusicClicked(float x, float y);

	void ResumeSelected();
	void ResumeDeselected();

	void MainMenuSelected();
	void MainMenuDeselected();

	void SoundsSelected();
	void SoundsDeselected();

	void MusicSelected();
	void MusicDeselected();

	UIButton* resumeGame;
	UIButton* mainMenu;
	UIButton* sounds;
	UIButton* music;

	DrawableTexture2D* pauseLogo;

	Texture* buttonTex;
	Texture* buttonTexHover;
	Texture* buttonTexClicked;

	Texture* musicNoMute;
	Texture* musicNoMuteHover;
	Texture* musicMute;
	Texture* musicMuteHover;

	Texture* soundNoMute;
	Texture* soundNoMuteHover;
	Texture* soundMute;
	Texture* soundMuteHover;

	Texture* pauseTex;

	bool musicMuted;
	bool soundMuted;

	GameScreen3D* racerScreen;
	GameScreen2D* hudScreen;
#if WINDOWS_BUILD
	Sound* Mainmenu_BGM;
	SoundEmitter* mainmusic;
#endif
//	GameTimer timer;
//	float CurrentTime;
//#if WINDOWS_BUILD
//	AudioTestClass* audio;
//#endif
};

