#include "Engine.h"

std::unordered_map<unsigned int, unsigned int> Engine::s_mCardOwnership;

Engine::Engine()
{
}


Engine::~Engine()
{
}

void Engine::parsePlayerInfo(const PlayerManager::PlayerStartStates &playerInfo)
{
	m_playerManager.parsePlayerStartStates(playerInfo);
}

void Engine::setUserCards(const std::vector<uint32_t> &vPlayerCards)
{
	m_playerManager.setUserPlayerCards(vPlayerCards);
}
