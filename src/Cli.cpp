#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <set>

#include "Cli.h"
#include "Rules.h"

Cli::Cli()
    :m_textWinBorder(0)
    ,m_textWin(0)
    ,m_statusWinBorder(0)
    ,m_statusWin(0)
{
    initscr();
    int x;
    int y;
    getmaxyx(stdscr, y, x);
    int screenWid = (x/2);

    m_textWinBorder =     newwin(y,   screenWid,   0, 0);
    m_textWin =           newwin(y-2, screenWid-2, 1, 1);

    m_statusWinBorder =   newwin(y,   screenWid,   0, screenWid);
    m_statusWin =         newwin(y-2, screenWid-2, 1, (screenWid + 1));

    box(m_textWinBorder, 0, 0);
    box(m_statusWinBorder, 0, 0);

    wrefresh(m_textWin);
    wrefresh(m_statusWin);
    wrefresh(m_textWinBorder);
    wrefresh(m_statusWinBorder);
    /*
    wprintw(m_statusWin, "Height: %d, Width: %d", y, x);
    wprintw(m_textWin, "Height: %d, Width: %d", y, x);
    wrefresh(m_textWin);
    wrefresh(m_statusWin);
    */
}

Cli::~Cli()
{
    delwin(m_textWin);
    delwin(m_statusWin);
    delwin(m_textWinBorder);
    delwin(m_statusWinBorder);
    endwin();
}

void Cli::refreshWindows()
{
    wrefresh(m_textWinBorder);
    wrefresh(m_statusWinBorder);
    wrefresh(m_statusWin);
    wrefresh(m_textWin);
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
            wclear(m_textWin);
            if (vClaimedCards.size()) {
                wprintw(m_textWin, "You've Picked: ");
                for (uint32_t uiCard : vClaimedCards) {
                    wprintw(m_textWin, "%d, ", uiCard);
                }
                wprintw(m_textWin, "\n");
            }
            wprintw(m_textWin, "Pick the cards in your hand!\n");
			uiSelection = pickACard();

			//Don't want any duplicate numbers
			bAccepted = (vClaimedCards.find(uiSelection) == vClaimedCards.end());
			if (!bAccepted) {
                wprintw(m_textWin, "I think you already have claimed that card. Try again...\n");
                wprintw(m_textWin, "(Hit Any Key to continue)\n");
                refreshWindows();
                wgetch(m_textWin);
			}
		} while (!bAccepted);
		vClaimedCards.insert(uiSelection);
	}
	for (uint32_t uiClaimedCard : vClaimedCards)
	{
		vCards.push_back(uiClaimedCard);
	}
    wprintw(m_textWin, "Selecting Cards finished!\n");
    refreshWindows();
}

uint32_t Cli::pickACard()
{
	uint32_t uiSelection = 0;
	bool bGoodCard = true;
    std::string sCardList = Rules::listCards();
    wclear(m_statusWin);
    wprintw(m_statusWin, sCardList.c_str());
	do {
		wprintw(m_textWin, "Enter the number of the card:");
        refreshWindows();
		uiSelection = getValidUserInt();
		bGoodCard = Rules::isAValidCard(uiSelection);
		if (!bGoodCard) {
            wprintw(m_textWin, "That's not an acceptable card. Try again.");
            wprintw(m_textWin, "(Hit any key to continue)\n");
            refreshWindows();
            wgetch(m_textWin);
		}
	} while (!bGoodCard);
    wclear(m_statusWin);
    refreshWindows();
	return uiSelection;
}

void Cli::getValidUserString(std::string &sInputString, uint32_t uiMaxChars /* = 20 */)
{
    int x;
    int y;
    getyx(m_textWin, y, x);
    wmove(m_textWin, y, x+1);
    refreshWindows();

    sInputString.clear();
    char str[uiMaxChars + 1];
    wgetnstr(m_textWin, str, uiMaxChars);
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
            wprintw(m_textWin, "That wasn't a number. Try again: ");
        }
	} while (!bGoodNumber);
	return uiUserInt;
}

uint32_t Cli::getPlayerCount()
{
    //wprintw(m_statusWin, "Height: , Width: ");
    mvwprintw(m_textWin, 1, 1, "How many people are playing tonight? : ");
    refreshWindows();
    std::string sResponse;
	uint32_t uiPlayerCount = getValidUserInt();
	while (!Rules::validPlayerCount(uiPlayerCount)) {
        wprintw(m_textWin, "Clue is played with %d to %d players. Try again:",unsigned(Rules::MIN_PLAYERS), unsigned(Rules::MAX_PLAYERS));
        refreshWindows();
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

	wprintw(m_textWin, sNameQuestion.c_str());
	getValidUserString(sName);
	wprintw(m_textWin, sCardCountQuestion.c_str());
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
			wprintw(m_textWin, "I think you made a mistake in your math there...\n");
			wprintw(m_textWin, "You said that, amongst hands, there were %d cards\n", unsigned(uiCardCount));
			wprintw(m_textWin, "There are %d cards in this game (3 of which should be hidden, not in any hands).\n", Rules::TOTAL_CARDS);
			wprintw(m_textWin, "Let's take it from the top.\n");
            wgetch(m_textWin);
            wclear(m_textWin);
            refreshWindows();
		} else {
            wclear(m_textWin);
			wprintw(m_textWin, "Alright! That's everyone!\n");
            refreshWindows();
		}
	} while (!bAcceptable);
}

