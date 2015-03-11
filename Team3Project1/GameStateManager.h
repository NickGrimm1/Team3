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
#include "StorageManager.h"
#include "InputListener.h"
#include "GamePad.h"
#ifdef WINDOWS_BUILD
#include "AudioEngine.h"
#include "NetworkManager.h"
#include "../Framework/Keyboard.h"
#include "../Framework/Mouse.h"
#endif
#if PS3_BUILD
#include <sys/timer.h>
#endif

using namespace std;
class GraphicsEngine;
#define GAME_FRAME_TIME 1000.0f / 60.0f
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
			
			// Want to start rendering immediately
			if (!GraphicsEngine::Initialize(instance->graphics))
				return false;	
			Instance()->graphics->Start("Graphics");
			if (!GraphicsEngine::LoadContent())
				return false;

			if (!PhysicsEngine::Initialize(instance->physics))
				return false;
			if (!StorageManager::Initialize(instance->storage))
				return false;
			if (!InputManager::Initialize(instance->input))
				return false;
#ifdef WINDOWS_BUILD
			if (!AudioEngine::Initialize(instance->audio))
				return false;
			if (!NetworkManager::Initialize(instance->network))
				return false;
#endif
			if (!DebugManager::Initialize(instance->debug))
				return false;
			instance->isLoaded = true;
		}
		return instance->isLoaded;
	}

	void Start() {
		// Start Threads
		Instance()->physics->Start("Physics");
		Instance()->input->Start("Input");

		while (instance->isRunning) {
#ifdef WINDOWS_BUILD
			while (Window::GetWindow().GetTimer()->GetMS() - instance->lastUpdate < GAME_FRAME_TIME) { ; }
			instance->lastUpdate = (float) Window::GetWindow().GetTimer()->GetMS();
			Window::GetWindow().UpdateWindow();
#endif
#if PS3_BUILD
			while (GameStateManager::GetTimer()->GetMS() - instance->lastUpdate < GAME_FRAME_TIME)
				sys_timer_usleep(100);
 			float msec = (float)GameStateManager::GetTimer()->GetMS() - instance->lastUpdate;
			instance->lastUpdate = (float)GameStateManager::GetTimer()->GetMS();
#endif
			for (unsigned int i = 0; i < gameScreens.size(); i++) {
				gameScreens[i]->Update();
			}
		}
		instance->Destroy();
	}

	void Exit()
	{
		if (instance != NULL)
		{
			instance->isRunning = false;
		}
	}

	void Destroy() {
		if (instance != NULL) {
			graphics->Terminate();
			physics->Terminate();
			input->Terminate();

			// Clean up
			graphics->Join();
			physics->Join();
			input->Join();

			vector<GameScreen*>::iterator i = instance->gameScreens.begin();
			while (i != instance->gameScreens.end())
			{
				(*i)->UnloadContent();
				delete *i;
				i = instance->gameScreens.erase(i);
			}

			// Unload Engine Assets
			GraphicsEngine::UnloadContent();
			// Destroy everything
			AssetManager::Destroy(); // Do not destroy before graphics in Windows Build - requires OpenGL context
			GraphicsEngine::Destroy();
			PhysicsEngine::Destroy();
			StorageManager::Destroy();
			InputManager::Destroy();
#if WINDOWS_BUILD
			AudioEngine::Destroy();
			NetworkManager::Destroy();
#endif
			DebugManager::Destroy();

			delete instance;
			instance = NULL;
		}
	}
	~GameStateManager()
	{
#if PS3_BUILD
	delete timer;
#endif
	}
#pragma region Framework Abstractions
	/**
	<summary>Gets the game's Graphics Engine.</summary>
	*/
	static GraphicsEngine* Graphics() { return Instance()->graphics; }
#if WINDOWS_BUILD
	/**
	<summary>Gets the game's Audio Engine.</summary>
	*/
	static AudioEngine* Audio() { return Instance()->audio; }
	/**
	<summary>Gets the game's Network Manager.</summary>
	*/
	static NetworkManager* Network() { return Instance()->network; }
#endif
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
#if WINDOWS_BUILD
	void MouseEvent(MouseEvents::EventType type, MouseEvents::MouseButtons button, T3Vector2& position)
	{
		for (unsigned int i = 0; i < instance->gameScreens.size(); i++)
			instance->gameScreens[i]->MouseEvent(type, button, position);
	}
	/**
	<summary>Notifies all screens in the stack that the mouse has moved.</summary>
	<param name='start'>The resolution independent co-ordinates of the mouse cursor at the start of the frame.</param>
	<param name='finish'>The resolution independent co-ordinates of the mouse cursor at the end of the frame.</param>
	*/
	void MouseMoved(T3Vector2& start, T3Vector2& finish)
	{
		for (unsigned int i = 0; i < instance->gameScreens.size(); i++)
			instance->gameScreens[i]->MouseMoved(start, finish);
	}
	/**
	<summary>Notifies all screens in the stack that the mouse scroll wheel has moved.</summary>
	<param name='amount'>The amount of the movement.</param>
	*/
	void MouseScrolled(T3Vector2& position, int amount)
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
#endif
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
	void GamepadAnalogueDisplacement(GamepadEvents::PlayerIndex playerID, GamepadEvents::AnalogueControl analogueControl, T3Vector2& amount)
	{
		for (unsigned int i = 0; i < gameScreens.size(); i++)
			gameScreens[i]->GamepadAnalogueDisplacement(playerID, analogueControl, amount);
	}
	void GamepadDisconnect(GamepadEvents::PlayerIndex playerID)
	{
		for (unsigned int i = 0; i < gameScreens.size(); i++)
			gameScreens[i]->GamepadDisconnect(playerID);
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
		vector<GameScreen*>::iterator i = instance->gameScreens.begin();
		while (i != instance->gameScreens.end())
		{
			(*i)->UnloadContent();
			delete *i;
			i = instance->gameScreens.erase(i);
		}

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
				(*i)->UnloadContent();
				delete *i;
				i = Instance()->gameScreens.erase(i);
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
		return instance;
	}
#if PS3_BUILD
	static GameTimer* GetTimer() { return instance->timer; }
#endif
private:
#pragma region Contructors
	/**
	<summary>Constructor.</summary>
	*/
	GameStateManager() 
	{
#if PS3_BUILD
	timer = new GameTimer();
#endif
	}
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
	AssetManager* assets;
	DebugManager* debug;
	InputManager* input;
	PhysicsEngine* physics;
	StorageManager* storage;
#ifdef WINDOWS_BUILD
	AudioEngine* audio;
	NetworkManager* network;
#endif
#pragma endregion
	vector<GameScreen*> gameScreens;
	bool isRunning;
	float lastUpdate;
#if PS3_BUILD
	GameTimer* timer;
#endif
};
