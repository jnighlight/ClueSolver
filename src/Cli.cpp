#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <stdexcept>

#include "Cli.h"
#include "Rules.h"

Cli::Cli()
    :m_textWinBorder(0)
    ,m_textWin(0)
    ,m_statusWinBorder(0)
    ,m_statusWin(0)
{
    initscr();
    //This looks a little complicated. Making 2 windows per display section: One for the border
    //  to be displayed, and a slightly smaller one inside that border for actual text
    int x;
    int y;
    getmaxyx(stdscr, y, x);
    int iScreenWid = (x/2);

    m_textWinBorder =     newwin(y,   iScreenWid,   0, 0);
    m_textWin =           newwin(y-2, iScreenWid-2, 1, 1);

    m_statusWinBorder =   newwin(y,   iScreenWid,   0, iScreenWid);
    m_statusWin =         newwin(y-2, iScreenWid-2, 1, (iScreenWid + 1));

    box(m_textWinBorder, 0, 0);
    box(m_statusWinBorder, 0, 0);

    wrefresh(m_textWin);
    wrefresh(m_statusWin);
    wrefresh(m_textWinBorder);
    wrefresh(m_statusWinBorder);
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
	std::set<uint32_t> claimedCards;
	for (uint32_t i = 0; i < uiUserCardCount; i++)
	{
		uint32_t uiSelection = 0;
		bool bAccepted = true;
		do {
            wclear(m_textWin);
            if (claimedCards.size()) {
                wprintw(m_textWin, "You've Picked: ");
                for (uint32_t uiCard : claimedCards) {
                    wprintw(m_textWin, "%u, ", uiCard);
                }
                wprintw(m_textWin, "\n");
            }
            wprintw(m_textWin, "Pick the cards in your hand!\n");
			uiSelection = pickACard();

			//Don't want any duplicate numbers
			bAccepted = (claimedCards.find(uiSelection) == claimedCards.end());
			if (!bAccepted) {
                wprintw(m_textWin, "I think you already have claimed that card. Try again...\n");
                wprintw(m_textWin, "(Hit Any Key to continue)\n");
                refreshWindows();
                wgetch(m_textWin);
			}
		} while (!bAccepted);
		claimedCards.insert(uiSelection);
	}
	for (uint32_t uiClaimedCard : claimedCards)
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
    //This looks a little confusing. Basically, I want the input prompt to show up in the text
    //  window below the last line of text.
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
        getValidUserString(sUserInput, 4); //we'll never need a number over 9999. No overflow
        try {
            //Technically, if they input a negative, it'll wrap here. Since we mostly use this
            //  as a selector or with a limits check, that's fine. They'll just be out of bounds
            //  or select an invalid option.
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
    wprintw(m_textWin, "How many people are playing tonight? : ");
    refreshWindows();
    std::string sResponse;
	uint32_t uiPlayerCount = getValidUserInt();
	while (!Rules::validPlayerCount(uiPlayerCount)) {
        wprintw(m_textWin, "Clue is played with %u to %u players. Try again:",unsigned(Rules::MIN_PLAYERS), unsigned(Rules::MAX_PLAYERS));
        refreshWindows();
		uiPlayerCount = getValidUserInt();
	}
	return uiPlayerCount;
}

uint32_t Cli::getSinglePlayerInfo(PlayerManager::PlayerStartState &playerStartState, eIsUser bIsUser)
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
		uint32_t uiCardCount = getSinglePlayerInfo(playerStartState.m_userPlayer, IS_USER);
		for (PlayerManager::PlayerStartState &playerToPopulate : playerStartState.m_vPlayerStartStates) {
			uiCardCount += getSinglePlayerInfo(playerToPopulate, IS_OTHER_PLAYER);
		}
		bAcceptable = uiCardCount == (Rules::TOTAL_CARDS - 3);
		if (!bAcceptable) {
			wprintw(m_textWin, "I think you made a mistake in your math there...\n");
			wprintw(m_textWin, "You said that, amongst hands, there were %u cards\n", unsigned(uiCardCount));
			wprintw(m_textWin, "There are %u cards in this game (3 of which should be hidden, not in any hands).\n", Rules::TOTAL_CARDS);
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
		wprintw(m_textWin, "%u. ",uiSelectionCounter);
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
                wprintw(m_statusWin, "%u. %s\n", (i + 1), playerDisplay.m_vOwnedCardNames[i].c_str());
            } else {
                wprintw(m_statusWin, "%u. -\n", (i + 1));
            }
        }
        wprintw(m_statusWin, "Cards not owned by player:\n");
        uint32_t uiNotOwnedCardCount = 0;
        uint32_t uiPerLine = 3;
        for (std::string sCardName : playerDisplay.m_vDefinitelyNotOwnedCardNames) {
            wprintw(m_statusWin, sCardName.c_str());
            ++uiNotOwnedCardCount;
            if (uiNotOwnedCardCount % uiPerLine == 0
                    && uiNotOwnedCardCount >= playerDisplay.m_vDefinitelyNotOwnedCardNames.size()) {
                wprintw(m_statusWin, "\n");
            } else {
                wprintw(m_statusWin, ", ");
            }
        }
        wprintw(m_statusWin, "\n");
        wprintw(m_statusWin, "Stopped Guesses:\n");
        for (const PlayerStatusForDisplay::GuessDisplay &guessDisplay : playerDisplay.m_vAnsweredGuesses) {
            std::string sPersonCard = Rules::getCardName(guessDisplay.m_uiPersonCard);
            std::string sWeaponCard = Rules::getCardName(guessDisplay.m_uiWeaponCard);
            std::string sPlaceCard = Rules::getCardName(guessDisplay.m_uiPlaceCard);
            wprintw(m_statusWin, "Person: %u - %s, "
                    ,guessDisplay.m_uiPersonCard
                    ,sPersonCard.c_str());
            wprintw(m_statusWin, "Weapon: %u - %s, "
                    ,guessDisplay.m_uiWeaponCard
                    ,sWeaponCard.c_str());
            wprintw(m_statusWin, "Place: %u - %s"
                    ,guessDisplay.m_uiPlaceCard
                    ,sPlaceCard.c_str());
            wprintw(m_statusWin, "\n");
        }
        wprintw(m_statusWin, "\n");
    }
    wprintw(m_statusWin, "\n");
    if (playerStatusForDisplay.hasSolvedValue()) {
        wprintw(m_statusWin, "SOLVED CARDS: \n");
        if (playerStatusForDisplay.getSolvedPerson() != 0) {
            wprintw(m_statusWin, "Person: %s\n",
                    Rules::getCardName(playerStatusForDisplay.getSolvedPerson()).c_str());
        }
        if (playerStatusForDisplay.getSolvedPlace() != 0) {
            wprintw(m_statusWin, "Place: %s\n",
                    Rules::getCardName(playerStatusForDisplay.getSolvedPlace()).c_str());
        }
        if (playerStatusForDisplay.getSolvedWeapon() != 0) {
            wprintw(m_statusWin, "Weapon: %s\n",
                    Rules::getCardName(playerStatusForDisplay.getSolvedWeapon()).c_str());
        }
    }
    refreshWindows();
}

