#include "GameTimer.h"

GameTimer::GameTimer()	
{
#if WINDOWS_BUILD
	QueryPerformanceFrequency((LARGE_INTEGER *)&frequency);
	QueryPerformanceCounter((LARGE_INTEGER *)&start);
#endif
#if PS3_BUILD
	startTime = sys_time_get_system_time();
#endif
	lastTime = GetMS();
}

#if PS3_BUILD
system_time_t GameTimer::GetTime() 
{
	return sys_time_get_system_time()-startTime;
};
#endif

/*
Returns the Milliseconds since timer was started
*/
double GameTimer::GetMS() 
{
#if WINDOWS_BUILD
	LARGE_INTEGER t;	
	QueryPerformanceCounter(&t);
	return (float)((t.QuadPart  - start.QuadPart) * 1000.0 / frequency.QuadPart);
#endif
#if PS3_BUILD
	return GetTime()/((double)sys_time_get_timebase_frequency()/100000.0);
#endif
}

double	 GameTimer::GetTimedMS() 
{
	float a		= GetMS();
	float b		= a-lastTime;
	lastTime	= a;
	return b;
}