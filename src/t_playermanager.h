#include <gtest/gtest.h>
#include "PlayerManager.h"
#include "Player.h"

class PlayerManagerTest : public ::testing::Test
{
    protected:
        //Called before every test
        PlayerManagerTest()
            :m_sUserPlayerName("userPlayer")
            ,m_sPlayer2Name("player2")
            ,m_sPlayer3Name("player3")
            ,m_uiUserPlayerHandSize(5)
            ,m_uiOtherPlayerHandSize(5)
            ,m_playerManager()
            ,m_playerStartStates(3)
            ,m_userPlayer()
            ,m_otherPlayer()
            ,m_uiPerson(1)
            ,m_uiPlace(2)
            ,m_uiWeapon(3)
            ,m_sGuesserName(m_sPlayer2Name)
            ,m_sStopper(m_sPlayer3Name)
            ,m_svPasses(0)
            ,m_guess(m_uiPerson, m_uiPlace, m_uiWeapon, m_sGuesserName, m_sStopper)
        {
            m_playerStartStates.m_userPlayer.setHandSize(m_uiUserPlayerHandSize);
            m_playerStartStates.m_userPlayer.setName(m_sUserPlayerName);

            m_playerStartStates.m_vPlayerStartStates.at(0).m_sName = m_sPlayer2Name;
            m_playerStartStates.m_vPlayerStartStates.at(0).m_uiHandSize = m_uiOtherPlayerHandSize;
            m_playerStartStates.m_vPlayerStartStates.at(1).m_sName = m_sPlayer3Name;
            m_playerStartStates.m_vPlayerStartStates.at(1).m_uiHandSize = m_uiOtherPlayerHandSize;

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

        bool setContains(const std::set<uint32_t> &set, uint32_t uiToBeFound)
        {
            return set.find(uiToBeFound) != set.end();
        }

        std::string m_sUserPlayerName;
        std::string m_sPlayer2Name;
        std::string m_sPlayer3Name;
        uint32_t m_uiUserPlayerHandSize;
        uint32_t m_uiOtherPlayerHandSize;
        PlayerManager m_playerManager;
        PlayerManager::PlayerStartStates m_playerStartStates;
        Player m_userPlayer;
        Player m_otherPlayer;
        uint32_t m_uiPerson;
        uint32_t m_uiPlace;
        uint32_t m_uiWeapon;
        std::string m_sGuesserName;
        std::string m_sStopper;
        std::vector<std::string> m_svPasses;
        Guess m_guess;
};

TEST_F(PlayerManagerTest, parsePlayerStartStatesWorks)
{
    PlayerManager playerManager;
    playerManager.parsePlayerStartStates(m_playerStartStates);
    EXPECT_EQ(m_uiUserPlayerHandSize, playerManager.m_userPlayer.m_uiHandSize);
    EXPECT_STREQ(m_sUserPlayerName.c_str(), playerManager.m_userPlayer.m_sName.c_str());

    ASSERT_EQ(2, playerManager.m_otherPlayers.size());
    EXPECT_EQ(m_uiOtherPlayerHandSize, playerManager.m_otherPlayers.at(0).m_uiHandSize);
    EXPECT_STREQ(m_sPlayer2Name.c_str(), playerManager.m_otherPlayers.at(0).m_sName.c_str());
};

TEST_F(PlayerManagerTest, addPassedGuessWorks)
{
    m_guess.m_svPasses.push_back(m_sPlayer2Name);
    m_playerManager.addPassedGuess(m_sPlayer2Name, m_guess);

    Player* pPlayer = m_playerManager.getPlayer(m_sPlayer2Name);
    EXPECT_EQ(3, pPlayer->m_setDefinitelyNotOwnedCards.size());
    EXPECT_TRUE(setContains(pPlayer->m_setDefinitelyNotOwnedCards, m_uiPerson));
    EXPECT_TRUE(setContains(pPlayer->m_setDefinitelyNotOwnedCards, m_uiPlace));
    EXPECT_TRUE(setContains(pPlayer->m_setDefinitelyNotOwnedCards, m_uiWeapon));
};

TEST_F(PlayerManagerTest, addMultiplePassedGuessWorks)
{
    uint32_t uiPlayer3Place = 5;
    uint32_t uiPlayer3Weapon = 6;

    Guess passGuess(m_uiPerson, m_uiPlace, m_uiWeapon, "", "");
    passGuess.m_svPasses.push_back(m_sPlayer2Name);

    m_playerManager.addPassedGuess(m_sPlayer2Name, passGuess);

    //Leave one modified as a test
    passGuess.m_svPasses.clear();
    passGuess.m_uiPlace = uiPlayer3Place;
    passGuess.m_uiWeapon = uiPlayer3Weapon;
    passGuess.m_svPasses.push_back(m_sPlayer3Name);
    m_playerManager.addPassedGuess(m_sPlayer3Name, passGuess);

    Player* pPlayer2 = m_playerManager.getPlayer(m_sPlayer2Name);
    ASSERT_TRUE(pPlayer2);
    EXPECT_EQ(3, pPlayer2->m_setDefinitelyNotOwnedCards.size());
    EXPECT_TRUE(setContains(pPlayer2->m_setDefinitelyNotOwnedCards, m_uiPerson));
    EXPECT_TRUE(setContains(pPlayer2->m_setDefinitelyNotOwnedCards, m_uiPlace));
    EXPECT_TRUE(setContains(pPlayer2->m_setDefinitelyNotOwnedCards, m_uiWeapon));

    Player* pPlayer3 = m_playerManager.getPlayer(m_sPlayer3Name);
    ASSERT_TRUE(pPlayer3);
    EXPECT_EQ(3, pPlayer3->m_setDefinitelyNotOwnedCards.size());
    EXPECT_TRUE(setContains(pPlayer3->m_setDefinitelyNotOwnedCards, m_uiPerson));
    EXPECT_TRUE(setContains(pPlayer3->m_setDefinitelyNotOwnedCards, uiPlayer3Place));
    EXPECT_TRUE(setContains(pPlayer3->m_setDefinitelyNotOwnedCards, uiPlayer3Weapon));
};

TEST_F(PlayerManagerTest, addSolvedGuessWorks)
{
    m_playerManager.addSolvedGuess(m_guess);

    Player* pPlayer = m_playerManager.getPlayer(m_sPlayer3Name);
    EXPECT_EQ(0, pPlayer->m_setOwnedCards.size());
    EXPECT_TRUE(pPlayer->m_setOwnedCards.find(m_uiPerson) == pPlayer->m_setOwnedCards.end());
    EXPECT_TRUE(pPlayer->m_setOwnedCards.find(m_uiPlace) == pPlayer->m_setOwnedCards.end());
    EXPECT_TRUE(pPlayer->m_setOwnedCards.find(m_uiWeapon) == pPlayer->m_setOwnedCards.end());

    ASSERT_EQ(1, pPlayer->m_lAnsweredGuesses.size());
    EXPECT_TRUE(pPlayer->m_lAnsweredGuesses.front().m_uiPerson == m_uiPerson);
    EXPECT_TRUE(pPlayer->m_lAnsweredGuesses.front().m_uiPlace == m_uiPlace);
    EXPECT_TRUE(pPlayer->m_lAnsweredGuesses.front().m_uiWeapon == m_uiWeapon);
};

TEST_F(PlayerManagerTest, addMultipleSolvedGuessWorks)
{
    uint32_t uiPlayer3Place = 5;
    uint32_t uiPlayer3Weapon = 6;

    Guess solvedGuess(m_uiPerson, m_uiPlace, m_uiWeapon, m_sPlayer3Name, m_sPlayer2Name);

    m_playerManager.addSolvedGuess(solvedGuess);

    //Leave one modified as a test
    solvedGuess.m_uiPlace = uiPlayer3Place;
    solvedGuess.m_uiWeapon = uiPlayer3Weapon;
    solvedGuess.m_sGuesserName = m_sPlayer2Name;
    solvedGuess.m_sStopper = m_sPlayer3Name;
    m_playerManager.addSolvedGuess(solvedGuess);


    Player* pPlayer2 = m_playerManager.getPlayer(m_sPlayer2Name);
    EXPECT_EQ(0, pPlayer2->m_setOwnedCards.size());
    EXPECT_FALSE(setContains(pPlayer2->m_setOwnedCards, m_uiPerson));
    EXPECT_FALSE(setContains(pPlayer2->m_setOwnedCards, m_uiPlace));
    EXPECT_FALSE(setContains(pPlayer2->m_setOwnedCards, m_uiWeapon));

    ASSERT_EQ(1, pPlayer2->m_lAnsweredGuesses.size());
    EXPECT_TRUE(pPlayer2->m_lAnsweredGuesses.front().m_uiPerson == m_uiPerson);
    EXPECT_TRUE(pPlayer2->m_lAnsweredGuesses.front().m_uiPlace == m_uiPlace);
    EXPECT_TRUE(pPlayer2->m_lAnsweredGuesses.front().m_uiWeapon == m_uiWeapon);

    Player* pPlayer3 = m_playerManager.getPlayer(m_sPlayer3Name);
    EXPECT_EQ(0, pPlayer3->m_setOwnedCards.size());
    EXPECT_FALSE(setContains(pPlayer3->m_setOwnedCards, m_uiPerson));
    EXPECT_FALSE(setContains(pPlayer3->m_setOwnedCards, uiPlayer3Place));
    EXPECT_FALSE(setContains(pPlayer3->m_setOwnedCards, uiPlayer3Weapon));

    ASSERT_EQ(1, pPlayer3->m_lAnsweredGuesses.size());
    EXPECT_TRUE(pPlayer3->m_lAnsweredGuesses.front().m_uiPerson == m_uiPerson);
    EXPECT_TRUE(pPlayer3->m_lAnsweredGuesses.front().m_uiPlace == uiPlayer3Place);
    EXPECT_TRUE(pPlayer3->m_lAnsweredGuesses.front().m_uiWeapon == uiPlayer3Weapon);
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
