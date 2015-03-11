#if WINDOWS_BUILD
#include "SoundManager.h"

map < string , Sound * > SoundManager::sounds;

 //void SoundManager :: 11 ( string name ) {
	// Sound * s = GetSound ( name );

	// if (! s ) {
	//	 string extension = name.substr ( name.length () -3 ,3);

	//	 if( extension == "wav") {
	//		 s = new Sound ();
	//		 s -> LoadFromWAV ( name );
	//		 alGenBuffers (1 ,& s -> buffer );
	//		 alBufferData (s -> buffer ,s -> GetOALFormat () ,s -> GetData () ,s -> GetSize () ,( ALsizei )s -> GetFrequency ());
	//	 }
	//	 else if( extension == "ogg") {
	//		 OggSound * ogg = new OggSound ();
	//		 ogg -> LoadFromOgg ( name );

	//		 s = ogg ;
	//	 }
	//	 else {
	//		 s = NULL ;
	//	 }
	//	 sounds.insert ( make_pair ( name , s ));
	// }
 //}
void SoundManager :: AddSound ( string name ) {
	 if (! GetSound ( name )) {
		 Sound * s = new Sound ();

		 string extension = name.substr ( name.length () -3 ,3);//the type of sound like wav

		 if( extension == "wav") {
			s-> LoadFromWAV ( name );//if wav, then call loadfrom WAV func to extract PCM data from the file

			alGenBuffers (1 ,& s -> buffer );//generate a buffer name 

			alBufferData (s -> buffer ,s -> GetOALFormat () ,s -> GetData () ,
						s -> GetSize () ,( ALsizei )s -> GetFrequency ());    // sound PCM data
		 }
		 else {
		 }
		 sounds.insert ( make_pair ( name , s ));
	 }
}


Sound * SoundManager :: GetSound ( string name ) {
	 map < string , Sound * >:: iterator s = sounds.find ( name );
	 return ( s != sounds.end () ? s-> second : NULL );
 }


void SoundManager :: DeleteSounds () {
	 for ( map < string , Sound * >:: iterator i = sounds.begin ();
			i != sounds.end (); ++ i ) {
		delete i -> second;
	 }
 }
#endif