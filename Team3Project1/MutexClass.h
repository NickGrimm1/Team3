#pragma once
#if WINDOWS_BUILD
#include <windows.h>
#endif
class MutexClass {
public:
	MutexClass();
	virtual ~MutexClass();
	virtual void lock_mutex();
	virtual void unlock_mutex();

protected:
#if WINDOWS_BUILD
	HANDLE mutex;
#endif
};