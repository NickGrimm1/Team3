#include "MutexClass.h"


MutexClass::MutexClass() {
	mutex = CreateMutex(
		NULL, // default security
		FALSE, // initially not owned
		NULL); // unamed mutex
}

MutexClass::~MutexClass() {
	CloseHandle(mutex);
}

void MutexClass::lock_mutex() {
	DWORD wait_result;
	wait_result = WaitForSingleObject(
		mutex, // handle to mutex
		INFINITE); // no time-out interval
}

void MutexClass::unlock_mutex() {
	ReleaseMutex(mutex);
}