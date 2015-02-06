/**
Version History:
0.0.5 03/02/2015
0.0.6 05/02/2015
	Added method to enable/disable input.
*/

/**
<summary>Abstract class for representing a screen within the game.
Holds entities that can be drawn and interacted with.

Author: Nick Grimm
Version: 0.0.5 03/02/2015.</summary>
*/
#pragma once
#include "../Framework/Vector2.h"
#include "InputListener.h"
#include "T3Rectangle.h"

class GameScreen : public T3Rectangle, public InputListener
{
public:
	/**
	<summary>Constructor.</summary>
	<param name='x'>The left boundary of the screen, given in resolution independent co-ordinates. Will ignore input outside and will not draw left of this line. Default is 0.</param>
	<param name='x'>The bottom boundary of the screen, given in resolution independent co-ordinates. Will ignore input outside and will not draw under of this line. Default is 0.</param>
	<param name='x'>The width of the screen, given in resolution independent co-ordinates. Will ignore input outside and will not draw right of the line formed by x + width. Default is 1.</param>
	<param name='x'>The height of the screen, given in resolution independent co-ordinates. Will ignore input outside and will not draw above the line formed by y + height. Default is 1.</param>
	*/
	GameScreen(float x = 0.0f, float y = 0.0f, float width = 1.0f, float height = 1.0f) : T3Rectangle(x, y, width, height)
	{ }
	virtual ~GameScreen();
	
	bool IsInputEnabled() { return inputEnabled; }
	void setInputEnabled(bool value) { inputEnabled = value; }
	
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
	bool inputEnabled;
};