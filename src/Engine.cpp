#include "Engine.h"

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
    //If it's not stopped by anyone, the only useful information is who it passed
    //  (IE if you own 2 of the cards in your guess, and no one stops, the engine will
    //   register that *NO ONE* owns that card, and will consider it the answer)
	if (!guess.m_sStopper.empty()) {
		m_playerManager.addSolvedGuess(guess);
	}
	m_playerManager.updatePlayerState();
    return m_playerManager.isSolved();
}

std::vector<std::string> Engine::getPlayerNames()
{
    return m_playerManager.getPlayerNames();
}

