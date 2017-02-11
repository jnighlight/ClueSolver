#pragma once
#include "Card.h"
class Weapon :
	public Card
{
public:
	Weapon(std::string sName);
	~Weapon();

	eType type() { return eType::WEAPON; }
};

