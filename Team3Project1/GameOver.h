#pragma once
#include "GameScreen2D.h"
#include "GamePad.h"
#include "UIButton.h"
#include "GameStateManager.h"
#include "GraphicsTestScreen.h"
#include "RacerGame.h"
#include "MenuScreen3D.h"
#include "Scoreboard.h"
#include "MainMenu.h"

class GameOver : public GameScreen2D
{
public:
	GameOver(GameScreen3D* mainGame, GameScreen2D* hud);
	virtual ~GameOver(void);

	virtual void LoadContent();
	virtual void Update();
	virtual void UnloadContent();

#if WINDOWS_BUILD
	virtual void KeyboardEvent(KeyboardEvents::EventType type, KeyboardEvents::Key key){};
	//virtual void MouseEvent(MouseEvents::EventType type, MouseEvents::MouseButtons button, T3Vector2& position) {};
	//virtual void MouseScrolled(T3Vector2& position, int amount) {};
	//virtual void MouseMoved(T3Vector2& start, T3Vector2& finish) {};

#endif
	virtual void GamepadEvent(GamepadEvents::PlayerIndex playerID, GamepadEvents::EventType type, GamepadEvents::Button button){};
	virtual void GamepadAnalogueDisplacement(GamepadEvents::PlayerIndex playerID, GamepadEvents::AnalogueControl analogueControl, T3Vector2& amount){};
	virtual void GamepadDisconnect(GamepadEvents::PlayerIndex playerID) {}

protected:

	void MainMenuClicked(float x, float y);
	void QuitGameClicked(float x, float y);
	void SoundsClicked(float x, float y);
	void MusicClicked(float x, float y);

	void MainMenuSelected();
	void MainMenuDeselected();

	void QuitGameSelected();
	void QuitGameDeselected();

	void SoundsSelected();
	void SoundsDeselected();

	void MusicSelected();
	void MusicDeselected();

	UIButton* mainMenu;
	UIButton* quitGame;

	UIButton* music;
	UIButton* sounds;

	DrawableTexture2D* gameOver;

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

	Texture* gameOverTex;

#if WINDOWS_BUILD
	Sound* Mainmenu_BGM;
	SoundEmitter* mainmusic;
#endif
	bool musicMuted;
	bool soundMuted;

	GameScreen3D* racerScreen;
	GameScreen2D* hudScreen;
};

