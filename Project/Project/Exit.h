#ifndef __Exit__
#define __Exit__

#include "entity.h"

class Room;


class Exit : public Entity
{
public:
	Exit(const char* direction, const char* opposite_direction, const char* description, Room* origin, Room* destination,bool locked,Entity* Key,const char* details);
	~Exit();

	void Look(const Room*) const;
	void LookDetails()const;

	const std::string& GetDirectionFrom(const Room* room) const;
	Room* GetDestinationFrom(const Room* room) const;

public:
	bool locked;
	Entity* key;

private:
	std::string opposite_direction;
	Room* destination;
	std::string details;
};

#endif //__Exit__