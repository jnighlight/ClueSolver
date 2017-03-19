#pragma once
#include <cstdint>
#include <ncurses.h>

#include "PlayerManager.h"
#include "PlayerStatusForDisplay.h"

class Cli
{
public:
    //TODO: Consolidate enum naming convention
	enum IsUser {
		IS_USER = 0,
		IS_OTHER_PLAYER
	};
	Cli();
	~Cli();
	uint32_t getPlayerCount();
	uint32_t getSinglePlayerInfo(PlayerManager::PlayerStartState &playerStartState, IsUser bIsUser);
	void getPlayerInfo(PlayerManager::PlayerStartStates &playerStartState);
	void getUserCards(uint32_t uiUserCardCount, std::vector<uint32_t> &vCards);
    void getGuess(Guess &guess, std::vector<std::string> vPlayerNames);
    void setStatus(const PlayerStatusForDisplay &playerStatusForDisplay);
    void showEndScreen();
private:
	uint32_t getValidUserInt();
	void getValidUserString(std::string &sInputString, uint32_t uiMaxChars = 20);
	uint32_t pickACard();
    void listPlayers(std::vector<std::string> vPlayerNames, bool bIncludeNone = false);
    void refreshWindows();

    WINDOW *m_textWinBorder;
    WINDOW *m_textWin;
    WINDOW *m_statusWinBorder;
    WINDOW *m_statusWin;
};

