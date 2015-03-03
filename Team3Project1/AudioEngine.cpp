# include  "AudioEngine.h"
# include "../FrameWork/SoundManager.h"
# include "../FrameWork/SoundEmitter.h"
#define RENDER_HZ  60.0f
 //AudioEngine* AudioEngine::instance = NULL;

 AudioEngine * AudioEngine :: instance = NULL ;// make sure only one AudioEngine is created

 //void AudioEngine::Run()
 //{
	//isRunning = true;
	//while (isRunning)
	//{
	//	Update((1000.0f / ( float ) RENDER_HZ ));
	//}
 //}

 AudioEngine :: AudioEngine ( unsigned int channels ) {
	 listener = NULL ;

	 masterVolume = 1.0f ;

	 cout << " Creating AudioEngine !" << endl ;

	 cout << " Found the following devices : "
		  << alcGetString ( NULL , ALC_DEVICE_SPECIFIER ) << endl ;

	 device = alcOpenDevice ( NULL ); // Open the ’best ’ device

	 if (! device ) {
		 cout << " AudioEngine creation failed ! No valid device !" << endl ;
		 return ;
	 }

	 cout << " AudioEngine created with device : "
		  << alcGetString ( device , ALC_DEVICE_SPECIFIER ) << endl ;

	 context = alcCreateContext ( device , NULL );
	 alcMakeContextCurrent ( context );

	 alDistanceModel (AL_NONE);// AL_LINEAR_DISTANCE_CLAMPED );

	 for ( unsigned int i = 0; i < channels ; ++ i ) {
		 ALuint source ;

		 alGenSources (1 ,& source );
		 ALenum error = alGetError ();

		 if( error == AL_NO_ERROR ) {
			sources.push_back (new OALSource ( source ));
			//cout << "Source " << i << ": " << source << endl;
		 }
		 else {
			 break ;
		 }
	 }
	 cout << " AudioEngine has " << sources.size ()
		  << " channels available !" << endl ;

	 camera = NULL;
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
		 T3Vector3 worldPos = T3Vector3(0,0,0);//listenerTransform.GetPositionVector ();

		 T3Vector3 dirup [2];
		 // forward
		 dirup [0].x = 0; //- listenerTransform.values [2];
		 dirup [0].y = 0;//- listenerTransform.values [6];
		 dirup [0].z = 1;//- listenerTransform.values [10];
		 // Up
		 dirup [1].x = 0;//listenerTransform.values [1];
		 dirup [1].y = 1;//listenerTransform.values [5];
		 dirup [1].z = 0; //listenerTransform.values [9];

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

//	 CullNodes ();
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


 void AudioEngine :: PlaySoundA ( Sound * s , T3Vector3 position ) {
	// alcMakeContextCurrent(context);
	 SoundEmitter * n = new SoundEmitter ();
	 n -> SetLooping ( false );
	 n -> SetIsGlobal(false);
	 //n -> SetTransform ( T3Matrix4 :: Translation ( position ));
	 n -> SetSound ( s );
	 temporaryEmitters.push_back ( n );
	// alcMakeContextCurrent(NULL);
 }

 void AudioEngine :: PlaySoundW ( Sound * s , SoundPriority p) {
	 alcMakeContextCurrent(context);
	 SoundEmitter * n = new SoundEmitter ();
	 n -> SetLooping ( true );
	 n -> SetSound ( s );
	 n -> SetIsGlobal ( true );
	 n -> SetPriority ( p );
	 temporaryEmitters.push_back ( n );
	// alcMakeContextCurrent(NULL);
 }


 void	AudioEngine::CullNodes() {
	for(vector<SoundEmitter*>::iterator i = emitters.begin(); i != emitters.end();) {

		float length;

		if((*i)->GetIsGlobal()) {
			length = 0.0f;
		}
		else{
			//length = (listener->GetWorldTransform().GetPositionVector() - (*i)->GetWorldTransform().GetPositionVector()).Length();
			length = ( listenerTransform.GetPositionVector () - (*i) -> position ).Length ();
		}
		
		if(length > (*i)->GetRadius() || !(*i)->GetSound() || (*i)->GetTimeLeft() < 0) {
			(*i)->DetachSource();	//Important!
			i = emitters.erase(i);
		}
		else{
			++i;
		}
	}
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