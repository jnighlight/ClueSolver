#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "Cli.h"
#include "Rules.h"



Cli::Cli()
{
}


Cli::~Cli()
{
}

void Cli::getUserCards(uint32_t uiUserCardCount, std::vector<uint32_t> &vCards)
{
	for (uint32_t i = 0; i < uiUserCardCount; i++)
	{
		bool bAccepted = true;
		do {
			listCards();
			std::cout << "Enter the number of a card you have";
			uint32_t uiSelection = getValidUserInt();
			if (checkAlreadyOwned()) {
				std::cout << "I think you already have claimed that card. Try again...";
				bAccepted = false;
			} else {
				bAccepted = true;
			}
		} while (bAccepted);
	}
}

void Cli::getValidUserString(std::string &sInputString)
{
	std::getline(std::cin, sInputString);
}

uint32_t Cli::getValidUserInt()
{
	std::string sUserInput;
	uint32_t uiUserInt = 0;
	bool bFirst = true;
	do {
		if (!bFirst) {
			std::cout << "Hm, it looks like that wasn't a number. Or it was 0. Which is baaasically not a number. Try again.\n";
		}
		std::getline(std::cin, sUserInput);
		std::stringstream strStream(sUserInput);
		strStream >> uiUserInt;
		bFirst = false;
	} while (uiUserInt == 0);
	return uiUserInt;
}

uint32_t Cli::getPlayerCount()
{
	std::string sUserInput;
	std::cout << "How many people are playing tonight?\n";

	uint32_t uiPlayerCount = getValidUserInt();
	while (!Rules::validPlayerCount(uiPlayerCount)) {
		std::cout << "Clue is best played with between " << unsigned(Rules::MIN_PLAYERS) << " and " << unsigned(Rules::MAX_PLAYERS) << " players. Try again!\n";
		uiPlayerCount = getValidUserInt();
	}
	return uiPlayerCount;
}

void Cli::getPlayerInfo(PlayerInfo &playerInfo)
{
	bool bAcceptable = false;
	do {
		bool bFirst = true;
		std::string sName;
		int result = 0;
		uint32_t uiCardCount = 0;
		for (auto playerToPopulate : playerInfo.m_vPlayerStartStates) {
			if (bFirst) {
				std::cout << "What's your name?\n";
				bFirst = false;
			}
			else {
				std::cout << "What's the next player's name?\n";
			}
			getValidUserString(sName);
			std::cout << "...And how many cards do they have?\n";
			uint32_t uiHandSize = getValidUserInt();

			playerToPopulate.setName(sName);
			playerToPopulate.setHandSize(uiHandSize);
			uiCardCount += uiHandSize;
		}
		bAcceptable = uiCardCount == (Rules::TOTAL_CARDS - 3);
		if (!bAcceptable) {
			std::cout << "I think you made a mistake in your math there...\n";
			std::cout << "You said that, amongst hands, there were " << unsigned(uiCardCount) << " cards\n";
			std::cout << "There are " << Rules::TOTAL_CARDS << " cards in this game (3 of which should be hidden, not in any hands).\n";
			std::cout << "Let's take it from the top.\n";
		} else {
			std::cout << "Alright! That's everyone!\n";
		}
	} while (!bAcceptable);
}