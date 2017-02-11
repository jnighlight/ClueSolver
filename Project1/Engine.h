#pragma once
#include <list>
#include <string>
#include <unordered_map>

#include "Person.h"
#include "Place.h"
#include "Player.h"
#include "PlayerInfo.h"
#include "Weapon.h"

class Engine
{
public:
	//This may need to be edited. It may go in the controller for the GUI to draw from/iterate through
	//Consider alternatives. It's not great to iterate through an enum. Maybe an object list with names and ID's would be
	//	better? Then we could dynamically create them from a string list. Only thing would be to get them into a hash using
	//	their getId() method...
	enum class eCards {
		SCARLET = 0,
		MUSTARD,
		WHITE,
		GREEN,
		PEACOCK,
		PLUM,
		CANDLESTICK,
		POISON,
		ROPE,
		GLOVES,
		KNIFE,
		LEAD_PIPE,
		REVOLVER,
		WRENCH,
		KITCHEN,
		BALLROOM,
		CONSERVATORY,
		BILLIARD_ROOM,
		LIBRARY,
		STUDY,
		HALL,
		LOUNGE,
		DINING_ROOM
	};

	Engine();
	virtual ~Engine();
	void parsePlayerInfo(const PlayerInfo &playerInfo);
	uint32_t getUserCardCount() { return m_lPlayers.front()->m_uiHandSize; };

private:

	void createCards();
	//This is going to be the hash for engine knowledge of card ownership. Easy to look up: Key is Card Id, Value is Player Id (with 0 meaning no ownership)
	static std::unordered_map<unsigned int, unsigned int> s_mCardOwnership;
	std::list<Player*> m_lPlayers;
};
