#include <iostream>
#include "globals.h"
#include "room.h"
#include "exit.h"
#include "item.h"
#include "player.h"

// ----------------------------------------------------
Player::Player(const char* title, const char* description, Room* room) :
	Creature(title, description, room)
{
	type = PLAYER;
}

// ----------------------------------------------------
Player::~Player()
{
}

// ----------------------------------------------------
void Player::Look(const vector<string>& args) const
{
	if (args.size() > 1)
	{	//Look over the room 
		for (list<Entity*>::const_iterator it = parent->container.begin(); it != parent->container.cend(); ++it)
		{
			if (Same((*it)->name, args[1])  )
			{
				(*it)->Look();
				return;
			}
			if((*it)->type == EXIT && Same(args[1], ((Exit*)(*it))->description)) {
				((Exit*)(*it))->LookDetails();
				return;
			}
		}
		//Look in inventory
		for (list<Entity*>::const_iterator it = container.begin(); it != container.cend(); ++it) {
			if (Same((*it)->name, args[1]))
			{
				(*it)->Look();
				return;
			}
		}

		//Look for player
		if (Same(args[1], "me"))
		{
			cout << "\n" << name << "\n";
			cout << description << "\n";
		}
	}
	else
	{
		parent->Look();
	}
}

// ----------------------------------------------------
bool Player::Go(const vector<string>& args)
{
	list<Exit*> exits = GetRoom()->GetExitsByDirection(args[1]);

	if (exits.size() <= 0)
	{
		cout << "\nThere is no exit at '" << args[1] << "'.\n";
		return false;
	}

	for (list<Exit*>::iterator it = exits.begin(); it != exits.end(); ++it) {
		Exit* ex = (Exit*)*it;
		if (!ex->locked) {
			cout << "\nYou take direction " << ex->GetDirectionFrom((Room*)parent) << "...\n";
			ChangeParentTo(ex->GetDestinationFrom((Room*)parent));
			parent->Look();

			return true;
		}
	}

	cout << "\nThat exit is locked.\n";
	return false;
}


// ----------------------------------------------------
bool Player::Take(const vector<string>& args)
{
	if (args.size()==6) {
		Item* item = (Item*)parent->Find(args[3], ITEM);

		// we could pick something from a container in our inventory ...
		if (item == NULL)
			item = (Item*)Find(args[3], ITEM);

		if (item == NULL)
		{
			cout << "\nCannot find '" << args[3] << "' in this room or in your inventory.\n";
			return false;
		}

		Item* subitem = (Item*)item->Find(args[1], ITEM);

		if (subitem == NULL)
		{
			cout << "\n" << item->name << " does not contain '" << args[1] << "'.\n";
			return false;
		}

		Item* tool = (Item*)parent->Find(args[5],ITEM);

		if (tool == NULL) {
			tool = (Item*)Find(args[5], ITEM);
		}

		if (tool == NULL) {
			cout << "\n Cannot find '" << args[5] << "' in this room or in your inventory.\n";
		}

		if (subitem->Contains(IMMOVABLE)) {
			cout << "\n You cant take this item from the room. \n";
			return false;
		}

		if (subitem->Contains(LIQUID) && !tool->Contains(LIQUID_CONTAINER)){
			cout << "\n You can't take '" << subitem->name << "' with '" << tool->name << "'.\n";
			return false;
		}
		if (!tool->Contains(CONTAINER)) {
			cout << "\n You cant put '" << subitem->name << "' in '" << tool->name << "'.\n";
			return false;
		}
		cout << "\nYou take " << subitem->name << " from " << item->name << ".\n";
		subitem->ChangeParentTo(tool);
	}
	else if (args.size() == 4)
	{
		Item* item = (Item*)parent->Find(args[3], ITEM);

		// we could pick something from a container in our inventory ...
		if (item == NULL)
			item = (Item*)Find(args[3], ITEM);

		if (item == NULL)
		{
			cout << "\nCannot find '" << args[3] << "' in this room or in your inventory.\n";
			return false;
		}

		Item* subitem = (Item*)item->Find(args[1], ITEM);

		if (subitem == NULL)
		{
			cout << "\n" << item->name << " does not contain '" << args[1] << "'.\n";
			return false;
		}
		if (subitem->Contains(IMMOVABLE)) {
			cout << "\n You cant take this item from the room. \n";
			return false;
		}
		if (subitem->Contains(LIQUID)) {
			cout << "\n You cant take  this item with your hands. \n";
			return false;
		}
		cout << "\nYou take " << subitem->name << " from " << item->name << ".\n";
		subitem->ChangeParentTo(this);
	}
	else if (args.size() == 2)
	{
		Item* item = (Item*)parent->Find(args[1], ITEM);

		if (item == NULL)
		{
			cout << "\nThere is no item here with that name.\n";
			return false;
		}

		if(item->Contains(CURSED)) {
			Die();
			return false;
		}

		if (item->Contains(IMMOVABLE)) {
			cout << "\n You cant take this item from the room. \n";
			return false;
		}
		if (item->Contains(LIQUID)) {
			cout << "\n You cant take  this item with your hands. \n";
			return false;
		}

		cout << "\nYou take " << item->name << ".\n";
		item->ChangeParentTo(this);
	}

	return true;
}

// ----------------------------------------------------
void Player::Inventory() const
{
	list<Entity*> items;
	FindAll(ITEM, items);

	if (items.size() == 0)
	{
		cout << "\nYou do not own any item.\n";
		return;
	}

	for (list<Entity*>::const_iterator it = items.begin(); it != items.cend(); ++it)
	{
		if (*it == weapon)
			cout << "\n" << (*it)->name << " (as weapon)";
		else if (*it == armour)
			cout << "\n" << (*it)->name << " (as armour)";
		else
			cout << "\n" << (*it)->name;
	}

	cout << "\n";
}

