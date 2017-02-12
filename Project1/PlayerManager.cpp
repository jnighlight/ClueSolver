#include <iostream>
#include "PlayerManager.h"

PlayerManager::PlayerManager(const PlayerStartStates &playerStartStates)
{
	parsePlayerStartStates(playerStartStates);
}

void PlayerManager::parsePlayerStartStates(const PlayerStartStates &playerStartStates)
{
	m_userPlayer.setName(playerStartStates.m_userPlayer.m_sName);
	m_userPlayer.setHandSize(playerStartStates.m_userPlayer.m_uiHandSize);
	for (PlayerManager::PlayerStartState otherPlayerStartState : playerStartStates.m_vPlayerStartStates)
	{
		Player otherPlayer(otherPlayerStartState.m_sName, otherPlayerStartState.m_uiHandSize);
	}
	Player m_userPlayer;
}

void PlayerManager::setUserPlayerCards(const std::vector<uint32_t> &vPlayerCards)
{
	for (uint32_t uiCard : vPlayerCards)
	{
		m_userPlayer.addCard(uiCard);
	}
}

void PlayerManager::processGuess(const Guess &guess)
{
	if (guess.m_bSolved) {
		addGuess(guess.m_sStopper, guess.m_uiPerson, guess.m_uiPlace, guess.m_uiWeapon);
	} else { //...REALLY not sure what to do here. All of the cards are either owned by guesser or the answer...

	}
	//foreach passed players: add pass (IE, they *DONT* have those cards)
}

Player* PlayerManager::getPlayer(const std::string &sPlayerName)
{
	for (Player player : m_otherPlayers)
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
	for (Player player : m_otherPlayers)
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
	for (uint32_t uiCard : vGuessCards)
	{
		if (uiCard != 0)
		{
			return uiCard;
		}
	}
}

void PlayerManager::addGuess(const std::string &sSolver,
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
	if (!pPlayer->ownsOneOfTheseCards(vGuessCards)) {
		uint32_t uiCardsLeftInGuess = removeOwnedCardsFromGuess(vGuessCards);
		if (uiCardsLeftInGuess == 1) {
			uint32_t uiOnlyCardLeft = getOnlyCard(vGuessCards);
			pPlayer->addCard(uiOnlyCardLeft);
		} else {
			pPlayer->addGuess(vGuessCards);
		}
	}
}

PlayerManager::~PlayerManager()
{
}
