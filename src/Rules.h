#pragma once
#include <cstdint>
#include <vector>
#include <string>

class Rules
{
public:
	Rules();
	~Rules();
	static const uint32_t MAX_PLAYERS;
	static const uint32_t MIN_PLAYERS;
    enum eCardType {
        ePerson = 0,
        eWeapon,
        ePlace,
        eUnknownType,
       };

    struct CardTypeRange {
        CardTypeRange()
            :m_uiMin(0)
            ,m_uiMax(0) {}

        CardTypeRange(uint32_t uiMin, uint32_t uiMax)
            :m_uiMin(uiMin)
            ,m_uiMax(uiMax) {}

        uint32_t m_uiMin;
        uint32_t m_uiMax;
    };

	static const uint32_t PEOPLE_CARD_COUNT;
	static const uint32_t WEAPON_CARD_COUNT;
	static const uint32_t PLACE_CARD_COUNT;
	static const std::vector<std::string> s_lPeople;
	static const std::vector<std::string> s_lWeapons;
	static const std::vector<std::string> s_lPlaces;
	static const uint32_t TOTAL_CARDS;
	static bool validPlayerCount(uint32_t uiPlayerCount);
	static std::string listCards();
	static bool isAValidCard(uint32_t uiCardNumber);
    static bool isOneOfEachCardType(const std::vector<uint32_t> &vGuessCards);
    static bool isPersonCard(uint32_t uiCard);
    static bool isWeaponCard(uint32_t uiCard);
    static bool isPlaceCard(uint32_t uiCard);
    static Rules::eCardType getCardType(uint32_t uiCard);
    static std::string getCardName(uint32_t uiCard);
    static Rules::CardTypeRange getCardTypeRange(Rules::eCardType eCardType);
};
