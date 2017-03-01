#pragma once
#include "Card.h"
class Place :
	public Card
{
public:
	Place(std::string sName);
	virtual ~Place();

	eType type() { return eType::PLACE; }
};

