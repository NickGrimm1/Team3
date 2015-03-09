/**
<summary>Static Singleton.
The game's audio engine.

Author: Nick Grimm
Version: 0.0.1 03/02/2015.</summary>
*/
 #pragma once
 #include <vector> // I imagine we'll probably need one eventually. It's just here so NULL is defined atm.
 # include <algorithm>
 # include "Thread.h"
 # include "../FrameWork/Sound.h"
 // # include "SoundEmitter.h"
 # include "../OpenAL 1.1 SDK/include/al.h"
 # include "../OpenAL 1.1 SDK/include/alc.h"
 # include "../FrameWork/T3Vector3.h"
 # include "../FrameWork/SceneNode.h"
 # include "../FrameWork/T3Matrix4.h"
 # include "../FrameWork/camera.h"
 //# include   "../FrameWork/Vehicle.h"
 using std :: vector ;

 class GameEntity;
 class SoundEmitter ;

 enum SoundPriority {
	SOUNDPRIORTY_LOW ,
	SOUNDPRIORITY_MEDIUM ,
	SOUNDPRIORITY_HIGH ,
	SOUNDPRIORITY_ALWAYS
 };

 struct OALSource {
    ALuint source ;
	bool inUse ;

	OALSource ( ALuint src ) {
		source = src ;
		inUse = false ;
	  }
};


class AudioEngine/*: public Thread*/
{
	// TODO: Implement Audio Engine.
public:
	static bool Initialize(AudioEngine*& out,unsigned int channels = 32) 
	{
		if (instance == NULL)
			instance = new AudioEngine(channels);
		
		out = instance;
		return true; 
	}
	static void Destroy()
	{
		if (instance != NULL)
			delete instance;
		instance = NULL;
	}

	inline static AudioEngine * GetSoundSystem () { return instance ;}
	 
	void SetListenerTransform ( const T3Matrix4 & transform ) {listenerTransform = transform ;}
	T3Matrix4 GetListenerTransform () {return listenerTransform ;}
	void SetListener(DrawableEntity3D* entity) {listener = entity;}

    void AddSoundEmitter ( SoundEmitter * s ) { emitters.push_back(s);}
	void RemoveSoundEmitter ( SoundEmitter * s );

	void Update (float msec);

	void SetMasterVolume ( float value );

	void PlaySound ( Sound * s , T3Vector3 position, bool isLooping = false );
	void PlaySound ( Sound * s , DrawableEntity3D* entity , bool isLooping = false);
	void PlaySound ( Sound * s , SoundPriority p = SOUNDPRIORTY_LOW, bool isLooping = false);
	
	Sound * GetSound ( string name );

	/*void Run();
	void Terminate() { isRunning = false; }*/
	/*void AddSound(string filePath);
	void RemoveSound(string filePath);*/

private:
	AudioEngine(unsigned int channels = 32) ;
	~AudioEngine(void) ;
	void UpdateListener ();
	void UpdateTemporaryEmitters ( float msec );

	void DetachSources ( vector < SoundEmitter * >:: iterator from ,
	vector < SoundEmitter * >:: iterator to );
	void AttachSources ( vector < SoundEmitter * >:: iterator from ,
	vector < SoundEmitter * >:: iterator to );

	void CullNodes ();
	OALSource * GetSource ();

	T3Matrix4 listenerTransform ;
	float masterVolume ;
	ALCcontext * context ;
	ALCdevice * device ;
	DrawableEntity3D *listener;

	vector < OALSource * > sources ;
	vector < SoundEmitter * > emitters ;
	vector < SoundEmitter * > frameEmitters ;
	vector < SoundEmitter * > temporaryEmitters ;

	static AudioEngine* instance;
	
	//bool isRunning;
};