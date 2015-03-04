#include "NetworkTest.h"
#include "NetworkCommon.h"
#include "TcpSocket.h"
#include "ServerSocket.h"


NetworkTest::NetworkTest(void)
{
	connected = false;
}


NetworkTest::~NetworkTest(void)
{
}

void NetworkTest::LoadContent() {
	server = GameStateManager::Network()->StartServer(PORT, 1);
}

void NetworkTest::Update() {
	if (!connected) {
		unsigned int newConnections = server->GetNewConnections();
		if (newConnections > 0) {
			connected = true;
			connection = server->GetConnection(0);
		}
		else return;
	}

	char buffer[50];
	unsigned int bytes = connection->Receive(buffer, 50);
	if (bytes > 0) {
		cout << "data received" << endl;
		cout << buffer << endl;
		
		string text = "Message Received\n";
		connection->Send(text.c_str(), text.length());
	}
}


void NetworkTest::UnloadContent() {
	delete server;
}