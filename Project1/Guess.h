#pragma once
#ifndef GUESS_H
#define GUESS_H
#include <vector>

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
	bool m_bSolved;
	std::string m_sStopper;
	std::vector<std::string> m_svPasses;
};

#endif // !GUESS_H