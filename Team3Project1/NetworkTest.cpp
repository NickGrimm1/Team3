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
#ifdef TEST_SERVER
	server = GameStateManager::Network()->StartServer(PORT1, 1);
#endif
}

void NetworkTest::Update() {
	if (!connected) {
		connection = GameStateManager::Network()->ConnectToServer(SCOREBOARD_HOST_IP, SCOREBOARD_PORT);
		//connection = GameStateManager::Network()->ConnectToServer(LOCALHOST, PORT1);
		if (!connection) {
			return;
		}
		cout << "connection established" << endl;
		
		char header[1024];
		string host(SCOREBOARD_HOSTNAME);
		sprintf(header,
        "GET /getScores.php HTTP/1.1\r\n"
        "Host: %s\r\n"
        "User-Agent: Mozilla Firefox/4.0\r\n"
		"Accept: text/plain\r\n"
		"Accept-Language: en-US,en;q=0.5\r\n"
		"Accept-Encoding: identity\r\n"
		"Connection: keep-alive\r\n\r\n\0", 
		host.c_str());

		connection->Send(header, strlen(header));

        char buffer[1024];
		int i = 0;
		if (connection->Receive(buffer + i, 1024 - i) > 0) {
	 		cout << string(buffer) << endl;
		}
		connected = true;
	}
		
/*
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
	*/
}


void NetworkTest::UnloadContent() {
	delete server;
}

#endif