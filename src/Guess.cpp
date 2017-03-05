#include "Guess.h"

Guess::Guess()
{
}

Guess::Guess(uint32_t uiPerson,
			 uint32_t uiPlace,
			 uint32_t uiWeapon,
			 const std::string &sGuesserName,
			 const std::string &sStopper)
	:m_uiPerson(uiPerson)
	,m_uiPlace(uiPlace)
	,m_uiWeapon(uiWeapon)
	,m_sGuesserName(sGuesserName)
	,m_sStopper(sStopper)
{
}

Guess::~Guess()
{
}

void Guess::addCard(uint32_t uiCard, Rules::eCardType eCardType)
{
    switch (eCardType) {
        case Rules::ePerson:
            m_uiPerson = uiCard;
            break;
        case Rules::eWeapon:
            m_uiWeapon = uiCard;
            break;
        case Rules::ePlace:
            m_uiPlace = uiCard;
            break;
        default:
            //TODO: Throw exception
            break;
    };
}
