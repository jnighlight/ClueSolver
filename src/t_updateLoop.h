#include <gtest/gtest.h>
#include "PlayerManager.h"
#include "Player.h"

class UpdateLoopTest : public ::testing::Test
{
    protected:
        //Called before every test
        UpdateLoopTest()
            :m_sUserPlayerName("userPlayer")
            ,m_sPlayer2Name("player2")
            ,m_sPlayer3Name("player3")
            ,m_uiUserPlayerHandSize(5)
            ,m_uiOtherPlayerHandSize(5)
            ,m_playerManager()
            ,m_playerStartStates(3)
            ,m_uiPerson(1)
            ,m_uiPlace(2)
            ,m_uiWeapon(3)
            ,m_sGuesserName(m_sPlayer2Name)
            ,m_sStopper(m_sPlayer3Name)
            ,m_svPasses(0)
            ,m_answeredGuess(m_uiPerson, m_uiWeapon, m_uiPlace)
            ,m_pUserPlayer(0)
            ,m_pPlayer2(0)
            ,m_pPlayer3(0)
        {
            m_playerStartStates.m_userPlayer.setHandSize(m_uiUserPlayerHandSize);
            m_playerStartStates.m_userPlayer.setName(m_sUserPlayerName);

            PlayerManager::PlayerStartState player2StartState(m_uiOtherPlayerHandSize,
                                                                  m_sPlayer2Name);
            PlayerManager::PlayerStartState player3StartState(m_uiOtherPlayerHandSize,
                                                                  m_sPlayer3Name);
            m_playerStartStates.m_vPlayerStartStates.push_back(player2StartState);
            m_playerStartStates.m_vPlayerStartStates.push_back(player3StartState);
            m_playerManager.parsePlayerStartStates(m_playerStartStates);
            m_pUserPlayer = m_playerManager.getPlayer(m_sUserPlayerName);
            m_pPlayer2 = m_playerManager.getPlayer(m_sPlayer2Name);
            m_pPlayer3 = m_playerManager.getPlayer(m_sPlayer3Name);
        }

        //Called after every test
        virtual ~UpdateLoopTest()
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
        uint32_t m_uiPerson;
        uint32_t m_uiPlace;
        uint32_t m_uiWeapon;
        std::string m_sGuesserName;
        std::string m_sStopper;
        std::vector<std::string> m_svPasses;
        Player::AnsweredGuess m_answeredGuess;
        Player* m_pUserPlayer;
        Player* m_pPlayer2;
        Player* m_pPlayer3;
};

/*-----------------------------Player::processStoredGuess tests-------------------------*/
TEST_F(UpdateLoopTest, processStoredGuess_returnsTrashIfPlayerOwnsAnyCard)
{
    m_pPlayer2->addCard(m_uiPerson);
    EXPECT_EQ(Player::eTrash, m_pPlayer2->processStoredGuess(m_answeredGuess, &m_playerManager));

    EXPECT_EQ(1, m_pPlayer2->m_setOwnedCards.size());
    EXPECT_EQ(0, m_pPlayer3->m_setOwnedCards.size());
    EXPECT_EQ(0, m_pUserPlayer->m_setOwnedCards.size());
};

TEST_F(UpdateLoopTest, processStoredGuess_zeroesOutCardsOwnedByOthers)
{
    m_pPlayer3->addCard(m_uiPerson);
    EXPECT_EQ(Player::eNotSolved, m_pPlayer2->processStoredGuess(m_answeredGuess, &m_playerManager));
    EXPECT_EQ(0, m_answeredGuess.m_uiPerson);
    EXPECT_NE(0, m_answeredGuess.m_uiPlace);
    EXPECT_NE(0, m_answeredGuess.m_uiWeapon);

    EXPECT_EQ(0, m_pPlayer2->m_setOwnedCards.size());
    EXPECT_EQ(1, m_pPlayer3->m_setOwnedCards.size());
    EXPECT_EQ(0, m_pUserPlayer->m_setOwnedCards.size());
};

TEST_F(UpdateLoopTest, processStoredGuess_zeroesOutCardsThatArentOwnedByPlayer)
{
    m_pPlayer2->m_setDefinitelyNotOwnedCards.insert(m_uiPlace);
    EXPECT_EQ(Player::eNotSolved, m_pPlayer2->processStoredGuess(m_answeredGuess, &m_playerManager));
    EXPECT_EQ(0, m_answeredGuess.m_uiPlace);
    EXPECT_NE(0, m_answeredGuess.m_uiPerson);
    EXPECT_NE(0, m_answeredGuess.m_uiWeapon);

    EXPECT_EQ(0, m_pPlayer2->m_setOwnedCards.size());
    EXPECT_EQ(0, m_pPlayer3->m_setOwnedCards.size());
    EXPECT_EQ(0, m_pUserPlayer->m_setOwnedCards.size());
};

