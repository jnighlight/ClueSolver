#pragma once

#include <vector>
#include <string>
#include <cstdint>

#include "Rules.h"

class Guess
{
public:
	Guess();
	Guess(uint32_t uiPerson,
		  uint32_t uiPlace,
		  uint32_t uiWeapon,
		  const std::string &sGuesserName,
		  const std::string &sStopper);
	virtual ~Guess();

	uint32_t m_uiPerson;
	uint32_t m_uiPlace;
	uint32_t m_uiWeapon;
	std::string m_sGuesserName;
	std::string m_sStopper;
	std::vector<std::string> m_svPasses;
    bool m_bUserGuess;
    uint32_t m_uiUserAnswerRecieved;

    void addCard(uint32_t uiCard, Rules::eCardType eCardType);
    void clear();
};
