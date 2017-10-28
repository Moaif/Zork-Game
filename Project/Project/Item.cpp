#include <iostream>
#include "room.h"
#include "exit.h"
#include "globals.h"
#include "item.h"

using namespace std;

Item::Item(const char* title, const char* description, Entity* parent,vector<ItemType> item_type,bool locked, Entity* key) :
	Entity(title, description, parent),locked(locked),key(key),item_type(item_type)
{
	type = ITEM;
}


Item::~Item()
{}


void Item::Look() const
{
	Cout("\n" + name + "\n");
	Cout(description + "\n");

	if (Contains(ItemType::CONTAINER) || Contains(ItemType::LIQUID_CONTAINER)) {
		if (locked) {
			Cout(keyDescription + "\n");
		}
		else
		{
			Cout("It contains: \n");
			for (vector<Entity*>::const_iterator it = container.cbegin(); it != container.cend(); ++it) {
				Cout((*it)->name + "\n");
			}
		}
	}
}


void Item::ChangeItemType(ItemType old,ItemType remplace) {
	for (vector<ItemType>::iterator it= item_type.begin(); it != item_type.end();++it)
	{
		if (*it == old) {
			*it = remplace;
		}
	}
}


bool Item::Contains(ItemType type)const {
	for (vector<ItemType>::const_iterator it = item_type.cbegin(); it != item_type.cend(); ++it)
	{
		if (*it == type) {
			return true;
		}
	}
	return false;
}

void Item::DropItems() {
	while (container.size() != 0) {
		container[0]->ChangeParentTo(parent);
	}
}
