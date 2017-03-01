#include "Player.h"
#include "PlayerManager.h"

Player::Player(std::string sName, unsigned int uiHandSize)
	:m_sName(sName)
	,m_uiHandSize(uiHandSize)
{
}

Player::~Player()
{
}

void Player::addGuess(const std::vector<uint32_t> &vCards)
{
	if (vCards.size() == 3)
	{
		Player::AnsweredGuess answeredGuess(vCards[0], vCards[1], vCards[2]);
		m_lAnsweredGuesses.push_back(answeredGuess);
	} else {
		std::cout << "Player::" << __FUNCTION__ << ", Incoming guess of incorrect size: ";
		std::cout << vCards.size();
	}
}

void Player::addCard(uint32_t uiCardToAdd)
{
	m_vOwnedCards.insert(uiCardToAdd);
}

bool Player::ownsCard(uint32_t uiCard) const
{
	return m_vOwnedCards.find(uiCard) != m_vOwnedCards.end();
}

bool Player::ownsOneOfTheseCards(const std::vector<uint32_t> &vCards) const
{
	for (uint32_t uiCard : vCards)
	{
		if (m_vOwnedCards.find(uiCard) != m_vOwnedCards.end())
		{
			return true;
		}
	}
	return false;
}

void Player::addNotOwnedCards(const std::vector<uint32_t> &vNotOwnedCards)
{
	for (uint32_t uiNotOwnedCard : vNotOwnedCards)
	{
		m_vDefinitelyNotOwnedCards.insert(uiNotOwnedCard);
	}
}

//Returns a boolean for whether or not the given guess is finished being processed
bool Player::processStoredGuess(AnsweredGuess &answeredGuess, PlayerManager* pPlayerManager)
{
	//If I know I own any of these cards, the guess can provide me no more information
	if (ownsCard(answeredGuess.m_uiPlace) ||
		ownsCard(answeredGuess.m_uiPerson) ||
		ownsCard(answeredGuess.m_uiWeapon))
	{
		return true;
	}
	//If someone ELSE owns any of these cards, zero them out
	if (pPlayerManager->isOwned(answeredGuess.m_uiPlace))
	{
		answeredGuess.m_uiPlace = 0;
	}
	if (pPlayerManager->isOwned(answeredGuess.m_uiPerson))
	{
		answeredGuess.m_uiPerson = 0;
	}
	if (pPlayerManager->isOwned(answeredGuess.m_uiWeapon))
	{
		answeredGuess.m_uiWeapon = 0;
	}
	//Finally, if there's only one card left in the guess, ITS MINE!!! YES!
	if (answeredGuess.isSolved())
	{
		uint32_t uiSolvedCard = answeredGuess.getSolved();
		addCard(uiSolvedCard);
		pPlayerManager->cardClaimedBlast(uiSolvedCard);
		return true;
	}
	//(If there are NO cards left, I f***ed up bad somewhere. Why do I have this guess
	//	if I don't have the card to solve it?)
	if (answeredGuess.isEmpty())
	{
		std::cout << "Player::" << __FUNCTION__ << ", Player has empty guess! How'd he get it?";
		return true;
	}
	return false;
}

void Player::checkForSolutions(PlayerManager* pPlayerManager)
{
	/*
	TODO: Start back here.
	std::list<AnsweredGuess>::iterator i = m_lAnsweredGuesses.begin();
	for (while i != m_lAnsweredGuesses.end())
	{
		if (processStoredGuess(guess, pPlayerManager))
		{
			m_lAnsweredGuesses.
		}
	}
	*/
}

//---------------------ANSWERED GUESS METHOD-------------------------
bool Player::AnsweredGuess::isSolved()
{
	uint32_t uiStillPresent = 0;
	if (m_uiPlace != 0)
	{
		++uiStillPresent;
	}
	if (m_uiPerson != 0)
	{
		++uiStillPresent;
	}
	if (m_uiWeapon != 0)
	{
		++uiStillPresent;
	}
	return uiStillPresent == 1;
}

uint32_t Player::AnsweredGuess::getSolved()
{
    //There's a little bit of code duplication here. We'll likely call this before getting solved anyways.
    //But having this check helps me sleep at night
    if (!isSolved())
    {
        return 0;
    }
	if (m_uiPlace != 0)
	{
		return m_uiPlace;
	}
	if (m_uiPerson != 0)
	{
		return m_uiPerson;
	}
	if (m_uiWeapon != 0)
	{
		return m_uiWeapon;
	}
	return 0;
}
