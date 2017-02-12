#pragma once
#include <cstdint>

#include "PlayerInfo.h"

class Cli
{
public:
	Cli();
	~Cli();
	uint32_t getPlayerCount();
	void getPlayerInfo(PlayerInfo &playerInfo);
	void getUserCards(uint32_t uiUserCardCount, std::vector<uint32_t> &vCards);
private:
	uint32_t getValidUserInt();
	void getValidUserString(std::string &sInputString);
	uint32_t pickACard();
	void listsCards();
};

