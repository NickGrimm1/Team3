/******************************************************************************
Class:GameTimer
Author:Rich Davison
Description:Wraps Windows PerformanceCounter. GameTimers keep track of how much
time has passed since they were last polled - so you could use multiple
GameTimers to trigger events at different time periods. 

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////
#pragma once
#if WINDOWS_BUILD
#include "Windows.h"
#endif
#if PS3_BUILD
#include <sys/sys_time.h>
#endif

class GameTimer	{
public:
	GameTimer();
	~GameTimer() { }

	//How many milliseconds have passed since the GameTimer was created
	double	GetMS();
	//How many milliseconds have passed since GetTimedMS was last called
	double	GetTimedMS();

protected:
	double lastTime; //Last time GetTimedMS was called
#if WINDOWS_BUILD
	LARGE_INTEGER	start;			//Start of timer
	LARGE_INTEGER	frequency;		//Ticks Per Second
#endif
#if PS3_BUILD
	system_time_t GetTime();
	system_time_t startTime;
#endif				
};