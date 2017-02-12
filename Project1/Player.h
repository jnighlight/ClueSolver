#pragma once
#include <string>
#include <vector>
#include "Card.h"

class Player
{
public:
	Player() {};
	Player(std::string sName, unsigned int uiHandSize);
	virtual ~Player();

	std::string m_sName;
	unsigned int m_uiHandSize;
	std::vector<unsigned int> m_vOwnedCards;
	std::vector<unsigned int> m_vDefinitelyNotOwnedCards;
	void addCard(uint32_t uiCardToAdd);
	bool ownsCard(uint32_t uiCard);
	void setName(std::string sName) { m_sName = sName; };
	void setHandSize(uint32_t uiHandSize) { m_uiHandSize = uiHandSize; };
};