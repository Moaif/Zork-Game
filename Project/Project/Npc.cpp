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
	phase = 0;
	type = CREATURE;
}

// ----------------------------------------------------
Npc::~Npc()
{

}

// ----------------------------------------------------
bool Npc::Go(const vector<string>& args)
{
	list<Exit*> exits = GetRoom()->GetExitsByDirection(args[1]);

	if (exits.size() <= 0)
	{
		return false;
	}

	for (list<Exit*>::iterator it = exits.begin(); it != exits.end(); ++it) {
		Exit* ex = (Exit*)*it;
		if (!ex->locked) {
			ChangeParentTo(ex->GetDestinationFrom((Room*)parent));
			Player* player = (Player*)parent->Find(PLAYER);
			if (player != NULL) {
				cout << "\nLuis enter the room.\n";
			}

			return true;
		}
	}
	return false;
}

// ----------------------------------------------------

bool Npc::Attack(const vector<string>& args) 
{
	return false;
}

// ----------------------------------------------------
void Npc::Turn() 
{
	if (inLobby && !posessed) {
		Player* player = (Player*)parent->Find(PLAYER);
		if (player !=nullptr) {
			if (player->IsAlive()) {
				cout << "\nYour friend touch the chalice and something start to spawn from it. Before you can advise him, a ghost enter in his body.";
				Item* item = (Item*)parent->Find("chalice", ITEM);
				item->ChangeItemType(CURSED, COMMON);
				posessed = true;
				name = "Mordecai";
				description = "Your friend beeing possessed by a ghost";
				cout << endl;
				cout << "\n" << "Ghost: \t" << "What a wonderfull day, isn't it?.Im Mordecai a poor citicien death in this church when the war started years ago.Im here trapped in that chalice, if we destroy it, i can go free and release your friends body.\n";
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

	if (parent->Find(PLAYER) == NULL && !posessed) {
		vector<string> args;
		args.push_back("go");
		args.push_back("east");
		Go(args);
		inLobby = true;
	}

	
}

void Npc::Talk() {
	if (!stuned) {
		if (posessed) {
			Player* player = (Player*)parent->Find(PLAYER);
			switch (phase)
			{
			case -1:
				cout << "\n" << "Mordecai: \t" << "What a wonderfull day, isn't it?.Im Mordecai a poor citicien death in this church when the war started years ago, I've possesed your friend when he touched the chalice.Im trapped in that chalice, if we destroy it, i can go free and release your friends body.\n";
				++phase;
				break;
			case 0:
				if (player->Find("Lightning", ITEM)) {
					cout << endl;
					cout << "\n" << "Mordecai: \t" << "Well done, you found 'Lightning', now we can go for the next task\n";
					++phase;
				}
				cout << endl;
				cout << "\n" << "Mordecai: \t" << "Your first taks to destroy the chalice is search 'Lightning' the holy sword somewhere in this church\n";
				break;
			case 1:
				if (player->Find("Chalice", ITEM)->Find("Wine", ITEM)) {
					cout << endl;
					cout << "\n" << "Mordecai: \t" << "Well done, you found the 'Wine', now we can go for the last task\n";
					++phase;
				}
				cout << endl;
				cout << "\n" << "Mordecai: \t" << "Your second taks to destroy the chalice is search 'Wine' the blood of our saviour somewhere in this church\n";
				break;
			case 2:
				cout << endl;
				cout << "\n" << "Mordecai: \t" << "Now it's time to break this curse, place the filled chalice on the center nave altar and cut it with 'Lightning\n";
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
	//Armas romas: roca,cruz,maza
	if (posessed) {
		string wName = weapon->name;
		if (Same(wName, "Rock")) {
			stuned = 3;
			cout << "\n You stuned " << name << ".\n";
		}
		else if (Same(wName , "Cross")) {
			stuned = 5;
			cout << "\n You stuned " << name << ".\n";
		}
		else if (Same(wName , "Mace")) {
			stuned = 10;
			cout << "\n You stuned " << name << ".\n";
		}
	}
	else
	{
		cout << "\n" << name << " avoid your hit and look's at you scared.\n";
	}
}

void Npc::TieUp() {
	tiedUp = true;
}