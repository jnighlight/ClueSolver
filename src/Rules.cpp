#include "Rules.h"

#include <ncurses.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

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
	printw("Persons:\n");
	for (std::string sPersonCard : s_lPeople)
	{
		printw("%d. ",uiSelectionCounter);
        printw(sPersonCard.c_str());
		printw(" ");
		++uiSelectionCounter;
	}
	printw("\n");
	printw("Weapons:\n");
	for (std::string sWeaponCard : s_lWeapons)
	{
		printw("%d. ",uiSelectionCounter);
		printw(sWeaponCard.c_str());
		printw(" ");
		++uiSelectionCounter;
	}
	printw("\n");
	printw("Places:\n");
	for (std::string sPlaceCard : s_lPlaces)
	{
		printw("%d. ",uiSelectionCounter);
		printw(sPlaceCard.c_str());
		printw(" ");
		++uiSelectionCounter;
	}
	printw("\n");
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
