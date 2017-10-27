#include <iostream>
#include "globals.h"
#include "room.h"
#include "exit.h"
#include "npc.h"
#include "item.h"
#include "player.h"

using namespace std;

Npc::Npc(const char* name, const char* description, Room* room,vector<Item*> items,float hitpoints) :
Creature(name,description,room,hitpoints),prohibitedItems(items)
{
	inLobby = false;
	posessed = false;
	chargedAttack = false;
	phase = 0;
	type = CREATURE;

	//Attack
	//If dodging 15% atac faillure and 35% proc of reduce half-dmg
	attackProcs[0][0] = 85;
	attackProcs[0][1] = 50;
	//Else 5% proc of atac faillure and 10%proc half dmg
	attackProcs[1][0] = 95;
	attackProcs[1][1] = 85;

	//Stun
	//If dodging 90% stun dodge and 10% proc of reduce half-duration
	stunProcs[0][0] = 10;
	stunProcs[0][1] = 0;
	//If stuning 5% proc of stun faillure 25%proc half duration
	stunProcs[1][0] = 95;
	stunProcs[1][1] = 70;
	//Else 75% proc of stun faillure 20%proc half duration
	stunProcs[2][0] = 25;
	stunProcs[2][1] = 5;

}


Npc::~Npc()
{

}


void Npc::Combat() 
{
	if (!IsAlive() || IsStuned() || IsTiedUp())
		return;

	Creature *target = (Creature*)parent->Find(PLAYER);
	if (!target->IsAlive()) {
		return;
	}

	if (target == nullptr) {
		Cout ("\n'" + target->name + "' is not in the room.\n");
		return;
	}

	if (form == NpcForms::HIGH || form == NpcForms::LOW) {
		int temp = RAND() % 100; //50%attack 20% dodge 10%charged attack 20%stun
		if (temp >= 50)
		{
			combat_target = target;
			action = Action::ATTACK;
			turnCout(name + " prepares his next move " + "!");
		}
		else if (temp >= 30) {
			combat_target = target;
			chargedAttack = true;
			action = Action::ATTACK;
			turnCout(name + " charge power for his next attack " + "!");
		}
		else if (temp >=10) {
			combat_target = target;
			action = Action::STUN;
			turnCout(name + " starts with a spell casting " + "!");
		}
		else
		{
			Dodge();
			turnCout(name + " prepares his next move " + "!");
		}
	}
	else if (form == NpcForms::MEDIUM) {
		int temp = RAND() % 100; //40%attack 20% dodge 30% charged attack 10%stun
		if (temp >= 60)
		{
			combat_target = target;
			action = Action::ATTACK;
			turnCout(name + " prepares his next move " + "!");
		}
		else if (temp >=30)
		{
			combat_target = target;
			chargedAttack = true;
			action = Action::ATTACK;
			turnCout(name + " charge power for his next attack " + "!");
		}
		else if (temp >= 20) {
			combat_target = target;
			action = Action::STUN;
			turnCout(name + " starts with a spell casting " + "!");
		}
		else
		{
			Dodge();
			turnCout(name + " prepares his next move " + "!");
		}
	}
}


void Npc::ReceiveAttack(float damage) {
	if (posessed) {
		if (action == Action::DODGE) {
			turnCout(name + " tries to dodge");
		}
		Creature::ReceiveAttack(damage);
	}
	else
	{
		turnCout("You cant attack your friend");
	}
}

void Npc::ReceiveStun(float duration) {
	if (posessed) {

		if (form == NpcForms::NONE) {
			stuned = duration;
			turnCout("Stun hits");
			return;
		}

		Creature::ReceiveStun(duration);
	}
	else {
		string temp= name + " avoid your hit and look's at you scared.";
		turnCout(temp);
	}
}
void Npc::Turn() 
{

	//Automatic moves when possessed
	if (posessed && !IsTiedUp() && !IsStuned()) {
		RandomMove();
	}

	//Npc become possessed
	if (inLobby && !posessed) {
		Player* player = (Player*)parent->Find(PLAYER);
		if (player !=nullptr) {
			if (player->IsAlive()) {
				turnCout("Your friend touch the chalice and something start to spawn from it. Before you can advise him, a ghost enter in his body.");
				Item* item = (Item*)parent->Find("chalice", ITEM);
				item->ChangeItemType(ItemType::CURSED, ItemType::COMMON);
				posessed = true;
				name = "Mordecai";
				description = "Your friend beeing possessed by a ghost";
				turnCout("");
				turnCout("Ghost: \tWhat a wonderfull day, isn't it?. Im Mordecai a poor citizen death in this church when the war started years ago. Im here trapped in that chalice, if we destroy it, i can go free and release your friends body.");
			}
		}
		else
		{
			Item* item = (Item*)parent->Find("chalice", ITEM);
			item->ChangeItemType(ItemType::CURSED, ItemType::COMMON);
			posessed = true;
			name = "Mordecai";
			description = "Your friend beeing possessed by a ghost";
			phase = -1;
		}
	}

	//First automatic move, when player enter the church
	if (parent->Find(PLAYER) == NULL && !posessed) {
		vector<string> args;
		args.push_back("go");
		args.push_back("east");
		Go(args);
		inLobby = true;
	}

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
						turnCout(name + " attacks " + combat_target->name);
						if (chargedAttack) {
							combat_target->ReceiveAttack(basicDmg * 3);
						}
						else
						{
							combat_target->ReceiveAttack(basicDmg);
						}
					}
					break;
					case Action::STUN:
					{
						turnCout(name + " ended his stun spell " + combat_target->name);

						combat_target->ReceiveStun(basicDmg*2);
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
		else {
			if (IsInCombat()) {
				stuned -= 5;
				if (!IsStuned()) {
					if (parent->Find(PLAYER) != nullptr) {
						turnCout(name + " is no more stuned.");
					}
				}

			}
			else {
				--stuned;
				if (!IsStuned()) {
					if (parent->Find(PLAYER) != nullptr) {
						turnCout(name + " is no more stuned.");
					}
				}
			}
		}
	}

	if (form != NpcForms::NONE) {
		Combat();
	}
}

