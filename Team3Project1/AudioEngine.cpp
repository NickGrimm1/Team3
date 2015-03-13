#if WINDOWS_BUILD

#include  "AudioEngine.h"
#include "../FrameWork/SoundManager.h"
#include "../FrameWork/SoundEmitter.h"
//#include "AudioTestClass.h"
#define RENDER_HZ  60.0f
 //AudioEngine* AudioEngine::instance = NULL;

 AudioEngine * AudioEngine :: instance = NULL ;// make sure only one AudioEngine is created

 void AudioEngine::Run()
 {
	isRunning = true;
	while (isRunning)
	{
		while (Window::GetWindow().GetTimer()->GetMS() - lastFrameTimeStamp <(1000.0f/(float) RENDER_HZ )){ ; } // Fix the timestep
		float msec = (float) Window::GetWindow().GetTimer()->GetMS() - lastFrameTimeStamp;
		lastFrameTimeStamp = (float) Window::GetWindow().GetTimer()->GetMS();
		Update (msec);
	}
 }

 AudioEngine :: AudioEngine ( unsigned int channels ) {
	 listener = new DrawableEntity3D() ;

	 masterVolume = 1.0f ;

	 device = alcOpenDevice ( NULL ); // Open the ’best ’ device

	 if (! device ) {
		 return ;
	 }

	 context = alcCreateContext ( device , NULL );
	 alcMakeContextCurrent ( context );

//	 alDistanceModel (AL_NONE);
	 alDistanceModel (AL_LINEAR_DISTANCE_CLAMPED );

	 
	 for ( unsigned int i = 0; i < channels ; ++ i ) {
		 ALuint source ;

		 alGenSources (1 ,& source );
		 ALenum error = alGetError ();

		 if( error == AL_NO_ERROR ) {
			sources.push_back (new OALSource ( source ));
		 }
		 else {
			 break ;
		 }
	 }

	 //camera = NULL;
	// alcMakeContextCurrent(NULL);
 }


 AudioEngine ::~ AudioEngine ( void ) {
	 alcMakeContextCurrent ( NULL );
	 for ( vector < OALSource * >:: iterator i = sources.begin (); i != sources.end (); ++ i ) {
		 alDeleteSources (1 , &(* i ) -> source );
		 delete (* i );
	 }
	 alcDestroyContext ( context );
	 alcCloseDevice ( device );
 }

 void AudioEngine :: SetMasterVolume ( float value ) {
	 value = max (0.0f , value );
	 value = min (1.0f , value );
	 masterVolume = value ;
	 alListenerf ( AL_GAIN , masterVolume );
 }


 void AudioEngine :: UpdateListener () {
	 if( listener ) {
		 listenerTransform = T3Matrix4::Translation(listener->GetOriginPosition()) * listener->GetRotation().ToMatrix();
		 T3Vector3 worldPos = listener->GetOriginPosition();
		 T3Vector3 dirup [2];
		 // forward
		 dirup [0].x = listenerTransform.values [2];
		 dirup [0].y = listenerTransform.values [6];
		 dirup [0].z = listenerTransform.values [10];
		 // Up
		 dirup [1].x = listenerTransform.values [1];
		 dirup [1].y = listenerTransform.values [5];
		 dirup [1].z = listenerTransform.values [9];

		 alListenerfv ( AL_POSITION ,( float *)& worldPos );
		 alListenerfv ( AL_ORIENTATION ,( float *)& dirup );
	 }
 }

 
 void AudioEngine :: Update(float msec) {
	 alcMakeContextCurrent(context);

	 UpdateListener ();
	 
	
	 if (temporaryEmitters.size() > 0) {
		UpdateTemporaryEmitters ( msec ); 
	 }
	 for ( vector < SoundEmitter * >:: iterator i = emitters.begin (); i != emitters.end (); ++ i ) {
		 frameEmitters.push_back ((* i ));
		 (* i ) -> Update ( msec );
	 }


	 CullNodes ();
	 if( frameEmitters.size () > sources.size ()) {
		 std :: sort ( frameEmitters.begin () , frameEmitters.end () ,SoundEmitter :: CompareNodesByPriority );//Sort the remaining nodes by their priority

		 DetachSources ( frameEmitters.begin ()+( sources.size ()+1) ,frameEmitters.end ());//Detach sources from nodes that are too low priority when there’s not enough sources to go around

		 AttachSources ( frameEmitters.begin () ,frameEmitters.begin () + sources.size ());//Attach sound sources to high priority nodes that don’t have sources
	 }
	 else {
		 AttachSources ( frameEmitters.begin () , frameEmitters.end ());
	 }

	 frameEmitters.clear ();
	// alcMakeContextCurrent(NULL);
 }

 void AudioEngine :: UpdateTemporaryEmitters ( float msec ) {
	 for( vector < SoundEmitter * >:: iterator i = temporaryEmitters.begin (); i != temporaryEmitters.end (); ) {
		 if ((* i ) -> GetTimeLeft () < 0.001f && !(* i ) -> GetLooping ()) {
			 delete (* i );
			 i = temporaryEmitters.erase ( i );
		 }
		 else {
			 frameEmitters.push_back ((* i ));
			 (* i ) -> Update ( msec );
			 ++ i ;
		 }
	 }
 }

  SoundEmitter* AudioEngine :: PlaySound ( Sound * s , T3Vector3 position , bool isLooping) {
	 alcMakeContextCurrent(context);
	 SoundEmitter * n = new SoundEmitter ();
	 T3Matrix4 tempmatrix = T3Matrix4();
	 tempmatrix.ToIdentity();
	 tempmatrix.SetPositionVector(position);
	 n -> SetTransform(tempmatrix);
	 n -> SetLooping ( isLooping );
	 n -> SetIsGlobal( false );
	 n -> SetSound ( s ); 
	 n -> SetRadius(2000.0f);

	 if (isLooping) {
		 emitters.push_back(n);
	 }
	 else {
		 temporaryEmitters.push_back ( n );
	 }
	 return n;
	 // alcMakeContextCurrent(NULL);
 }

 void AudioEngine :: PlaySound ( Sound * s , DrawableEntity3D* entity , bool isLooping) {
	 alcMakeContextCurrent(context);
	 SoundEmitter * n = new SoundEmitter ();
	 T3Matrix4 tempmatrix = T3Matrix4();
	 tempmatrix.ToIdentity();
	 tempmatrix.SetPositionVector(entity->GetOriginPosition());
	 n -> SetTransform(tempmatrix);
	 n -> SetLooping ( isLooping );
	 n -> SetIsGlobal( false );
	 n -> SetSound ( s ); 
	 n -> SetRadius(2000.0f);
	 n -> SetTarget(entity);

	 if (isLooping) {
		 emitters.push_back(n);
	 }
	 else {
		 temporaryEmitters.push_back ( n );
	 }
	 // alcMakeContextCurrent(NULL);
 }
 SoundEmitter* AudioEngine::PlaySound(Sound* s,SoundPriority p, bool isBGM,  bool isLooping )
 {
	 alcMakeContextCurrent(context);
	 SoundEmitter * n = new SoundEmitter ();
	 n -> SetLooping ( isLooping );
	 n -> SetSound ( s );
	 if(isBGM)
	 {
		 n -> SetIsGlobal ( true );
		 n->SetVolume(0.5);
	 }
	 else
	 {
		 n -> SetIsGlobal (false);
	 }
	 n -> SetPriority ( p );
	 //n -> SetVolume(1.0f);
	 //n -> SetRadius(0.0f);
 	 if (isLooping) {
			emitters.push_back ( n );
	 }
	 else {
		    temporaryEmitters.push_back(n);
	 }
	 return n;
 }

 void AudioEngine::StopSound(SoundEmitter* sound)
 {
	 for (unsigned int i = 0; i < temporaryEmitters.size();) {
		 if (temporaryEmitters[i] == sound) {
			 sound->DetachSource();
			 temporaryEmitters.erase(temporaryEmitters.begin() + i);
		 }
		 else {
			i++;
		 }
	 }

	 for (unsigned int i = 0; i <emitters.size();) {
		 if (emitters[i] == sound) {
			 sound->DetachSource();
			 emitters.erase(emitters.begin() + i);
		 }
		 else {
			i++;
		 }
	 }

	 delete sound;
 }
 //void AudioEngine :: PlaySound ( Sound * s, bool isLooping , SoundPriority p) {
	// //alcMakeContextCurrent(context);
	// SoundEmitter * n = new SoundEmitter ();
	// n -> SetLooping ( isLooping );
	// n -> SetSound ( s );
	// n -> SetIsGlobal ( true );
	// n -> SetPriority ( p );
	// //n -> SetVolume(1.0f);
	// //n -> SetRadius(0.0f);
 //	 if (isLooping) {
	//		emitters.push_back ( n );
	// }
	// else {
	//	    temporaryEmitters.push_back(n);
	// }
	//	// alcMakeContextCurrent(NULL);
 //}


 void	AudioEngine::CullNodes() {
	//for(vector<SoundEmitter*>::iterator i = frameEmitters.begin(); i != emitters.end();) {

	  for(vector<SoundEmitter*>::iterator i = frameEmitters.begin(); i != frameEmitters.end();) {

		float length;

		if((*i)->GetIsGlobal()) {
			length = 0.0f;
		}
		else{
			//length = (listener->GetWorldTransform().GetPositionVector() - (*i)->GetWorldTransform().GetPositionVector()).Length();
			length= (listenerTransform.GetPositionVector ()- (*i) -> GetTransform().GetPositionVector()).Length();
		}
		
		if(length > (*i)->GetRadius() || !(*i)->GetSound() || (*i)->GetTimeLeft() < 0) {
			(*i)->DetachSource();	//Important!
			i = frameEmitters.erase(i);
		}
		else{
			++i;
		}
	 }
	//}
}

 void AudioEngine :: DetachSources ( vector < SoundEmitter * >:: iterator from , vector < SoundEmitter * >:: iterator to ) {
	 for ( vector < SoundEmitter * >:: iterator i = from ; i != to ; ++ i ) {
		(* i ) -> DetachSource ();
	 }
 }

 void AudioEngine :: AttachSources ( vector < SoundEmitter * >:: iterator from , vector < SoundEmitter * >:: iterator to ) {
	 for ( vector < SoundEmitter * >:: iterator i = from ; i != to ; ++ i ) {
		 if (!(* i ) -> GetSource ()) {
			(* i ) -> AttachSource ( GetSource ());
		 }
	 }
 }

 OALSource * AudioEngine :: GetSource () {
	 for ( vector < OALSource * >:: iterator i = sources.begin (); i != sources.end (); ++ i ) {
		 OALSource * s = * i ;
		 if (! s -> inUse ) {
			return s ;
		 }
	 }
	 return NULL ;
 }

Sound * AudioEngine::GetSound ( string name ) {
	//alcMakeContextCurrent(context);
	Sound* sound = SoundManager::GetSound(name);
	//alcMakeContextCurrent(context);
	return sound;
}

#endif
