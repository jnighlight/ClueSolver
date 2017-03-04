#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <ncurses.h>

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
            //TODO: This does not work...wat?
			bAccepted = (vClaimedCards.find(uiSelection) == vClaimedCards.end());
			if (!bAccepted) {
                printw("I think you already have claimed that card. Try again...\n");
                refresh();
			}
		} while (!bAccepted);
		vClaimedCards.insert(uiSelection);
	}
	for (uint32_t uiClaimedCard : vClaimedCards)
	{
		vCards.push_back(uiClaimedCard);
	}
    printw("Selecting Cards finished!\n");
    refresh();
}

/*
uint32_t Cli::pickAPlayer()
{
}
*/

uint32_t Cli::pickACard()
{
	uint32_t uiSelection = 0;
	bool bGoodCard = true;
	do {
		Rules::listCards();
		printw("Enter the number of a card you have: ");
        refresh();
		uiSelection = getValidUserInt();
		bGoodCard = Rules::isAValidCard(uiSelection);
		if (!bGoodCard) {
            printw("That's not an acceptable card. Try again.");
            refresh();
		}
	} while (!bGoodCard);
	return uiSelection;
}

void Cli::getValidUserString(std::string &sInputString, uint32_t uiMaxChars /* = 20 */)
{
    sInputString.clear();
    char str[uiMaxChars + 1];
    getnstr(str, uiMaxChars);
    sInputString = str;
}

uint32_t Cli::getValidUserInt()
{
	std::string sUserInput;
	uint32_t uiUserInt = 0;
	bool bFirst = true;
	do {
		if (!bFirst) {
            printw("Hm, it looks like that wasn't a number. Or it was 0. Which is baaasically not a number. Try again.\n");
			//std::cout << "Hm, it looks like that wasn't a number. Or it was 0. Which is baaasically not a number. Try again.\n";
		}
        getValidUserString(sUserInput);
		std::stringstream strStream(sUserInput);
		strStream >> uiUserInt;
		bFirst = false;
	} while (uiUserInt == 0);
	return uiUserInt;
}

uint32_t Cli::getPlayerCount()
{
    printw("How many people are playing tonight?\n");
    refresh();
    std::string sResponse;
	uint32_t uiPlayerCount = getValidUserInt();
    //printw("you printed: %s", sResponse.c_str());
	while (!Rules::validPlayerCount(uiPlayerCount)) {
        printw("Clue is best played with between %d and %d players. Try again!\n",unsigned(Rules::MIN_PLAYERS), unsigned(Rules::MAX_PLAYERS));
        refresh();
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

	printw(sNameQuestion.c_str());
	getValidUserString(sName);
	printw(sCardCountQuestion.c_str());
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
		for (PlayerManager::PlayerStartState &playerToPopulate : playerStartState.m_vPlayerStartStates) {
			uiCardCount += getSinglePlayerInfo(playerToPopulate, IS_OTHER_PLAYER);
		}
		bAcceptable = uiCardCount == (Rules::TOTAL_CARDS - 3);
		if (!bAcceptable) {
			printw("I think you made a mistake in your math there...\n");
			printw("You said that, amongst hands, there were %d cards\n", unsigned(uiCardCount));
			printw("There are %d cards in this game (3 of which should be hidden, not in any hands).\n", Rules::TOTAL_CARDS);
			printw("Let's take it from the top.\n");
            getch();
            clear();
            refresh();
		} else {
            clear();
			printw("Alright! That's everyone!\n");
            refresh();
		}
	} while (!bAcceptable);
}

/*
void Cli::getGuess(Guess &guess)
{
    std::cout << "Time to add guesses! Which player made the guess?\n";
    std::cout << "What cards were in the guess?\n";
        uint32_t uiFirstCard = pickACard();
        uint32_t uiSecondCard = pickACard();
        uint32_t uiThirdCard = pickACard();
        Rules::ensureOneCardOfEachType(uiFirstCard, uiSecondCard, uiThirdCard);
        guess.addCard(uiFirstCard, Rules::getCardType(uiFirstCard));
        guess.addCard(uiSecondCard, Rules::getCardType(uiSecondCard));
        guess.addCard(uiThirdCard, Rules::getCardType(uiThirdCard));
    std::cout << "Which players passed on the guess?\n";
    std::cout << "Which player solved the guess?\n";
}
*/

//This is nasty ladies and gentlemen. I want to replace this with curse soon. But for now, consider it
//  technical debt
void Cli::clearScreen()
{
    std::cout << std::string(100, '\n');
}
