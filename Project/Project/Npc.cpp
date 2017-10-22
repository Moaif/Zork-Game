#include <iostream>
#include "globals.h"
#include "room.h"
#include "exit.h"
#include "npc.h"
#include "item.h"
#include "player.h"

using namespace std;

Npc::Npc(const char* name, const char* description, Room* room,vector<Item*> items) :
Creature(name,description,room),prohibitedItems(items)
{
	inLobby = false;
	posessed = false;
	chargedAttack = false;
	phase = 0;
	type = CREATURE;
}


Npc::~Npc()
{

}


void Npc::Dodge() {
	if (!IsAlive() || IsStuned() || IsTiedUp())
		return;

	action = Action::DODGE;
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
		cout << "\n'" << target->name << "' is not in the room.\n";
		return;
	}

	if (form == NpcForms::HIGH || form == NpcForms::LOW) {
		int temp = RAND() % 100; //50%attack 20% dodge 10%charged attack 20%stun
		if (temp >= 50)
		{
			combat_target = target;
			action = Action::ATTACK;
			string temp = name + " prepares his next move " + "!";
			turnCout(temp);
		}
		else if (temp >= 30) {
			combat_target = target;
			chargedAttack = true;
			action = Action::ATTACK;
			string temp = name + " charge power for his next attack " + "!";
			turnCout(temp);
		}
		else if (temp >=10) {
			combat_target = target;
			action = Action::STUN;
			string temp = name + " starts with a spell casting " + "!";
			turnCout(temp);
		}
		else
		{
			Dodge();
			string temp = name + " prepares his next move " + "!";
			turnCout(temp);
		}
	}
	else if (form == NpcForms::MEDIUM) {
		int temp = RAND() % 100; //40%attack 20% dodge 30% charged attack 10%stun
		if (temp >= 60)
		{
			combat_target = target;
			action = Action::ATTACK;
			string temp= name + " prepares his next move " + "!";
			turnCout(temp);
		}
		else if (temp >=30)
		{
			combat_target = target;
			chargedAttack = true;
			action = Action::ATTACK;
			string temp= name + " charge power for his next attack " + "!";
			turnCout(temp);
		}
		else if (temp >= 20) {
			combat_target = target;
			action = Action::STUN;
			string temp = name + " starts with a spell casting " + "!";
			turnCout(temp);
		}
		else
		{
			Dodge();
			string temp = name + " prepares his next move " + "!";
			turnCout(temp);
		}
	}
}


