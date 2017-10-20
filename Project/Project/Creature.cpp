#include <iostream>
#include "globals.h"
#include "room.h"
#include "exit.h"
#include "item.h"
#include "creature.h"

using namespace std;

Creature::Creature(const char* title, const char* description, Room* room) :
	Entity(title, description, (Entity*)room),basicDmg(basicDmg)
{
	type = CREATURE;
	hit_points = 1;
	weapon = nullptr;
	combat_target = nullptr;
	inCombat = false;
	action = Action::NONE;
}


Creature::~Creature()
{}


void Creature::Look(const vector<string>& args) const
{
	if (IsAlive())
	{
		cout << "\n" << name << "\n";
		cout << description << "\n";
	}
	else
	{
		cout << name << "'s corpse\n";
		cout << "Here lies dead: " << description << "\n";
	}
}


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
			if (PlayerInRoom()) {
				string temp = name + " leaves the room.";
				turnCout(temp);
			}
			ChangeParentTo(ex->GetDestinationFrom((Room*)parent));
			if (PlayerInRoom()) {
				string temp= name + " enter the room.";
				turnCout(temp);
			}
			return;
		}
	}
}


Room* Creature::GetRoom() const
{
	return (Room*)parent;
}


bool Creature::PlayerInRoom() const
{
	return parent->Find(PLAYER) != nullptr;
}


bool Creature::IsAlive() const
{
	return hit_points > 0;
}


void Creature::Turn()
{
	if (IsAlive()) {
		if (!IsStuned()) {
			if (combat_target != nullptr)
			{
				if (parent->Find(combat_target) == true)
				{
					switch (action)
					{
					case Action::ATTACK:
					{
						string temp = name + " try to attack " + combat_target->name;
						turnCout(temp);
						if (weapon == nullptr) {
							combat_target->ReceiveAttack(basicDmg);
						}
						else
						{
							combat_target->ReceiveAttack(weapon->weapondDmg);
						}
					}
					break;
					case Action::STUN:
					{
						string temp = name + " try to stun " + combat_target->name;
						turnCout(temp);

						combat_target->ReceiveStun(weapon->weapondDmg);
					}
					break;
					default:
						break;
					}
				}
			}
			combat_target = nullptr;
			action = Action::NONE;
		}
		//If in combat, stun disappear faster
		else {
			if (IsInCombat()) {
				stuned -= 5;
				if (!IsStuned()) {
					if (parent->Find(PLAYER) != nullptr) {
						string temp = name + " is no more stuned.";
						turnCout(temp);
					}
				}

			}
			else {
				--stuned;
				if (!IsStuned()) {
					if (parent->Find(PLAYER) != nullptr) {
						string temp = name + " is no more stuned.";
						turnCout(temp);
					}
				}
			}
		}
	}
}



void Creature::Talk() {

}


void Creature::TieUp() {
	if (!IsAlive())
		return;

	tiedUp = true;
}


bool Creature::IsStuned() const{
	return stuned > 0;
}


bool Creature::IsTiedUp() const{
	return tiedUp;
}

bool Creature::IsInCombat()const {
	return inCombat;
}


void Creature::Dodge() {
	if (!IsAlive() || IsStuned() || IsTiedUp())
		return;

	action = Action::DODGE;
	string temp = name + " prepares to dodge.";
	turnCout(temp);
}



void Creature::ReceiveAttack(float damage)
{
	//If dodging 60% proc of reduce half-dmg and 30% atac faillure
	if (action == Action::DODGE){
		int temp = RAND() % 100;//Random value between 0-99
		if (temp >= 70)
		{
			string temp = name + " dodged the attack";
			turnCout(temp);
		}
		else if (temp >= 10) {
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
	//5% proc of atac faillure 10%proc half dmg
	else
	{
		int temp = RAND() % 100; 
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
	if (hit_points <= 0) {
		Die();
	}
}

void Creature::ReceiveStun(float duration) {
	if (IsStuned()) {
		duration /= 2;
	}
	//If dodging 50% proc of reduce half-duration and 25% atac faillure
	if (!(action == Action::DODGE)) {
		int temp = RAND() % 100;
		if (temp >= 75)
		{
			string temp = name + " dodged the stun";
			turnCout(temp);
		}
		else if (temp >= 25) {
			string temp = name + " dodged half of the stun";
			turnCout(temp);
			stuned = duration / 2;
		}
		else
		{
			turnCout("Stun hits");
			stuned = duration;
		}
	}
	//5% proc of atac faillure 10%proc half duration
	else
	{
		int temp = RAND() % 100;
		if (temp >= 95)
		{
			turnCout("Stun missed");
		}
		else if (temp >= 85) {
			turnCout("Stun hits, but with only half force");
			stuned = duration / 2;
		}
		else
		{
			turnCout("Stun hits");
			stuned = duration;
		}
	}
}

void Creature::StartCombat(Creature* c1) {
	c1->inCombat = true;
	inCombat = true;
}

void Creature::EndCombat(Creature* c1) {
	c1->inCombat = false;
	inCombat = false;
}


void Creature::Die()
{
	if (PlayerInRoom())
	{
		cout << "\n" << name << " dies.\n";
	}
}

