#include "Rules.h"

#include <stdio.h>
#include <iostream>
#include <string>

const uint32_t Rules::MAX_PLAYERS = 6;
const uint32_t Rules::MIN_PLAYERS = 3;

const std::vector<std::string> Rules::s_lPeople = { "Scarlet", "Mustard", "White", "Green", "Peacock", "Plum" };
const std::vector<std::string> Rules::s_lWeapons = { "Candlestick", "Poison", "Rope", "Gloves", "Knife", "Lead Pipe", "Revolver", "Wrench" };
const std::vector<std::string> Rules::s_lPlaces= {"Kitchen", "Ballroom", "Conservatory", "Billiard Room", "Library", "Study", "Hall", "Lounge", "Dining Room"};
const uint32_t Rules::PEOPLE_CARD_COUNT = (Rules::s_lPeople.size());
const uint32_t Rules::WEAPON_CARD_COUNT = (Rules::s_lWeapons.size());
const uint32_t Rules::PLACE_CARD_COUNT = (Rules::s_lPlaces.size());
const uint32_t Rules::TOTAL_CARDS = (Rules::s_lPeople.size() + Rules::s_lWeapons.size() + Rules::s_lPlaces.size());

bool Rules::validPlayerCount(uint32_t uiPlayerCount)
{
	return (uiPlayerCount <= MAX_PLAYERS && uiPlayerCount >= MIN_PLAYERS);
}

void Rules::listCards()
{
	uint32_t uiSelectionCounter = 1;
	std::cout << "Persons:\n";
	for each (std::string sPersonCard in s_lPeople)
	{
		std::cout << uiSelectionCounter << ". " << sPersonCard;
		std::cout << "  ";
		++uiSelectionCounter;
	}
	std::cout << "\n";
	std::cout << "Weapons:\n";
	for each (std::string sWeaponCard in s_lWeapons)
	{
		std::cout << uiSelectionCounter << ". " << sWeaponCard;
		std::cout << "  ";
		++uiSelectionCounter;
	}
	std::cout << "\n";
	std::cout << "Places:\n";
	for each (std::string sPlaceCard in s_lPlaces)
	{
		std::cout << uiSelectionCounter << ". " << sPlaceCard;
		std::cout << "  ";
		++uiSelectionCounter;
	}
	std::cout << "\n";
}

bool Rules::isAValidCard(uint32_t uiCardNumber)
{
	return uiCardNumber > 0 && uiCardNumber < Rules::TOTAL_CARDS;
}

Rules::Rules()
{
}


Rules::~Rules()
{
}
