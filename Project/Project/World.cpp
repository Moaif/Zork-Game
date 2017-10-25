#include <iostream>
#include "globals.h"
#include "entity.h"
#include "creature.h"
#include "item.h"
#include "exit.h"
#include "room.h"
#include "player.h"
#include "world.h"
#include "npc.h"
#include "json.hpp"
#include <fstream>

using json = nlohmann::json;
using namespace std;


World::World()
{
	clock_t timer = clock();
	turnFrec = INIT_TurnFrec;
	//Load data
	LoadJson("Info.json");

	//Subscribe to global
	worldSubscribe(this);
}


World::~World()
{
	for (list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
		delete *it;

	entities.clear();
}


bool World::Turn(vector<string>& args)
{
	bool ret = true;

	if (args.size() > 0 && args[0].length() > 0)
		ret = ParseCommand(args);

	double elapsedTime = (clock() - timer) / CLOCKS_PER_SEC;
	if (elapsedTime >= turnFrec) {
		GameLoop();
		timer = clock();
	}

	return ret;
}


void World::GameLoop()
{

	for (list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
		(*it)->Turn();
	turnCout("------------------------------------------------------------------------------------------");
}


bool World::ParseCommand(vector<string>& args)
{
	bool ret = true;
	int phase = player->GetPhase();
	player->SetPhase(0);

	if (player->IsStuned()) {
			cout << "\nYou are stunned.\n";
			return ret;
	}

	switch (args.size())
	{
	case 1: // commands with no arguments ------------------------------
	{
		if (Same(args[0], "look"))
		{
			player->Look(args);
		}
		else if (Same(args[0],"quit")) {

		}
		else if (Same(args[0], "north"))
		{
			args.push_back("north");
			player->Go(args);
		}
		else if (Same(args[0], "south"))
		{
			args.push_back("south");
			player->Go(args);
		}
		else if (Same(args[0], "east"))
		{
			args.push_back("east");
			player->Go(args);
		}
		else if (Same(args[0], "west"))
		{
			args.push_back("west");
			player->Go(args);
		}
		else if (Same(args[0], "inventory"))
		{
			player->Inventory();
		}
		else if (Same(args[0],"dodge") || Same(args[0], "avoid") || Same(args[0], "evade")) {
			player->Dodge();
		}
		else
			ret = false;
		break;
	}
	case 2: // commands with one argument ------------------------------
	{
		if (Same(args[0], "look"))
		{
			player->Look(args);
		}
		else if (Same(args[0], "go"))
		{
			player->Go(args);
		}
		else if (Same(args[0], "take") || Same(args[0], "pick"))
		{
			player->Take(args);
		}
		else if (Same(args[0], "drop"))
		{
			player->Drop(args);
		}

		else if (Same(args[0], "attack") || Same(args[0], "hit"))
		{
			player->Attack(args);
		}
		else if (Same(args[0],"talk") || Same(args[0],"speak")) {
			player->Talk(args);
		}
		else if (Same(args[0], "read")) {
			player->Read(args);
		}
		else if (Same(args[0],"pray")) {
			player->Pray(args,phase);
		}
		else
			ret = false;
		break;
	}
	case 4: // commands with three arguments ------------------------------
	{
		if (Same(args[0], "unlock") || Same(args[0], "open"))
		{
			player->UnLock(args);
		}
		else if (Same(args[0], "break") || Same(args[0], "destroy")) {
			player->Break(args);
		}
		else if (Same(args[0], "lock"))
		{
			player->Lock(args);
		}
		else if (Same(args[0], "take") || Same(args[0], "pick"))
		{
			player->Take(args);
		}
		else if (Same(args[0], "drop") || Same(args[0], "put"))
		{
			player->Drop(args);
		}
		else if (Same(args[0], "stun") || Same(args[0], "knock_out") || Same(args[0], "daze")) {
			player->Stun(args);
		}
		else if (Same(args[0], "tie") || Same(args[0], "attach")) {
			player->TieUp(args);
		}
		else if (Same(args[0],"read")) {
			player->Read(args);
		}
		else if (Same(args[0], "pray")) {
			player->Pray(args,phase);
		}
		else if (Same(args[0],"touch")) {
			player->Touch(args,phase);
		}
		else if (Same(args[0], "attack") || Same(args[0], "hit")) {
			player->Attack(args);
		}
		else
			ret = false;
		break;
	}
	case 6://commands with four arguments ----------------------------------
		if (Same(args[0], "take") || Same(args[0], "pick")) {
			player->Take(args);
		}
		else if(Same(args[0],"drop") || Same(args[0],"put"))
		{
			player->Drop(args);
		}
		else if (Same(args[0],"pour")) {
			player->Pour(args);
		}
		else if (Same(args[0],"pierce")) {
			player->Pierce(args);
		}
		else
		{
			ret = false;
		}
		break;
	default:
		ret = false;
	}

	return ret;
}

void World::SetTurnFrec(double value) {
	turnFrec = value;
}

void World::LoadJson(string path) {
	json input;
	ifstream ifs(path);
	ifs >> input;

	map<Exit*, string> keyExitDependency;
	map<Item*, string> keyItemDependency;
	map<Entity*, string> positionDependency;
	map<string, Entity*> idMap;


	//Rooms
	for (unsigned int i = 0; i < input["room"].size(); ++i) {
		string id = input["room"][i]["id"];
		string name = input["room"][i]["name"];
		string description = input["room"][i]["description"];

		Room * temp = new Room(name.c_str(), description.c_str());
		entities.push_back(temp);
		idMap[id] = temp;
	}

	//Exits
	for (unsigned int i = 0; i < input["exit"].size(); ++i) {
		string id = input["exit"][i]["id"];
		string direction = input["exit"][i]["direction"];
		string oposite_direction = input["exit"][i]["oposite_direction"];
		string description = input["exit"][i]["description"];
		string origin = input["exit"][i]["origin"];
		string destination = input["exit"][i]["destination"];
		bool locked = input["exit"][i]["locked"];
		string key = input["exit"][i]["key"];
		string details = input["exit"][i]["details"];

		Exit* temp = new Exit(direction.c_str(), oposite_direction.c_str(), description.c_str(), (Room*)idMap[origin], (Room*)idMap[destination], locked, NULL, details.c_str());
		if (key != "") {
			keyExitDependency[temp] = key;
		}
		entities.push_back(temp);
		idMap[id] = temp;

	}

	//Items
	for (unsigned int i = 0; i < input["item"].size(); ++i) {
		string id = input["item"][i]["id"];
		string name = input["item"][i]["name"];
		string description = input["item"][i]["description"];
		string position = input["item"][i]["position"];
		vector<ItemType> type;
		for (unsigned int j = 0; j < input["item"][i]["type"].size(); ++j) {
			if (Same(input["item"][i]["type"][j], "common")) {
				type.push_back(ItemType::COMMON);
			}
			else if (Same(input["item"][i]["type"][j], "weapon")) {
				type.push_back(ItemType::WEAPON);
			}
			else if (Same(input["item"][i]["type"][j], "immovable")) {
				type.push_back(ItemType::IMMOVABLE);
			}
			else if (Same(input["item"][i]["type"][j], "cursed")) {
				type.push_back(ItemType::CURSED);
			}
			else if (Same(input["item"][i]["type"][j], "container")) {
				type.push_back(ItemType::CONTAINER);
			}
			else if (Same(input["item"][i]["type"][j], "liquid_container")) {
				type.push_back(ItemType::LIQUID_CONTAINER);
			}
			else if (Same(input["item"][i]["type"][j], "liquid")) {
				type.push_back(ItemType::LIQUID);
			}
			else if (Same(input["item"][i]["type"][j], "book")) {
				type.push_back(ItemType::BOOK);
			}
			else if (Same(input["item"][i]["type"][j], "destructible")) {
				type.push_back(ItemType::DESTRUCTIBLE);
			}
		}
		bool locked = input["item"][i]["locked"];
		string key = input["item"][i]["key"];

		string wType = input["item"][i]["wType"];
		float wDamage = input["item"][i]["wDamage"];
		string bText = input["item"][i]["bText"];
		int maxItems = input["item"][i]["maxItems"];
		string cText = input["item"][i]["cText"];
		float dmgResistance = input["item"][i]["dmgResistance"];
		string kDescription = input["item"][i]["kDescription"];

		Item * temp = new Item(name.c_str(), description.c_str(), NULL, type, locked, NULL);

		if (wType != "") {
			if (Same(wType, "blunt")) {
				temp->weaponType = WeaponType::BLUNT;
			}
			else if (Same(wType, "sharp")) {
				temp->weaponType = WeaponType::SHARP;
			}
		}

		if (wDamage != 0) {
			temp->weapondDmg = wDamage;
		}

		if (bText != "") {
			temp->bookText = bText;
		}

		if (maxItems != 0) {
			temp->maxItems = maxItems;
		}

		if (cText != "") {
			temp->cursedText = cText;
		}

		if (dmgResistance != 0) {
			temp->dmgResistance = dmgResistance;
		}

		if (kDescription != "") {
			temp->keyDescription = kDescription;
		}

		map<string, Entity*>::iterator it = idMap.find(position);
		if (it != idMap.end()) {
			temp->ChangeParentTo(it->second);
		}
		else
		{
			positionDependency[temp] = position;
		}

		if (key != "") {
			keyItemDependency[temp] = key;
		}
		entities.push_back(temp);
		idMap[id] = temp;
	}

	//Player
	string idP = input["player"]["id"];
	string nameP = input["player"]["name"];
	string descriptionP = input["player"]["description"];
	string positionP = input["player"]["position"];

	float hitpointsP = input["player"]["hitpoints"];
	float basicDmgP = input["player"]["basicDmg"];

	player = new Player(nameP.c_str(), descriptionP.c_str(), (Room*)idMap[positionP], hitpointsP, basicDmgP);

	entities.push_back(player);
	idMap[idP] = player;

	//NPC
	string id = input["npc"]["id"];
	string name = input["npc"]["name"];
	string description = input["npc"]["description"];
	string position = input["npc"]["position"];
	vector<Item*> items;
	for (unsigned int i = 0; i < input["npc"]["items"].size(); ++i) {
		string offset = input["npc"]["items"][i];
		items.push_back((Item*)idMap[offset]);
	}
	float hitpoints = input["npc"]["hitpoints"];

	Npc* npc = new Npc(name.c_str(), description.c_str(), (Room*)idMap[position], items,hitpoints);

	entities.push_back(npc);
	idMap[id] = npc;

	player->SetNpcG(npc);
	


	//Solve dependencies
	for (map <Entity*, string>::iterator it = positionDependency.begin(); it != positionDependency.end();++it) {
		it->first->ChangeParentTo(idMap[it->second]);
	}

	for (map <Exit*, string>::iterator it = keyExitDependency.begin(); it != keyExitDependency.end(); ++it) {
		it->first->key = idMap[it->second];
	}

	for (map<Item*, string>::iterator it = keyItemDependency.begin(); it != keyItemDependency.end(); ++it) {
		it->first->key = idMap[it->second];
	}

}