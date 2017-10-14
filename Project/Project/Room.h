#ifndef __Room__
#define __Room__

#include <list>
#include "entity.h"
#include <string>

class Exit;
class Item;


class Room : public Entity
{
public:
	Room(const char* name, const char* description);
	~Room();

	void Look() const;

	Exit* GetExit(const string& direction) const;
	list<Exit*> GetExitsByDirection(const string& direction) const;
	void BlockAllExits();

public:
};

#endif //__Room__