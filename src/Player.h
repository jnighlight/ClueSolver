#pragma once
#include <string>
#include <set>
#include <vector>
#include <list>
#include <iostream>
#include "Card.h"
#include "Guess.h"

class PlayerManager;

class Player
{
public:
	struct AnsweredGuess {
		AnsweredGuess()
			:m_uiPlace(0)
			,m_uiPerson(0)
			,m_uiWeapon(0)
        {};
		AnsweredGuess(uint32_t uiPlace, uint32_t uiPerson, uint32_t uiWeapon)
			:m_uiPlace(uiPlace)
			,m_uiPerson(uiPerson)
			,m_uiWeapon(uiWeapon)
		{};

		~AnsweredGuess() {};

		uint32_t m_uiPlace;
		uint32_t m_uiPerson;
		uint32_t m_uiWeapon;

		bool isSolved();
		uint32_t getSolved();
		bool isEmpty() { return (m_uiPerson == 0 && m_uiPlace == 0 && m_uiWeapon == 0); }
	};

	Player() {};
	Player(std::string sName, unsigned int uiHandSize);
	virtual ~Player();

	std::string m_sName;
	unsigned int m_uiHandSize;
	std::set<uint32_t> m_vOwnedCards;
	std::set<uint32_t> m_vDefinitelyNotOwnedCards;
	std::list<AnsweredGuess> m_lAnsweredGuesses;

    bool isSolved() const;
	void addCard(uint32_t uiCardToAdd);
	bool addGuess(const std::vector<uint32_t> &vCards);
	void addNotOwnedCards(const std::vector<uint32_t> &vNotOwnedCards);
    bool isDefinitelyNotOwned(uint32_t uiCard);
	bool checkForSolutions(PlayerManager* pPlayerManager);

    enum guessState {
        eNotSolved = 0,
        eSolved,
        eTrash
    };

    Player::guessState processStoredGuess(AnsweredGuess &answeredGuess, PlayerManager* pPlayerManager);

	bool ownsCard(uint32_t uiCard) const;
	bool ownsOneOfTheseCards(const std::vector<uint32_t> &vCards) const;

	void setName(std::string sName) { m_sName = sName; };
	void setHandSize(uint32_t uiHandSize) { m_uiHandSize = uiHandSize; };
};
