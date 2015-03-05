#include "UdpSocket.h"


UdpSocket::UdpSocket(void) : socket(socket)
{
}


UdpSocket::~UdpSocket(void)
{
	closesocket(socket);
}

bool UdpSocket::Send(const char* data, unsigned int length) {

return false;
}

unsigned int UdpSocket::Receive(char* data, unsigned int max_length) {

	return 0;
}