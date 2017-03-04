#pragma once
#include <cstdint>

#include "PlayerManager.h"

class Cli
{
public:
    //TODO: Consolidate enum naming convention
	enum IsUser {
		IS_USER = 0,
		IS_OTHER_PLAYER
	};
	Cli();
	~Cli();
	uint32_t getPlayerCount();
	uint32_t getSinglePlayerInfo(PlayerManager::PlayerStartState &playerStartState, IsUser bIsUser);
	void getPlayerInfo(PlayerManager::PlayerStartStates &playerStartState);
	void getUserCards(uint32_t uiUserCardCount, std::vector<uint32_t> &vCards);
private:
	uint32_t getValidUserInt();
	void getValidUserString(std::string &sInputString, uint32_t uiMaxChars = 20);
	uint32_t pickACard();
};

