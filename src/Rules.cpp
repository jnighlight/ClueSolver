#include "Rules.h"

#include <ncurses.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

const uint32_t Rules::MAX_PLAYERS = 6;
const uint32_t Rules::MIN_PLAYERS = 3;

const std::vector<std::string> Rules::s_lPeople = { "Scarlet", "Mustard", "White", "Green", "Peacock", "Plum" };
const std::vector<std::string> Rules::s_lWeapons = { "Candlestick", "Poison", "Rope", "Gloves", "Knife", "Lead Pipe", "Revolver", "Wrench" };
//const std::vector<std::string> Rules::s_lWeapons = { "Candlestick", "Rope", "Knife", "Lead Pipe", "Revolver", "Wrench" };
const std::vector<std::string> Rules::s_lPlaces= {"Kitchen", "Ballroom", "Conservatory", "Billiard Room", "Library", "Study", "Hall", "Lounge", "Dining Room"};
const uint32_t Rules::PEOPLE_CARD_COUNT = (Rules::s_lPeople.size());
const uint32_t Rules::WEAPON_CARD_COUNT = (Rules::s_lWeapons.size());
const uint32_t Rules::PLACE_CARD_COUNT = (Rules::s_lPlaces.size());
const uint32_t Rules::TOTAL_CARDS = (Rules::s_lPeople.size() + Rules::s_lWeapons.size() + Rules::s_lPlaces.size());

bool Rules::validPlayerCount(uint32_t uiPlayerCount)
{
	return (uiPlayerCount <= MAX_PLAYERS && uiPlayerCount >= MIN_PLAYERS);
}

std::string Rules::listCards()
{
    std::string sCardList;
	uint32_t uiSelectionCounter = 1;
	sCardList += "Persons:\n";
	for (std::string sPersonCard : s_lPeople)
	{
		sCardList += std::to_string(uiSelectionCounter) + ". ";
        sCardList += sPersonCard.c_str();
		sCardList += "\n";
		++uiSelectionCounter;
	}
	sCardList += "\nWeapons:\n";
	for (std::string sWeaponCard : s_lWeapons)
	{
		sCardList += std::to_string(uiSelectionCounter) + ". ";
		sCardList += sWeaponCard.c_str();
		sCardList += "\n";
		++uiSelectionCounter;
	}
	sCardList += "\n";
	sCardList += "Places:\n";
	for (std::string sPlaceCard : s_lPlaces)
	{
		sCardList += std::to_string(uiSelectionCounter) + ". ";
		sCardList += sPlaceCard.c_str();
		sCardList += "\n";
		++uiSelectionCounter;
	}
	sCardList += "\n";
    return sCardList;
}

std::string Rules::getCardName(uint32_t uiCard)
{
    Rules::eCardType eRulesCardType = Rules::getCardType(uiCard);
    uint32_t uiCardLoc = uiCard;
    switch (eRulesCardType) {
        case (ePerson):
            return Rules::s_lPeople[uiCard - 1 ];
            break;
        case (eWeapon):
            uiCardLoc = uiCard - PEOPLE_CARD_COUNT;
            return Rules::s_lWeapons[uiCardLoc - 1 ];
            break;
        case (ePlace):
            uiCardLoc = uiCard - PEOPLE_CARD_COUNT;
            uiCardLoc -= WEAPON_CARD_COUNT;
            return Rules::s_lPlaces[uiCardLoc - 1 ];
            break;
        default:
            break;
    }
    return "";
}

bool Rules::isAValidCard(uint32_t uiCardNumber)
{
	return uiCardNumber > 0 && uiCardNumber <= Rules::TOTAL_CARDS;
}

bool Rules::isPersonCard(uint32_t uiCard)
{
    return uiCard > 0 && uiCard <= PEOPLE_CARD_COUNT;
}

bool Rules::isWeaponCard(uint32_t uiCard)
{
    return uiCard > PEOPLE_CARD_COUNT && uiCard <= (PEOPLE_CARD_COUNT + WEAPON_CARD_COUNT);
}

bool Rules::isPlaceCard(uint32_t uiCard)
{
    return uiCard > (PEOPLE_CARD_COUNT + WEAPON_CARD_COUNT)
        && uiCard <= (PEOPLE_CARD_COUNT + WEAPON_CARD_COUNT + PLACE_CARD_COUNT);
}

Rules::eCardType Rules::getCardType(uint32_t uiCard)
{
    if (Rules::isPersonCard(uiCard)) {
        return Rules::ePerson;
    } else if (Rules::isWeaponCard(uiCard)) {
        return Rules::eWeapon;
    } else if (Rules::isPlaceCard(uiCard)) {
        return Rules::ePlace;
    }
    printw("Rules::getCardType - THIS CARD DON'T FIT YO");
    return Rules::eUnknownType;
    //TODO: throw exception
}

bool Rules::isOneOfEachCardType(const std::vector<uint32_t> &vGuessCards)
{
    uint32_t uiPersonCards = 0;
    uint32_t uiPlaceCards = 0;
    uint32_t uiWeaponCards = 0;
    for (uint32_t uiCard : vGuessCards) {
        if (isPersonCard(uiCard)) {
            ++uiPersonCards;
        } else if (isWeaponCard(uiCard)) {
            ++uiWeaponCards;
        } else if (isPlaceCard(uiCard)) {
            ++uiPlaceCards;
        }
    }
	return uiPersonCards == 1 && uiPlaceCards == 1 && uiWeaponCards == 1;
}

Rules::Rules()
{
}


Rules::~Rules()
{
}
