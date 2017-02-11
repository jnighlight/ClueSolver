#pragma once
#include <string>
#include <vector>
#include "Card.h"

class Player
{
public:
	Player(std::string sName, unsigned int uiHandSize);
	virtual ~Player();

	std::string m_sName;
	unsigned int m_uiHandSize;
	std::vector<unsigned int> m_vOwnedCards;
	std::vector<unsigned int> m_vDefinitelyNotOwnedCards;
};