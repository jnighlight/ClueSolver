#pragma once

#include <vector>
#include <string>
#include <cstdint>

#include "Rules.h"
#include "Player.h"
#include "PlayerManager.h"

class PlayerStatusForDisplay
{
public:
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

    std::list<PlayerDisplay> m_vPlayerDisplays;

    void addPlayer(const Player &player, const PlayerManager* pPlayerManager);
    void addPlayerCards(const Player &player, PlayerDisplay &playerDisplay);
    void addPlayerGuesses(const Player &player, PlayerDisplay &playerDisplay);
    void addPlayerNotOwnedCards(const Player &player,
                                PlayerDisplay &playerDisplay,
                                const PlayerManager* pPlayerManager);
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
    uint32_t m_uiSolvedPerson;
    uint32_t m_uiSolvedPlace;
    uint32_t m_uiSolvedWeapon;
    bool m_bHasSolvedValue;
};
