#include "Thread.h"

/**
* standard thread function
*/
DWORD thread_ftn(LPVOID T) {
	Thread* t = static_cast<Thread*>(T);
	t->isRunning = true;
	t->Run();
	return NULL;
}

void Thread::Start() {
	thread_handle = CreateThread(
		NULL, // default security
		0, // default stack size
		(LPTHREAD_START_ROUTINE)&thread_ftn, // thread function name
		(LPVOID)this, // argument to thread function
		0, // use default creation flags
		&tid);
}

void Thread::Join() {
	WaitForSingleObject(thread_handle, INFINITE);
}

void Thread::Terminate()
{
	isRunning = false;
}
