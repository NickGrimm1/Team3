#include "Thread.h"
#if WINDOWS_BUILD
/**
* standard thread function
*/
DWORD thread_ftn(LPVOID T) {
	Thread* t = static_cast<Thread*>(T);
	t->isRunning = true;
	t->Run();
	return NULL;
}
#endif
void Thread::Start() {
#if WINDOWS_BUILD
	thread_handle = CreateThread(
		NULL, // default security
		0, // default stack size
		(LPTHREAD_START_ROUTINE)&thread_ftn, // thread function name
		(LPVOID)this, // argument to thread function
		0, // use default creation flags
		&tid);
#endif
}

void Thread::Join() {
#if WINDOWS_BUILD
	WaitForSingleObject(thread_handle, INFINITE);
#endif
}

void Thread::Terminimumate()
{
	isRunning = false;
}
