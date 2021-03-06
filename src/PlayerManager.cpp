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

PlayerStatusForDisplay PlayerManager::getPlayerStatusForDisplay() const
{
    PlayerStatusForDisplay playerStatusForDisplay;
    //Only going to display other players. The user knows their own cards
    for (const Player &otherPlayer : m_otherPlayers) {
        playerStatusForDisplay.addPlayer(otherPlayer, this);
    }
    addAnswerCards(playerStatusForDisplay);
    return playerStatusForDisplay;
}

void PlayerManager::addAnswerCards(PlayerStatusForDisplay &playerStatusForDisplay) const
{
    uint32_t uiSolvedPerson = isTypeSolved(Rules::ePerson);
    if (uiSolvedPerson != 0) {
        playerStatusForDisplay.setSolvedPerson(uiSolvedPerson);
    }
    uint32_t uiSolvedPlace = isTypeSolved(Rules::ePlace);
    if (uiSolvedPlace != 0) {
        playerStatusForDisplay.setSolvedPlace(uiSolvedPlace);
    }
    uint32_t uiSolvedWeapon = isTypeSolved(Rules::eWeapon);
    if (uiSolvedWeapon != 0) {
        playerStatusForDisplay.setSolvedWeapon(uiSolvedWeapon);
    }
}

uint32_t PlayerManager::isTypeSolved(Rules::eCardType eCardType) const
{
    Rules::CardTypeRange cardTypeRange = Rules::getCardTypeRange(eCardType);
    uint32_t uiCardsNotOwned = 0;
    uint32_t uiLatestNotOwned = 0;
    for (uint32_t i = cardTypeRange.m_uiMin; i <= cardTypeRange.m_uiMax; i++) {
        //If at any point we can confirm NO ONE owns a card, it has to be in the center
        if (confirmNoOneOwnsCard(i)) {
            return i;
        }
        if (!isOwned(i)) {
            ++uiCardsNotOwned;
            uiLatestNotOwned = i;
        }
    }
    if (uiCardsNotOwned == 1) {
        return uiLatestNotOwned;
    }
    return 0;
}

bool PlayerManager::confirmNoOneOwnsCard(uint32_t uiCard) const
{
    bool bNoOneDefinitelyOwnsIt = true;
    if (m_userPlayer.ownsCard(uiCard)) {
        bNoOneDefinitelyOwnsIt = false;
    }
    for (const Player &player : m_otherPlayers)
    {
        if (!player.hasBeenPassedOn(uiCard))
        {
            bNoOneDefinitelyOwnsIt = false;
        }
    }
    return bNoOneDefinitelyOwnsIt;
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
    //  a new card is claimed, we do another full run through. Each solution has the possibility
    //  of generating a different solution, so we run through until we've solved everything we can
    do
    {
        bSolutionFound = m_userPlayer.checkForSolutions(this);
        for (Player &player : m_otherPlayers)
        {
            bSolutionFound |= player.checkForSolutions(this);
        }
    } while (bSolutionFound);
}

bool PlayerManager::isSolved()
{
    uint32_t uiSolvedPerson = isTypeSolved(Rules::ePerson);
    uint32_t uiSolvedPlace = isTypeSolved(Rules::ePlace);
    uint32_t uiSolvedWeapon = isTypeSolved(Rules::eWeapon);
    return uiSolvedPerson != 0 && uiSolvedPlace != 0 && uiSolvedWeapon != 0;
}

void PlayerManager::addPassedGuess(const std::string &sPasser, const Guess &guess)
{
	Player* pPlayer = getPlayer(sPasser);
	if (!pPlayer) {
		std::cout << "PlayerManager::" << __FUNCTION__ << ", No player returned.";
		return;
	}
	pPlayer->addPassedCards(guess);
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
