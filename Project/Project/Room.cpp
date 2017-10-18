#include <iostream>
#include "globals.h"
#include "exit.h"
#include "item.h"
#include "creature.h"
#include "room.h"


Room::Room(const char* title, const char* description) :
	Entity(title, description, NULL)
{
	type = ROOM;
}


Room::~Room()
{
}


void Room::Look() const
{
	cout << "\n" << name << "\n";
	cout << description;


	// List exits --
	for (vector<Entity*>::const_iterator it = container.cbegin(); it != container.cend(); ++it)
	{
		
		bool write = true;
		if ((*it)->type == EXIT)
		{
			Exit* ex = (Exit*)*it;
			ex->Look(this);
		}
	}

	// List items --
	for (vector<Entity*>::const_iterator it = container.cbegin(); it != container.cend(); ++it)
	{
		if ((*it)->type == ITEM)
		{
			Item* item = (Item*)*it;
			cout << "\nThere is an item here: " << item->name;
		}
	}

	// List creatures --
	for (vector<Entity*>::const_iterator it = container.cbegin(); it != container.cend(); ++it)
	{
		if ((*it)->type == CREATURE)
		{
			Creature* cr = (Creature*)*it;
			cout << "\nThere is someone else here: " << cr->name;
			if (cr->IsAlive() == false)
				cout << " (dead)";
		}
	}

	cout << "\n";
}


Exit* Room::GetExit(const string& description) const
{
	for (vector<Entity*>::const_iterator it = container.cbegin(); it != container.cend(); ++it)
	{
		if ((*it)->type == EXIT)
		{
			Exit* ex = (Exit*)*it;
			if (Same(ex->description, description))
				return ex;
		}
	}

	return NULL;
}


list<Exit*> Room::GetExitsByDirection(const string& direction) const
{
	list<Exit*> sol;
	for (vector<Entity*>::const_iterator it = container.cbegin(); it != container.cend(); ++it)
	{
		if ((*it)->type == EXIT)
		{
			Exit* ex = (Exit*)*it;
			if (Same(ex->GetDirectionFrom(this), direction))
				sol.push_back(ex);
		}
	}

	return sol;
}

void Room::BlockAllExits()  {
	vector<Entity*>::iterator it = container.begin();

	while (it != container.end())
	{
		if ((*it)->type == EXIT)
		{
			it=container.erase(it);
		}
		else
		{
			++it;
		}
	}
}