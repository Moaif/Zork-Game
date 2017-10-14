#include <iostream>
#include "globals.h"
#include "room.h"
#include "exit.h"
#include "npc.h"
#include "item.h"
#include "player.h"

// ----------------------------------------------------
Npc::Npc(const char* name, const char* description, Room* room) :
Creature(name,description,room)
{
	inLobby = false;
	posessed = false;
	chargedAttack = false;
	phase = 0;
	type = CREATURE;
}

// ----------------------------------------------------
Npc::~Npc()
{

}


// ----------------------------------------------------

void Npc::Combat() 
{
	if (!IsAlive() || IsStuned() || IsTiedUp())
		return;

	Creature *target = (Creature*)parent->Find(PLAYER);

	if (target == nullptr) {
		cout << "\n'" << target->name << "' is not in the room.\n";
		return;
	}

	if (form == NpcForms::HIGH || form == NpcForms::LOW) {
		int temp = rand() % 100; //70%attack 20% dodge 10%charged attack
		if (temp >= 30)
		{
			combat_target = target;
			string temp =  name + " prepares to attack " + target->name + "!\n";
			turnCout(temp);
		}
		else if (temp >= 20) {
			combat_target = target;
			chargedAttack = true;
			string temp = name + " charge power for his next attack " + "!\n";
			turnCout(temp);
		}
		else
		{
			Dodge();
		}
	}
	else if (form == NpcForms::MEDIUM) {
		int temp = rand() % 100; //40%attack 30% dodge 30% charged attack
		if (temp >= 60)
		{
			combat_target = target;
			string temp= name + " prepares to attack " + target->name + "!\n";
			turnCout(temp);
		}
		else if (temp >=30)
		{
			combat_target = target;
			chargedAttack = true;
			string temp= name + " charge power for his next attack " + "!\n";
			turnCout(temp);
		}
		else
		{
			Dodge();
		}
	}
}

// ----------------------------------------------------
void Npc::ReceiveAttack(float damage) {
	if (posessed) {
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
		dodging = false;
		if (hit_points <= 0) {
			Die();
		}
	}
	else
	{
		turnCout("You cant attack your friend");
	}
}

// ----------------------------------------------------
void Npc::Turn() 
{

	if (parent->Find(PLAYER) == NULL && !posessed) {
		vector<string> args;
		args.push_back("go");
		args.push_back("east");
		Go(args);
		inLobby = true;
	}

	if (inLobby && !posessed) {
		Player* player = (Player*)parent->Find(PLAYER);
		if (player !=nullptr) {
			if (player->IsAlive()) {

				string temp="Your friend touch the chalice and something start to spawn from it. Before you can advise him, a ghost enter in his body.";
				turnCout(temp);
				Item* item = (Item*)parent->Find("chalice", ITEM);
				item->ChangeItemType(CURSED, COMMON);
				posessed = true;
				name = "Mordecai";
				description = "Your friend beeing possessed by a ghost";
				temp= "Ghost: \tWhat a wonderfull day, isn't it?.Im Mordecai a poor citicien death in this church when the war started years ago.Im here trapped in that chalice, if we destroy it, i can go free and release your friends body.\n";
				turnCout(temp);
			}
		}
		else
		{
			Item* item = (Item*)parent->Find("chalice", ITEM);
			item->ChangeItemType(CURSED, COMMON);
			posessed = true;
			name = "Mordecai";
			description = "Your friend beeing possessed by a ghost";
			phase = -1;
		}
	}


	if (combat_target != nullptr) {
		if (combat_target->IsAlive() && IsAlive()) {
			if (parent->Find(combat_target) == true)
			{
				string temp = name + " attacks " + combat_target->name;
				turnCout(temp);
				if (chargedAttack) {
					combat_target->ReceiveAttack((basicDmg * 3));
				}
				else
				{
					combat_target->ReceiveAttack(basicDmg);
				}
			}
			if (!combat_target->IsAlive()) {
				return;
			}
			combat_target = nullptr;
		}
	}

	if (form != NpcForms::NONE) {
		Combat();
	}
	
}

