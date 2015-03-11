#pragma once
#if WINDOWS_BUILD
#include <windows.h>
#endif
#include <iostream>
#include <vector>
#include <string>
#if PS3_BUILD
#include<sys/ppu_thread.h>
#include<sys/paths.h>
#include<stdlib.h>

#define PPU_STACK_SIZE 32768
#define PPU_PRIORITY 200
#endif
using std::vector;
using std::string;

/**
* Abstract Thread
*/
class Thread
{
public:
#if WINDOWS_BUILD
	Thread()
	{
		isRunning = false;
	}
#endif
#if PS3_BUILD
	Thread(void (*runs)(uint64_t arg))
		: Run(runs)
	{
		isRunning = false;
	}
#endif
#if WINDOWS_BUILD
	virtual ~Thread(){ CloseHandle(thread_handle);}
#endif
	virtual void Start(std::string name = "");
	virtual void Join();
	virtual void Terminate();
#if WINDOWS_BUILD
	virtual DWORD get_id() const {return tid;}
#endif
protected:
#if WINDOWS_BUILD
	virtual void Run() = 0;
	friend DWORD thread_ftn(LPVOID T);
	HANDLE thread_handle;
	DWORD tid;
#endif
#if PS3_BUILD
	void (*Run)(uint64_t arg);
	sys_ppu_thread_t thread;
	int return_val;
#endif
	bool isRunning;
private:
	Thread(const Thread& src);
	Thread& operator=(const Thread& rhs);
};

