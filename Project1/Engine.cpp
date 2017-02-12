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

void Engine::processGuess(const Guess &guess)
{
	if (guess.m_bSolved) {
		m_playerManager.addGuess(guess.m_sStopper, guess.m_uiPerson, guess.m_uiPlace, guess.m_uiWeapon);
	}
}
