#if WINDOWS_BUILD
# pragma once

 # include <vector>
 # include <algorithm>

 # include "Sound.h"
// # include "SoundEmitter.h"
 # include "../OpenAL 1.1 SDK/include/al.h"
 # include "../OpenAL 1.1 SDK/include/alc.h"
 # include "T3Vector3.h"
 # include "SceneNode.h"
 # include "T3Matrix4.h"
 using std :: vector ;

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

 //enum SoundPriority ;

 class SoundSystem {
 public :
	 static void Initialise ( unsigned int channels = 32) {
		if (! instance ) { instance = new SoundSystem ( channels );}
	 }
		/*channel This can be used to limit the
		number of individual sources OpenAL will try to generate - handy for limiting the performance hit of
		our new SoundSystem on resource limited devices!
		*/

	 static void Destroy () { delete instance ;}

	 inline static SoundSystem * GetSoundSystem () { return instance ;}
	 
	 void SetListenerTransform ( const T3Matrix4 & transform ) {listenerTransform = transform ;}
	 T3Matrix4 GetListenerTransform () {return listenerTransform ;}

	 void AddSoundEmitter ( SoundEmitter * s ) { emitters.push_back(s);}
	 void RemoveSoundEmitter ( SoundEmitter * s );

	 void Update ( float msec );

	 void SetMasterVolume ( float value );

	 void PlaySoundA ( Sound * s , T3Vector3 position );
	 void PlaySoundW ( Sound * s , SoundPriority p = SOUNDPRIORTY_LOW );


 protected :
	 SoundSystem ( unsigned int channels = 32);
	 ~ SoundSystem ( void );

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
	 SceneNode * listener ;

	 vector < OALSource * > sources ;
	 vector < SoundEmitter * > emitters ;
	 vector < SoundEmitter * > frameEmitters ;
	 vector < SoundEmitter * > temporaryEmitters ;


	 static SoundSystem * instance ;
 };

#endif