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
void Thread::Start(std::string name) {
#if WINDOWS_BUILD
	thread_handle = CreateThread(
		NULL, // default security
		0, // default stack size
		(LPTHREAD_START_ROUTINE)&thread_ftn, // thread function name
		(LPVOID)this, // argument to thread function
		0, // use default creation flags
		&tid);
#endif
#if PS3_BUILD
	return_val = sys_ppu_thread_create(&thread, Run, (uint64_t)this, PPU_PRIORITY,PPU_STACK_SIZE,SYS_PPU_THREAD_CREATE_JOINABLE, name.c_str());
	if (return_val != CELL_OK)
	{
		std::cout << "Thread initialise failed with error code: " << return_val <<std::endl;
		return;
	}
#endif
}

void Thread::Join() {
#if WINDOWS_BUILD
	WaitForSingleObject(thread_handle, INFINITE);
#endif
#if PS3_BUILD
	uint64_t i;
	sys_ppu_thread_join(thread, &i);
#endif
}

void Thread::Terminate()
{
	isRunning = false;
}

