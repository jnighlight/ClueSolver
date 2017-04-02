#pragma once
#include <cstdint>
#include <vector>
#include <string>

/**
 * @brief A static rules object for reference and for easy editing.
 *
 * This rules object contains the hard data about the game itself, including the cards, their names,
 *  the max and min players, and methods for checking to see if data relating to the rules is correct.
 *  It's also a nice and simple place to modify if you want to change the game rules up
 */
class Rules
{
public:
	Rules();
	~Rules();
    /** The maximum number of players allowed */
	static const uint32_t MAX_PLAYERS;
    /** The minimum number of players allowed */
	static const uint32_t MIN_PLAYERS;
    /** Enum to specify a type of card */
    enum eCardType {
        ePerson = 0,
        eWeapon,
        ePlace,
        eUnknownType,
       };

    /** Simple struct for returning the acceptable range of numbers for a card type*/
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

    /** Number of People cards */
	static const uint32_t PEOPLE_CARD_COUNT;
    /** Number of Weapon cards */
	static const uint32_t WEAPON_CARD_COUNT;
    /** Number of Place cards */
	static const uint32_t PLACE_CARD_COUNT;

    /** List of People Card Names */
	static const std::vector<std::string> s_lPeople;
    /** List of Weapon Card Names */
	static const std::vector<std::string> s_lWeapons;
    /** List of Place Card Names */
	static const std::vector<std::string> s_lPlaces;

    /** Total number of cards in game */
	static const uint32_t TOTAL_CARDS;

    /**
     * @brief Check to see if the number of players is valid
     *
     * @param [in]  uiPlayerCount   The number of players to check
     *
     * @return True if the number of players is acceptable
     */
	static bool validPlayerCount(uint32_t uiPlayerCount);

    /**
     * @brief Returns a std::string with a list of all possible cards
     *
     * @return a string representation of a list of all possible cards
     */
	static std::string listCards();

    /**
     * @brief Checks if card is a valid card
     *
     * @param [in]  uiCardNumber    The number of the card to check
     *
     * @return True if it's a valid card
     */
	static bool isAValidCard(uint32_t uiCardNumber);

    /**
     * @brief Checks if there is one card of each type in the vector
     *
     * @param [in]  vGuessCards Vector containing cards
     *
     * @return True if there is one of each type of card in the vector
     */
    static bool isOneOfEachCardType(const std::vector<uint32_t> &vGuessCards);

    static bool isPersonCard(uint32_t uiCard);
    static bool isWeaponCard(uint32_t uiCard);
    static bool isPlaceCard(uint32_t uiCard);

    static Rules::eCardType getCardType(uint32_t uiCard);
    static std::string getCardName(uint32_t uiCard);

    /**
     * @brief Get the Card number range for a certain type of card
     *
     * @param [in]  eCardType   The card type we want a range for
     *
     * @return A Rules struct with a min and a max, representing a range
     */
    static Rules::CardTypeRange getCardTypeRange(Rules::eCardType eCardType);
};
