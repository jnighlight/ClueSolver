#include "Engine.h"

std::unordered_map<unsigned int, unsigned int> Engine::s_mCardOwnership;

Engine::Engine()
{
}


Engine::~Engine()
{
}

PlayerStatusForDisplay Engine::getPlayerStatus()
{
    return m_playerManager.getPlayerStatusForDisplay();
}

void Engine::parsePlayerInfo(const PlayerManager::PlayerStartStates &playerInfo)
{
	m_playerManager.parsePlayerStartStates(playerInfo);
}

void Engine::setUserCards(const std::vector<uint32_t> &vPlayerCards)
{
	m_playerManager.setUserPlayerCards(vPlayerCards);
}

bool Engine::processGuess(const Guess &guess)
{
	for (std::string sPassedPlayers : guess.m_svPasses)
	{
		m_playerManager.addPassedGuess(sPassedPlayers, guess);
	}
	if (!guess.m_sStopper.empty()) {
		m_playerManager.addSolvedGuess(guess);
	} else {
		//TODO: Have to add case for not solved. IE all answers are EITHER owned by guesser
		//	OR they are the actual answer. Surely there's some information to be gleaned there?
	}
	m_playerManager.updatePlayerState();
    return m_playerManager.isSolved();
}

std::vector<std::string> Engine::getPlayerNames()
{
    return m_playerManager.getPlayerNames();
}