TEST_F(UpdateLoopTest, processStoredGuess_addsSolvedCardToOwnedOfSolvingPlayer)
{
    m_pPlayer2->m_setDefinitelyNotOwnedCards.insert(m_uiPlace);
    m_pPlayer3->addCard(m_uiPerson);
    EXPECT_EQ(Player::eSolved, m_pPlayer2->processStoredGuess(m_answeredGuess, &m_playerManager));
    EXPECT_EQ(0, m_answeredGuess.m_uiPlace);
    EXPECT_EQ(0, m_answeredGuess.m_uiPerson);
    EXPECT_NE(0, m_answeredGuess.m_uiWeapon);
    EXPECT_TRUE(m_pPlayer2->ownsCard(m_uiWeapon));

    EXPECT_EQ(1, m_pPlayer2->m_setOwnedCards.size());
    EXPECT_EQ(1, m_pPlayer3->m_setOwnedCards.size());
    EXPECT_EQ(0, m_pUserPlayer->m_setOwnedCards.size());
};

TEST_F(UpdateLoopTest, processStoredGuess_returnsTrashIfCardEndsUpEmpty)
{
    m_answeredGuess.m_uiPlace = 0;
    m_answeredGuess.m_uiPerson = 0;
    m_answeredGuess.m_uiWeapon = 0;
    EXPECT_EQ(Player::eTrash, m_pPlayer2->processStoredGuess(m_answeredGuess, &m_playerManager));

    EXPECT_EQ(0, m_pPlayer2->m_setOwnedCards.size());
    EXPECT_EQ(0, m_pPlayer3->m_setOwnedCards.size());
    EXPECT_EQ(0, m_pUserPlayer->m_setOwnedCards.size());
};

/*-----------------------------Player::checkForSolutions tests-------------------------*/
TEST_F(UpdateLoopTest, checkForSolutions_solvesAllSolveableGuesses)
{
    m_pPlayer2->m_setDefinitelyNotOwnedCards.insert(m_uiPlace);
    m_pPlayer2->m_setDefinitelyNotOwnedCards.insert(m_uiPerson);
    m_pPlayer2->addGuess(m_answeredGuess);

    Player::AnsweredGuess secondSolveableGuess(4,5,6);
    m_pPlayer2->m_setDefinitelyNotOwnedCards.insert(4);
    m_pPlayer2->m_setDefinitelyNotOwnedCards.insert(5);
    m_pPlayer2->addGuess(secondSolveableGuess);

    EXPECT_TRUE(m_pPlayer2->checkForSolutions(&m_playerManager));

    EXPECT_EQ(2, m_pPlayer2->m_setOwnedCards.size());
    EXPECT_TRUE(setContains(m_pPlayer2->m_setOwnedCards, m_uiWeapon));
    EXPECT_TRUE(setContains(m_pPlayer2->m_setOwnedCards, 6));

    EXPECT_EQ(0, m_pPlayer3->m_setOwnedCards.size());
    EXPECT_EQ(0, m_pUserPlayer->m_setOwnedCards.size());
};

TEST_F(UpdateLoopTest, checkForSolutions_removesAllSolvedAndTrashGuesses)
{
    m_pPlayer3->addCard(10);
    m_pPlayer3->addCard(11);
    m_pPlayer3->addCard(12);

    m_pPlayer2->m_setDefinitelyNotOwnedCards.insert(m_uiPlace);
    m_pPlayer2->m_setDefinitelyNotOwnedCards.insert(m_uiPerson);
    m_pPlayer2->addGuess(m_answeredGuess);

    Player::AnsweredGuess secondSolveableGuess(4,5,6);
    m_pPlayer2->m_setDefinitelyNotOwnedCards.insert(4);
    m_pPlayer2->m_setDefinitelyNotOwnedCards.insert(5);
    m_pPlayer2->addGuess(secondSolveableGuess);

    Player::AnsweredGuess trashGuess(0,0,0);
    m_pPlayer2->addGuess(trashGuess);

    //This guess is null and void, because it contains cards we already own
    Player::AnsweredGuess cantAnswerGuess(m_uiWeapon,6,0);
    m_pPlayer2->addGuess(cantAnswerGuess);

    //This guess only contains cards another player owns
    Player::AnsweredGuess otherPlayersCardsGuess(10,11,12);
    m_pPlayer2->addGuess(otherPlayersCardsGuess);

    EXPECT_TRUE(m_pPlayer2->checkForSolutions(&m_playerManager));
    EXPECT_EQ(0, m_pPlayer2->m_lAnsweredGuesses.size());

    EXPECT_EQ(2, m_pPlayer2->m_setOwnedCards.size());
    EXPECT_TRUE(setContains(m_pPlayer2->m_setOwnedCards, m_uiWeapon));
    EXPECT_TRUE(setContains(m_pPlayer2->m_setOwnedCards, 6));

    EXPECT_EQ(3, m_pPlayer3->m_setOwnedCards.size());
    EXPECT_EQ(0, m_pUserPlayer->m_setOwnedCards.size());
};