// ----------------------------------------------------
bool Player::Drop(const vector<string>& args)
{
	if (args.size() == 2)
	{
		Item* item = (Item*)Find(args[1], ITEM);

		if (item == NULL)
		{
			cout << "\nThere is no item on you with that name.\n";
			return false;
		}

		cout << "\nYou drop " << item->name << "...\n";
		item->ChangeParentTo(parent);

		return true;
	}
	else if (args.size() == 4)
	{
		Item* item = (Item*)Find(args[1], ITEM);

		if (item == NULL)
		{
			cout << "\nCan not find '" << args[1] << "' in your inventory.\n";
			return false;
		}

		Item* container = (Item*)parent->Find(args[3], ITEM);

		if (container == NULL) {
			container = (Item*)Find(args[3], ITEM);
		}

		if (container == NULL)
		{
			cout << "\nCan not find '" << args[3] << "' in your inventory or in the room.\n";
			return false;
		}
		if (container->Contains(CONTAINER) && !item->Contains(LIQUID)) {
			cout << "\nYou put " << item->name << " into " << container->name << ".\n";
			item->ChangeParentTo(container);
			return true;
		}
		cout << "\n You cant put '" << item->name << "' in '" << container->name << "'.\n";
	}
	if (args.size() == 6) {
	
		Item* actualContainer = (Item*)parent->Find(args[3], ITEM);

		if (actualContainer == NULL) {
			actualContainer = (Item*)Find(args[3], ITEM);
		}

		if (actualContainer == NULL)
		{
			cout << "\nCan not find '" << args[3] << "' in your inventory or in the room.\n";
			return false;
		}

		Item* item = (Item*)actualContainer->Find(args[1], ITEM);

		if (item == NULL)
		{
			cout << "\nCan not find '" << args[1] << "' in '"<< args[3]<<"'.\n";
			return false;
		}

		Item* container = (Item*)parent->Find(args[5], ITEM);

		if (container == NULL) {
			container = (Item*)Find(args[5], ITEM);
		}

		if (container == NULL) {
			cout << "\nCan not find '" << args[5] << "' in your inventory or in the room.\n";
			return false;
		}

		if (container->Contains(CONTAINER) && !item->Contains(LIQUID)) {
			cout << "\nYou put " << item->name << " into " << container->name <<" from " << actualContainer->name << ".\n";
			item->ChangeParentTo(container);
			return true;
		}
		if (container->Contains(LIQUID_CONTAINER) && item->Contains(LIQUID)) {
			cout << "\nYou put " << item->name << " into " << container->name << " from " << actualContainer->name << ".\n";
			item->ChangeParentTo(container);
			return true;
		}

		cout << "\n You cant put '" << item->name << "' in '" << container->name << "'.\n";
	}

	return false;
}


// ----------------------------------------------------
bool Player::Attack(const vector<string>& args)
{
	Creature *target = (Creature*)parent->Find(args[1], CREATURE);

	if (target == NULL)
	{
		cout << "\n" << args[1] << " is not here.";
		return false;
	}

	combat_target = target;
	cout << "\nYou jump to attack " << target->name << "!\n";
	return true;
}


// ----------------------------------------------------
bool Player::Lock(const vector<string>& args)
{
	if (!IsAlive())
		return false;

	Exit* exit = GetRoom()->GetExit(args[1]);

	if (exit == NULL)
	{
		cout << "\nThere is no exit at '" << args[1] << "'.\n";
		return false;
	}

	if (exit->locked == true)
	{
		cout << "\nThat exit is already locked.\n";
		return false;
	}

	Item* item = (Item*)Find(args[3], ITEM);

	if (item == NULL)
	{
		cout << "\nItem '" << args[3] << "' not found in your inventory.\n";
		return false;
	}

	if (exit->key != item)
	{
		cout << "\nItem '" << item->name << "' is not the key for " << exit->GetDirectionFrom((Room*)parent) << ".\n";
		return false;
	}

	cout << "\nYou lock " << exit->GetDirectionFrom((Room*)parent) << "...\n";

	exit->locked = true;

	return true;
}

// ----------------------------------------------------
bool Player::UnLock(const vector<string>& args)
{
	if (!IsAlive())
		return false;

	Exit* exit = GetRoom()->GetExit(args[1]);

	if (exit == NULL)
	{
		cout << "\nThere is no '" << args[1] << "'.\n";
		return false;
	}

	if (exit->locked == false)
	{
		cout << "\nThat exit is not locked.\n";
		return false;
	}

	Item* item = (Item*)Find(args[3], ITEM);

	if (item == NULL)
	{
		cout << "\n'" << args[3] << "' not found in your inventory.\n";
		return false;
	}
	if (exit->key == NULL) {
		if (item->Contains(WEAPON)) {
			cout << "\nYou break the " << exit->description << "...\n";
			exit->locked = false;
			exit->description = "Broken " + exit->description;
			return true;
		}
		else
		{
			cout << "\n" << item->name << " is not enough to break " << exit->description << "\n";
			return false;
		}
	}
	if (exit->key != item)
	{
		cout << "\nYou cant unlock '" << exit-> description << "' with " << item->name << ".\n";
		return false;
	}

	cout << "\nYou unlock " << exit->GetDirectionFrom((Room*)parent) << "...\n";

	exit->locked = false;

	return true;
}

void Player::Talk(const vector<string>& args) {
	Creature* creature = (Creature*)parent->Find(args[1],CREATURE);
	if (creature == NULL) {
		cout << "\n'" << args[1] << "' is not in this room";
		return;
	}
	creature->Talk();

}