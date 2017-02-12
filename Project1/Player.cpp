#include "Player.h"



Player::Player(std::string sName, unsigned int uiHandSize)
	:m_sName(sName)
	,m_uiHandSize(uiHandSize)
{
}

Player::~Player()
{
}

void Player::addCard(uint32_t uiCardToAdd)
{
	m_vOwnedCards.push_back(uiCardToAdd);
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