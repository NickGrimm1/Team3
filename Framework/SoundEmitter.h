#if WINDOWS_BUILD 
# pragma once
 # include "SceneNode.h"
 # include "Sound.h"
 # include "../Team3Project1/AudioEngine.h"
 # define NUM_STREAM_BUFFERS 3 // Part 2

 //enum SoundPriority {
	// SOUNDPRIORTY_LOW ,
	// SOUNDPRIORITY_MEDIUM ,
	// SOUNDPRIORITY_HIGH ,
	// SOUNDPRIORITY_ALWAYS
 //};

 struct OALSource;

 //class SoundSystem;

 class SoundEmitter {
 public :
	 SoundEmitter ( void );
	 SoundEmitter ( Sound * s );
	 ~ SoundEmitter ( void );

	 void Reset ();//As we have multiple constructors, we’re going to keep the initialisation common to both of them in another function - Reset
	 
	 void SetSound ( Sound * s );
	 inline Sound * GetSound () { return sound;}

	 inline void SetPriority ( SoundPriority p ){ priority = p;}
	 inline SoundPriority GetPriority () { return priority;}		// priority

	 inline void SetVolume ( float volume ) {volume1 = minimum (1.0f , maximum (0.0f , volume ));}
	 inline float GetVolume () { return volume1;}		//volume

	 inline void SetLooping ( bool state ) { isLooping = state;}
	 inline bool GetLooping () { return isLooping;}   //sound loop or not

	 inline void SetTransform (T3Matrix4 position) {Position=position;}
	 inline T3Matrix4 GetTransform() {return Position;}

	 inline void SetRadius ( float value ) {radius = maximum (0.0f , value );} // maximum radius which it can be heard
	 inline float GetRadius () { return radius;}
	 
	 inline void			SetPitch(float value)		 {pitch = value;}
	 inline float			GetPitch()					 {return pitch;}

	 inline float GetTimeLeft () { return timeLeft;}

	 inline OALSource * GetSource () { return currentSource;}

	 void SetTarget ( SceneNode * s ) { target = s;}
	 SceneNode* target;
	 void AttachSource ( OALSource * s ); //attach them from a SoundEmitter
	 void DetachSource ();//detach them from a SoundEmitter

	 static bool CompareNodesByPriority ( SoundEmitter *a , SoundEmitter * b );

	 virtual void Update ( float msec );

     T3Vector3 position;

	 bool GetIsGlobal () { return isGlobal ;}
     void SetIsGlobal ( bool value ) { isGlobal = value ;}
 protected :
	 Sound * sound;
	 OALSource * currentSource;
	 //OALSource * oalSource;
	 SoundPriority priority;
	 
	 float volume1;
	 float radius;
	 float pitch;
	 bool isLooping;
	 float timeLeft;
	 bool isGlobal ;
	 T3Matrix4 Position;
	 double streamPos ;
	 ALuint streamBuffers [ NUM_STREAM_BUFFERS ];

 };
#endif
