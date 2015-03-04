#pragma once
#if WINDOWS_BUILD
class BaseSocket
{
public:
	virtual bool Send(const char* data, unsigned int length) =0;
	virtual unsigned int Receive(char* data, unsigned int max_length) =0;
};

#endif