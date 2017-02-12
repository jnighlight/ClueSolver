#include "PlayerManager.h"

PlayerManager::PlayerManager(const PlayerStartStates &playerStartStates)
{
	parsePlayerStartStates(playerStartStates);
}

void PlayerManager::parsePlayerStartStates(const PlayerStartStates &playerStartStates)
{
	m_userPlayer.setName(playerStartStates.m_userPlayer.m_sName);
	m_userPlayer.setHandSize(playerStartStates.m_userPlayer.m_uiHandSize);
	for (PlayerManager::PlayerStartState otherPlayerStartState : playerStartStates.m_vPlayerStartStates)
	{
		Player otherPlayer(otherPlayerStartState.m_sName, otherPlayerStartState.m_uiHandSize);
	}
	Player m_userPlayer;
}


PlayerManager::~PlayerManager()
{
}
