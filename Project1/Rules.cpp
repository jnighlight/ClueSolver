#include "Rules.h"

const uint32_t Rules::MAX_PLAYERS = 6;
const uint32_t Rules::MIN_PLAYERS = 3;

const std::vector<std::string> Rules::s_lPeople = { "Scarlet", "Mustard", "White", "Green", "Peacock", "Plum" };
const std::vector<std::string> Rules::s_lWeapons = { "Candlestick", "Poison", "Rope", "Gloves", "Knife", "Lead Pipe", "Revolver", "Wrench" };
const std::vector<std::string> Rules::s_lPlaces= {"Kitchen", "Ballroom", "Conservatory", "Billiard Room", "Library", "Study", "Hall", "Lounge", "Dining Room"};
const uint32_t Rules::TOTAL_CARDS = (Rules::s_lPeople.size() + Rules::s_lWeapons.size() + Rules::s_lPlaces.size());

bool Rules::validPlayerCount(uint32_t uiPlayerCount)
{
	return (uiPlayerCount <= MAX_PLAYERS && uiPlayerCount >= MIN_PLAYERS);
}

Rules::Rules()
{
}


Rules::~Rules()
{
}
