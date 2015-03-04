#ifdef WINDOWS_BUILD
#include "NetworkTest.h"
#include "NetworkCommon.h"
#include "TcpSocket.h"
#include "ServerSocket.h"

//#define TEST_SERVER
NetworkTest::NetworkTest(void)
{
	connected = false;
}


NetworkTest::~NetworkTest(void)
{
}

void NetworkTest::LoadContent() {
	server = GameStateManager::Network()->StartServer(PORT1, 1);
}

void NetworkTest::Update() {
	if (!connected) {
#ifdef TEST_SERVER
		unsigned int newConnections = server->GetNewConnections();
		if (newConnections > 0) {
			connected = true;
			connection = server->GetConnection(0);
		}
		else return;
	
#else
		connection = GameStateManager::Network()->ConnectToServer(LOCALHOST, PORT2);
		if (connection == NULL) {
			cout << "no connection" << endl;
			return;
		}
		connected = true;
#endif
	}	

#ifdef TEST_SERVER
	char buffer[50];
	unsigned int bytes = connection->Receive(buffer, 50);
	if (bytes > 0) {
		cout << "data received" << endl;
		cout << buffer << endl;
		
		string text = "Message Received\n";
		connection->Send(text.c_str(), text.length());
	}
#else
	string text = "Message Received\n";
	connection->Send(text.c_str(), text.length());
#endif
}


void NetworkTest::UnloadContent() {
	delete server;
}

#endif