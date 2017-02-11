#include "Game.h"
#include "Cli.h"
#include "Engine.h"
#include "Guess.h"
#include "PlayerInfo.h"


Game::Game()
{
}

void Game::start()
{
	bool bSolved = false;
	uint32_t uiPlayerCount = m_cli.getPlayerCount();
	PlayerInfo playerInfo(uiPlayerCount);
	m_cli.getPlayerInfo(playerInfo);
	uint32_t uiUserCardCount = playerInfo.m_vPlayerStartStates[0].m_uiHandSize;
	m_engine.parsePlayerInfo(playerInfo);
	m_cli.getUserCards(uiUserCardCount);
	/*
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
