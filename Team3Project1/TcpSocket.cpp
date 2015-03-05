#if WINDOWS_BUILD
#include "TcpSocket.h"

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
			cout << "TcpSocket::Send() - Failed to send data, " << data << endl;
			cout << "Failed with error: " << WSAGetLastError() << " - " << gai_strerror(WSAGetLastError()) << endl;
			return false;
		}
		bytesToSend -= bytesSent;
	}
	return (bytesToSend == length);
}

unsigned int TcpSocket::Receive(char* data, unsigned int max_length) {
	int bytesReceived = recv(socket, data, max_length, 0);
	if (bytesReceived == SOCKET_ERROR) {
		if (WSAGetLastError() == WSAEWOULDBLOCK) {
			return 0; // using non-blocking sockets and there is no data to receive at this time
		}
		else {
			cout << "TcpSocket::Send() - Failed to recieve data" << endl;
			cout << "Failed with error: " << WSAGetLastError() << " - " << gai_strerror(WSAGetLastError()) << endl;
		}
	}
	return bytesReceived;
}
#endif