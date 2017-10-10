#ifndef __Room__
#define __Room__

#include <list>
#include "entity.h"
#include <string>

class Exit;
class Item;

using namespace std;

class Room : public Entity
{
public:
	Room(const char* name, const char* description);
	~Room();

	void Look() const;

	Exit* GetExit(const string& direction) const;
	list<Exit*> GetExitsByDirection(const string& direction) const;

public:
};

#endif //__Room__