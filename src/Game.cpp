#include <ncurses.h>

#include "Game.h"
#include "Cli.h"
#include "Engine.h"
#include "Guess.h"
#include "PlayerManager.h"


Game::Game()
{
    initscr();
}

Game::~Game()
{
    endwin();
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
	m_engine.setUserCards(vPlayerCards);
	//We are now in a state that requires players to start making guesses.
	//This will be our display break, where we will supply our display information
	//	to the CLI, and let it display until it recieves a guess, which we will send
	//	to the engine for processing
    guessLoop();
}

void Game::guessLoop()
{
	Guess guess;
    m_cli.getGuess(guess, m_engine.getPlayerNames());
    /*
	Guess guess;
	GameState gameState;
	while (!bSolved) {
		m_cli.getGuess(guess, m_engine.getPlayerNames());
		bSolved = m_engine.processGuess(guess);
		m_cli.display(m_engine.getGameState());
		guess.clear();
	}
    */
}
