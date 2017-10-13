#include <iostream>
#include "globals.h"
#include "room.h"
#include "exit.h"
#include "item.h"
#include "creature.h"

// ----------------------------------------------------
Creature::Creature(const char* title, const char* description, Room* room) :
	Entity(title, description, (Entity*)room),basicDmg(basicDmg)
{
	type = CREATURE;
	hit_points = 1;
	weapon = nullptr;
	combat_target = nullptr;
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
void Creature::Go(const vector<string>& args)
{
	if (!IsAlive() || IsStuned() || IsTiedUp())
		return;
	

	list<Exit*> exits = GetRoom()->GetExitsByDirection(args[1]);

	if (exits.size() <= 0)
	{
		return;
	}

	for (list<Exit*>::iterator it = exits.begin(); it != exits.end(); ++it) {
		Exit* ex = (Exit*)*it;
		if (!ex->locked) {
			ChangeParentTo(ex->GetDestinationFrom((Room*)parent));
			if (PlayerInRoom()) {
				cout << "\n" << name << " enter the room.\n";
			}
			return;
		}
	}
}

// ----------------------------------------------------
void Creature::Take(const vector<string>& args)
{
	
}

// ----------------------------------------------------
void Creature::Inventory() const
{
}

// ----------------------------------------------------
void Creature::Lock(const vector<string>& args)
{
	
}

// ----------------------------------------------------
void Creature::UnLock(const vector<string>& args)
{
	
}

// ----------------------------------------------------
void Creature::Drop(const vector<string>& args)
{
	
}

// ----------------------------------------------------
Room* Creature::GetRoom() const
{
	return (Room*)parent;
}

// ----------------------------------------------------
bool Creature::PlayerInRoom() const
{
	return parent->Find(PLAYER) != nullptr;
}

// ----------------------------------------------------
bool Creature::IsAlive() const
{
	return hit_points > 0;
}

// ----------------------------------------------------
void Creature::Turn()
{
	if (IsAlive() && !IsStuned() && !IsTiedUp()) {
		if (combat_target != nullptr)
		{
			if (parent->Find(combat_target) == true)
			{
				string temp = name + " attacks " + combat_target->name;
				turnCout(temp);
				if (weapon == nullptr) {
					combat_target->ReceiveAttack(basicDmg);
				}
				else
				{
					combat_target->ReceiveAttack(weapon->weapondDmg);
				}
			}
			else
				combat_target = nullptr;
		}
		combat_target = nullptr;
		dodging = false;
	}
}

// ----------------------------------------------------
void Creature::Talk() {

}

// ----------------------------------------------------
void Creature::Stun(Item* weapon) {
	if (!IsAlive())
		return;

	stuned =(int) weapon->weapondDmg;
	cout << "\n You stuned " << name << ".\n";
}

// ----------------------------------------------------
void Creature::TieUp() {
	if (!IsAlive())
		return;

	tiedUp = true;
}

// ----------------------------------------------------
bool Creature::IsStuned() const{
	return stuned > 0;
}

// ----------------------------------------------------
bool Creature::IsTiedUp() const{
	return tiedUp;
}

// ----------------------------------------------------
void Creature::Attack(const vector<string>& args)
{
	if (!IsAlive() || IsStuned() || IsTiedUp())
		return;

	dodging = false;
	if (args.size() == 2) {
		Creature *target = (Creature*)parent->Find(args[1], CREATURE);

		if (target == nullptr) {
			cout << "\n'" << args[1] << "' is not in the room.\n";
			return;
		}

		if (!target->IsAlive()) {
			cout << "\n'" << args[1] << "' is aready dead.\n";
			return;
		}

		weapon = nullptr;
		combat_target = target;
		cout << "\n" << name << " prepares to attack " << target->name << " with bared hands"  << "!\n";
	}
	else if (args.size() == 4) {
		Creature *target = (Creature*)parent->Find(args[1], CREATURE);

		if (target == nullptr) {
			cout << "\n'" << args[1] << "' is not in the room.\n";
			return;
		}

		if (!target->IsAlive()) {
			cout << "\n'" << args[1] << "' is aready dead.\n";
			return;
		}

		Item* item = (Item*)Find(args[3],ITEM);

		if (item == nullptr) {
			cout << "\n'" << args[3] << "' is not in your inventory";
		}

		weapon = item;
		combat_target = target;
		cout << "\n" << name << " prepares to attack " << target->name << " with "<< item->name<< "!\n";
	}
}

// ----------------------------------------------------
void Creature::Dodge() {
	if (!IsAlive() || IsStuned() || IsTiedUp())
		return;

	combat_target = nullptr;
	dodging = true;
	cout << "\n" << name << " prepares to dodge.\n";
}


// ----------------------------------------------------
void Creature::ReceiveAttack(float damage)
{
	//5% proc of atac faillure 10%proc half dmg
	if (!dodging) {
		int temp = rand() % 100; //Random value between 0-99
		if (temp >= 95)
		{
			turnCout("Attack missed");
		}
		else if (temp >= 85) {
			turnCout("Attack hits, but with only half force");
			hit_points -= damage / 2;
		}
		else
		{
			turnCout("Attack hits");
			hit_points -= damage;
		}
	}
	//If dodging 35% proc of reduce half-dmg and 15% atac faillure
	else
	{
		int temp = rand() % 100;
		if (temp >= 85)
		{
			string temp = name + " dodged the attack";
			turnCout(temp);
		}
		else if (temp >= 50) {
			string temp = name + " dodged half of the attack";
			turnCout(temp);
			hit_points -= damage / 2;
		}
		else
		{
			turnCout("Attack hits");
			hit_points -= damage;
		}
	}

	if (hit_points <= 0) {
		Die();
	}
}

// ----------------------------------------------------
void Creature::Die()
{
	if (PlayerInRoom())
	{
		cout << "\n" << name << " dies.\n";
	}
}

