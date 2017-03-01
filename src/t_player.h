#include <gtest/gtest.h>
#include "Player.h"

class PlayerTest : public ::testing::Test
{
    protected:
        //Called before every test
        PlayerTest()
            :m_sName("testName")
            ,m_uiHandSize(5)
            ,m_player(m_sName, m_uiHandSize)
        {
        }

        //Called after every test
        virtual ~PlayerTest()
        {
        }

        std::string m_sName;
        uint32_t m_uiHandSize;
        Player m_player;
};

TEST_F(PlayerTest, playerOwnsGivenCard)
{
    m_player.addCard(10);
    EXPECT_FALSE(m_player.ownsCard(9));
    EXPECT_TRUE(m_player.ownsCard(10));
    EXPECT_FALSE(m_player.ownsCard(11));
};

TEST_F(PlayerTest, vectorOfWrongSizeCausesAddGuessToFail)
{
    std::vector<uint32_t> vCards;
    vCards.push_back(1);
    EXPECT_FALSE(m_player.addGuess(vCards));
    vCards.push_back(2);
    EXPECT_FALSE(m_player.addGuess(vCards));
    vCards.push_back(3);
    vCards.push_back(4);
    EXPECT_FALSE(m_player.addGuess(vCards));
};

TEST_F(PlayerTest, addGuessAddsGuess)
{
    std::vector<uint32_t> vCards;
    vCards.push_back(1);
    vCards.push_back(2);
    vCards.push_back(3);
    EXPECT_TRUE(m_player.addGuess(vCards));
    EXPECT_EQ(1, m_player.m_lAnsweredGuesses.size());
};

class AnsweredGuessTest : public ::testing::Test
{
    protected:
        //Called before every test
        AnsweredGuessTest()
            :m_uiPlace(1)
            ,m_uiPerson(2)
            ,m_uiWeapon(3)
            ,m_answeredGuess(m_uiPlace, m_uiPerson, m_uiWeapon)
            ,m_answeredGuessPlace(m_uiPlace, 0, 0)
            ,m_answeredGuessPerson(0, m_uiPerson, 0)
            ,m_answeredGuessWeapon(0, 0, m_uiWeapon)
            ,m_answeredGuessEmpty(0, 0, 0)
        {
        }

        //Called after every test
        virtual ~AnsweredGuessTest()
        {
        }

        uint32_t m_uiPlace;
        uint32_t m_uiPerson;
        uint32_t m_uiWeapon;
        Player::AnsweredGuess m_answeredGuess;
        Player::AnsweredGuess m_answeredGuessPlace;
        Player::AnsweredGuess m_answeredGuessPerson;
        Player::AnsweredGuess m_answeredGuessWeapon;
        Player::AnsweredGuess m_answeredGuessEmpty;
};

TEST_F(AnsweredGuessTest, freshAnsweredGuessTestIsEmpty)
{
    Player::AnsweredGuess answeredGuess;
    EXPECT_TRUE(answeredGuess.isEmpty());
};

TEST_F(AnsweredGuessTest, emptyPopulatedAnsweredGuessTestIsEmpty)
{
    EXPECT_TRUE(m_answeredGuessEmpty.isEmpty());
}

TEST_F(AnsweredGuessTest, singularlyPopulatedAnsweredGuessTestIsNotEmpty)
{
    EXPECT_FALSE(m_answeredGuessPlace.isEmpty());
    EXPECT_FALSE(m_answeredGuessPerson.isEmpty());
    EXPECT_FALSE(m_answeredGuessWeapon.isEmpty());
};

TEST_F(AnsweredGuessTest, fullyPopulatedAnsweredGuessTestIsNotEmpty)
{
    EXPECT_FALSE(m_answeredGuess.isEmpty());
};

TEST_F(AnsweredGuessTest, guessesWithSingleCardAreSolved)
{
    EXPECT_TRUE(m_answeredGuessPlace.isSolved());
    EXPECT_TRUE(m_answeredGuessPerson.isSolved());
    EXPECT_TRUE(m_answeredGuessWeapon.isSolved());

    EXPECT_FALSE(m_answeredGuess.isSolved());
    EXPECT_FALSE(m_answeredGuessEmpty.isSolved());
};

TEST_F(AnsweredGuessTest, getSolvedWillReturnSolvedCardIfSolved)
{
    EXPECT_EQ(m_uiPlace, m_answeredGuessPlace.getSolved());
    EXPECT_EQ(m_uiPerson, m_answeredGuessPerson.getSolved());
    EXPECT_EQ(m_uiWeapon, m_answeredGuessWeapon.getSolved());

    EXPECT_EQ(0, m_answeredGuess.isSolved());
    EXPECT_EQ(0, m_answeredGuessEmpty.isSolved());
};