void Npc::Talk() {
	if (!stuned) {
		if (posessed) {
			Player* player = (Player*)parent->Find(PLAYER);
			switch (phase)
			{
			case -1:
			{
				cout << "\n" << "Mordecai: \t" << "What a wonderfull day, isn't it?.Im Mordecai a poor citicien death in this church when the war started years ago, I've possesed your friend when he touched the chalice.Im trapped in that chalice, if we destroy it, i can go free and release your friends body.\n";
				++phase;
			}
				break;
			case 0:
			{
				if (player->Find("Lightning", ITEM)) {
					cout << endl;
					cout << "\n" << "Mordecai: \t" << "Well done, you found 'Lightning', now we can go for the next task\n";
					++phase;
					break;
				}
				cout << endl;
				cout << "\n" << "Mordecai: \t" << "Your first taks to destroy the chalice is search 'Lightning' the holy sword somewhere in this church\n";
			}
				break;
			case 1:
			{
				Item* container = (Item*)player->Find("Chalice", ITEM);
				if (container != nullptr) {
					if (container->Find("Wine", ITEM) != nullptr) {
						cout << endl;
						cout << "\n" << "Mordecai: \t" << "Well done, you found the 'Wine', now we can go for the last task\n";
						++phase;
						break;
					}
				}
				cout << endl;
				cout << "\n" << "Mordecai: \t" << "Your second taks to destroy the chalice is search 'Wine' the blood of our saviour somewhere in this church\n";
			}
			break;
			case 2: {
					cout << endl;
					cout << "\n" << "Mordecai: \t" << "Now it's time to break this curse, place the filled chalice on the center nave's altar and pierce it with 'Lightning\n";
			}
				break;
			default:
				break;
			}
		}
		else {
			cout << endl;
			cout << "\n" << "Friend: \t" << "How we will enter in that church?.Maybe the door or the window?\n";
		}
	}
	else
	{
		cout << "\nHe is stuned,he can't talk.\n";
	}
}

void Npc::Stun(Item* weapon) {
	if (posessed) {
		if (form == NpcForms::NONE) {
			stuned = (int)weapon->weapondDmg;
			cout << "\n You stuned " << name << ".\n";
		}
		else
		{
			cout << "\nYou cant stun "<< name <<".\n";
		}
	}
	else
	{
		cout << "\n" << name << " avoid your hit and look's at you scared.\n";
	}
}


void Npc::Exorciced() {
	((Room*)parent)->BlockAllExits();
	hit_points = 30;
	basicDmg = 5;
	form = NpcForms::LOW;
	stuned = false;
	tiedUp = false;
	cout << "\nWhen you touch your friend with the cross, he start spawning smoke from his entire body covering all exits. When it stops, your friend lies on the floor, and near him you can see an small horned demon.\n";
	name = "Lucifer";
	cout <<"\n"<< name <<":\t" << "You! you betrayed me, now i will show you no mercy!.\n";
}

void Npc::Killed() {
	((Room*)parent)->BlockAllExits();
	hit_points = 66;
	basicDmg = 6;
	form = NpcForms::MEDIUM;
	stuned = false;
	tiedUp = false;
	cout << "\n You just murdered your friend, when from its body a dense smoke start spawning blocking all exits and summoning a demon to this world.\n";
	name = "Lucifer";
	cout << "\n" << name << ":\t" << "You just killed your friend, you would be a nice demon. However you frustrate my full resurection, and you will pay for it.\n";
}

void Npc::FinalForm(Room* room) {
	ChangeParentTo(room);
	room->BlockAllExits();
	hit_points = 666;
	basicDmg = 666;
	form = NpcForms::HIGH;
	stuned = false;
	tiedUp = false;
	cout << "\n When you cut the chalice, the room turns red and all exits disapeared, while a strange smoke start spawning in front of you.\n";
	name = "Lucifer";
	cout << "\n" << name << ":\t" << "Thanks to you now i've recovered my true form! Now you must die!.\n";
}

void Npc::Die() {
	if (form == NpcForms::NONE) {
		cout << "\n" << name << " dies.\n";
		Killed();
	}
	else
	{
		cout << "\n" << name << " dies.\n";
		Win();
	}
}