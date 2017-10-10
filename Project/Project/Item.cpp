#include <iostream>
#include "room.h"
#include "exit.h"
#include "globals.h"
#include "item.h"

// ----------------------------------------------------
Item::Item(const char* title, const char* description, Entity* parent, vector<ItemType> item_type) :
	Entity(title, description, parent)
{
	this->item_type = item_type;
	type = ITEM;
}

// ----------------------------------------------------
Item::~Item()
{}

// ----------------------------------------------------
void Item::Look() const
{
	cout << "\n" << name << "\n";
	cout << description << "\n";

	list<Entity*> stuff;
	FindAll(ITEM, stuff);

	if (stuff.size() > 0)
	{
		cout << "It contains: " << "\n";
		for (list<Entity*>::const_iterator it = stuff.begin(); it != stuff.cend(); ++it)
			cout << (*it)->name << "\n";
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

bool Item::Contains(ItemType type) {
	for (vector<ItemType>::iterator it = item_type.begin(); it != item_type.end(); ++it)
	{
		if (*it == type) {
			return true;
		}
	}
	return false;
}
