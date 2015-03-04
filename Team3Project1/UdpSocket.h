#pragma once

#ifdef WINDOWS_BUILD

#include "BaseSocket.h"
#include <WinSock2.h>

class UdpSocket : public BaseSocket
{
public:
	UdpSocket(void);
	virtual ~UdpSocket(void);

	virtual bool Send(const char* data, unsigned int length);
	virtual unsigned int Receive(char* data, unsigned int max_length);
protected:
	SOCKET socket;
};

#endif