void Cli::listPlayers(std::vector<std::string> vPlayerNames, bool bIncludeNone /* = false */)
{
	uint32_t uiSelectionCounter = 1;
	wprintw(m_textWin, "Players:\n");
    if (bIncludeNone) {
		wprintw(m_textWin, "0. None");
		wprintw(m_textWin, " ");
    }
	for (std::string sPerson: vPlayerNames)
	{
		wprintw(m_textWin, "%d. ",uiSelectionCounter);
        wprintw(m_textWin, sPerson.c_str());
		wprintw(m_textWin, " ");
		++uiSelectionCounter;
	}
	wprintw(m_textWin, "\n");
}

void Cli::setStatus(const PlayerStatusForDisplay &playerStatusForDisplay)
{
    wclear(m_statusWin);
    for (const PlayerStatusForDisplay::PlayerDisplay &playerDisplay : playerStatusForDisplay.m_vPlayerDisplays) {
        wprintw(m_statusWin, "Player: %s", playerDisplay.m_sName.c_str());
        wprintw(m_statusWin, "\n");
        for (uint32_t i = 0; i < playerDisplay.m_uiCardCount; i++)
        {
            if (i < playerDisplay.m_vOwnedCardNames.size()) {
                wprintw(m_statusWin, "%d. %s\n", (i + 1), playerDisplay.m_vOwnedCardNames[i].c_str());
            } else {
                wprintw(m_statusWin, "%d. -\n", (i + 1));
            }
        }
        wprintw(m_statusWin, "\n");
    }
    refreshWindows();
}

//TODO: Try to decouple the UI with the logic for better unit testing
void Cli::getGuess(Guess &guess, std::vector<std::string> vPlayerNames)
{
    //wclear(m_textWin);
    wclear(m_textWin);
    wprintw(m_textWin, "Which player made the guess?\n");
    listPlayers(vPlayerNames);
    refreshWindows();
    uint32_t uiGuesser = 0;
    do {
        uiGuesser = getValidUserInt();
        if (uiGuesser <= 0 || uiGuesser > vPlayerNames.size()) {
            wprintw(m_textWin, "That was not an option. Try again\n");
        }
    } while (uiGuesser <= 0 || uiGuesser > vPlayerNames.size());
    guess.m_sGuesserName =  vPlayerNames[uiGuesser-1];

    std::vector<uint32_t> vGuessCards = { 0, 0, 0};
    bool bGotAcceptableCards = false;
    do {
        wclear(m_textWin);
        wprintw(m_textWin, "Guesser: %s\n", guess.m_sGuesserName.c_str());
        wprintw(m_textWin, "What cards were in the guess?\n");
        refreshWindows();
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
            wprintw(m_textWin, "You must select one of each type of card. Try again\n");
            wgetch(m_textWin);
        }
    } while (!bGotAcceptableCards);

    wclear(m_textWin);
    wprintw(m_textWin, "Guesser: %s\n", guess.m_sGuesserName.c_str());
    wprintw(m_textWin, "Which player solved the guess?\n");
    listPlayers(vPlayerNames, true);
    uint32_t uiSolver = 0;
    do {
        uiSolver = getValidUserInt();
        if (uiSolver > vPlayerNames.size()) {
            wprintw(m_textWin, "That was not an option. Try again\n");
        } else if (uiSolver == uiGuesser) {
            wprintw(m_textWin, "The Solver cannot be the Guesser. Try again\n");
        }
    } while (uiSolver < 0 || uiSolver > (vPlayerNames.size()+1) || uiSolver == uiGuesser);

    wclear(m_textWin);
    wprintw(m_textWin, "Guesser: %s\n", guess.m_sGuesserName.c_str());
    if (uiSolver != 0) {
        guess.m_sStopper = vPlayerNames[uiSolver-1];
        wprintw(m_textWin, "Solver: %s\n", guess.m_sStopper.c_str());
    }
    wprintw(m_textWin, "Which players passed on the guess?\n");
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
            wprintw(m_textWin, "That was not an option. Try again\n");
            continue;
        } else if (!bValidPasser) {
            wprintw(m_textWin, "The Solver and/or the Guesser can't pass. Try again\n");
        } else if (uiPasser != 0) {
            passers.insert(uiPasser);
        }
    } while (!bRealPlayer || !bValidPasser || uiPasser != 0);

    /* --------- These really should be setters and getters -----------*/
    wprintw(m_textWin, "The guesser!: ");
    wprintw(m_textWin, guess.m_sGuesserName.c_str());
    wprintw(m_textWin, "\n");
    if (uiSolver != 0) {
        guess.m_sStopper = vPlayerNames[uiSolver-1];
        wprintw(m_textWin, "The solver!: ");
        wprintw(m_textWin, guess.m_sStopper.c_str());
    }
    wprintw(m_textWin, "The passers!\n");
    for (uint32_t uiPasser : passers) {
        guess.m_svPasses.push_back(vPlayerNames[uiPasser - 1]);
        wprintw(m_textWin, vPlayerNames[uiPasser - 1].c_str());
        wprintw(m_textWin, "\n");
    }
    refreshWindows();
    wgetch(m_textWin);
}
