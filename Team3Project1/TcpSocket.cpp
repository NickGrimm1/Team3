#if WINDOWS_BUILD
#include "TcpSocket.h"
#include "NetworkCommon.h"

#include <ws2tcpip.h>
#include <iostream>

using namespace std;

TcpSocket::TcpSocket(SOCKET socket) : socket(socket)
{
}

TcpSocket::~TcpSocket(void)
{
	closesocket(socket);
}

bool TcpSocket::Send(const char* data, unsigned int length) {
	unsigned int bytesToSend = length;
	while (bytesToSend > 0) {
		int bytesSent = send(socket, data + length - bytesToSend, bytesToSend, 0);
		if (bytesSent == SOCKET_ERROR) {
			return false;
		}
		bytesToSend -= bytesSent;
	}
	return (bytesToSend == 0);
}

unsigned int TcpSocket::Receive(char* data, unsigned int max_length, bool blocking) {

	if (blocking) {
		unsigned long blocking = BLOCKING;
		ioctlsocket(socket, FIONBIO, &blocking);
	}

	unsigned int bytesReceived = recv(socket, data, max_length, 0);
	if (bytesReceived == SOCKET_ERROR) {
		if (WSAGetLastError() != WSAEWOULDBLOCK) { // using non-blocking sockets and there is no data to receive at this time
		}
		bytesReceived = 0;
	}

	// Restore to non-blocking
	unsigned long nonblocking = NON_BLOCKING;
	ioctlsocket(socket, FIONBIO, &nonblocking);
	
	return bytesReceived;
}
#endif