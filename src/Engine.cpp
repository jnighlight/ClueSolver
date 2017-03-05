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
	for (std::string sPassedPlayers : guess.m_svPasses)
	{
		m_playerManager.addPassedGuess(sPassedPlayers, guess.m_uiPerson, guess.m_uiPlace, guess.m_uiWeapon);
	}
	if (guess.m_bSolved) {
		m_playerManager.addSolvedGuess(guess.m_sStopper, guess.m_uiPerson, guess.m_uiPlace, guess.m_uiWeapon);
	} else {
		//TODO: Have to add case for not solved. IE all answers are EITHER owned by guesser
		//	OR they are the actual answer. Surely there's some information to be gleaned there?
	}
	m_playerManager.updatePlayerState();
    //return m_playerManager.isSolved();
}

std::vector<std::string> Engine::getPlayerNames()
{
    return m_playerManager.getPlayerNames();
}

