#pragma once
#include "string.h"
#include "cocos2d.h"
using namespace std;

class Player
{
private:
	std::string playerId;
	std::string playerName;
	std::string playerScore;

public:
	Player(std::string playerId, std::string playerName, std::string playerScore)
	{
		this->playerId = playerId;
		this->playerName = playerName;
		this->playerScore = playerScore;
	}

	void setPlayerId(std::string playerId)
	{
		this->playerId = playerId;
	}
	void setPlayerName(std::string playerName)
	{
		this->playerName = playerName;
	}
	void setPlayerScore(std::string playerScore)
	{
		this->playerScore =  playerScore;
	}

	std::string getPlayerId()
	{
		return playerId;
	}
	std::string getPlayerName()
	{
		return playerName;
	}
	std::string getPlayerScore()
	{
		return playerScore;
	}
};