/**
<summary>Static Singleton.
The game's audio engine.

Author: Nick Grimm
Version: 0.0.1 03/02/2015.</summary>
*/
#pragma once
#include <vector> // I imagine we'll probably need one eventually. It's just here so NULL is defined atm.

class AudioEngine
{
	// TODO: Implement Audio Engine.
public:
	static bool Initialize(AudioEngine*& out) 
	{
		if (instance == NULL)
			instance = new AudioEngine();
		
		out = instance;
		return true; 
	}
	static void Destroy()
	{
		if (instance != NULL)
			delete instance;
		instance = NULL;
	}
private:
	AudioEngine() { }
	~AudioEngine() { }
	static AudioEngine* instance;
};