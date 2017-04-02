#include "PlayerStatusForDisplay.h"

void PlayerStatusForDisplay::addPlayer(const Player &player, const PlayerManager* pPlayerManager)
{
    PlayerDisplay playerDisplay;
    playerDisplay.m_sName = player.m_sName;
    playerDisplay.m_uiCardCount = player.m_uiHandSize;

    addPlayerCards(player, playerDisplay);
    addPlayerGuesses(player, playerDisplay);
    addPlayerNotOwnedCards(player, playerDisplay, pPlayerManager);

    m_vPlayerDisplays.push_back(playerDisplay);
}

//This method should be in player display
void PlayerStatusForDisplay::addPlayerCards(const Player &player, PlayerDisplay &playerDisplay)
{
    for (uint32_t uiCard : player.m_setOwnedCards) {
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

void PlayerStatusForDisplay::addPlayerNotOwnedCards(const Player &player,
                                                    PlayerDisplay &playerDisplay,
                                                    const PlayerManager* pPlayerManager)
{
    for (uint32_t uiCard : player.m_setPassedCards) {
        //If the card is owned by someone else, it's evident that it's not owned by this player
        if (!pPlayerManager->isOwned(uiCard)) {
            playerDisplay.m_vDefinitelyNotOwnedCardNames.push_back(Rules::getCardName(uiCard));
        }
    }
}
