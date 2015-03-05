#if WINDOWS_BUILD

#include "AudioTestClass.h"
#include "GameStateManager.h"
//#include "../Framework/SoundManager.h"
#include "../Framework/T3Vector3.h"
#include "DrawableEntity3D.h"
#include <Windows.h>
#include <MMsystem.h>
#pragma comment(lib, "winmm.lib")

#define RENDER_HZ 60.0f

AudioTestClass::AudioTestClass(void)
{
	//GameStateManager::Audio()->PlaySoundA (laser,T3Vector3(0,0,0));
	//PlaySound(TEXT("../Sounds/Tokyo Drift2.wav"),NULL,SND_ASYNC|SND_LOOP);
}


AudioTestClass::~AudioTestClass(void)
{
	SetCamera(NULL);
}

void AudioTestClass::LoadContent() {
	/*laser2= SoundManager :: GetSound("../Sounds/36847__ecodtr__laserrocket2.wav");
	laser1= SoundManager :: GetSound("../Sounds/56900__syna-max__war.wav");
	laser= SoundManager :: GetSound("../Sounds/vehicle085.wav");*/
	SoundManager::AddSound(SOUNDSDIR"56900__syna-max__war.wav");
	SoundManager::AddSound(SOUNDSDIR"vehicle085.wav");
	SoundManager::AddSound(SOUNDSDIR"Tokyo Drift2.wav");

	laser = GameStateManager::Audio()->GetSound(SOUNDSDIR"Tokyo Drift2.wav");
	laser1 = GameStateManager::Audio()->GetSound (SOUNDSDIR"56900__syna-max__war.wav");
	laser2 = GameStateManager::Audio()->GetSound (SOUNDSDIR"vehicle085.wav");

	GameStateManager::Audio()->PlaySoundW(laser, SOUNDPRIORITY_ALWAYS);
	GameStateManager::Audio()->PlaySoundA(laser1, T3Vector3(0,0,0));
}

void AudioTestClass::Update() {
	//GameStateManager::Audio()->PlaySoundW(laser,SOUNDPRIORITY_ALWAYS);
	//GameStateManager::Audio()->PlaySoundW(GameStateManager::Audio()->GetSound("../Sounds/Tokyo Drift2.wav"),SOUNDPRIORITY_ALWAYS);
	GameStateManager::Audio()->Update ((1000.0f / ( float ) RENDER_HZ ));
	//AudioEngine::GetSoundSystem()->Update ((1000.0f / ( float ) RENDER_HZ ));

}

void AudioTestClass::KeyboardEvent(KeyboardEvents::EventType type, KeyboardEvents::Key key) {
	switch (type) {
	case KeyboardEvents::KEY_HELD:
	
	case KeyboardEvents::KEY_DOWN:
		switch(key){
	
		//case KeyboardEvents::KEYBOARD_4:
		//	GameStateManager::Audio()->PlaySoundW(laser, SOUNDPRIORITY_ALWAYS);
		//	//GameStateManager::Audio()->PlaySoundA (laser,T3Vector3(0,0,0));
		//	break;
		case KeyboardEvents::KEYBOARD_1:
			GameStateManager::Audio()-> PlaySoundA (laser1,T3Vector3(0,0,0));//SOUNDPRIORTY_LOW);
			break;
		case KeyboardEvents::KEYBOARD_2:
			GameStateManager::Audio()-> PlaySoundA (laser2,T3Vector3(0,0,0));//SOUNDPRIORITY_ALWAYS);
			break;
	    /*case KeyboardEvents::KEYBOARD_4:
			PlaySound(TEXT("../Sounds/36847__ecodtr__laserrocket2.wav"),NULL,SND_ASYNC|SND_LOOP);
		break;*/
		}
	break;
	
	case KeyboardEvents::KEY_PRESS:
		switch (key) {
		case KeyboardEvents::KEYBOARD_ESCAPE:
			GameStateManager::Instance()->Exit();
			break;
		} 
	}
}

#endif