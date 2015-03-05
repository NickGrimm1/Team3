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

	Texture* buttonTex;
	Texture* buttonTexHover;
	Texture* buttonTexClicked;
	Texture* scoreBoardTex;

	Texture* musicNoMute;
	Texture* musicNoMuteHover;
	Texture* musicMute;
	Texture* musicMuteHover;

	Texture* soundNoMute;
	Texture* soundNoMuteHover;
	Texture* soundMute;
	Texture* soundMuteHover;

	bool musicMuted;
	bool soundMuted;

	DrawableText2D* pressStart;
	GamepadEvents::PlayerIndex playerOne;
};