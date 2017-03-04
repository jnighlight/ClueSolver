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

bool Player::addGuess(const std::vector<uint32_t> &vCards)
{
	if (vCards.size() != 3)
	{
		std::cout << "Player::" << __FUNCTION__ << ", Incoming guess of incorrect size: ";
		std::cout << vCards.size();
        return false;
	}

    Player::AnsweredGuess answeredGuess(vCards.at(0), vCards.at(1), vCards.at(2));
    m_lAnsweredGuesses.push_back(answeredGuess);
    return true;
}

void Player::addCard(uint32_t uiCardToAdd)
{
	m_vOwnedCards.insert(uiCardToAdd);
}

bool Player::ownsCard(uint32_t uiCard) const
{
	return m_vOwnedCards.find(uiCard) != m_vOwnedCards.end();
}

bool Player::ownsOneOfTheseCards(const std::vector<uint32_t> &vCards) const
{
	for (uint32_t uiCard : vCards)
	{
		if (m_vOwnedCards.find(uiCard) != m_vOwnedCards.end())
		{
			return true;
		}
	}
	return false;
}

void Player::addNotOwnedCards(const std::vector<uint32_t> &vNotOwnedCards)
{
	for (uint32_t uiNotOwnedCard : vNotOwnedCards)
	{
		m_vDefinitelyNotOwnedCards.insert(uiNotOwnedCard);
	}
}

bool Player::isDefinitelyNotOwned(uint32_t uiCard)
{
	return m_vDefinitelyNotOwnedCards.find(uiCard) != m_vDefinitelyNotOwnedCards.end();
}

//Here we return an internal enum. We could return a bool based on whether we need to remove the
//  guess or not, but then we wouldn't be able to tell the difference between a trash guess we are
//  throwing away, and the actual gain of knowledge that requires a recalc
Player::guessState Player::processStoredGuess(AnsweredGuess &answeredGuess, PlayerManager* pPlayerManager)
{
    //TODO: Refactor this. 3 of the same line of code is ew
	//If I know I own any of these cards, the guess can provide me no more information
	if (ownsCard(answeredGuess.m_uiPlace) ||
		ownsCard(answeredGuess.m_uiPerson) ||
		ownsCard(answeredGuess.m_uiWeapon))
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
	//(If there are NO cards left, I f***ed up bad somewhere. Why do I have this guess
	//	if I don't have the card to solve it?)
	if (answeredGuess.isEmpty())
	{
		std::cout << "Player::" << __FUNCTION__ << ", Player has empty guess! How'd he get it?";
		return Player::eTrash;
	}
	return Player::eNotSolved;
}

bool Player::checkForSolutions(PlayerManager* pPlayerManager)
{
    bool bSolutionFound = false;
	std::list<AnsweredGuess>::iterator i = m_lAnsweredGuesses.begin();
    Player::guessState processGuessResult  = Player::eTrash;
	while (i != m_lAnsweredGuesses.end())
	{
        processGuessResult = processStoredGuess((*i), pPlayerManager);
        //Technically, in the solved case, I could not break and just leak to the next
        //  switch statement. I just happen to think that's disgusting.
        switch (processGuessResult) {
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
                //TODO: Error case. Exception?
                break;
        }
	}
    return bSolutionFound;
}

//---------------------ANSWERED GUESS METHOD-------------------------
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
