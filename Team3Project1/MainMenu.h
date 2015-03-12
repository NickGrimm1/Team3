/**
<summary>Its a main menu. Lets user launch game/quit game etc.
This will be how the actual game is started and quit.

Author: Sherif Salem
Version: 0.0.1 02/03/2015.</summary>
*/


#pragma once
#include "GameScreen2D.h"
#include "GamePad.h"
#include "UIButton.h"
#include "GameStateManager.h"
#include "GraphicsTestScreen.h"
#include "RacerGame.h"
#include "MenuScreen3D.h"
#include "Scoreboard.h"

class MainMenu : public GameScreen2D
{
public:
	MainMenu(void);
	~MainMenu(void);

	virtual void LoadContent();
	virtual void Update();
	virtual void UnloadContent();

#if WINDOWS_BUILD
	virtual void MouseScrolled(T3Vector2& position, int amount) { }
	virtual void KeyboardEvent(KeyboardEvents::EventType type, KeyboardEvents::Key key) { }
	virtual void MouseEvent(MouseEvents::EventType type, MouseEvents::MouseButtons button, T3Vector2& position);
	virtual void MouseMoved(T3Vector2& start, T3Vector2& finish);
#endif
	virtual void GamepadDisconnect(GamepadEvents::PlayerIndex playerID);
	virtual void GamepadEvent(GamepadEvents::PlayerIndex playerID, GamepadEvents::EventType type, GamepadEvents::Button button);
private:

	void NewGameClicked(float x, float y);
	void ControlsClicked(float x, float y);
	void QuitClicked(float x, float y);
	void MusicClicked(float x, float y);
	void SoundClicked(float x, float  y);

	void NewGameSelected();
	void NewGameDeselcted();

	void ControlsSelcted();
	void ControlsDeselected();

	void QuitSelected();
	void QuitDeselected();

	void MusicSelected();
	void MusicDeselected();

	void SoundSelected();
	void SoundDeselected();

	UIButton*	newGame;
	UIButton*	controls;
	UIButton*	quitGame;
	UIButton*	sounds;
	UIButton*	music;

	DrawableTexture2D*	scoreBoard;
	DrawableTexture2D*	titleArt;

	Texture* buttonTex;
	Texture* buttonTexHover;
	Texture* buttonTexClicked;
	Texture* scoreBoardTex;
	Texture* titleArtTex;

	Texture* musicNoMute;
	Texture* musicNoMuteHover;
	Texture* musicMute;
	Texture* musicMuteHover;

	Texture* soundNoMute;
	Texture* soundNoMuteHover;
	Texture* soundMute;
	Texture* soundMuteHover;

	Sound* Mainmenu_BGM;
	SoundEmitter* mainmusic;
	bool musicMuted;
	bool soundMuted;
	bool mainmenu_bgm;
	MenuScreen3D* wallpaper;
	DrawableText2D* pressStart;
	GamepadEvents::PlayerIndex playerOne;
	
	GameTimer timer;
	double connectionTime;
	float CurrentTime;
#if WINDOWS_BUILD
	Scoreboard* scoreBoardConn;
	vector<pair<DrawableText2D*, DrawableText2D*>> scores;
#endif
};