void Npc::Talk() {
	if (!stuned) {
		if (posessed) {
			if (form == NpcForms::NONE) {
				Player* player = (Player*)parent->Find(PLAYER);
				switch (phase)
				{
				case -1:
				{
					Cout("\n" + name + ":\t" + "What a wonderfull day, isn't it?. Im Mordecai a poor citizen death in this church when the war started years ago, I've possesed your friend when he touched the chalice. Im trapped in that chalice, if we destroy it, i can go free and release your friends body.\n");
					++phase;
				}
				break;
				case 0:
				{
					if (player->Find("Lightning", ITEM)) {
						cout << endl;
						Cout("\n" + name + ":\t" + "Well done, you found 'Lightning', now we can go for the next task\n");
						++phase;
						break;
					}
					cout << endl;
					Cout("\n" + name + ":\t" + "Your first task to destroy the chalice is search 'Lightning' the holy sword somewhere in this church\n");
				}
				break;
				case 1:
				{
					Item* container = (Item*)player->Find("Chalice", ITEM);
					if (container != nullptr) {
						if (container->Find("Wine", ITEM) != nullptr) {
							cout << endl;
							Cout("\n" + name + ":\t" + "Well done, you found the 'Wine', now we can go for the last task\n");
							++phase;
							break;
						}
					}
					cout << endl;
					Cout("\n" + name + ":\t" + "Your second task to destroy the chalice is fill the chalice with 'Wine' the blood of our saviour somewhere in this church\n");
				}
				break;
				case 2: {
					cout << endl;
					Cout("\n" + name + ":\t" + "Now it's time to break this curse, place the filled chalice on the center nave's altar and pierce it with 'Lightning'\n");
				}
						break;
				default:
					break;
				}
			}
			else
			{
				cout << endl;
				Cout("\n" + name + ":\t" + "I will kill you, DIE, DIE, DIE!");
			}
		}
		else {
			cout << endl;
			Cout("\n" + name + ":\t" + "How we will enter in that church?. Maybe the door or the window?\n");
		}
	}
	else
	{
		Cout("\nHe is stuned, he can't talk.\n");
	}
}

void Npc::Observe(Item* item) {
	if (!IsTiedUp() && !IsStuned() && IsAlive()) {
		for (vector<Item*>::const_iterator it = prohibitedItems.begin(); it != prohibitedItems.cend(); ++it) {
			if (*it == item) {
				Cout("\n" + name + " saw you taking " + item->name + ".\n");
				Stab();
			}
		}
	}
}

void Npc::Stab() {
	if (!IsTiedUp() && !IsStuned() && IsAlive()) {
		Player* player = (Player*)parent->Find(PLAYER);
		if (player != nullptr) {
			Cout("\n" + name + " stabs you.\n");
			player->Die();
		}
	}
}

void Npc::Exorciced() {
	((Room*)parent)->BlockAllExits();
	hit_points = 40;
	basicDmg = 5;
	form = NpcForms::LOW;
	stuned = false;
	tiedUp = false;
	Cout("\nWhen you touch your friend with the cross, he start spawning smoke from his entire body covering all exits. When it stops, your friend lies on the floor, and near him you can see an small horned demon.\n");
	name = "Lucifer";
	Cout("\n" + name + ":\t" + "You! you betrayed me, now i will show you no mercy!.\n");
	StartCombat((Creature*)parent->Find(PLAYER));
	SetTurnFrec(COMBATFrec);
}

void Npc::Killed() {
	((Room*)parent)->BlockAllExits();
	hit_points = 100;
	basicDmg = 7;
	form = NpcForms::MEDIUM;
	stuned = false;
	tiedUp = false;
	Cout("\nYou just murdered your friend, when from its body a dense smoke start spawning, blocking all exits and summoning a demon to this world.\n");
	name = "Lucifer";
	Cout("\n" + name + ":\t" + "You just killed your friend, you would be a nice demon. However you frustrate my full resurection, and you will pay for it.\n");
	StartCombat((Creature*)parent->Find(PLAYER));
	SetTurnFrec(COMBATFrec);
}

void Npc::FinalForm(Room* room) {
	ChangeParentTo(room);
	room->BlockAllExits();
	hit_points = 200;
	basicDmg = 10;
	form = NpcForms::HIGH;
	stuned = false;
	tiedUp = false;
	Cout("\nWhen you cut the chalice, the room turns red and all exits disapeared, while a strange smoke start spawning in front of you.\n");
	name = "Lucifer";
	Cout("\n" + name + ":\t" + "Thanks to you now i've recovered my true form! Now you must die!.\n");
	StartCombat((Creature*)parent->Find(PLAYER));
	SetTurnFrec(COMBATFrec);
}

void Npc::Die() {
	if (form == NpcForms::NONE) {
		Cout("\n" + name + " dies.\n");
		Killed();
	}
	else
	{
		EndCombat((Creature*)parent->Find(PLAYER));
		Cout("\n" + name + " dies.\n");
		Win();
	}
}

void Npc::RandomMove() {
	//25% proc for all directions, if there is no exit in that dir, it's the same than stand in the room
	double value = RAND() % 100;
	string direction;
	if (value >= 75) {
		direction = "north";
	}
	else if (value >= 50)
	{
		direction = "south";
	}
	else if (value >= 25) 
	{
		direction = "east";
	}
	else
	{
		direction = "west";
	}
	Go({"go",direction});
}