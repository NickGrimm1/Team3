# include "SoundEmitter.h"


 SoundEmitter :: SoundEmitter ( void ) {
	Reset ();
	pitch = 1;
 }
 SoundEmitter :: SoundEmitter ( Sound * s ) {
	 Reset ();
	 SetSound ( s );
	 pitch = 1;
 }

 void SoundEmitter :: Reset () {
	 priority = SOUNDPRIORTY_LOW ;
	 volume = 1.0f ;
	 radius = 500.0f ;
	 timeLeft = 0.0f ;
	 isLooping = true ;
	 currentSource = NULL ;
	 sound = NULL ;
	 // We ’re adding stuff to the existing constructor !!!
	 streamPos = 0;
	 isGlobal = false ;
	 for( unsigned int i = 0; i < NUM_STREAM_BUFFERS ; ++ i ) {
		streamBuffers [ i ] = 0;
	 }
 }

 SoundEmitter ::~ SoundEmitter ( void ) {
	 DetachSource ();
 }

 bool SoundEmitter :: CompareNodesByPriority ( SoundEmitter *a , SoundEmitter * b ) {
	 return (a -> priority> b -> priority ) ? true :false ;
 }

 void SoundEmitter :: SetSound ( Sound * s ) {
	 sound = s ;
	 DetachSource ();
	 if( sound ) {
		 timeLeft = sound -> GetLength ();
		 if( sound -> IsStreaming ()) { // new bit !
			alGenBuffers ( NUM_STREAM_BUFFERS , streamBuffers );
		 }
		 else {
			alDeleteBuffers ( NUM_STREAM_BUFFERS , streamBuffers );

		 }
	 }
	 if( s ) {
		timeLeft = s -> GetLength ();
	 }
	 
 }

 void SoundEmitter :: AttachSource ( OALSource * s ) {
	 currentSource = s ;

	 if (! currentSource ) {
		return ;
	 }
	 currentSource->inUse = true ;

	 alSourceStop ( currentSource -> source );
	 alSourcef ( currentSource -> source , AL_MAX_DISTANCE , radius );
	 alSourcef ( currentSource -> source , AL_REFERENCE_DISTANCE , radius * 0.2f );

	 if( sound -> IsStreaming ()) {
		 streamPos = timeLeft ; // Seeks to correct position in stream
		 int numBuffered = 0; // How many buffers have we filled ?
		 while ( numBuffered < NUM_STREAM_BUFFERS ) {
			 double streamed = sound -> StreamData ( streamBuffers [ numBuffered ] , streamPos ); // Stream in some more data
			 if( streamed ) {
				 streamPos -= streamed ;
				 ++ numBuffered ; // Filled another buffer !
			 }
			 else {
				break ; // Can ’t fill any more buffers ...
			 }
		 }
		 alSourceQueueBuffers ( currentSource -> source , numBuffered , & streamBuffers [0]); // Queue up buffers
	 }
	 else {
		 alSourcei ( currentSource -> source , AL_BUFFER , sound -> GetBuffer ());
		 ALuint error = alGetError();
		 alSourcef ( currentSource -> source , AL_SEC_OFFSET ,( sound -> GetLength ()/ 1000.0) - ( timeLeft / 1000.0));
		 error = alGetError();
		 if (error != 0) return;
	 }
	 ALuint error = alGetError();
	 alSourcePlay ( currentSource -> source );
	 error = alGetError();
	 if (error != 0) return;

 }

 void SoundEmitter :: DetachSource () {
	 if (! currentSource ) {
		 return ;
	 }
		 alSourcef ( currentSource -> source , AL_GAIN ,0.0f );
		 alSourceStop ( currentSource -> source );
		 if( sound && sound -> IsStreaming ()) { // Part 2
			 int numProcessed = 0;
			 ALuint tempBuffer ;
			 alGetSourcei ( currentSource -> source , AL_BUFFERS_PROCESSED ,& numProcessed );
			 while ( numProcessed -- ) {
				alSourceUnqueueBuffers ( currentSource -> source , 1 , & tempBuffer );
			 }
		 }
		 alSourcei ( currentSource -> source , AL_BUFFER ,0);
		 currentSource -> inUse = false ;
		 currentSource = NULL ;
 }

 void SoundEmitter :: Update ( float msec ) {
	 timeLeft -= (msec*pitch) ;

	 while ( isLooping && timeLeft < 0.0f ) {
		timeLeft += sound -> GetLength ();
	 }

	 if( currentSource ) {
		 T3Vector3 pos ;
		 if( GetIsGlobal ()) {
			pos = AudioEngine :: GetSoundSystem () ->GetListenerTransform (). GetPositionVector ();
		 }

		 else{
			 if( target ) {
				pos = T3Vector3(0,0,0);/*target -> GetWorldTransform ().GetPositionVector ();*/// set sound source's current position
						  }
			 else {
				pos = this -> position ;
			          }
		 }
		 alSourcefv ( currentSource -> source , AL_POSITION ,(float *)& pos );

		 if( sound -> IsStreaming ()) {
			 int numProcessed ;
			 alGetSourcei ( currentSource -> source , AL_BUFFERS_PROCESSED ,& numProcessed );
			 alSourcei ( currentSource -> source , AL_LOOPING ,0);

			 while ( numProcessed --) {
				 ALuint freeBuffer ;
				 // Pop off the buffer
				 alSourceUnqueueBuffers ( currentSource -> source , 1 , & freeBuffer );

				 streamPos -= sound -> StreamData ( freeBuffer , streamPos );
				 alSourceQueueBuffers ( currentSource -> source , 1 , & freeBuffer );

				 if( streamPos < 0 && isLooping ) {
					 streamPos += sound -> GetLength ();
				 }
			 }
		 }
		 else {
			 alSourcei ( currentSource -> source , AL_LOOPING , isLooping ? 1 : 0);
		 }

		 alSourcef ( currentSource -> source , AL_GAIN , volume );
		 alSourcei ( currentSource -> source , AL_LOOPING , isLooping ? 1 : 0);
		 alSourcef (currentSource->source	,AL_PITCH	,pitch);
		 alSourcef ( currentSource -> source , AL_MAX_DISTANCE , radius );
		 alSourcef ( currentSource -> source , AL_REFERENCE_DISTANCE , radius *0.2f );
	 }
 }
