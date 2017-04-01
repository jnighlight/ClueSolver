#pragma once
#include <list>
#include <string>
#include <unordered_map>

#include "Guess.h"
#include "Player.h"
#include "PlayerManager.h"
#include "PlayerStatusForDisplay.h"

class Engine
{
public:

	Engine();
	virtual ~Engine();

    /**
     * @brief Passes the incoming player info to the internam playerManager
     *
     * @param [in]  playerInfo  Incoming PlayerInfo from the CLI to go to the playerManager
     */
	void parsePlayerInfo(const PlayerManager::PlayerStartStates &playerInfo);
    /**
     * @brief Sets the user player's cards based on the cards in the incoming vector
     *
     * @param [in]  vPlayerCards    The vector containing the cards owned by the player character
     */
	void setUserCards(const std::vector<uint32_t> &vPlayerCards);

    /**
     * @brief Takes in a guess and feeds any useful information to the player manager
     *
     * @param [in]  guess   The guess coming from the CLI 
     */
	bool processGuess(const Guess &guess);

    /**
     * @brief Retrieves a list of player names from the player manager
     *
     * @return A vector populated with the names of the players
     */
    std::vector<std::string> getPlayerNames();

    /**
     * @brief Grabs the player status from the player manager to display in the CLI
     *
     * @return a PlayerStatusForDisplay to be consumed and displayed by the CLI
     */
    PlayerStatusForDisplay getPlayerStatus();

private:
	PlayerManager m_playerManager;
};
