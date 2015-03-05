#ifdef WINDOWS_BUILD

#include "Scoreboard.h"
#include "BaseSocket.h"
#include "GameStateManager.h"
#include "NetworkCommon.h"

Scoreboard::Scoreboard(void)
{
	scoreboardConnected = false;
	scoreboardRequestSent = false;
	scoreboardRequestSucceeded = false;
	scoreboardRetrieved = false;

	scoreboardHost = SCOREBOARD_HOSTNAME;
	ConnectToScoreboard();
	if (!scoreboardConnected) return;

	RetrieveScoreboard();
	

}

bool Scoreboard::ConnectToScoreboard() {
	scoreboardDB = GameStateManager::Network()->ConnectToServer(SCOREBOARD_HOST_IP, SCOREBOARD_PORT);
	scoreboardConnected = (scoreboardDB != NULL);
	return scoreboardConnected;
}

Scoreboard::~Scoreboard(void)
{
}

bool Scoreboard::RetrieveScoreboard() {
	char buffer[1024];
	
	// Send Message Request
	if (!scoreboardRequestSent) {
		sprintf(buffer, 
	        "GET /getScores.php HTTP/1.1\r\n"
	        "Host: %s\r\n"
	        "User-Agent: Mozilla Firefox/4.0\r\n"
			"Accept: text/plain\r\n"
			"Accept-Language: en-US,en;q=0.5\r\n"
			"Accept-Encoding: identity\r\n"
			"Connection: keep-alive\r\n\r\n", 
			scoreboardHost.c_str());

		if (!scoreboardDB->Send(buffer, strlen(buffer))) {
			return false;
		}
		scoreboardRequestSent = true;
	}

	unsigned int bytesReceived = 0;
	unsigned int i = 0;
	while ((i = scoreboardDB->Receive(buffer + bytesReceived, 1024 - bytesReceived)) > 0) {
		bytesReceived += i;
	}
	if (bytesReceived > 0) {
		string httpResponse(buffer, buffer + bytesReceived);
		cout << httpResponse << endl;

		// Parse HTTP Response


	}
	return true;
}

bool Scoreboard::PostScore(string name, unsigned int score) {
	char buffer[1024];
	if (score > GetLowestScore()) {
		sprintf(buffer,
        "POST /postScore.php?n=%s&s=%d HTTP/1.1\r\n"
        "Host: %s\r\n"
        "User-Agent: Mozilla Firefox/4.0\r\n"
        "Content-Type: text/html\r\n"
        "Accept-Language: en-US,en;q=0.5\r\n"
		"Accept-Encoding: identity\r\n"
		"Connection: keep-alive\r\n\r\n", 
        name.c_str(),
		score,
		scoreboardHost.c_str());
	}
	return true;
}

bool Scoreboard::IsScoreboardLoaded() const {
	return scoreboardRetrieved;
}
	
unsigned int Scoreboard::GetLowestScore() const {
	if (scoreboard.size() > 0) {
		return atoi(scoreboard.back().second.c_str());
	}
	else { // no scores loaded
		return 0;
	}
}

string Scoreboard::GetName(unsigned int position) const {
	if (scoreboard.size() > 0) {
		return scoreboard[position].first;
	}
	else { // no scores loaded
		return "";
	}
}

string Scoreboard::GetScore(unsigned int position) const {
	if (scoreboard.size() > 0) {
		return scoreboard[position].second;
	}
	else { // no scores loaded
		return "";
	}
}

unsigned int Scoreboard::GetSize() const {
	return scoreboard.size();
}

#endif