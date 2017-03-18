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

        bool vectorContains(const std::vector<uint32_t> &vVector, uint32_t uiToBeFound)
        {
            bool bContains = false;
            for (uint32_t uiUint32 : vVector)
            {
                if (uiUint32 == uiToBeFound) {
                    bContains = true;
                }
            }
            return bContains;
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
    uint32_t uiPerson = 1;
    uint32_t uiPlace = 2;
    uint32_t uiWeapon = 3;
    m_playerManager.addPassedGuess(m_sOtherPlayerName,
            uiPerson,
            uiPlace,
            uiWeapon);
    Player* pPlayer = m_playerManager.getPlayer(m_sOtherPlayerName);
    EXPECT_EQ(3, pPlayer->m_setDefinitelyNotOwnedCards.size());
    EXPECT_TRUE(pPlayer->m_setDefinitelyNotOwnedCards.find(uiPerson) != m_otherPlayer.m_setDefinitelyNotOwnedCards.end());
    EXPECT_TRUE(pPlayer->m_setDefinitelyNotOwnedCards.find(uiPlace) != m_otherPlayer.m_setDefinitelyNotOwnedCards.end());
    EXPECT_TRUE(pPlayer->m_setDefinitelyNotOwnedCards.find(uiWeapon) != m_otherPlayer.m_setDefinitelyNotOwnedCards.end());
};

TEST_F(PlayerManagerTest, addMultiplePassedGuessWorks)
{
    uint32_t uiOtherPerson = 1;
    uint32_t uiOtherPlace = 2;
    uint32_t uiOtherWeapon = 3;
    //Throw in an overlapping "Not Owned" for testing
    uint32_t uiPlayerPerson = 3;
    uint32_t uiPlayerPlace = 5;
    uint32_t uiPlayerWeapon = 6;
    m_playerManager.addPassedGuess(m_sOtherPlayerName,
            uiOtherPerson,
            uiOtherPlace,
            uiOtherWeapon);
    m_playerManager.addPassedGuess(m_sUserPlayerName,
            uiPlayerPerson,
            uiPlayerPlace,
            uiPlayerWeapon);
    Player* pOtherPlayer = m_playerManager.getPlayer(m_sOtherPlayerName);
    EXPECT_EQ(3, pOtherPlayer->m_setDefinitelyNotOwnedCards.size());
    EXPECT_TRUE(pOtherPlayer->m_setDefinitelyNotOwnedCards.find(uiOtherPerson) != m_otherPlayer.m_setDefinitelyNotOwnedCards.end());
    EXPECT_TRUE(pOtherPlayer->m_setDefinitelyNotOwnedCards.find(uiOtherPlace) != m_otherPlayer.m_setDefinitelyNotOwnedCards.end());
    EXPECT_TRUE(pOtherPlayer->m_setDefinitelyNotOwnedCards.find(uiOtherWeapon) != m_otherPlayer.m_setDefinitelyNotOwnedCards.end());

    Player* pPlayerPlayer = m_playerManager.getPlayer(m_sUserPlayerName);
    EXPECT_EQ(3, pPlayerPlayer->m_setDefinitelyNotOwnedCards.size());
    EXPECT_TRUE(pPlayerPlayer->m_setDefinitelyNotOwnedCards.find(uiPlayerPerson) != m_otherPlayer.m_setDefinitelyNotOwnedCards.end());
    EXPECT_TRUE(pPlayerPlayer->m_setDefinitelyNotOwnedCards.find(uiPlayerPlace) != m_otherPlayer.m_setDefinitelyNotOwnedCards.end());
    EXPECT_TRUE(pPlayerPlayer->m_setDefinitelyNotOwnedCards.find(uiPlayerWeapon) != m_otherPlayer.m_setDefinitelyNotOwnedCards.end());
};