TEST_F(UpdateLoopTest, checkForSolutions_returnsTrueOnlyIfSolutionsAreFound)
{
    m_pPlayer3->addCard(10);
    m_pPlayer3->addCard(11);
    m_pPlayer3->addCard(12);

    m_pPlayer2->m_setDefinitelyNotOwnedCards.insert(m_uiPlace);
    m_pPlayer2->m_setDefinitelyNotOwnedCards.insert(m_uiPerson);
    m_pPlayer2->addGuess(m_answeredGuess);

    Player::AnsweredGuess secondSolveableGuess(4,5,6);
    m_pPlayer2->m_setDefinitelyNotOwnedCards.insert(4);
    m_pPlayer2->m_setDefinitelyNotOwnedCards.insert(5);
    m_pPlayer2->addGuess(secondSolveableGuess);

    //Parseing through options that cause solutions
    EXPECT_TRUE(m_pPlayer2->checkForSolutions(&m_playerManager));
    EXPECT_EQ(0, m_pPlayer2->m_lAnsweredGuesses.size());

    EXPECT_EQ(2, m_pPlayer2->m_setOwnedCards.size());
    EXPECT_TRUE(setContains(m_pPlayer2->m_setOwnedCards, m_uiWeapon));
    EXPECT_TRUE(setContains(m_pPlayer2->m_setOwnedCards, 6));

    EXPECT_EQ(3, m_pPlayer3->m_setOwnedCards.size());
    EXPECT_EQ(0, m_pUserPlayer->m_setOwnedCards.size());

    Player::AnsweredGuess trashGuess(0,0,0);
    m_pPlayer2->addGuess(trashGuess);

    //This guess is null and void, because it contains cards we already own
    Player::AnsweredGuess cantAnswerGuess(m_uiWeapon,6,0);
    m_pPlayer2->addGuess(cantAnswerGuess);

    //This guess only contains cards another player owns
    Player::AnsweredGuess otherPlayersCardsGuess(10,11,12);
    m_pPlayer2->addGuess(otherPlayersCardsGuess);

    //Parseing through options that DONT cause solutions
    EXPECT_FALSE(m_pPlayer2->checkForSolutions(&m_playerManager));
    EXPECT_EQ(0, m_pPlayer2->m_lAnsweredGuesses.size());

    EXPECT_EQ(2, m_pPlayer2->m_setOwnedCards.size());
    EXPECT_TRUE(setContains(m_pPlayer2->m_setOwnedCards, m_uiWeapon));
    EXPECT_TRUE(setContains(m_pPlayer2->m_setOwnedCards, 6));

    EXPECT_EQ(3, m_pPlayer3->m_setOwnedCards.size());
    EXPECT_EQ(0, m_pUserPlayer->m_setOwnedCards.size());
};

TEST_F(UpdateLoopTest, checkForSolutions_leavesUnsolvedGuessesAlone)
{
    m_pPlayer3->addCard(10);
    m_pPlayer3->addCard(11);
    m_pPlayer3->addCard(12);

    m_pPlayer2->m_setDefinitelyNotOwnedCards.insert(m_uiPlace);
    m_pPlayer2->m_setDefinitelyNotOwnedCards.insert(m_uiPerson);
    m_pPlayer2->addGuess(m_answeredGuess);

    Player::AnsweredGuess unsolvedGuessWithChangedValue(10,20,30);
    m_pPlayer2->addGuess(unsolvedGuessWithChangedValue);

    //This guess only contains cards another player owns
    Player::AnsweredGuess otherPlayersCardsGuess(10,11,12);
    m_pPlayer2->addGuess(otherPlayersCardsGuess);

    EXPECT_TRUE(m_pPlayer2->checkForSolutions(&m_playerManager));
    EXPECT_EQ(1, m_pPlayer2->m_lAnsweredGuesses.size());

    EXPECT_EQ(0, m_pPlayer2->m_lAnsweredGuesses.front().m_uiPerson);
    EXPECT_EQ(20, m_pPlayer2->m_lAnsweredGuesses.front().m_uiWeapon);
    EXPECT_EQ(30, m_pPlayer2->m_lAnsweredGuesses.front().m_uiPlace);

    EXPECT_EQ(1, m_pPlayer2->m_setOwnedCards.size());
    EXPECT_TRUE(setContains(m_pPlayer2->m_setOwnedCards, m_uiWeapon));

    EXPECT_EQ(3, m_pPlayer3->m_setOwnedCards.size());
    EXPECT_EQ(0, m_pUserPlayer->m_setOwnedCards.size());
};
