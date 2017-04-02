#pragma once
#include <string>
#include <set>
#include <vector>
#include <list>
#include <iostream>
#include "Guess.h"

class PlayerManager;

/**
 * @brief An object representing all the knowledge we currently know about a given player
 */
class Player
{
public:
    /**
     * @brief An internal representation of a guess that a player has stopped
     */
	struct AnsweredGuess {
		AnsweredGuess()
			:m_uiPlace(0)
			,m_uiPerson(0)
			,m_uiWeapon(0)
        {};
		AnsweredGuess(const Guess &guess)
			:m_uiPlace(guess.m_uiPlace)
			,m_uiPerson(guess.m_uiPerson)
			,m_uiWeapon(guess.m_uiWeapon)
		{};
		AnsweredGuess(uint32_t uiPerson, uint32_t uiWeapon, uint32_t uiPlace)
			:m_uiPlace(uiPlace)
			,m_uiPerson(uiPerson)
			,m_uiWeapon(uiWeapon)
		{};

		~AnsweredGuess() {};

		uint32_t m_uiPlace;
		uint32_t m_uiPerson;
		uint32_t m_uiWeapon;

        /**
         * @brief Returns true if the guess has been solved
         *
         * @return If the guess only has one card left, IE is solved
         */
		bool isSolved();

        /**
         * @brief If the card has been solved, grabs the answer
         *
         * @return Returns the card number if solved, 0 otherwise
         */
		uint32_t getSolved();

		bool isEmpty() { return (m_uiPerson == 0 && m_uiPlace == 0 && m_uiWeapon == 0); }
	};

	Player()
     :m_uiHandSize(0) {};

	Player(std::string sName, unsigned int uiHandSize);
	virtual ~Player();

    /** The name of the player */
	std::string m_sName;

    /** The number of cards in the player's hand */
	unsigned int m_uiHandSize;

    /** The set of cards the player owns */
	std::set<uint32_t> m_setOwnedCards;

    /** The set of cards the player definitely does not own */
	std::set<uint32_t> m_setDefinitelyNotOwnedCards;

    /** The list of guesses the player has answered that we haven't solved yet */
	std::list<AnsweredGuess> m_lAnsweredGuesses;

    /**
     * @brief Function to determine if we know all of the player's cards
     *
     * @return True if the character's owned card count is equal to their hand size
     */
    bool isSolved() const;

    /**
     * @brief Add a card to the player's owned cards
     *
     * @param [in]  uiCardToAdd The card to add to the Player's owned cards
     */
	void addCard(uint32_t uiCardToAdd);

    /**
     * @brief Add a AnsweredGuess to the list of answeredGuesses
     *
     * @param [in]  answeredGuess   The guess to add to the Player's list
     */
    void addGuess(const AnsweredGuess &answeredGuess);

    /**
     * @brief Add all cards in a guess to the list of cards the player definitely does not own
     *
     * @param [in]  guess   The guess containing the cards the player definitely does not own
     */
    void addNotOwnedCards(const Guess &guess);

    /**
     * @brief If player definitely does not own the card
     *
     * A sort of "GOTCHA" here is that the not owned status of a card is NOT updated if the card
     *  is claimed by another player.
     *  IE if player 1 takes card 1, player 2 does NOT record that he definitely doesn't own card 1
     *  In other words, the "Are definitely owned" and "are not definitely owned" card lists among
     *  players are NOT exact mirrors. This is only ok because every time we check if a card is
     *  definitely not owned, we ALSO all players to make sure that it is not owned. Just know that
     *  this method doesn't necessarily tell the whole story.
     *
     * @param [in]  uiCard  The card the player may or may not own
     * @return True if the player definitely does not own the card
     */
    bool isDefinitelyNotOwned(uint32_t uiCard) const;

    /**
     * @brief Checks for possible solutions for this player, may modify the cards that this player
     *  does or does not definitely own
     *
     * @param [in]  pPlayerManager  The playerManager to query for other Player's card states
     * @return True if there was a solution found, thus changing the state of this player
     */
	bool checkForSolutions(PlayerManager* pPlayerManager);

    /** Enum to determine the state of an AnsweredGuess */
    enum eGuessState {
        eNotSolved = 0,
        eSolved,
        eTrash
    };

    /**
     * @brief Processes a stored AnsweredGuess to check if it can be solved or if it's trash
     *
     * @param [in]  answeredGuess   The answeredGuess to check for a possible solution
     * @param [in]  pPlayerManager  The playerManager to query for other Player's card states
     * @return eGuessState based on the new state of the guess
     */
    Player::eGuessState processStoredGuess(AnsweredGuess &answeredGuess, PlayerManager* pPlayerManager);

    /**
     * @brief If the player owns the card
     *
     * @param [in]  uiCard  The card the player may or may not own
     * @return True if the card is owned by the player
     */
	bool ownsCard(uint32_t uiCard) const;

    /**
     * @brief If the player owns one of the cards in the answeredGuess
     *
     * @param [in]  answeredGuess   The answered guess containing cards to check against the player
     * @return True if the player owns one of the cards in the guess
     */
    bool ownsOneOfTheseCards(const AnsweredGuess &answeredGuess) const;

	void setName(std::string sName) { m_sName = sName; };
	void setHandSize(uint32_t uiHandSize) { m_uiHandSize = uiHandSize; };
};
