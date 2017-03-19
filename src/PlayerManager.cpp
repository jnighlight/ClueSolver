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
	if (m_userPlayer.m_sName.compare(sPlayerName) == 0)
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

bool PlayerManager::isOwned(uint32_t uiCard) const
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
//Modifies incoming guess to remove cards
uint32_t PlayerManager::removeOwnedCardsFromGuess(Player::AnsweredGuess &answeredGuess) const
{
	uint32_t uiOwned = 0;
    if (isOwned(answeredGuess.m_uiPerson)) {
        answeredGuess.m_uiPerson = 0;
        ++uiOwned;
    }
    if (isOwned(answeredGuess.m_uiPlace)) {
        answeredGuess.m_uiPlace = 0;
        ++uiOwned;
    }
    if (isOwned(answeredGuess.m_uiWeapon)) {
        answeredGuess.m_uiWeapon = 0;
        ++uiOwned;
    }
	return 3 - uiOwned;
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

//Makes a local copy of guess for modification and storage
void PlayerManager::addSolvedGuess(const Guess &guess)
{
	Player* pPlayer = getPlayer(guess.m_sStopper);
	if (!pPlayer) {
		std::cout << "PlayerManager::" << __FUNCTION__ << ", No player returned.";
		return;
	}
    //If the user guessed, we automagically get the card. Awesome!!!
    if (guess.m_bUserGuess) {
        pPlayer->addCard(guess.m_uiUserAnswerRecieved);
    } else {
        //Mostly stripping out the Player names and making a local editable copy
        Player::AnsweredGuess answeredGuess(guess);
        //If the Solver already owns one of these cards, we CAN'T infer more information from
        //	the solution of this guess (But we can infer info from who it passed)
        if (!pPlayer->ownsOneOfTheseCards(answeredGuess)) {
            uint32_t uiCardsLeft = removeOwnedCardsFromGuess(answeredGuess);
            if (uiCardsLeft != 0) {
                pPlayer->addGuess(answeredGuess);
            }
        }
    }
}

void PlayerManager::updatePlayerState()
{
    bool bSolutionFound = false;
    //We're going to run through each user's stored guesses to check for new info. If, at any point,
    //  a new card is claimed, we do another full run through. This is to make sure that this 
    //  claiming of the card does not cause another card to be able to be claimed
    do
    {
        bSolutionFound = false;
        bSolutionFound = m_userPlayer.checkForSolutions(this);
        for (Player &player : m_otherPlayers)
        {
            bSolutionFound |= player.checkForSolutions(this);
        }
    } while (bSolutionFound);
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

void PlayerManager::addPassedGuess(const std::string &sPasser, const Guess &guess)
{
	Player* pPlayer = getPlayer(sPasser);
	if (!pPlayer) {
		std::cout << "PlayerManager::" << __FUNCTION__ << ", No player returned.";
		return;
	}
	pPlayer->addNotOwnedCards(guess);
}

std::vector<std::string> PlayerManager::getPlayerNames()
{
    std::vector<std::string> vPlayerNames;
    vPlayerNames.push_back(m_userPlayer.m_sName);
    for (const Player& player : m_otherPlayers) {
        vPlayerNames.push_back(player.m_sName);
    }
    return vPlayerNames;
}
