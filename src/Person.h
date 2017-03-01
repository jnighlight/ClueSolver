#pragma once
#include "Card.h"
class Person :
	public Card
{
public:
	Person(std::string sName);
	virtual ~Person();

	eType type() { return eType::PERSON; }
};

