#if WINDOWS_BUILD

#include "AudioTestClass.h"
#include "GameStateManager.h"
//#include "../Framework/SoundManager.h"
#include "../Framework/T3Vector3.h"
#include "DrawableEntity3D.h"
#include <Windows.h>
#include <MMsystem.h>
#include <time.h>
#pragma comment(lib, "winmm.lib")


#define RENDER_HZ 60.0f


AudioTestClass::AudioTestClass(void)
{
	
	//GameStateManager::Audio()->PlaySoundA (laser,T3Vector3(0,0,0));
	//PlaySound(TEXT("../Sounds/Tokyo Drift2.wav"),NULL,SND_ASYNC|SND_LOOP);
	bgm1=false;
	bgm2=false;
	bgm3=false;
	pressonce = false;
	pressonce2 = false;
	pressonce3 = false;
}


AudioTestClass::~AudioTestClass(void)
{
	SetCamera(NULL);
}

void AudioTestClass::LoadContent() {

	SoundManager::AddSound(SOUNDSDIR"alarm1.wav");
	SoundManager::AddSound(SOUNDSDIR"engine.wav");
	SoundManager::AddSound(SOUNDSDIR"bgm1_60sec.wav");
	SoundManager::AddSound(SOUNDSDIR"bgm2_42sec.wav");
	SoundManager::AddSound(SOUNDSDIR"bgm3.wav");
	BGM1 = GameStateManager::Audio()->GetSound(SOUNDSDIR"bgm1_60sec.wav");
	BGM2 = GameStateManager::Audio()->GetSound(SOUNDSDIR"bgm2_42sec.wav");
	BGM3 = GameStateManager::Audio()->GetSound(SOUNDSDIR"bgm3.wav");
	laser1 = GameStateManager::Audio()->GetSound (SOUNDSDIR"engine.wav");
	laser3 = GameStateManager::Audio()->GetSound (SOUNDSDIR"alarm1.wav");

	//GameStateManager::Audio()-> PlaySound (BGM3,SOUNDPRIORITY_ALWAYS, false);//SOUNDPRIORTY_LOW);
	GameStateManager::Audio()-> PlaySound (laser1,T3Vector3(0,0,0), true);
	GameStateManager::Audio()-> PlaySound (laser3,T3Vector3(100,0,0), true);
}

void AudioTestClass::Update() {
	//GameStateManager::Audio()->PlaySoundW(laser,SOUNDPRIORITY_ALWAYS);
	//GameStateManager::Audio()->PlaySoundW(GameStateManager::Audio()->GetSound("../Sounds/Tokyo Drift2.wav"),SOUNDPRIORITY_ALWAYS);
	GameStateManager::Audio()->Update ((1000.0f / ( float ) RENDER_HZ ));
	CurrentTime=(float)timer.GetMS();
	/*if(bgm1==false&&CurrentTime>=0&&CurrentTime<=62000){
		GameStateManager::Audio()-> PlaySound (BGM1,SOUNDPRIORITY_ALWAYS, false);
		bgm1=true;
	}
	if(bgm2==false&&CurrentTime>62000&&CurrentTime<=104000){
		GameStateManager::Audio()-> PlaySound (BGM2,SOUNDPRIORITY_ALWAYS, false);
		bgm2=true;
	}
	if(bgm3==false&&CurrentTime>104000){
		GameStateManager::Audio()-> PlaySound (BGM3,SOUNDPRIORITY_ALWAYS, true);
		bgm3=true;
	}*/
}

void AudioTestClass::KeyboardEvent(KeyboardEvents::EventType type, KeyboardEvents::Key key) {
//	switch (type) {
//	case KeyboardEvents::KEY_HELD:
//		switch(key){
//		//	case KeyboardEvents::KEYBOARD_W:
//		//	GameStateManager::Audio()-> PlaySound (laser1,SOUNDPRIORITY_ALWAYS, false);
//		}
//	case KeyboardEvents::KEY_PRESS:
//		switch(key){
//		//case KeyboardEvents::KEYBOARD_W:
//		//	if(!pressonce2){
//		//	GameStateManager::Audio()-> PlaySound (laser1,SOUNDPRIORTY_LOW , false);
//		//	break;
//		//	pressonce2 = true;
//		//	}
//		//	
//		//case KeyboardEvents::KEYBOARD_SPACE:
//		//	if (!pressonce) {
//		//	GameStateManager::Audio()-> PlaySound (laser3,SOUNDPRIORTY_LOW, false);//SOUNDPRIORTY_LOW);
//		//	break;
//		//	pressonce = true;
//		//	}
//	
//		case KeyboardEvents::KEYBOARD_ESCAPE:
//			GameStateManager::Instance()->Exit();
//			break;
//
//	    /*case KeyboardEvents::KEYBOARD_4:
//			PlaySound(TEXT("../Sounds/36847__ecodtr__laserrocket2.wav"),NULL,SND_ASYNC|SND_LOOP);
//		break;*/
//		}
//	break;
//	
////	case KeyboardEvents::KEY_PRESS:
////		switch (key) {
////		case KeyboardEvents::KEYBOARD_ESCAPE:
////			GameStateManager::Instance()->Exit();
////			break;
////		} 
//	}
}
#endif