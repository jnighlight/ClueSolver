#pragma once
#include <string>

class Card
{
public:
	enum class eType {
		UNDEFINED = 0,
		WEAPON,
		PERSON,
		PLACE
	};

	Card(std::string sName);
	virtual ~Card();


	std::string m_sName;

	virtual eType type() = 0;
	std::string name() { return m_sName; };
};
