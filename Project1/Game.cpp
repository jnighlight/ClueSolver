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
	std::vector<uint32_t> vPlayerCards;
	uint32_t uiPlayerCount = m_cli.getPlayerCount();
	//Oooh, a playerInfo object is just to be passed to cli for populating.
	//But, lets one up that. Lets just have the playerInfo object store all the players, and just populate them at the start
	m_cli.getPlayerInfo(pPlayerManager);
	//Currently, PlayerInfo is the PlayerManager. m_vPlayerStartStates[0] will always be the controlling player (Should just make an accessor)
	//Lets fix this too: let's have a PlayerManager, with a controller player and a list of others. that way, there's less confusion, and it's better documented in the code
	uint32_t uiUserCardCount = pPlayerManager.m_vPlayerStartStates[0].m_uiHandSize;
	m_engine.parsePlayerInfo(pPlayerManager);
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
