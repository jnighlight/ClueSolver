#pragma once
#include <cstdint>
#include <vector>
class Rules
{
public:
	Rules();
	~Rules();
	static const uint32_t MAX_PLAYERS;
	static const uint32_t MIN_PLAYERS;
	static const std::vector<std::string> s_lPeople;
	static const std::vector<std::string> s_lWeapons;
	static const std::vector<std::string> s_lPlaces;
	static const uint32_t TOTAL_CARDS;
	static bool validPlayerCount(uint32_t uiPlayerCount);
};