TEST_F(PlayerManagerTest, addSolvedGuessWorks)
{
    uint32_t uiPerson = 1;
    uint32_t uiPlace = 2;
    uint32_t uiWeapon = 3;
    m_playerManager.addSolvedGuess(m_sOtherPlayerName,
            uiPerson,
            uiPlace,
            uiWeapon);
    Player* pPlayer = m_playerManager.getPlayer(m_sOtherPlayerName);
    EXPECT_EQ(0, pPlayer->m_setOwnedCards.size());
    EXPECT_TRUE(pPlayer->m_setOwnedCards.find(uiPerson) == pPlayer->m_setOwnedCards.end());
    EXPECT_TRUE(pPlayer->m_setOwnedCards.find(uiPlace) == pPlayer->m_setOwnedCards.end());
    EXPECT_TRUE(pPlayer->m_setOwnedCards.find(uiWeapon) == pPlayer->m_setOwnedCards.end());

    ASSERT_EQ(1, pPlayer->m_lAnsweredGuesses.size());
    EXPECT_TRUE(pPlayer->m_lAnsweredGuesses.front().m_uiPerson == uiPerson);
    EXPECT_TRUE(pPlayer->m_lAnsweredGuesses.front().m_uiPlace == uiPlace);
    EXPECT_TRUE(pPlayer->m_lAnsweredGuesses.front().m_uiWeapon == uiWeapon);
};

TEST_F(PlayerManagerTest, addMultipleSolvedGuessWorks)
{
    uint32_t uiOtherPerson = 1;
    uint32_t uiOtherPlace = 2;
    uint32_t uiOtherWeapon = 3;
    //Throw in an overlapping "Not Owned" for testing
    uint32_t uiPlayerPerson = 3;
    uint32_t uiPlayerPlace = 5;
    uint32_t uiPlayerWeapon = 6;
    m_playerManager.addSolvedGuess(m_sOtherPlayerName,
            uiOtherPerson,
            uiOtherPlace,
            uiOtherWeapon);
    m_playerManager.addSolvedGuess(m_sUserPlayerName,
            uiPlayerPerson,
            uiPlayerPlace,
            uiPlayerWeapon);
    Player* pOtherPlayer = m_playerManager.getPlayer(m_sOtherPlayerName);
    EXPECT_EQ(0, pOtherPlayer->m_setOwnedCards.size());
    EXPECT_TRUE(pOtherPlayer->m_setOwnedCards.find(uiOtherPerson) == pOtherPlayer->m_setOwnedCards.end());
    EXPECT_TRUE(pOtherPlayer->m_setOwnedCards.find(uiOtherPlace) == pOtherPlayer->m_setOwnedCards.end());
    EXPECT_TRUE(pOtherPlayer->m_setOwnedCards.find(uiOtherWeapon) == pOtherPlayer->m_setOwnedCards.end());

    ASSERT_EQ(1, pOtherPlayer->m_lAnsweredGuesses.size());
    EXPECT_TRUE(pOtherPlayer->m_lAnsweredGuesses.front().m_uiPerson == uiOtherPerson);
    EXPECT_TRUE(pOtherPlayer->m_lAnsweredGuesses.front().m_uiPlace == uiOtherPlace);
    EXPECT_TRUE(pOtherPlayer->m_lAnsweredGuesses.front().m_uiWeapon == uiOtherWeapon);

    Player* pUserPlayer = m_playerManager.getPlayer(m_sUserPlayerName);
    EXPECT_EQ(0, pUserPlayer->m_setOwnedCards.size());
    EXPECT_TRUE(pUserPlayer->m_setOwnedCards.find(uiPlayerPerson) == pUserPlayer->m_setOwnedCards.end());
    EXPECT_TRUE(pUserPlayer->m_setOwnedCards.find(uiPlayerPlace) == pUserPlayer->m_setOwnedCards.end());
    EXPECT_TRUE(pUserPlayer->m_setOwnedCards.find(uiPlayerWeapon) == pUserPlayer->m_setOwnedCards.end());

    ASSERT_EQ(1, pUserPlayer->m_lAnsweredGuesses.size());
    EXPECT_TRUE(pUserPlayer->m_lAnsweredGuesses.front().m_uiPerson == uiPlayerPerson);
    EXPECT_TRUE(pUserPlayer->m_lAnsweredGuesses.front().m_uiPlace == uiPlayerPlace);
    EXPECT_TRUE(pUserPlayer->m_lAnsweredGuesses.front().m_uiWeapon == uiPlayerWeapon);
};

/*
TEST_F(PlayerManagerTest, updatePlayerState_solvingGuessCorrectlyAllocatesUnownedCardsToOthers)
{
};

TEST_F(PlayerManagerTest, updatePlayerState_solvesGuessWithAllButOneCardOwnedByOthers)
{
};

TEST_F(PlayerManagerTest, updatePlayerState_solvesGuessWithAllButOneCardOwnedByOthers)
{
};
*/
