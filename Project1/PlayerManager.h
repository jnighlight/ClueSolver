#pragma once
#include <string>
#include <vector>
#include "Player.h"

class PlayerManager
{
public:
	struct PlayerStartState {

		PlayerStartState::PlayerStartState()
		{};

		PlayerStartState::PlayerStartState(unsigned int uiHandSize, std::string sName)
			:m_uiHandSize(uiHandSize)
			,m_sName(sName)
		{};

		void setHandSize(unsigned int uiHandSize) { m_uiHandSize = uiHandSize; };
		void setName(std::string sName) { m_sName = sName; };

		unsigned int m_uiHandSize;
		std::string m_sName;
	};

	struct PlayerStartStates{
		PlayerStartStates(uint32_t uiNumberOfPlayers)
			:m_vPlayerStartStates(uiNumberOfPlayers - 1)
		{};

		PlayerStartState m_userPlayer;
		uint32_t getUserPlayerHandSize() { return m_userPlayer.m_uiHandSize; };
		std::vector<PlayerStartState> m_vPlayerStartStates;
	};

	PlayerManager() {};
	PlayerManager(const PlayerStartStates &playerStartStates);
	virtual ~PlayerManager();

	void parsePlayerStartStates(const PlayerStartStates &playerStartStates);
	void setUserPlayerCards(const std::vector<uint32_t> &vPlayerCards);

	Player m_userPlayer;
	std::vector<Player> m_otherPlayers;
};

