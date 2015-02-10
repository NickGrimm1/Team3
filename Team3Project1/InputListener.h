/**
<summary>Interface for an input listener. Required to get around circular refenencing between the engine layer and the framework layer.

Author: Nick Grimm
Version: 1.0.0 05/02/2015.</summary>
*/
#pragma once
#include "..\Framework\Mouse.h"
#include "..\Framework\Keyboard.h"
#include "GamePad.h"

class InputListener
{
public:
	/**
	<summary>Notifies all screens in the stack of a mouse event.</summary>
	<param name='type'>The event type.</param>
	<param name='position'>The resolution independent co-ordinates of the mouse cursor.</param>
	*/
	virtual void MouseEvent(MouseEvents::EventType type, MouseEvents::MouseButtons button, Vector2& position) = 0;
	/**
	<summary>Notifies all screens in the stack that the mouse has moved.</summary>
	<param name='start'>The resolution independent co-ordinates of the mouse cursor at the start of the frame.</param>
	<param name='finish'>The resolution independent co-ordinates of the mouse cursor at the end of the frame.</param>
	*/
	virtual void MouseMoved(Vector2& finish) = 0;
	/**
	<summary>Notifies all screens in the stack that the mouse scroll wheel has moved.</summary>
	<param name='amount'>The amount of the movement.</param>
	*/
	virtual void MouseScrolled(Vector2& position, float amount) = 0;
	/**
	<summary>Notifies all screens in the stack of a keyboard event.</summary>
	<param name='type'>The event type.</param>
	<param name='key'>The key.</param>
	*/
	virtual void KeyboardEvent(KeyboardEvents::EventType type, KeyboardEvents::Key key) = 0;
	/**
	<summary>Notifies all screens in the stack of a gamepad event.</summary>
	<param name='playerID'>The ID for the controller.</param>
	<param name='type'>The event type.</param>
	<param name='button'>The button.</param>
	*/
	virtual void GamepadEvent(GamepadEvents::PlayerIndex playerID, GamepadEvents::EventType type, GamepadEvents::Button button) = 0;
	/**
	<summary>Notifies all screens in the stack that an analogue control is displaced.</summary>
	<param name='playerID'>The ID for the controller.</param>
	<param name='analogueControl'>The control that is displaced.</param>
	<param name='amount'>The amount of the displacement. For the triggers, only the x co-ordinate is used.</param>
	*/
	virtual void GamepadAnalogueDisplacement(GamepadEvents::PlayerIndex playerID, GamepadEvents::AnalogueControl analogueControl, Vector2& amount) = 0;
};