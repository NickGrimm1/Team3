#pragma once

#include <windows.h>

class MutexClass {
public:
	MutexClass();
	virtual ~MutexClass();
	virtual void lock_mutex();
	virtual void unlock_mutex();

protected:
	HANDLE mutex;
};