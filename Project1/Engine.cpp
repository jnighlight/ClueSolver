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
