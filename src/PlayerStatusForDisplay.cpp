#include "PlayerStatusForDisplay.h"

void PlayerStatusForDisplay::addPlayer(const Player &player)
{
    PlayerDisplay playerDisplay;
    playerDisplay.m_sName = player.m_sName;
    playerDisplay.m_uiCardCount = player.m_uiHandSize;

    addPlayerCards(player, playerDisplay);
    addPlayerGuesses(player, playerDisplay);

    m_vPlayerDisplays.push_back(playerDisplay);
}

//This method should be in player display
void PlayerStatusForDisplay::addPlayerCards(const Player &player, PlayerDisplay &playerDisplay)
{
    for (uint32_t uiCard : player.m_vOwnedCards) {
        playerDisplay.m_vOwnedCardNames.push_back(Rules::getCardName(uiCard));
    }
}

//This one should be too
void PlayerStatusForDisplay::addPlayerGuesses(const Player &player, PlayerDisplay &playerDisplay)
{
    for (const Player::AnsweredGuess &answeredGuess : player.m_lAnsweredGuesses) {
        GuessDisplay guessDisplay;
        guessDisplay.m_uiPersonCard = answeredGuess.m_uiPerson;
        guessDisplay.m_uiWeaponCard = answeredGuess.m_uiWeapon;
        guessDisplay.m_uiPlaceCard = answeredGuess.m_uiPlace;
        playerDisplay.m_vAnsweredGuesses.push_back(guessDisplay);
    }
}
