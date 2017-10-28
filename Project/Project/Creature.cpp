#include <iostream>
#include "globals.h"
#include "room.h"
#include "exit.h"
#include "item.h"
#include "creature.h"

using namespace std;

Creature::Creature(const char* title, const char* description, Room* room,float hitpoints=1) :
	Entity(title, description, (Entity*)room)
{
	type = CREATURE;
	hit_points = hitpoints;
	weapon = nullptr;
	combat_target = nullptr;
	inCombat = false;
	action = Action::NONE;
	//Attack
	//If dodging 30% atac faillure and60% proc of reduce half-dmg
	attackProcs[0][0] = 70;
	attackProcs[0][1] = 10;
	//Else 5% proc of atac faillure and 10%proc half dmg
	attackProcs[1][0] = 95;
	attackProcs[1][1] = 85;

	//Stun
	//If dodging 25% stun dodge and 50% proc of reduce half-duration
	stunProcs[0][0] = 75;
	stunProcs[0][1] = 25;
	//If stuning 5% proc of stun faillure 10%proc half duration
	stunProcs[1][0] = 95;
	stunProcs[1][1] = 85;
	//Else 5% proc of stun faillure 10%proc half duration
	stunProcs[2][0] = 95;
	stunProcs[2][1] = 85;
}


Creature::~Creature()
{}


void Creature::Look(const vector<string>& args) const
{
	if (IsAlive())
	{
		Cout("\n" + name + "\n");
		Cout(description + "\n");
	}
	else
	{
		Cout(name + "'s corpse\n");
		Cout("Here lies dead: " + description + "\n");
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
				turnCout(name + " leaves the room.");
			}
			ChangeParentTo(ex->GetDestinationFrom((Room*)parent));
			if (PlayerInRoom()) {
				turnCout(name + " enter the room.");
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
						turnCout(name + " try to attack " + combat_target->name);
						if (weapon == nullptr) {
							combat_target->ReceiveAttack(basicDmg);
						}
						else
						{
							combat_target->ReceiveAttack(weapon->weapondDmg);
						}
						action = Action::NONE;
					}
					break;
					case Action::STUN:
					{
						turnCout(name + " try to stun " + combat_target->name);

						combat_target->ReceiveStun(weapon->weapondDmg);
						action = Action::NONE;
					}
					break;
					default:
						break;
					}
				}
			}
			combat_target = nullptr;
		}
		//If in combat, stun disappear faster
		else {
			if (IsInCombat()) {
				stuned -= 5;
				if (!IsStuned()) {
					if (parent->Find(PLAYER) != nullptr) {
						turnCout(name + " is no more stunned.");
					}
				}

			}
			else {
				--stuned;
				if (!IsStuned()) {
					if (parent->Find(PLAYER) != nullptr) {
						turnCout(name + " is no more stunned.");
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
}



void Creature::ReceiveAttack(float damage)
{
	if (action == Action::DODGE){
		int temp = RAND() % 100;//Random value between 0-99
		if (temp >= attackProcs[0][0])
		{
			turnCout(name + " dodged the attack");
		}
		else if (temp >= attackProcs[0][1]) {
			turnCout(name + " dodged half of the attack");
			hit_points -= damage / 2;
		}
		else
		{
			turnCout("Attack hits");
			hit_points -= damage;
		}
		action = Action::NONE;
	}
	else
	{
		int temp = RAND() % 100; 
		if (temp >= attackProcs[1][0])
		{
			turnCout("Attack missed");
		}
		else if (temp >= attackProcs[1][1]) {
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
	if (action == Action::DODGE) {
		int temp = RAND() % 100;
		if (temp >= stunProcs[0][0])
		{
			turnCout(name + " dodged the stun");
		}
		else if (temp >= stunProcs[0][1]) {
			turnCout(name + " dodged half of the stun");
			stuned = duration / 2;
		}
		else
		{
			turnCout("Stun hits");
			stuned = duration;
		}
		action = Action::NONE;
	}
	else if( action == Action::STUN)//This will only be used by monster, cause player will allways go first, then he will be doing dodging, or doing nothing at this point
	{
		int temp = RAND() % 100;
		if (temp >= stunProcs[1][0])
		{
			turnCout("Stun missed");
		}
		else if (temp >= stunProcs[1][1]) {
			turnCout("Stun hits, but with only half force");
			stuned = duration / 2;
		}
		else
		{
			turnCout("Stun hits");
			stuned = duration;
		}
	}
	else
	{
		int temp = RAND() % 100;
		if (temp >= stunProcs[2][0])
		{
			turnCout("Stun missed");
		}
		else if (temp >= stunProcs[2][1]) {
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
		Cout("\n" + name + " dies.\n");
	}
}

