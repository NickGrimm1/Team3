#if WINDOWS_BUILD 
# include "SoundSystem.h"
 # include"SoundManager.h"
 # include "SoundEmitter.h"


 SoundSystem * SoundSystem :: instance = NULL ;// make sure only one SoundSystem is created

 SoundSystem :: SoundSystem ( unsigned int channels ) {
	 listener = NULL ;
	 masterVolume = 1.0f ;

	 cout << " Creating SoundSystem !" << endl ;

	 cout << " Found the following devices : "
		  << alcGetString ( NULL , ALC_DEVICE_SPECIFIER ) << endl ;

	 device = alcOpenDevice ( NULL ); // Open the ’best ’ device

	 if (! device ) {
		 cout << " SoundSystem creation failed ! No valid device !" << endl ;
		 return ;
	 }

	 cout << " SoundSystem created with device : "
		  << alcGetString ( device , ALC_DEVICE_SPECIFIER ) << endl ;

	 context = alcCreateContext ( device , NULL );
	 alcMakeContextCurrent ( context );

	 alDistanceModel ( AL_LINEAR_DISTANCE_CLAMPED );

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
	 cout << " SoundSystem has " << sources.size ()
		  << " channels available !" << endl ;
	 SoundManager::AddSound("../../Sounds/14615__man__canon.wav");
	 SoundManager::AddSound("../../Sounds/36847__ecodtr__laserrocket2.wav");
	 SoundManager::AddSound("../../Sounds/56900__syna-maximum__war.wav");
	 SoundManager::AddSound("../../Sounds/vehicle085.wav");
 }


 SoundSystem ::~ SoundSystem ( void ) {
	 alcMakeContextCurrent ( NULL );
	 for ( vector < OALSource * >:: iterator i = sources.begin (); i != sources.end (); ++ i ) {
		 alDeleteSources (1 , &(* i ) -> source );
		 delete (* i );
	 }
	 alcDestroyContext ( context );
	 alcCloseDevice ( device );
 }

 void SoundSystem :: SetMasterVolume ( float value ) {
	 value = maximum (0.0f , value );
	 value = minimum (1.0f , value );
	 masterVolume = value ;
	 alListenerf ( AL_GAIN , masterVolume );
 }

 void SoundSystem :: UpdateListener () {
	 if( listener ) {
		 T3Vector3 worldPos = listenerTransform.GetPositionVector ();

		 T3Vector3 dirup [2];
		 // forward
		 dirup [0].x = - listenerTransform.values [2];
		 dirup [0].y = - listenerTransform.values [6];
		 dirup [0].z = - listenerTransform.values [10];
		 // Up
		 dirup [1].x = listenerTransform.values [1];
		 dirup [1].y = listenerTransform.values [5];
		 dirup [1].z = listenerTransform.values [9];

		 alListenerfv ( AL_POSITION ,( float *)& worldPos );
		 alListenerfv ( AL_ORIENTATION ,( float *)& dirup );
	 }
 }

 
 void SoundSystem :: Update ( float msec ) {
	 UpdateListener ();
	 UpdateTemporaryEmitters ( msec ); 
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
 }

 void SoundSystem :: UpdateTemporaryEmitters ( float msec ) {
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


 void SoundSystem :: PlaySoundA ( Sound * s , T3Vector3 position ) {
	 SoundEmitter * n = new SoundEmitter ();
	 n -> SetLooping ( false );
	 //n -> SetTransform ( T3Matrix4 :: Translation ( position ));
	 n -> SetSound ( s );
	 temporaryEmitters.push_back ( n );
 }

 void SoundSystem :: PlaySoundW ( Sound * s , SoundPriority p) {
	 SoundEmitter * n = new SoundEmitter ();
	 n -> SetLooping ( false );
	 n -> SetSound ( s );
	 n -> SetIsGlobal ( true );
	 n -> SetPriority ( p );
	 temporaryEmitters.push_back ( n );
 }


 void	SoundSystem::CullNodes() {
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

 void SoundSystem :: DetachSources ( vector < SoundEmitter * >:: iterator from , vector < SoundEmitter * >:: iterator to ) {
	 for ( vector < SoundEmitter * >:: iterator i = from ; i != to ; ++ i ) {
		(* i ) -> DetachSource ();
	 }
 }

 void SoundSystem :: AttachSources ( vector < SoundEmitter * >:: iterator from , vector < SoundEmitter * >:: iterator to ) {
	 for ( vector < SoundEmitter * >:: iterator i = from ; i != to ; ++ i ) {
		 if (!(* i ) -> GetSource ()) {
			(* i ) -> AttachSource ( GetSource ());
		 }
	 }
 }

 OALSource * SoundSystem :: GetSource () {
	 for ( vector < OALSource * >:: iterator i = sources.begin (); i != sources.end (); ++ i ) {
		 OALSource * s = * i ;
		 if (! s -> inUse ) {
			return s ;
		 }
	 }
	 return NULL ;
 }
#endif