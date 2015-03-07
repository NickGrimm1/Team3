#pragma once
#ifdef WINDOWS_BUILD

#include <string>
#include <vector>

using namespace std;

class BaseSocket;

class Scoreboard
{
public:
	Scoreboard(void);
	~Scoreboard(void);

	// Web requests
	bool RetrieveScoreboard();
	bool PostScore(string name, unsigned int score);

	// Local requests
	bool IsScoreboardLoaded() const;
	unsigned int GetLowestScore() const;
	string GetName(unsigned int position) const;
	string GetScore(unsigned int position) const;
	unsigned int GetSize() const;

protected:
	bool ConnectToScoreboard();
	vector<pair<string, string>> scoreboard; //<name, score>
	bool scoreboardLoaded;

	string scoreboardHost;
	BaseSocket* scoreboardDB;
};

#endif