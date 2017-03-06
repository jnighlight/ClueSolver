#pragma once
#include <string>
#include <vector>

#include "Guess.h"
#include "Player.h"
#include "PlayerStatusForDisplay.h"

class PlayerManager
{
public:
	struct PlayerStartState {

		PlayerStartState()
		{};

		PlayerStartState(unsigned int uiHandSize, std::string sName)
			:m_uiHandSize(uiHandSize)
			,m_sName(sName)
		{};

		void setHandSize(unsigned int uiHandSize) { m_uiHandSize = uiHandSize; };
		void setName(std::string sName) { m_sName = sName; };

		unsigned int m_uiHandSize;
		std::string m_sName;
	};

	struct PlayerStartStates{
		PlayerStartStates(uint32_t uiNumberOfPlayers)
			:m_vPlayerStartStates(uiNumberOfPlayers - 1)
		{};

		PlayerStartState m_userPlayer;
		uint32_t getUserPlayerHandSize() { return m_userPlayer.m_uiHandSize; };
		std::vector<PlayerStartState> m_vPlayerStartStates;
	};

	PlayerManager() {};
	PlayerManager(const PlayerStartStates &playerStartStates);
	virtual ~PlayerManager();

	void parsePlayerStartStates(const PlayerStartStates &playerStartStates);
	void setUserPlayerCards(const std::vector<uint32_t> &vPlayerCards);
    void updatePlayerState();
    //PlayerStatusDisplay  getPlayerStatusForDisplay();
	void addSolvedGuess(const std::string &sSolver,
				  uint32_t uiPerson,
				  uint32_t uiPlace,
				  uint32_t uiWeapon);
	void addPassedGuess(const std::string &sPasser,
						uint32_t uiPerson,
						uint32_t uiPlace,
						uint32_t uiWeapon);
	bool isOwned(uint32_t uiCard);
	void checkForNewSolutions();
    bool isSolved();
    PlayerStatusForDisplay getPlayerStatusForDisplay();
    std::vector<std::string> getPlayerNames();

	Player m_userPlayer;
	std::vector<Player> m_otherPlayers;

private:
	Player* getPlayer(const std::string &sPlayerName);
	uint32_t removeOwnedCardsFromGuess(std::vector<uint32_t> &vGuessCards);
	uint32_t getOnlyCard(const std::vector<uint32_t> &vGuessCards);
};

