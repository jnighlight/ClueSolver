#pragma once
#include <cstdint>
#include <ncurses.h>

#include "PlayerManager.h"
#include "PlayerStatusForDisplay.h"

/**
 * @brief The Command Line Interface object, displays data to the user
 *
 * This class controls 2 (technically 4) Curses windows to display game state and retrieve
 *  information from the user. It recieves objects and data from the Game object to parses,
 *  and sends input data back for processing by the engine
 *
 */
class Cli
{
public:
    /**
     * @brief An enum to distinguish between the user player and the rest of the players
     *
     * This information may be useful to decide whether or not to display a users info (the
     *  player user already knows their own cards) and whether to query for the answer to a guess
     *  (If the player made a guess and it's solved, they'll get a card answered from it)
     *
     */
	enum eIsUser {
		IS_USER = 0,
		IS_OTHER_PLAYER
	};

    /**
     * @brief The constructor of the CLI class, sets up the curses windows
     */
	Cli();

    /**
     * @brief The destructor of the CLI class, removes the windows
     */
	~Cli();

    /**
     * @brief Retrieves player count data from the user
     *
     * @return The number of players playing the Clue game
     */
	uint32_t getPlayerCount();

    /**
     * @brief Retrieves information for a single player (name and number of cards in hand)
     *
     * @param [out] playerStartState    The playerStartState to be populated with the gathered info
     * @param [in]  bIsUser             If the playerStartState represents the user player
     *
     * @return The number of cards that a player claimed
     */
	uint32_t getSinglePlayerInfo(PlayerManager::PlayerStartState &playerStartState, eIsUser bIsUser);

    /**
     * @brief Retrieves information for a single player (name and number of cards in hand)
     *
     * @param [out] playerStartState    The playerStartState to be populated with the gathered info
     * @param [in]  bIsUser             If the playerStartState represents the user player
     *
     */
	void getPlayerInfo(PlayerManager::PlayerStartStates &playerStartState);

    /**
     * @brief Populates a vector with the cards owned by the player character
     *
     * @param [in]  uiUserCardCount    The number of cards that the player character has in hand
     * @param [out] vCards             The vector of cards to populate
     *
     */
	void getUserCards(uint32_t uiUserCardCount, std::vector<uint32_t> &vCards);

    /**
     * @brief Populates a guess based on information from the user
     *
     * This method is the meat and potatoes of the UI. It populates a guess object with information
     *  input from the user, which is passed to the engine to be parsed. There are several parts
     *  to a guess. The guesser, the cards they guessed, who solved the guess, who it passed, and
     *  (if the guesser is the player) the card that was shown to the player.
     *
     * @param [out] guess           The guess object to be populated
     * @param [in]  vPlayerNames    The vector of player names for refrencing players
     *
     */
    void getGuess(Guess &guess, std::vector<std::string> vPlayerNames);

    /**
     * @brief Sets the status window with a the information from a PlayerStatusForDisplay object
     *
     * @param [in]  playerStatusForDisplay   The information with which to populate the status screen
     *
     */
    void setStatus(const PlayerStatusForDisplay &playerStatusForDisplay);

    /**
     * @brief Shows the ending screen with all collected data
     */
    void showEndScreen();
private:

    /**
     * @brief Retrieves a valid user integer from the curses CLI
     *
     * @return The incoming uint32_t from the user
     */
	uint32_t getValidUserInt();

    /**
     * @brief Retrieves a valid user string from the curses CLI
     *
     * We limit the incoming amount of characters from the user to make sure that no funny
     *  business happens with overflow or anything. Also useful for limiting integer selection
     *  to a specific number
     *
     * @param [out] sInputString    The string to populate with the user's input
     * @param [in]  uiMaxChars      The Maximum amount of characters from the user
     */
	void getValidUserString(std::string &sInputString, uint32_t uiMaxChars = 20);

    /**
     * @brief Lists all cards to the user and asks them to select one
     *
     * @return The number of the card selected by the user
     */
	uint32_t pickACard();

    /**
     * @brief Lists the players given
     *
     * @param [in]  vPlayerNames    The vector of player names to list
     * @param [in]  bIncludeNone    Whether or not to output "None" as an option
     */
    void listPlayers(std::vector<std::string> vPlayerNames, bool bIncludeNone = false);

    /**
     * @brief Simple helper method to clear the windows of all output
     */
    void refreshWindows();

    /**
     * Window specifically to show the text window border
     */
    WINDOW *m_textWinBorder;
    /**
     * Window for user interaction to take place
     */
    WINDOW *m_textWin;

    /**
     * Window specifically to show the status window border
     */
    WINDOW *m_statusWinBorder;
    /**
     * Window for game status to be displayed
     */
    WINDOW *m_statusWin;
};

