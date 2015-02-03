/**
<summary>Abstract class for representing a screen within the game.
Holds entities that can be drawn and interacted with.

Author: Nick Grimm
Version: 0.0.1 03/02/2015.</summary>
*/
#pragma once
#include "Entity2D.h"
#include "Rectangle.h"
#include "Vector2.h"

class GameScreen : Entity2D
{
public:
	/**
	<summary>Constructor.</summary>
	<param name='screenBounds'>The boundary of the screen, given in resolution independent co-ordinates. Will ignore input outside and will not draw outside of these bounds. Default is 0,0,1,1.</param>
	*/
	GameScreen(Rectangle& screenBounds = Rectangle(0.0f, 0.0f, 1.0f, 1.0f));
	virtual ~GameScreen();
	/**
	<summary>Allows the screen to run an update loop.</summary>
	*/
	virtual void Update() { }
	/**
	<summary>Allows the game to pause.</summary>
	*/
	virtual void Pause() { }
	/**
	<summary>Allows the game to resume.</summary>
	*/
	virtual void Resume() { }
	/**
	<summary>Scrolls the screen by a finite amount.</summary>
	<param name='start'>The start of the scroll.</param>
	<param name='finish'>The end of the scroll.</param>
	*/
	virtual void Scroll(Vector2& start, Vector2& finish);
	/**
	<summary>Loads assets and performs initial logic for setting up the screen.</summary>
	*/
	virtual void LoadContent() = 0;
	#pragma region Input Handling
	/**
	<summary>Handles a mouse event.</summary>
	<param name='type'>The event type.</param>
	<param name='position'>The resolution independent co-ordinates of the mouse cursor.</param>
	*/
	virtual void MouseEvent(MouseEvents::EventType type, Vector2& position) = 0;
	/**
	<summary>Handles when the mouse has moved.</summary>
	<param name='start'>The resolution independent co-ordinates of the mouse cursor at the start of the frame.</param>
	<param name='finish'>The resolution independent co-ordinates of the mouse cursor at the end of the frame.</param>
	*/
	virtual void MouseMoved(Vector2& start, Vector2& finish) = 0;
	/**
	<summary>Handles when the mouse scroll wheel has moved.</summary>
	<param name='amount'>The amount of the movement.</param>
	*/
	virtual void MouseScrolled(float amount) = 0;
	/**
	<summary>Handles a keyboard event.</summary>
	<param name='type'>The event type.</param>
	<param name='key'>The key.</param>
	*/
	virtual void KeyboardEvent(KeyboardEvents::EventType type, KeyboardEvents::Key key) = 0;
	/**
	<summary>Handles a gamepad event.</summary>
	<param name='playerID'>The ID for the controller.</param>
	<param name='type'>The event type.</param>
	<param name='button'>The button.</param>
	*/
	virtual void GamepadEvent(GamepadEvents::PlayerIndex playerID, GamepadEvents::EventType type, GamepadEvents::Button button) = 0;
	/**
	<summary>Handles when an analogue control is displaced.</summary>
	<param name='playerID'>The ID for the controller.</param>
	<param name='analogueControl'>The control that is displaced.</param>
	<param name='amount'>The amount of the displacement. For the triggers, only the x co-ordinate is used.</param>
	*/
	virtual void GamepadAnalogueDisplacement(GamepadEvents::PlayerIndex playerID, GamePadEvents::AnalogueControl analogueControl, Vector2& amount) = 0;
#pragma endregion
protected:
	/**
	<summary>The current offset (scroll deviation) of the screen.</summary>
	*/
	Vector2 offset;
	/**
	<summary>The maximum scroll of the screen in the x direction. (x is left, y is right).</summary>
	*/
	Vector2 maxHorizontalScroll;
	/**
	<summary>The maximum scroll of the screen in the y direction. (x is up, y is down).</summary>
	*/
	Vector2 maxVerticalScroll;
};