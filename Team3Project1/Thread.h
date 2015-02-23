#pragma once
#include <windows.h>
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
	virtual ~Thread(){ CloseHandle(thread_handle);}
	virtual void Start();
	virtual void Join();
	virtual void Terminate();
	virtual DWORD get_id() const {return tid;}
protected:
	virtual void Run() = 0;
	friend DWORD thread_ftn(LPVOID T);
	HANDLE thread_handle;
	DWORD tid;
private:
	Thread(const Thread& src);
	Thread& operator=(const Thread& rhs);
	bool isRunning;
};
