#include <gtest/gtest.h>
#include "PlayerManager.h"
#include "Player.h"

class PlayerManagerTest : public ::testing::Test
{
    protected:
        //Called before every test
        PlayerManagerTest()
            :m_sUserPlayerName("testName")
            ,m_sOtherPlayerName("otherTestName")
            ,m_uiUserPlayerHandSize(5)
            ,m_uiOtherPlayerHandSize(5)
            ,m_playerManager()
            ,m_playerStartStates(2)
            ,m_userPlayer()
            ,m_otherPlayer()
            ,m_guess()
        {
            m_playerStartStates.m_userPlayer.setHandSize(m_uiUserPlayerHandSize);
            m_playerStartStates.m_userPlayer.setName(m_sUserPlayerName);
            PlayerManager::PlayerStartState otherPlayerStartState(m_uiOtherPlayerHandSize,
                                                                  m_sOtherPlayerName);
            m_playerStartStates.m_vPlayerStartStates.push_back(otherPlayerStartState);
            m_playerManager.parsePlayerStartStates(m_playerStartStates);
        }

        //Called after every test
        virtual ~PlayerManagerTest()
        {
        }

        std::string m_sUserPlayerName;
        std::string m_sOtherPlayerName;
        uint32_t m_uiUserPlayerHandSize;
        uint32_t m_uiOtherPlayerHandSize;
        PlayerManager m_playerManager;
        PlayerManager::PlayerStartStates m_playerStartStates;
        Player m_userPlayer;
        Player m_otherPlayer;
        Guess m_guess;
};

TEST_F(PlayerManagerTest, parsePlayerStartStatesWorks)
{
    PlayerManager playerManager;
    playerManager.parsePlayerStartStates(m_playerStartStates);
    EXPECT_EQ(m_uiUserPlayerHandSize, playerManager.m_userPlayer.m_uiHandSize);
    EXPECT_STREQ(m_sUserPlayerName.c_str(), playerManager.m_userPlayer.m_sName.c_str());

    ASSERT_EQ(1, playerManager.m_otherPlayers.size());
    EXPECT_EQ(m_uiOtherPlayerHandSize, playerManager.m_otherPlayers.at(0).m_uiHandSize);
    EXPECT_STREQ(m_sOtherPlayerName.c_str(), playerManager.m_otherPlayers.at(0).m_sName.c_str());
};

TEST_F(PlayerManagerTest, addPassedGuessWorks)
{
};

TEST_F(PlayerManagerTest, addMultiplePassedGuessWorks)
{
};

TEST_F(PlayerManagerTest, addSolvedGuessWorks)
{
};

TEST_F(PlayerManagerTest, addMultipleSolvedGuessWorks)
{
};
