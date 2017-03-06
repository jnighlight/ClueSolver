#include <iostream>
#include <ncurses.h>
#include "PlayerManager.h"

PlayerManager::PlayerManager(const PlayerStartStates &playerStartStates)
{
	parsePlayerStartStates(playerStartStates);
}

PlayerManager::~PlayerManager()
{
}
PlayerStatusForDisplay PlayerManager::getPlayerStatusForDisplay()
{
    PlayerStatusForDisplay playerStatusForDisplay;
    playerStatusForDisplay.addPlayer(m_userPlayer);
    for (Player &otherPlayer : m_otherPlayers) {
        playerStatusForDisplay.addPlayer(otherPlayer);
    }
    return playerStatusForDisplay;
}

void PlayerManager::parsePlayerStartStates(const PlayerStartStates &playerStartStates)
{
	m_userPlayer.setName(playerStartStates.m_userPlayer.m_sName);
	m_userPlayer.setHandSize(playerStartStates.m_userPlayer.m_uiHandSize);
	for (const PlayerManager::PlayerStartState &otherPlayerStartState : playerStartStates.m_vPlayerStartStates)
	{
		Player otherPlayer(otherPlayerStartState.m_sName, otherPlayerStartState.m_uiHandSize);
        m_otherPlayers.push_back(otherPlayer);
	}
	Player m_userPlayer;
}

void PlayerManager::setUserPlayerCards(const std::vector<uint32_t> &vPlayerCards)
{
	for (const uint32_t uiCard : vPlayerCards)
	{
		m_userPlayer.addCard(uiCard);
	}
}

Player* PlayerManager::getPlayer(const std::string &sPlayerName)
{
	if (m_userPlayer.m_sName.compare(sPlayerName))
	{
		return &m_userPlayer;
	}
	for (Player &player : m_otherPlayers)
	{
		if (player.m_sName.compare(sPlayerName) == 0) {
			return &player;
		}
	}
	std::cout << "PlayerManager::" << __FUNCTION__ << ", Requested player not found: ";
	std::cout << sPlayerName;
	return 0;
}

bool PlayerManager::isOwned(uint32_t uiCard)
{
	if (m_userPlayer.ownsCard(uiCard))
	{
		return true;
	}
	for (const Player &player : m_otherPlayers)
	{
		if (player.ownsCard(uiCard))
		{
			return true;
		}
	}
	return false;
}

//Returns the number of cards in the guess that AREN'T owned
uint32_t PlayerManager::removeOwnedCardsFromGuess(std::vector<uint32_t> &vGuessCards)
{
	uint32_t uiOwned = 0;
	for (uint32_t i = 0; i < vGuessCards.size(); i++)
	{
		if (isOwned(vGuessCards[i]))
		{
			vGuessCards[i] = 0;
			++uiOwned;
		}
	}
	return vGuessCards.size() - uiOwned;
}

//Assumes that only one card in the incoming vector is NOT 0
uint32_t PlayerManager::getOnlyCard(const std::vector<uint32_t> &vGuessCards)
{
	for (const uint32_t uiCard : vGuessCards)
	{
		if (uiCard != 0)
		{
			return uiCard;
		}
	}
	std::cout << "PlayerManager::" << __FUNCTION__ << ", No cards found!";
	return 0;
}

void PlayerManager::addSolvedGuess(const std::string &sSolver,
						     uint32_t uiPerson,
							 uint32_t uiPlace,
							 uint32_t uiWeapon)
{
	Player* pPlayer = getPlayer(sSolver);
	if (!pPlayer) {
		std::cout << "PlayerManager::" << __FUNCTION__ << ", No player returned.";
		return;
	}
	std::vector<uint32_t> vGuessCards = { uiPerson, uiPlace, uiWeapon };

	//If the Solver already owns one of these cards, we CAN'T infer more information from
	//	the solution of this guess (But we can infer info from who it passed)
	if (!pPlayer->ownsOneOfTheseCards(vGuessCards) && removeOwnedCardsFromGuess(vGuessCards) != 0) {
        pPlayer->addGuess(vGuessCards);
        /*
         * JUST do this from the engine after every add. We can optimize later
        bool bSolutionFound = pPlayer->checkForSolutions(this);
        if (bSolutionFound)
        {
            updatePlayerState();
        }
        */
    }
}

void PlayerManager::updatePlayerState()
{
    bool bSolutionFound = false;
    //We're going to run through each user's stored guesses to check for new info. If, at any point,
    //  a new card is claimed, we do another full run through. This is to make sure that this 
    //  claiming of the card does not cause another card to be able to be claimed
    while (bSolutionFound)
    {
        bSolutionFound = false;
        bSolutionFound = m_userPlayer.checkForSolutions(this);
        for (Player &player : m_otherPlayers)
        {
            bSolutionFound |= player.checkForSolutions(this);
        }
    }
}

bool PlayerManager::isSolved()
{
    bool bIsSolved = true;
    for (const Player& otherPlayer : m_otherPlayers) {
        if (!otherPlayer.isSolved()) {
            bIsSolved = false;
        }
    }
    return bIsSolved;
}

void PlayerManager::addPassedGuess(const std::string &sPasser,
						     uint32_t uiPerson,
							 uint32_t uiPlace,
							 uint32_t uiWeapon)
{
	Player* pPlayer = getPlayer(sPasser);
	if (!pPlayer) {
		std::cout << "PlayerManager::" << __FUNCTION__ << ", No player returned.";
		return;
	}
	std::vector<uint32_t> vGuessCards = { uiPerson, uiPlace, uiWeapon };
	pPlayer->addNotOwnedCards(vGuessCards);
}

std::vector<std::string> PlayerManager::getPlayerNames()
{
    std::vector<std::string> vPlayerNames;
    vPlayerNames.push_back(m_userPlayer.m_sName);
    printw("Adding name");
    printw(m_userPlayer.m_sName.c_str());
    printw("\n");
    printw("Number of other players: ");
    printw("%d", m_otherPlayers.size());
    printw("\n");
    for (const Player& player : m_otherPlayers) {
        vPlayerNames.push_back(player.m_sName);
        printw("Adding name");
        printw(player.m_sName.c_str());
        printw("\n");
    }
    return vPlayerNames;
}
