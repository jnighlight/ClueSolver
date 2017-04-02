#pragma once

#include <vector>
#include <string>
#include <cstdint>

#include "Rules.h"
#include "Player.h"
#include "PlayerManager.h"

/**
 * @brief An object containing all relevant status information to display to the user
 *
 */
class PlayerStatusForDisplay
{
public:
    /** @brief A logical representation of any unsolved guesses a player may have */
    struct GuessDisplay {
        GuessDisplay()
            :m_uiPersonCard(0)
            ,m_uiWeaponCard(0)
            ,m_uiPlaceCard(0)
        {};

        uint32_t m_uiPersonCard;
        uint32_t m_uiWeaponCard;
        uint32_t m_uiPlaceCard;
    };

    /**
     * @brief Object containing player status, IE name, hand size, and cards that are/are not owned
     */
    struct PlayerDisplay {
        PlayerDisplay()
            :m_uiCardCount(0) {};

        std::string m_sName;
        uint32_t m_uiCardCount;
        std::vector<std::string> m_vOwnedCardNames;
        std::vector<std::string> m_vDefinitelyNotOwnedCardNames;
        std::vector<PlayerStatusForDisplay::GuessDisplay> m_vAnsweredGuesses;
    };

	PlayerStatusForDisplay()
    :m_uiSolvedPerson(0)
    ,m_uiSolvedPlace(0)
    ,m_uiSolvedWeapon(0)
    ,m_bHasSolvedValue(false)
    {};

	virtual ~PlayerStatusForDisplay() {};

    /** A list for displaying the stats of each player*/
    std::list<PlayerDisplay> m_vPlayerDisplays;

    /**
     * @brief Copies data from the player into the internal m_vPlayerDisplays
     *
     * @param [in]  player          The player from whom to copy the information
     * @param [in]  pPlayerManager  The player manager for checking if passed cards should be included
     */
    void addPlayer(const Player &player, const PlayerManager* pPlayerManager);

    void setSolvedPerson(uint32_t uiPerson) {
        m_uiSolvedPerson = uiPerson;
        m_bHasSolvedValue = true;
    };
    void setSolvedPlace(uint32_t uiPlace)
    {
        m_uiSolvedPlace = uiPlace;
        m_bHasSolvedValue = true;
    };
    void setSolvedWeapon(uint32_t uiWeapon)
    {
        m_uiSolvedWeapon = uiWeapon;
        m_bHasSolvedValue = true;
    };

    uint32_t getSolvedPerson() const { return m_uiSolvedPerson; };
    uint32_t getSolvedPlace() const { return m_uiSolvedPlace; };
    uint32_t getSolvedWeapon() const { return m_uiSolvedWeapon; };
    bool hasSolvedValue() const { return m_bHasSolvedValue; };
private:
    /**
     * @brief Adds the cards owned by the player to that player's PlayerDisplay
     *
     * @param [in]  player          Player from which to grab owned cards
     * @param [out] playerDisplay   PlayerDisplay to populate with owned cards
     */
    void addPlayerCards(const Player &player, PlayerDisplay &playerDisplay);

    /**
     * @brief Adds the guesses owned by the player to that player's PlayerDisplay
     *
     * @param [in]  player          Player from which to grab owned guesses
     * @param [out] playerDisplay   PlayerDisplay to populate with owned guesses
     */
    void addPlayerGuesses(const Player &player, PlayerDisplay &playerDisplay);

    /**
     * @brief Adds the cards not owned by the player to that player's PlayerDisplay
     *
     * @param [in]  player          Player from which to grab not owned cards
     * @param [out] playerDisplay   PlayerDisplay to populate with not owned cards
     * @param [in]  pPlayerManager  PlayerManager to check not owned cards against. We won't display
     *                              not owned cards if another player owns them, it should be obvious
     */
    void addPlayerNotOwnedCards(const Player &player,
                                PlayerDisplay &playerDisplay,
                                const PlayerManager* pPlayerManager);

    uint32_t m_uiSolvedPerson;
    uint32_t m_uiSolvedPlace;
    uint32_t m_uiSolvedWeapon;

    /** If this display has a solved value in it */
    bool m_bHasSolvedValue;
};
