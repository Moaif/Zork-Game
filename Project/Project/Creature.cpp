#include <iostream>
#include "globals.h"
#include "room.h"
#include "exit.h"
#include "item.h"
#include "creature.h"
#include "Main.h"

// ----------------------------------------------------
Creature::Creature(const char* title, const char* description, Room* room) :
	Entity(title, description, (Entity*)room)
{
	type = CREATURE;
	hit_points = 1;
	min_damage = max_damage = min_protection = max_protection = 0;
	weapon = armour = NULL;
	combat_target = NULL;
}

// ----------------------------------------------------
Creature::~Creature()
{}

// ----------------------------------------------------
void Creature::Look(const vector<string>& args) const
{
	if (IsAlive())
	{
		cout << name << "\n";
		cout << description << "\n";
	}
	else
	{
		cout << name << "'s corpse\n";
		cout << "Here lies dead: " << description << "\n";
	}
}

// ----------------------------------------------------
bool Creature::Go(const vector<string>& args)
{
	if (!IsAlive())
		return false;

	Exit* exit = GetRoom()->GetExit(args[1]);

	if (exit == NULL)
		return false;

	if (PlayerInRoom())
		cout << name << "goes " << args[1] << "...\n";

	ChangeParentTo(exit->GetDestinationFrom((Room*)parent));

	return true;
}

// ----------------------------------------------------
bool Creature::Take(const vector<string>& args)
{
	if (!IsAlive())
		return false;

	Item* item = (Item*)parent->Find(args[1], ITEM);

	if (args.size() > 1)
	{
		// we could pick something from a container in our inventory ...
		if (item == NULL)
			item = (Item*)Find(args[1], ITEM);

		if (item == NULL)
			return false;

		Item* subitem = (Item*)item->Find(args[3], ITEM);

		if (subitem == NULL)
			return false;

		if (PlayerInRoom())
			cout << name << " looks into " << item->name << "...\n";

		item = subitem;
	}

	if (item == NULL)
		return false;

	if (PlayerInRoom())
		cout << name << " takes " << item->name << ".\n";

	item->ChangeParentTo(this);

	return true;
}

// ----------------------------------------------------
void Creature::Inventory() const
{
	list<Entity*> items;
	FindAll(ITEM, items);

	if (items.size() == 0)
	{
		cout << name << " does not own any items\n";
		return;
	}

	cout << "\n" << name << " owns:\n";
	for (list<Entity*>::const_iterator it = items.begin(); it != items.cend(); ++it)
	{
		if (*it == weapon)
			cout << (*it)->name << " (as weapon)\n";
		else if (*it == armour)
			cout << (*it)->name << " (as armour)\n";
		else
			cout << (*it)->name << "\n";
	}
}

// ----------------------------------------------------
bool Creature::Lock(const vector<string>& args)
{
	if (!IsAlive())
		return false;

	Exit* exit = GetRoom()->GetExit(args[1]);

	if (exit == NULL || exit->locked == true)
		return false;

	Item* item = (Item*)Find(args[3], ITEM);

	if (item == NULL || exit->key != item)
		return false;

	if (PlayerInRoom())
		cout << "\n" << name << "locks " << exit->GetDirectionFrom((Room*)parent) << "...\n";

	exit->locked = true;

	return true;
}

// ----------------------------------------------------
bool Creature::UnLock(const vector<string>& args)
{
	if (!IsAlive())
		return false;

	Exit* exit = GetRoom()->GetExit(args[1]);

	if (exit == NULL || exit->locked == false)
		return false;

	Item* item = (Item*)Find(args[3], ITEM);

	if (item == NULL || exit->key != item)
		return false;

	if (PlayerInRoom())
		cout << "\n" << name << "unlocks " << exit->GetDirectionFrom((Room*)parent) << "...\n";

	exit->locked = false;

	return true;
}

// ----------------------------------------------------
bool Creature::Drop(const vector<string>& args)
{
	if (!IsAlive())
		return false;

	Item* item = (Item*)Find(args[1], ITEM);

	if (item == NULL)
		return false;

	if (PlayerInRoom())
		cout << name << " drops " << item->name << "...\n";

	item->ChangeParentTo(parent);

	return true;
}

// ----------------------------------------------------
Room* Creature::GetRoom() const
{
	return (Room*)parent;
}

// ----------------------------------------------------
bool Creature::PlayerInRoom() const
{
	return parent->Find(PLAYER) != NULL;
}

// ----------------------------------------------------
bool Creature::IsAlive() const
{
	return hit_points > 0;
}

// ----------------------------------------------------
void Creature::Turn()
{
	if (combat_target != NULL)
	{
		if (parent->Find(combat_target) == true)
			MakeAttack();
		else
			combat_target = NULL;
	}
}

// ----------------------------------------------------
void Creature::Talk() {

}

// ----------------------------------------------------
void Creature::Stun(Item*) {

}

// ----------------------------------------------------
void Creature::TieUp() {

}

// ----------------------------------------------------
bool Creature::IsStuned() {
	return stuned > 0;
}
// ----------------------------------------------------
bool Creature::Attack(const vector<string>& args)
{
	Creature *target = (Creature*)parent->Find(args[1], CREATURE);

	if (target == NULL)
		return false;

	combat_target = target;
	cout << "\n" << name << " attacks " << target->name << "!\n";
	return true;
}

// ----------------------------------------------------
int Creature::MakeAttack()
{
	return 0;
}

// ----------------------------------------------------
int Creature::ReceiveAttack(int damage)
{
	return 0;
}

// ----------------------------------------------------
void Creature::Die()
{
	if (PlayerInRoom())
	{
		hit_points = 0;
		cout << "\n" << name << " dies.\n";
		if (type == PLAYER) {
			EndGame();
		}
	}
}