//TODO: This is by far the nastiest part of this code base. This would be a place for improvement 
void Cli::getGuess(Guess &guess, std::vector<std::string> vPlayerNames)
{
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

    bool bUserGuessed = (uiGuesser == 1);

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
    bool bValidNumber = true;
    do {
        bValidNumber = true;
        uiSolver = getValidUserInt();
        if (uiSolver > vPlayerNames.size()) {
            wprintw(m_textWin, "That was not an option. Try again\n");
            bValidNumber = false;
        } else if (uiSolver == uiGuesser) {
            wprintw(m_textWin, "The Solver cannot be the Guesser. Try again\n");
            bValidNumber = false;
        }
    } while (!bValidNumber);

    wclear(m_textWin);
    wprintw(m_textWin, "Guesser: %s\n", guess.m_sGuesserName.c_str());
    guess.m_bUserGuess = false;
    guess.m_uiUserAnswerRecieved = 0;
    if (uiSolver != 0) {
        guess.m_sStopper = vPlayerNames.at(uiSolver-1);
        wprintw(m_textWin, "Solver: %s\n", guess.m_sStopper.c_str());

        if (bUserGuessed) {
            bool bAcceptable = false;
            wprintw(m_textWin, "What card did they show you?\n");
            wprintw(m_textWin, "1. %s\n", Rules::getCardName(guess.m_uiPerson).c_str());
            wprintw(m_textWin, "2. %s\n", Rules::getCardName(guess.m_uiPlace).c_str());
            wprintw(m_textWin, "3. %s\n", Rules::getCardName(guess.m_uiWeapon).c_str());
            wprintw(m_textWin, "Enter 1, 2, or 3: ");
            uint32_t uiAnsweredCard = 0;
            do {
                uiAnsweredCard = getValidUserInt();
                if (uiAnsweredCard >= 1 && uiAnsweredCard <= 3) {
                    bAcceptable = true;
                } else {
                    wprintw(m_textWin, "\nThat was not a 1, 2 or 3. Try again: ");
                }
            } while (!bAcceptable);
            guess.m_bUserGuess = true;
            switch(uiAnsweredCard) {
                case 1 :
                    guess.m_uiUserAnswerRecieved = guess.m_uiPerson;
                    break;
                case 2 :
                    guess.m_uiUserAnswerRecieved = guess.m_uiPlace;
                    break;
                case 3 :
                    guess.m_uiUserAnswerRecieved = guess.m_uiWeapon;
                    break;
                default:
                    wprintw(m_textWin, "ERROR: Answer recieved was not of any type!\n");
                    break;
            }
        }
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

    for (uint32_t uiPasser : passers) {
        guess.m_svPasses.push_back(vPlayerNames[uiPasser - 1]);
    }

    refreshWindows();
}

void Cli::showEndScreen()
{
    wprintw(m_textWin, "GAME HAS BEEN WON! Congratulations!\n");
    wprintw(m_textWin, "Game state at end of game can be seen in the status window\n");
    wprintw(m_textWin, "Press Any Key to end. Thanks for Playing!\n");
    refreshWindows();
    wgetch(m_textWin);
}
