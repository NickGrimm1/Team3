/**
<summary>Abstract class for representing a screen within the game.
Holds entities that can be drawn and interacted with.

Author: Nick Grimm
Version: 0.0.1 03/02/2015.</summary>
*/
#pragma once
#include "Entity2D.h"
#include "../Framework/Vector2.h"
#include "InputListener.h"
#include "Rectangle.h"

class GameScreen : public Entity2D, public InputListener
{
public:
	/**
	<summary>Constructor.</summary>
	<param name='screenBounds'>The boundary of the screen, given in resolution independent co-ordinates. Will ignore input outside and will not draw outside of these bounds. Default is 0,0,1,1.</param>
	*/
	GameScreen(Rectangle& screenBounds = Rectangle(0.0f, 0.0f, 1.0f, 1.0f)) : screenBounds(screenBounds)
	{ }
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
	Rectangle screenBounds;
};