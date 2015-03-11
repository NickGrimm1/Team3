#pragma once
#if WINDOWS_BUILD
#include <windows.h>
#endif
#if PS3_BUILD
#include <cell/sync/mutex.h>
#endif

class MutexClass 
{
public:
	MutexClass();
	virtual ~MutexClass();
	virtual void lock_mutex();
	virtual void unlock_mutex();

protected:
#if WINDOWS_BUILD
	HANDLE mutex;
#endif
#if PS3_BUILD
	CellSyncMutex mutex;
#endif
};