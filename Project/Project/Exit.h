#ifndef __Exit__
#define __Exit__

#include "entity.h"

class Room;

using namespace std;

class Exit : public Entity
{
public:
	Exit(const char* direction, const char* opposite_direction, const char* description, Room* origin, Room* destination,bool locked,Entity* Key,const char* details);
	~Exit();

	void Look(const Room*) const;
	void LookDetails()const;

	const string& GetDirectionFrom(const Room* room) const;
	Room* GetDestinationFrom(const Room* room) const;

public:
	bool locked;
	string opposite_direction;
	Room* destination;
	Entity* key;
	const char* details;
};

#endif //__Exit__