void Npc::ReceiveAttack(float damage) {
	if (posessed) {
		//If dodging 35% proc of reduce half-dmg and 15% atac faillure
		if (action == Action::DODGE) {
			int temp = RAND() % 100; //Random value between 0-99
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

		if (IsStuned()) {
			duration /= 2;
		}

		//If dodging 10% proc of reduce half-duration and 90% atac faillure
		if (action == Action::DODGE) {

			int temp = RAND() % 100;//Random value between 0-99
			if (temp >= 10)
			{
				string temp = name + " dodged the stun";
				turnCout(temp);
			}
			else  {
				string temp = name + " dodged half of the stun";
				turnCout(temp);
				stuned = duration / 2;
			}
			
		}
		// If casting stun, 5%atac faillure 25% half duration
		else if (action == Action::STUN) {
			int temp = RAND() % 100;
			if (temp >= 95)
			{
				turnCout("Stun missed");
			}
			else if (temp >= 70) {
				turnCout("Stun hits, but with only half force");
				stuned = duration / 2;
			}
			else
			{
				turnCout("Stun hits");
				stuned = duration;
			}
		}
		//75% proc of stun faillure 20%proc half duration		
		else
		{
			int temp = RAND() % 100; 
			if (temp >= 25)
			{
				turnCout("Stun missed");
			}
			else if (temp >= 5) {
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

				string temp="Your friend touch the chalice and something start to spawn from it. Before you can advise him, a ghost enter in his body.";
				turnCout(temp);
				Item* item = (Item*)parent->Find("chalice", ITEM);
				item->ChangeItemType(ItemType::CURSED, ItemType::COMMON);
				posessed = true;
				name = "Mordecai";
				description = "Your friend beeing possessed by a ghost";
				temp= "Ghost: \tWhat a wonderfull day, isn't it?.Im Mordecai a poor citizen death in this church when the war started years ago.Im here trapped in that chalice, if we destroy it, i can go free and release your friends body.";
				turnCout(temp);
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
						string temp = name + " attacks " + combat_target->name;
						turnCout(temp);
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
						string temp = name + " ended his stun spell " + combat_target->name;
						turnCout(temp);

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
					cout << "\n" << name << ":\t" << "What a wonderfull day, isn't it?.Im Mordecai a poor citizen death in this church when the war started years ago, I've possesed your friend when he touched the chalice.Im trapped in that chalice, if we destroy it, i can go free and release your friends body.\n";
					++phase;
				}
				break;
				case 0:
				{
					if (player->Find("Lightning", ITEM)) {
						cout << endl;
						cout << "\n" << name << ":\t" << "Well done, you found 'Lightning', now we can go for the next task\n";
						++phase;
						break;
					}
					cout << endl;
					cout << "\n" << name << ":\t" << "Your first task to destroy the chalice is search 'Lightning' the holy sword somewhere in this church\n";
				}
				break;
				case 1:
				{
					Item* container = (Item*)player->Find("Chalice", ITEM);
					if (container != nullptr) {
						if (container->Find("Wine", ITEM) != nullptr) {
							cout << endl;
							cout << "\n" << name << ":\t" << "Well done, you found the 'Wine', now we can go for the last task\n";
							++phase;
							break;
						}
					}
					cout << endl;
					cout << "\n" << name << ":\t" << "Your second task to destroy the chalice is fill the chalice with 'Wine' the blood of our saviour somewhere in this church\n";
				}
				break;
				case 2: {
					cout << endl;
					cout << "\n" << name << ":\t" << "Now it's time to break this curse, place the filled chalice on the center nave's altar and pierce it with 'Lightning\n";
				}
						break;
				default:
					break;
				}
			}
			else
			{
				cout << endl;
				cout << "\n" << name << ":\t" << "I will kill you, DIE, DIE, DIE!";
			}
		}
		else {
			cout << endl;
			cout << "\n" << name<< ":\t" << "How we will enter in that church?.Maybe the door or the window?\n";
		}
	}
	else
	{
		cout << "\nHe is stuned,he can't talk.\n";
	}
}

void Npc::Observe(Item* item) {
	if (!IsTiedUp() && !IsStuned() && IsAlive()) {
		for (vector<Item*>::const_iterator it = prohibitedItems.begin(); it != prohibitedItems.cend(); ++it) {
			if (*it == item) {
				cout << "\n"<< name << " saw you taking " << item->name << ".\n";
				Stab();
			}
		}
	}
}

void Npc::Stab() {
	if (!IsTiedUp() && !IsStuned() && IsAlive()) {
		Player* player = (Player*)parent->Find(PLAYER);
		if (player != nullptr) {
			cout << "\n" << name << " stabs you.\n";
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
	cout << "\nWhen you touch your friend with the cross, he start spawning smoke from his entire body covering all exits. When it stops, your friend lies on the floor, and near him you can see an small horned demon.\n";
	name = "Lucifer";
	cout <<"\n"<< name <<":\t" << "You! you betrayed me, now i will show you no mercy!.\n";
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
	cout << "\nYou just murdered your friend, when from its body a dense smoke start spawning blocking all exits and summoning a demon to this world.\n";
	name = "Lucifer";
	cout << "\n" << name << ":\t" << "You just killed your friend, you would be a nice demon. However you frustrate my full resurection, and you will pay for it.\n";
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
	cout << "\nWhen you cut the chalice, the room turns red and all exits disapeared, while a strange smoke start spawning in front of you.\n";
	name = "Lucifer";
	cout << "\n" << name << ":\t" << "Thanks to you now i've recovered my true form! Now you must die!.\n";
	StartCombat((Creature*)parent->Find(PLAYER));
	SetTurnFrec(COMBATFrec);
}

void Npc::Die() {
	if (form == NpcForms::NONE) {
		cout << "\n" << name << " dies.\n";
		Killed();
	}
	else
	{
		EndCombat((Creature*)parent->Find(PLAYER));
		cout << "\n" << name << " dies.\n";
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