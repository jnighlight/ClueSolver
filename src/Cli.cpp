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

uint32_t Cli::pickACard()
{
	uint32_t uiSelection = 0;
	bool bGoodCard = true;
	do {
        clear();
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
	bool bGoodNumber = false;
	do {
        getValidUserString(sUserInput);
        try {
            uiUserInt = std::stoi(sUserInput);
            bGoodNumber = true;
        } catch (std::invalid_argument e) {
            printw("Hm, it looks like that wasn't a number. Try again.\n");
        }
	} while (!bGoodNumber);
	return uiUserInt;
}

uint32_t Cli::getPlayerCount()
{
    printw("How many people are playing tonight?\n");
    refresh();
    std::string sResponse;
	uint32_t uiPlayerCount = getValidUserInt();
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

void Cli::listPlayers(std::vector<std::string> vPlayerNames, bool bIncludeNone /* = false */)
{
	uint32_t uiSelectionCounter = 1;
	printw("Players:\n");
    if (bIncludeNone) {
		printw("0. None");
		printw(" ");
    }
	for (std::string sPerson: vPlayerNames)
	{
		printw("%d. ",uiSelectionCounter);
        printw(sPerson.c_str());
		printw(" ");
		++uiSelectionCounter;
	}
	printw("\n");
}

//TODO: Try to decouple the UI with the logic for better unit testing
void Cli::getGuess(Guess &guess, std::vector<std::string> vPlayerNames)
{
    //clear();
    printw("Time to add guesses! Which player made the guess?\n");
    listPlayers(vPlayerNames);
    refresh();
    uint32_t uiGuesser = 0;
    do {
        uiGuesser = getValidUserInt();
        if (uiGuesser <= 0 || uiGuesser > vPlayerNames.size()) {
            printw("That was not an option. Try again\n");
        }
    } while (uiGuesser <= 0 || uiGuesser > vPlayerNames.size());

    std::vector<uint32_t> vGuessCards = { 0, 0, 0};
    bool bGotAcceptableCards = false;
    do {
        clear();
        printw("What cards were in the guess?\n");
        refresh();
        for (uint32_t &uiCard : vGuessCards)
        {
            uiCard = pickACard();
        }
        if (Rules::isOneOfEachCardType(vGuessCards)) {
            for (uint32_t &uiCard : vGuessCards)
            {
                Rules::eCardType eCardType = Rules::getCardType(uiCard);
                guess.addCard(uiCard, eCardType);
            }
            bGotAcceptableCards = true;
        } else {
            printw("You must select one of each type of card. Try again\n");
            getch();
        }
    } while (!bGotAcceptableCards);

    printw("Which player solved the guess?\n");
    listPlayers(vPlayerNames, true);
    uint32_t uiSolver = 0;
    do {
        uiSolver = getValidUserInt();
        if (uiSolver > vPlayerNames.size()) {
            printw("That was not an option. Try again\n");
        } else if (uiSolver == uiGuesser) {
            printw("The Solver cannot be the Guesser. Try again\n");
        }
    } while (uiSolver <= 0 || uiSolver > (vPlayerNames.size()+1) || uiSolver == uiGuesser);

    printw("Which players passed on the guess?\n");
    listPlayers(vPlayerNames, true);
    std::set<uint32_t> passers;
    uint32_t uiPasser = 0;
    bool bValidPasser = false;
    bool bRealPlayer = true;
    do {
        uiPasser = getValidUserInt();
        bRealPlayer = uiPasser <= vPlayerNames.size();
        bValidPasser = (uiPasser != uiGuesser);
        if (uiPasser != 0) {
            bValidPasser &= (uiPasser != uiSolver);
        }
        if (!bRealPlayer) {
            printw("That was not an option. Try again\n");
            continue;
        } else if (!bValidPasser) {
            printw("The Solver and/or the Guesser can't pass. Try again\n");
        } else if (uiPasser != 0) {
            passers.insert(uiPasser);
        }
    } while (!bRealPlayer || !bValidPasser || uiPasser != 0);

    clear();
    guess.m_sGuesserName =  vPlayerNames[uiGuesser-1];
    printw("The guesser!: ");
    printw(guess.m_sGuesserName.c_str());
    printw("\n");
    if (uiSolver != 0) {
        guess.m_sStopper = vPlayerNames[uiSolver-1];
        printw("The solver!: ");
        printw(guess.m_sStopper.c_str());
    }
    printw("The passers!\n");
    for (uint32_t uiPasser : passers) {
        printw(vPlayerNames[uiPasser - 1].c_str());
        printw("\n");
    }
    refresh();
    getch();
}
