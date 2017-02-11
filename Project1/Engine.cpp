#include "Engine.h"

std::unordered_map<unsigned int, unsigned int> Engine::s_mCardOwnership;

Engine::Engine()
{
}


Engine::~Engine()
{
	for (Player* pPlayer : m_lPlayers) {
		if (pPlayer) {
			delete(pPlayer);
			pPlayer = 0;
		}
	}
}

void Engine::parsePlayerInfo(const PlayerInfo &playerInfo)
{
	uint32_t uiPlayerId = 1; //We want to keep 0 to signify NO_PLAYER
	for (auto playerStartState : playerInfo.m_vPlayerStartStates) {
		Player* pPlayer = new Player(playerStartState.m_sName, playerStartState.m_uiHandSize);
		m_lPlayers.push_back(pPlayer);
	}
}

/*
void Engine::createCards()
{
	for (auto &sPerson : s_lPeople) {
	}
}
*/

