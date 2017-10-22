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

	void Look() const override;

	Exit* GetExit(const std::string& direction) const;
	std::list<Exit*> GetExitsByDirection(const std::string& direction) const;
	void BlockAllExits();

};

#endif //__Room__