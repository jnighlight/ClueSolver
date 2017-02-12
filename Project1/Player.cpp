#include "Player.h"

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

bool Player::ownsCard(uint32_t uiCard)
{
	for each (uint32_t card in m_vOwnedCards)
	{
		if (card == uiCard)
		{
			return true;
		}
	}
	return false;
}

bool Player::ownsOneOfTheseCards(const std::vector<uint32_t> &vCards)
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