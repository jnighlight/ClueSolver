#include "Game.h"
#include "Cli.h"
#include "Engine.h"
#include "Guess.h"
#include "PlayerManager.h"


Game::Game()
{
}

void Game::start()
{
	bool bSolved = false;
	std::vector<uint32_t> vPlayerCards;
	uint32_t uiPlayerCount = m_cli.getPlayerCount();
	//We make this to ensure capsulation. Just an object to pass info from CLI to Engine
	PlayerManager::PlayerStartStates playerStartStates(uiPlayerCount);
	m_cli.getPlayerInfo(playerStartStates);
	uint32_t uiUserCardCount = playerStartStates.getUserPlayerHandSize();
	m_engine.parsePlayerInfo(playerStartStates);
	m_cli.getUserCards(uiUserCardCount, vPlayerCards);
	/*
	playerInfo.m_vPlayerStartStates[0]
	m_engine.setUserCards();
	Guess guess;
	GameState gameState;
	while (!bSolved) {
		m_cli.getGuess(guess);
		bSolved = m_engine.parseGuess(guess, gameState);
		m_cli.display(gameState);
		guess.clear();
	}
	*/
}
Game::~Game()
{
}
