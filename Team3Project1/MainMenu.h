#pragma once

#include "GameScreen2D.h"
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
#endif
	virtual void GamepadEvent(GamepadEvents::PlayerIndex playerId, GamepadEvents::EventType type, GamepadEvents::Button button) { }
	virtual void GamepadAnalogueDisplacement(GamepadEvents::PlayerIndex playerID, GamepadEvents::AnalogueControl analogueControl, T3Vector2& amount) { }
private:

	void NewGameClicked(float x, float y);
	void ControlsClicked(float x, float y);
	void QuitClicked(float x, float y);

	void NewGameSelected();
	void NewGameDeselcted();

	void ControlsSelcted();
	void ControlsDeselected();

	void QuitSelected();
	void QuitDeselcted();

	UIButton*	newGame;
	UIButton*	controls;
	UIButton*	quitGame;

	DrawableTexture2D*	scoreBoard;

	Texture* buttonTex;
	Texture* buttonTexHover;
	Texture* buttonTexClicked;
	Texture* scoreBoardTex;
};