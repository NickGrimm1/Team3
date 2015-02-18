/**
<summary>Static Singleton class.
Holds references to all framework abstractions in the layer below.
Keeps track of all gamescreens currently active (ie being rendered, accepting input and being updated.
Keeps track of pause & loading gamestates.
Drives the Game Loop.

Author: Nick Grimm
Version: 1.0.0 03/02/2015.</summary>
*/
#pragma once
#include <vector>
#include "GameScreen.h"
#include "PhysicsEngine.h"
#include "GraphicsEngine.h"
#include "AssetManager.h"
#include "InputManager.h"
#include "DebugManager.h"
#include "AudioEngine.h"
#include "StorageManager.h"
#include "NetworkManager.h"
#include "../Framework/Keyboard.h"
#include "../Framework/Mouse.h"
#include "InputListener.h"
#include "GamePad.h"
#include <iostream>

using namespace std;

class GameStateManager : public InputListener
{
public:
	/**
	<summary>Initializes the Game State Manager.</summary>
	*/
	static bool Initialize()
	{
		// Initialize
		if (instance == NULL)
		{
			instance = new GameStateManager();
			Instance()->isRunning = true;

			if (!AssetManager::Initialize(instance->assets))
				return false;
			if (!GraphicsEngine::Initialize(instance->graphics))
				return false;	
			if (!GraphicsEngine::LoadContent())
				return false;	
			if (!PhysicsEngine::Initialize(instance->physics))
				return false;
			if (!StorageManager::Initialize(instance->storage))
				return false;
			if (!InputManager::Initialize(instance->input))
				return false;
			if (!AudioEngine::Initialize(instance->audio))
				return false;
			if (!NetworkManager::Initialize(instance->network))
				return false;
			if (!DebugManager::Initialize(instance->debug))
				return false;
			instance->isLoaded = true;
		}
		return instance->isLoaded;
	}

	void Start() {
		// Start Threads
		Instance()->graphics->Start();
		Instance()->physics->Start();
		Instance()->input->Start();

		while (instance->isRunning) {
			Window::GetWindow().UpdateWindow();
			for (unsigned int i = 0; i < gameScreens.size(); i++) {
				gameScreens[i]->Update();
			}
		}
	}

