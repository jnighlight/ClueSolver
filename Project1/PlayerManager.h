#pragma once
#include <string>
#include <vector>
#include "Player.h"
class PlayerManager
{
public:
	struct PlayerInfo {
		PlayerInfo(std::string sName, uint32_t uiCardCount)
			:m_sName(sName)
			,m_uiCardCount(uiCardCount)
		{}

		std::string m_sName;
		uint32_t m_uiCardCount;
	};

	PlayerManager();
	PlayerManager(unsigned int uiPlayerCount);
	virtual ~PlayerManager();
	Player* getUserPlayer() { return &m_userPlayer; };
	std::vector<Player>* getOtherPlayers() { return &m_vOtherPlayers; };
private:
	Player m_userPlayer;
	std::vector<Player> m_vOtherPlayers;
};

