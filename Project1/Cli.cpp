#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
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
	//Using a set here so I don't have to deal with iterating for a duplicate check each time
	std::set<uint32_t> vClaimedCards;
	for (uint32_t i = 0; i < uiUserCardCount; i++)
	{
		uint32_t uiSelection = 0;
		bool bAccepted = true;
		do {
			uint32_t uiSelection = pickACard();
			//Don't want any duplicate numbers
			bAccepted = (vClaimedCards.find(uiSelection) == vClaimedCards.end());
			if (!bAccepted) {
				std::cout << "I think you already have claimed that card. Try again...";
			}
		} while (!bAccepted);
		vClaimedCards.insert(uiSelection);
	}
	for each (uint32_t uiClaimedCard in vClaimedCards)
	{
		vCards.push_back(uiClaimedCard);
	}
	std::cout << "Selecting Cards finished!";
}

uint32_t Cli::pickACard()
{
	uint32_t uiSelection = 0;
	bool bGoodCard = true;
	do {
		Rules::listCards();
		std::cout << "Enter the number of a card you have: ";
		uiSelection = getValidUserInt();
		bGoodCard = Rules::isAValidCard(uiSelection);
		if (!bGoodCard) {
				std::cout << "That's not an acceptable card. Try again.";
		}
	} while (!bGoodCard);
	return uiSelection;
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

//Returns the number of cards that a player claimed, so we can keep the running total
uint32_t Cli::getSinglePlayerInfo(PlayerManager::PlayerStartState &playerStartState, IsUser bIsUser)
{
	std::string sName;
	std::string sNameQuestion;
	std::string sCardCountQuestion;
	if (bIsUser == IS_USER) {
		sNameQuestion = "What's your name?\n";
		sCardCountQuestion = "...And how many cards do you have?\n";
	} else {
		sNameQuestion = "What's the next player's name?\n";
		sCardCountQuestion = "...And how many cards do they have?\n";
	}

	std::cout << sNameQuestion;
	getValidUserString(sName);
	std::cout << sCardCountQuestion;
	uint32_t uiHandSize = getValidUserInt();

	playerStartState.setName(sName);
	playerStartState.setHandSize(uiHandSize);
	return uiHandSize;
}

void Cli::getPlayerInfo(PlayerManager::PlayerStartStates &playerStartState)
{
	bool bAcceptable = false;
	do {
		bool bFirst = true;
		std::string sName;
		int result = 0;
		uint32_t uiCardCount = getSinglePlayerInfo(playerStartState.m_userPlayer, IS_USER);
		for (PlayerManager::PlayerStartState playerToPopulate : playerStartState.m_vPlayerStartStates) {
			uiCardCount += getSinglePlayerInfo(playerToPopulate, IS_OTHER_PLAYER);
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