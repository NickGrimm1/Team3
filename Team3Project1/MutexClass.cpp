#include "MutexClass.h"


MutexClass::MutexClass() {
#if WINDOWS_BUILD
	mutex = CreateMutex(
		NULL, // default security
		FALSE, // initially not owned
		NULL); // unamed mutex
#endif
}

MutexClass::~MutexClass() {
#if WINDOWS_BUILD	
	CloseHandle(mutex);
#endif
}

void MutexClass::lock_mutex() {
#if WINDOWS_BUILD
	DWORD wait_result;
	wait_result = WaitForSingleObject(
		mutex, // handle to mutex
		INFINITE); // no time-out interval
#endif
}

void MutexClass::unlock_mutex() {
#if WINDOWS_BUILD
	ReleaseMutex(mutex);
#endif
}
