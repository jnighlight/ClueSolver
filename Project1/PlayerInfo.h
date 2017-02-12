#pragma once
#include <string>
#include <vector>
class PlayerInfo
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

	PlayerInfo(unsigned int uiPlayerCount);
	virtual ~PlayerInfo();
	std::vector<PlayerStartState> m_vPlayerStartStates;
};