	void Exit()
	{
		graphics->Terminate();
		physics->Terminate();
		input->Terminate();

		// Clean up
		graphics->Join();
		physics->Join();
		input->Join();

		// Destroy everything
		GraphicsEngine::Destroy();
		AssetManager::Destroy();
		PhysicsEngine::Destroy();
		StorageManager::Destroy();
		InputManager::Destroy();
		AudioEngine::Destroy();
		NetworkManager::Destroy();
		DebugManager::Destroy();

		if (instance != NULL)
			delete instance;
		instance = NULL;
	}
	~GameStateManager()
	{
		
	}
#pragma region Framework Abstractions
	/**
	<summary>Gets the game's Graphics Engine.</summary>
	*/
	static GraphicsEngine* Graphics() { return Instance()->graphics; }
	/**
	<summary>Gets the game's Audio Engine.</summary>
	*/
	static AudioEngine* Audio() { return Instance()->audio; }
	/**
	<summary>Gets the game's Asset Manager.</summary>
	*/
	static AssetManager* Assets() { return Instance()->assets; }
	/**
	<summary>Gets the game's Debug Manager.</summary>
	*/
	static DebugManager* Debug() { return Instance()->debug; }
	/**
	<summary>Gets the game's Input Manager.</summary>
	*/
	static InputManager* Input() { return Instance()->input; }
	/**
	<summary>Gets the game's Physics Engine.</summary>
	*/
	static PhysicsEngine* Physics() { return Instance()->physics; }
	/**
	<summary>Gets the game's Storage Manager.</summary>
	*/
	static StorageManager* Storage() { return Instance()->storage; }
	/**
	<summary>Gets the game's Network Manager.</summary>
	*/
	static NetworkManager* Network() { return Instance()->network; }
#pragma endregion
#pragma region State Methods
	/**
	<summary>Calls the Update </summary>
	*/
	static void Update()
	{
		for (unsigned int i = 0; i < Instance()->gameScreens.size(); i++)
			Instance()->gameScreens[i]->Update();
	}
	/**
	<summary>Activates a global pause for the game. Note that individual game screens choose their own pause/resume behaviour.</summary>
	*/
	static void Pause()
	{
		for (unsigned int i = 0; i < Instance()->gameScreens.size(); i++)
			Instance()->gameScreens[i]->Pause();
	}
	/**
	<summary>Activates a global resume for the game. Note that individual game screens choose their own pause/resume behaviour.</summary>
	*/
	static void Resume()
	{
		for (unsigned int i = 0; i < Instance()->gameScreens.size(); i++)
			Instance()->gameScreens[i]->Resume();
	}
	/**
	<summary>Gets whether the engine has completed loading.</summary>
	*/
	static bool IsLoaded()
	{
		return Instance()->isLoaded;
	}
#pragma endregion
#pragma region Input Handling
	/**
	<summary>Notifies all screens in the stack of a mouse event.</summary>
	<param name='type'>The event type.</param>
	<param name='position'>The resolution independent co-ordinates of the mouse cursor.</param>
	*/
	void MouseEvent(MouseEvents::EventType type, MouseEvents::MouseButtons button, Vector2& position)
	{
		for (unsigned int i = 0; i < instance->gameScreens.size(); i++)
			instance->gameScreens[i]->MouseEvent(type, button, position);
	}
	/**
	<summary>Notifies all screens in the stack that the mouse has moved.</summary>
	<param name='start'>The resolution independent co-ordinates of the mouse cursor at the start of the frame.</param>
	<param name='finish'>The resolution independent co-ordinates of the mouse cursor at the end of the frame.</param>
	*/
	void MouseMoved(Vector2& finish)
	{
		for (unsigned int i = 0; i < instance->gameScreens.size(); i++)
			instance->gameScreens[i]->MouseMoved(finish);
	}
	/**
	<summary>Notifies all screens in the stack that the mouse scroll wheel has moved.</summary>
	<param name='amount'>The amount of the movement.</param>
	*/
	void MouseScrolled(Vector2& position, int amount)
	{
		for (unsigned int i = 0; i < instance->gameScreens.size(); i++)
			instance->gameScreens[i]->MouseScrolled(position, amount);
	}
	/**
	<summary>Notifies all screens in the stack of a keyboard event.</summary>
	<param name='type'>The event type.</param>
	<param name='key'>The key.</param>
	*/
	void KeyboardEvent(KeyboardEvents::EventType type, KeyboardEvents::Key key)
	{
		for (unsigned int i = 0; i < gameScreens.size(); i++)
			gameScreens[i]->KeyboardEvent(type, key);
	}
	/**
	<summary>Notifies all screens in the stack of a gamepad event.</summary>
	<param name='playerID'>The ID for the controller.</param>
	<param name='type'>The event type.</param>
	<param name='button'>The button.</param>
	*/
	void GamepadEvent(GamepadEvents::PlayerIndex playerID, GamepadEvents::EventType type, GamepadEvents::Button button)
	{
		for (unsigned int i = 0; i < gameScreens.size(); i++)
			gameScreens[i]->GamepadEvent(playerID, type, button);
	}
	/**
	<summary>Notifies all screens in the stack that an analogue control is displaced.</summary>
	<param name='playerID'>The ID for the controller.</param>
	<param name='analogueControl'>The control that is displaced.</param>
	<param name='amount'>The amount of the displacement. For the triggers, only the x co-ordinate is used.</param>
	*/
	void GamepadAnalogueDisplacement(GamepadEvents::PlayerIndex playerID, GamepadEvents::AnalogueControl analogueControl, Vector2& amount)
	{
		for (unsigned int i = 0; i < gameScreens.size(); i++)
			gameScreens[i]->GamepadAnalogueDisplacement(playerID, analogueControl, amount);
	}
#pragma endregion
#pragma region Screen Changes
	/**
	<summary>Empties the screen stack and adds to the given screen.</summary>
	<param name='gameScreen'>The new GameScreen to change to.</param>
	*/
	static void ChangeScreen(GameScreen* gameScreen)
	{
		gameScreen->LoadContent();
		Instance()->gameScreens.clear();
		Instance()->gameScreens.push_back(gameScreen);
	}
	/**
	<summary>Adds the new screen to the top of the stack, maintaining the order of screens below it. 2D screens will draw over all others in the order in which they were added.</summary>
	<param name='gameScreen'>The new GameScreen to add.</param>
	*/
	static void AddGameScreen(GameScreen* gameScreen)
	{
		gameScreen->LoadContent();
		Instance()->gameScreens.push_back(gameScreen);
	}
	/**
	<summary>Removes the first instance of a screen from the stack without clearing it. Useful for removing pause & loading screens.</summary>
	<param name='gameScreen'>The GameScreen to remove.</param>
	*/
	static void RemoveGameScreen(GameScreen* gameScreen)
	{
		for (vector<GameScreen*>::iterator i = Instance()->gameScreens.begin(); i != Instance()->gameScreens.end(); i++)
		{
			if (*i == gameScreen)
			{
				Instance()->gameScreens.erase(i);
				break;
			}
		}
	}
#pragma endregion
	/**
	<summary>Gets or creates the single instance of the Game State.</summary>
	*/
	static GameStateManager* Instance()
	{
		if (instance == NULL)
			instance = new GameStateManager();
		return instance;
	}
private:
#pragma region Contructors
	/**
	<summary>Constructor.</summary>
	*/
	GameStateManager() { }
	/**
	<summary>Copy Constructor. Private & not implemented. We don't want multiples of this class.</summary>
	*/
	GameStateManager(GameStateManager const&);
	/**
	<summary>Equals operator. Private & not implemented. We don't want multiples of this class.</summary>
	*/
	void operator=(GameStateManager const&);
#pragma endregion
#pragma region Instancing
	static GameStateManager* instance;
#pragma endregion
#pragma region Framework Components
	bool isLoaded;
	GraphicsEngine* graphics;
	AudioEngine* audio;
	AssetManager* assets;
	DebugManager* debug;
	InputManager* input;
	PhysicsEngine* physics;
	StorageManager* storage;
	NetworkManager* network;
#pragma endregion
	vector<GameScreen*> gameScreens;
	bool isRunning;
};