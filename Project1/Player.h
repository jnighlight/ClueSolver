#pragma once
#include <string>
#include <set>
#include <vector>
#include <list>
#include <iostream>
#include "Card.h"
#include "Guess.h"

class Player
{
public:
	struct AnsweredGuess {
		AnsweredGuess() {};
		AnsweredGuess(uint32_t uiPlace, uint32_t uiPerson, uint32_t uiWeapon)
			:m_uiPlace(uiPlace)
			,m_uiPerson(uiPerson)
			,m_uiWeapon(uiWeapon)
		{};

		~AnsweredGuess() {};

		uint32_t m_uiPlace;
		uint32_t m_uiPerson;
		uint32_t m_uiWeapon;
	};

	Player() {};
	Player(std::string sName, unsigned int uiHandSize);
	virtual ~Player();

	std::string m_sName;
	unsigned int m_uiHandSize;
	std::set<unsigned int> m_vOwnedCards;
	std::set<unsigned int> m_vDefinitelyNotOwnedCards;
	std::list<AnsweredGuess> m_lAnsweredGuesses;

	void addCard(uint32_t uiCardToAdd);
	void addGuess(const std::vector<uint32_t> &vCards);

	bool ownsCard(uint32_t uiCard);
	bool ownsOneOfTheseCards(const std::vector<uint32_t> &vCards);
	void setName(std::string sName) { m_sName = sName; };
	void setHandSize(uint32_t uiHandSize) { m_uiHandSize = uiHandSize; };
};