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
	ready=false;
	coins=false;
	pressonce = false;
	pressonce2 = false;

}


AudioTestClass::~AudioTestClass(void)
{
	SetCamera(NULL);
}

void AudioTestClass::LoadContent() {

	SoundManager::AddSound(SOUNDSDIR"1.33sec.wav");
	SoundManager::AddSound(SOUNDSDIR"bgm1_101sec.wav");
	SoundManager::AddSound(SOUNDSDIR"bgm2_60sec.wav");
	SoundManager::AddSound(SOUNDSDIR"getready3sec.wav");
	SoundManager::AddSound(SOUNDSDIR"gameover2.6sec.wav");
	SoundManager::AddSound(SOUNDSDIR"coins.wav");
	SoundManager::AddSound(SOUNDSDIR"engine_start.wav");
	BGM1 = GameStateManager::Audio()->GetSound(SOUNDSDIR"bgm1_101sec.wav");
	BGM2 = GameStateManager::Audio()->GetSound(SOUNDSDIR"bgm2_60sec.wav");
	Coins = GameStateManager::Audio()->GetSound(SOUNDSDIR"coins.wav");
	Engine_start = GameStateManager::Audio()->GetSound(SOUNDSDIR"engine_start.wav");
	Ready = GameStateManager::Audio()->GetSound (SOUNDSDIR"getready3sec.wav");
	Over = GameStateManager::Audio()->GetSound (SOUNDSDIR"gameover2.6sec.wav");
	//GameStateManager::Audio()->PlaySound (BGM1,SOUNDPRIORITY_ALWAYS,false, true);
	//GameStateManager::Audio()-> PlaySound (BGM3, false,SOUNDPRIORITY_ALWAYS);//SOUNDPRIORTY_LOW);
	/*GameStateManager::Audio()-> PlaySound (ready,T3Vector3(0,0,0), false);
	GameStateManager::Audio()-> PlaySound (over,T3Vector3(100,0,0), false);*/
}

void AudioTestClass::Update() {
	//GameStateManager::Audio()->PlaySoundW(laser,SOUNDPRIORITY_ALWAYS);
	//GameStateManager::Audio()->PlaySoundW(GameStateManager::Audio()->GetSound("../Sounds/Tokyo Drift2.wav"),SOUNDPRIORITY_ALWAYS);
	//GameStateManager::Audio()->Update ((1000.0f / ( float ) RENDER_HZ ))
	CurrentTime=timer.GetMS();
	if(coins==false){
		GameStateManager::Audio()-> PlaySound (Coins,T3Vector3(100,0,0), true);
		coins=true;
	}
	if(ready==false){
		GameStateManager::Audio()-> PlaySound (Ready,T3Vector3(0,0,0), true);
		ready=true;
	}

	if(bgm1==false&&CurrentTime>=0&&CurrentTime<=102000){
		GameStateManager::Audio()-> PlaySound (BGM1,SOUNDPRIORITY_ALWAYS,true, false);
		bgm1=true;
	}
	if(bgm2==false&&CurrentTime>102000){
		GameStateManager::Audio()-> PlaySound (BGM2,SOUNDPRIORITY_ALWAYS,true, true);
		bgm2=true;
	}
}

void AudioTestClass::KeyboardEvent(KeyboardEvents::EventType type, KeyboardEvents::Key key) {
	switch (type) {
	case KeyboardEvents::KEY_HELD:
		switch(key){
		//	case KeyboardEvents::KEYBOARD_W:
		//	GameStateManager::Audio()-> PlaySound (laser1,SOUNDPRIORITY_ALWAYS, false);
		}
	case KeyboardEvents::KEY_PRESS:
		switch(key){
		case KeyboardEvents::KEYBOARD_W:
			if(!pressonce2){
			pressonce2 = true;
			GameStateManager::Audio()-> PlaySound (Coins,SOUNDPRIORTY_LOW ,false, false);
			
			}
			break;
		case KeyboardEvents::KEYBOARD_SPACE:
			if (!pressonce) {
			pressonce = true;
			GameStateManager::Audio()-> PlaySound (Engine_start,SOUNDPRIORTY_LOW, false, false);//SOUNDPRIORTY_LOW);
			
			}
			break;
		case KeyboardEvents::KEYBOARD_ESCAPE:
			GameStateManager::Instance()->Exit();
			break;

			/*case KeyboardEvents::KEYBOARD_4:
			PlaySound(TEXT("../Sounds/36847__ecodtr__laserrocket2.wav"),NULL,SND_ASYNC|SND_LOOP);
		break;*/
		}
	break;
	
//	case KeyboardEvents::KEY_PRESS:
//		switch (key) {
//		case KeyboardEvents::KEYBOARD_ESCAPE:
//			GameStateManager::Instance()->Exit();
//			break;
//		} 
	}
}

#endif
