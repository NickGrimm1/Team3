#ifdef WINDOWS_BUILD

#include "Scoreboard.h"
#include "BaseSocket.h"
#include "GameStateManager.h"
#include "NetworkCommon.h"

#include <sstream>

Scoreboard::Scoreboard(void)
{
	scoreboardLoaded = false;
	scoreboardHost = SCOREBOARD_HOSTNAME;
}

bool Scoreboard::ConnectToScoreboard() {
	scoreboardDB = GameStateManager::Network()->ConnectToServer(SCOREBOARD_HOST_IP, SCOREBOARD_PORT);
	return (scoreboardDB != NULL);
}

Scoreboard::~Scoreboard(void)
{
	scoreboard.clear();
}

bool Scoreboard::RetrieveScoreboard() {
	scoreboard.clear();
	scoreboardLoaded = false;

	if (!ConnectToScoreboard()) return false;

	char buffer[1024];
	// Send Message Request
	sprintf_s(buffer, 1024, 
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
	
	unsigned int bytesReceived = 0;
	unsigned int i = 0;
	while ((i = scoreboardDB->Receive(buffer + bytesReceived, 1024 - bytesReceived, true)) > 0) {
		bytesReceived += i;
	}
	if (bytesReceived > 0) {
		string httpResponse(buffer, buffer + bytesReceived);
		//cout << httpResponse << endl;

		if (httpResponse.find(HTTP_OK) != string::npos) {
			stringstream httpStream(httpResponse);
		
			// Parse HTTP Response
			unsigned int scoreboardLength = 0;
			bool scoreboardStarted = false;
			while (!httpStream.eof()) {
				getline(httpStream, httpResponse);
				unsigned int pos = 0;

				if (httpResponse == "") continue;

				if (scoreboardStarted) {
					string name;
					string score;
					unsigned int delim = httpResponse.find(" ");
					name = httpResponse.substr(0, delim);
					while (name.length() < 3) name += " ";
					score = httpResponse.substr(delim + 1, string::npos);
					while (score.length() < 3) score = '0' + score;
					scoreboard.push_back(pair<string, string>(name, score));
					scoreboardLength -= httpResponse.length() + 1; // + 1 for EOL
				}
				else if ((pos = httpResponse.find(HTTP_CONTENT_LENGTH)) != string::npos) {
					scoreboardLength = atoi(httpResponse.substr(pos + strlen(HTTP_CONTENT_LENGTH), string::npos).c_str());
				}
				else if (httpResponse == "\r") { // End of HTTP Response
					scoreboardStarted = true;
				}
				// Don't care about the rest of the response
			}

			scoreboardLoaded = (scoreboardLength == 0);
		}
	}
	delete scoreboardDB;
	return scoreboardLoaded;
}

bool Scoreboard::PostScore(string name, unsigned int score) {
	if (!ConnectToScoreboard()) return false;

	char buffer[1024];
	sprintf_s(buffer, 1024,
    "GET /postScore.php?n=%s&s=%d HTTP/1.1\r\n"
    "Host: %s\r\n"
    "User-Agent: Mozilla Firefox/4.0\r\n"
    "Content-Type: text/html\r\n"
    "Accept-Language: en-US,en;q=0.5\r\n"
	"Accept-Encoding: identity\r\n"
	"Connection: keep-alive\r\n\r\n", 
    name.c_str(),
	score,
	scoreboardHost.c_str());
	
	cout << string(buffer, buffer + strlen(buffer)) << endl;

	if (!scoreboardDB->Send(buffer, strlen(buffer))) {
		cout << "Could not post score" << endl;	
		return false;
	}

	// Check score was posted correctly
	unsigned int bytesReceived = 0;
	unsigned int i = 0;
	while ((i = scoreboardDB->Receive(buffer + bytesReceived, 1024 - bytesReceived, true)) > 0) {
		bytesReceived += i;
	}
	cout << string(buffer, buffer + strlen(buffer)) << endl;
	delete scoreboardDB;
	return (bytesReceived > 0 && string(buffer, buffer + bytesReceived).find(HTTP_OK));
}

bool Scoreboard::IsScoreboardLoaded() const {
	return scoreboardLoaded;
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
	if (scoreboard.size() > position) {
		return scoreboard[position].first;
	}
	else { // no scores loaded
		return "___";
	}
}

string Scoreboard::GetScore(unsigned int position) const {
	if (scoreboard.size() > position) {
		return scoreboard[position].second;
	}
	else { // no scores loaded
		return "000";
	}
}

unsigned int Scoreboard::GetSize() const {
	return scoreboard.size();
}

#endif