#pragma once
#if WINDOWS_BUILD
#include <windows.h>
#endif
#include <iostream>
#include <vector>
#include <string>

using std::vector;
using std::string;

/**
* Abstract Thread
*/
class Thread
{
public:
	Thread()
	{
		isRunning = false;
	}
#if WINDOWS_BUILD
	virtual ~Thread(){ CloseHandle(thread_handle);}
#endif
	virtual void Start();
	virtual void Join();
	virtual void Terminimumate();
#if WINDOWS_BUILD
	virtual DWORD get_id() const {return tid;}
#endif
protected:
	virtual void Run() = 0;
#if WINDOWS_BUILD
	friend DWORD thread_ftn(LPVOID T);
	HANDLE thread_handle;
	DWORD tid;
#endif
	bool isRunning;
private:
	Thread(const Thread& src);
	Thread& operator=(const Thread& rhs);
};
