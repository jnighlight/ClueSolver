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

	PlayerStatusForDisplay() {};
	virtual ~PlayerStatusForDisplay() {};
    std::list<PlayerDisplay> m_vPlayerDisplays;
    void addPlayer(const Player &player, PlayerManager* pPlayerManager);
    void addPlayerCards(const Player &player, PlayerDisplay &playerDisplay);
    void addPlayerGuesses(const Player &player, PlayerDisplay &playerDisplay);
    void addPlayerNotOwnedCards(const Player &player, PlayerDisplay &playerDisplay, PlayerManager* pPlayerManager);
};
