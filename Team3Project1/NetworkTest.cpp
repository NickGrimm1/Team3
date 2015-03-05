#ifdef WINDOWS_BUILD
#include "NetworkTest.h"
#include "NetworkCommon.h"
#include "TcpSocket.h"
#include "ServerSocket.h"
#include "Scoreboard.h"

//#define TEST_SERVER
NetworkTest::NetworkTest(void)
{
	connected = false;
	Scoreboard* s = new Scoreboard();

	s->PostScore("NEW", 100);
	s->RetrieveScoreboard();
	delete s;
}


NetworkTest::~NetworkTest(void)
{
}

void NetworkTest::LoadContent() {
#ifdef TEST_SERVER
	server = GameStateManager::Network()->StartServer(PORT1, 1);
#endif
}

void NetworkTest::Update() {
#ifdef TEST_SERVER
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
#endif
}


void NetworkTest::UnloadContent() {
#if TEST_SERVER
	delete server;
#endif
}

#endif