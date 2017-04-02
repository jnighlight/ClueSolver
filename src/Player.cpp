#include "Player.h"
#include "PlayerManager.h"

Player::Player(std::string sName, unsigned int uiHandSize)
	:m_sName(sName)
	,m_uiHandSize(uiHandSize)
{
}

Player::~Player()
{
}

bool Player::isSolved() const
{
	return m_setOwnedCards.size() == m_uiHandSize;
}

void Player::addGuess(const AnsweredGuess &answeredGuess)
{
    //push_back makes a copy. It's a small struct, it's fine
    m_lAnsweredGuesses.push_back(answeredGuess);
}

void Player::addCard(uint32_t uiCardToAdd)
{
	m_setOwnedCards.insert(uiCardToAdd);
}

bool Player::ownsCard(uint32_t uiCard) const
{
	return m_setOwnedCards.find(uiCard) != m_setOwnedCards.end();
}

bool Player::ownsOneOfTheseCards(const AnsweredGuess &answeredGuess) const
{
    if (ownsCard(answeredGuess.m_uiPerson) || 
        ownsCard(answeredGuess.m_uiPlace) || 
        ownsCard(answeredGuess.m_uiWeapon)) {
        return true;
    }
	return false;
}

void Player::addNotOwnedCards(const Guess &guess)
{
    m_setDefinitelyNotOwnedCards.insert(guess.m_uiPerson);
    m_setDefinitelyNotOwnedCards.insert(guess.m_uiPlace);
    m_setDefinitelyNotOwnedCards.insert(guess.m_uiWeapon);
}

bool Player::isDefinitelyNotOwned(uint32_t uiCard) const
{
	return m_setDefinitelyNotOwnedCards.find(uiCard) != m_setDefinitelyNotOwnedCards.end();
}

//Here we return an internal enum. We could return a bool based on whether we need to remove the
//  guess or not, but then we wouldn't be able to tell the difference between a trash guess we are
//  throwing away, and the actual gain of knowledge that requires a recalc
Player::eGuessState Player::processStoredGuess(AnsweredGuess &answeredGuess, PlayerManager* pPlayerManager)
{
	//If I know I own any of these cards, the guess can provide me no more information
	if (ownsOneOfTheseCards(answeredGuess))
	{
		return Player::eTrash;
	}
	//If someone ELSE owns any of these cards, zero them out
	if (pPlayerManager->isOwned(answeredGuess.m_uiPlace))
	{
		answeredGuess.m_uiPlace = 0;
	}
	if (pPlayerManager->isOwned(answeredGuess.m_uiPerson))
	{
		answeredGuess.m_uiPerson = 0;
	}
	if (pPlayerManager->isOwned(answeredGuess.m_uiWeapon))
	{
		answeredGuess.m_uiWeapon = 0;
	}
    //If I know for a fact that I don't own this card (I've passed on a guess with it in it),
    //  I can remove that from the guess as well
	if (isDefinitelyNotOwned(answeredGuess.m_uiPlace))
	{
		answeredGuess.m_uiPlace = 0;
	}
	if (isDefinitelyNotOwned(answeredGuess.m_uiPerson))
	{
		answeredGuess.m_uiPerson = 0;
	}
	if (isDefinitelyNotOwned(answeredGuess.m_uiWeapon))
	{
		answeredGuess.m_uiWeapon = 0;
	}
	//Finally, if there's only one card left in the guess, ITS MINE!!! YES!
	if (answeredGuess.isSolved())
	{
		uint32_t uiSolvedCard = answeredGuess.getSolved();
		addCard(uiSolvedCard);
		return Player::eSolved;
	}
    //If there are no cards left, this guess is no longer useful to me. Junk it.
	if (answeredGuess.isEmpty())
	{
		return Player::eTrash;
	}
	return Player::eNotSolved;
}

bool Player::checkForSolutions(PlayerManager* pPlayerManager)
{
    bool bSolutionFound = false;
	std::list<AnsweredGuess>::iterator i = m_lAnsweredGuesses.begin();
    Player::eGuessState eProcessGuessResult = Player::eTrash;
    //We're gonna be removing while we iterate, so we can't use the fancy foreach syntax
	while (i != m_lAnsweredGuesses.end())
	{
        eProcessGuessResult = processStoredGuess((*i), pPlayerManager);
        //Technically, in the solved case, I could not break and just leak to the next
        //  switch statement. I just happen to think that's disgusting.
        switch (eProcessGuessResult) {
            case Player::eNotSolved :
                i++;
                break;
            case Player::eSolved :
                bSolutionFound = true;
                i = m_lAnsweredGuesses.erase(i);
                break;
            case Player::eTrash :
                i = m_lAnsweredGuesses.erase(i);
                break;
            default:
                std::cout << "Player::" << __FUNCTION__ << ", ERROR: Not defined player guess result";
                break;
        }
	}
    return bSolutionFound;
}

//---------------------ANSWERED GUESS METHODS-------------------------
bool Player::AnsweredGuess::isSolved()
{
	uint32_t uiStillPresent = 0;
	if (m_uiPlace != 0)
	{
		++uiStillPresent;
	}
	if (m_uiPerson != 0)
	{
		++uiStillPresent;
	}
	if (m_uiWeapon != 0)
	{
		++uiStillPresent;
	}
	return uiStillPresent == 1;
}

uint32_t Player::AnsweredGuess::getSolved()
{
    //There's a little bit of code duplication here. We'll likely call this before getting solved anyways.
    //But having this check helps me sleep at night
    if (!isSolved())
    {
        return 0;
    }
	if (m_uiPlace != 0)
	{
		return m_uiPlace;
	}
	if (m_uiPerson != 0)
	{
		return m_uiPerson;
	}
	if (m_uiWeapon != 0)
	{
		return m_uiWeapon;
	}
	return 0;
}
