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
			:m_vPlayerStartStates()
		{
            for (uint32_t i = 0; i < uiNumberOfPlayers-1; i++)
            {
                PlayerStartState otherPlayer;
                m_vPlayerStartStates.push_back(otherPlayer);
            }
        };

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
    void addSolvedGuess(const Guess &guess);
    void addPassedGuess(const std::string &sPasser, const Guess &guess);
	bool isOwned(uint32_t uiCard) const;
	void checkForNewSolutions();
    bool isSolved();
    PlayerStatusForDisplay getPlayerStatusForDisplay();
    std::vector<std::string> getPlayerNames();

	Player m_userPlayer;
	std::vector<Player> m_otherPlayers;
	Player* getPlayer(const std::string &sPlayerName);

private:
    uint32_t removeOwnedCardsFromGuess(Player::AnsweredGuess &answeredGuess) const;
	uint32_t getOnlyCard(const std::vector<uint32_t> &vGuessCards);
};

