#pragma once
#include <string>
#include <vector>

#include "Guess.h"
#include "Player.h"
#include "PlayerStatusForDisplay.h"

class PlayerStatusForDisplay;

/**
 * @brief An object to manage the states of the players of the game
 *
 * This object is used to interface with the players. If we pass a guess generated from the CLI
 *  here, it can parse that information out and feed it to the players, then run an update loop
 *  to determine the current game state.
 *
 */
class PlayerManager
{
public:
    /**
     * @brief Object to store the starting state of a player. Populated by the CLI
     */
	struct PlayerStartState {

		PlayerStartState()
			:m_uiHandSize(0)
		{};

		PlayerStartState(unsigned int uiHandSize, std::string sName)
			:m_uiHandSize(uiHandSize)
			,m_sName(sName)
		{};

		void setHandSize(unsigned int uiHandSize) { m_uiHandSize = uiHandSize; };
		void setName(std::string sName) { m_sName = sName; };

		unsigned int m_uiHandSize;
		std::string m_sName;
	};

    /**
     * @brief Data structure to hold multip PlayerStartStates, including the User's
     *
     * This data is populate by the CLI, and passed to the player manager to populate the player
     *  and start the game in an accurate state
     *
     */
	struct PlayerStartStates{
		PlayerStartStates(uint32_t uiNumberOfPlayers)
			:m_vPlayerStartStates()
		{
            //uiNumber of players is -1, to account for the user player
            for (uint32_t i = 0; i < uiNumberOfPlayers-1; i++)
            {
                PlayerStartState otherPlayer;
                m_vPlayerStartStates.push_back(otherPlayer);
            }
        };

		PlayerStartState m_userPlayer;
		uint32_t getUserPlayerHandSize() { return m_userPlayer.m_uiHandSize; };
		std::vector<PlayerStartState> m_vPlayerStartStates;
	};

	PlayerManager() {};
	PlayerManager(const PlayerStartStates &playerStartStates);
	virtual ~PlayerManager();

    /**
     * @brief Populates the internal Player objects with the information from the playerStartStates
     *
     * @param [in]  playerStartStates   The object containing the data representing the player's start states
     */
	void parsePlayerStartStates(const PlayerStartStates &playerStartStates);

    /**
     * @brief Sets the cards owned by the user player
     *
     * @param [in]  vPlayerCards    A vector of uints representing cards
     */
	void setUserPlayerCards(const std::vector<uint32_t> &vPlayerCards);

    /**
     * @brief Loops through the players, attempting to solve their answered guesses.
     *
     * This method will loop through each player, checking to see if the information in the latest
     *  guess has provided enough information for a new card to be claimed. If it has, the player's
     *  state is updated and we loop through each player again. We loop again because every card
     *  claimed may solve a guess for another player. We'll loop through up to once for every card
     *  claimed.
     */
    void updatePlayerState();

    /**
     * @brief Adds a solved guess to the player that has claimed they solved it, if it's useful
     *
     * @param [in]  guess   The guess that has been stopped by a player
     */
    void addSolvedGuess(const Guess &guess);

    /**
     * @brief Adds the passed guess to the player that passed it, adding it's cards to their recorded passed cards
     *
     * Makes a local copy of guess for modification and storage
     *
     * @param [in]  sPasser The name of the player that passed the card
     * @param [in]  guess   The guess that has been passed
     */
    void addPassedGuess(const std::string &sPasser, const Guess &guess);

    /**
     * @brief Checks all players to see if the card is owned
     *
     * @param [in]  uiCard  The ID representing the card
     *
     * @return True if the card is owned by any player
     */
	bool isOwned(uint32_t uiCard) const;

    /**
     * @brief Returns if the game is solved
     *
     * This checks to see if we can deduce the center card for each of the 3 categories. If we can,
     *  then the game has been solved and we know the answer for sure. 
     *
     * @return True if the game has been solved
     */
    bool isSolved();

    /**
     * @brief Confirms that no one owns a particular card
     *
     * @param [in]  uiCard  The card to ensure is unowned
     *
     * @return True if the card is definitely not owned by any player
     */
    bool confirmNoOneOwnsCard(uint32_t uiCard) const;

    /**
     * @brief Generates the information to be displayed for the state of the players
     *
     * @return A PlayerStatusForDisplay object populated with the state of the players
     */
    PlayerStatusForDisplay getPlayerStatusForDisplay() const;

    /**
     * @brief Returns a list of the player names
     *
     * @return A string vector of all player names
     */
    std::vector<std::string> getPlayerNames();

	Player m_userPlayer;
	std::vector<Player> m_otherPlayers;
	Player* getPlayer(const std::string &sPlayerName);

    /**
     * @brief Checks to see if a card category has been solved
     *
     * @param [in]  eCardType   The card type that may or may not be solved
     *
     * @return The number of the answer card if solved, otherwise returns 0
     */
    uint32_t isTypeSolved(Rules::eCardType eCardType) const;

private:
    /**
     * @brief Adds any solved cards to the PlayerStatusForDisplay
     *
     * @param [out] playerStatusForDisplay  The playerStatusForDisplay to populate with any solved cards
     */
    void addAnswerCards(PlayerStatusForDisplay &playerStatusForDisplay) const;

    /**
     * @brief Removed any cards that we know are owned from the answeredGuess
     *
     * @param [out]  answeredGuess  The answered guess to remove any owned cards from
     *
     * @return The number of cards left that are NOT owned in the answeredGuess
     */
    uint32_t removeOwnedCardsFromGuess(Player::AnsweredGuess &answeredGuess) const;
};

