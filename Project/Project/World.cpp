#include <iostream>
#include "globals.h"
#include "entity.h"
#include "creature.h"
#include "item.h"
#include "exit.h"
#include "room.h"
#include "player.h"
#include "world.h"
#include <vector>
#include "npc.h"

// ----------------------------------------------------

World::World()
{

	// Rooms ----
	Room* gardens = new Room("Gardens", "You are in front of a church, surrounded by the flowers of the church's garden.");
	Room* lobby = new Room("Lobby", "You are now inside the church, on a small room with low ligth, on the background of the room, you can see a shiny chalice.");
	Room* central_nave = new Room("Central Nave", "You move into the big central nave, the biggest part of the church.");
	Room* chapel = new Room("Chapel", "You are surrounded by all the religious items placed on the wall");
	Room* vestry = new Room("Vestry", "The small room seems to not have been open for years.");

	Exit* ex1 = new Exit("east", "west", "Window", gardens, lobby,true,NULL,"Stained glass window");
	Exit* ex2 = new Exit("east", "west", "Door", gardens, lobby,true,NULL,"A wooden door with a tipical key lock");
	Exit* ex3 = new Exit("east", "west", "Corridor", lobby, central_nave, false, NULL,"A simple corridor");
	Exit* ex4 = new Exit("north", "south", "Arch", central_nave, chapel, false, NULL,"A wonderfull arch made on stone");
	Exit* ex5 = new Exit("south","north","Door",central_nave,vestry,true,NULL,"A weird iron door with a big cross lock");

	entities.push_back(gardens);
	entities.push_back(lobby);
	entities.push_back(central_nave);
	entities.push_back(chapel);
	entities.push_back(vestry);

	entities.push_back(ex1);
	entities.push_back(ex2);
	entities.push_back(ex3);
	entities.push_back(ex4);
	entities.push_back(ex5);


	// Items -----
	//Garden
	Item* rock = new Item("Rock", "One simple rock", gardens, { WEAPON });
	rock->weaponType = BLUNT;
	Item* gargoyle = new Item("Gargoyle", "A gargoyle statue with a hole inside", gardens, { IMMOVABLE,CONTAINER });
	gargoyle->maxItems = 1;
	Item* key = new Item("Key", "Old iron key.", gargoyle);
	ex2->key = key;

	//Lobby
	Item* chalice = new Item("Chalice", "An empty gold chalice", lobby, { CURSED,LIQUID_CONTAINER });
	chalice->maxItems = 1;
	Item* cabinet = new Item("Cabinet", "An old wooden cabinet", lobby, {IMMOVABLE,CONTAINER});
	cabinet->maxItems = 3;
	Item* rope = new Item("Rope","A simple rope",cabinet);
	Item* showcase = new Item("Showcase", "A dirty glass showcase", lobby, {IMMOVABLE,CONTAINER});
	showcase->maxItems = 2;
	Item* dagger = new Item("Dagger", "An iron dagger", showcase, {WEAPON});
	dagger->weaponType = SHARP;

	//Central_Nave
	Item* altar = new Item("Altar", "A well furnished medium height altar", central_nave, { IMMOVABLE,CONTAINER });
	altar->maxItems = 3;
	Item* eBook = new Item("Book", "An old book with mysterious drawings", altar, {BOOK});
	eBook->bookText = "\n\t\t\t\t\tThe exorcism Book\n \n\tIn this book you will discover the ritual to exorcise any kind of evil.\n \n\t1 You need to throw holy water over the afected.\n\t2 You must pray chapter from a bible.\n\t3 Finally you have to touch him with a sacred cross.\n";
	Item* stoup = new Item("Stoup", "A stone stoup setter in the wall", central_nave, { IMMOVABLE,LIQUID_CONTAINER });
	stoup->maxItems = 1;
	Item* hwater = new Item("Holy_water", "Water purified by goods", stoup, {LIQUID});


	//Chapel
	Item* chapel_chest = new Item("Chest", "A big beautiful chest", chapel, {IMMOVABLE,CONTAINER});
	chapel_chest->maxItems = 2;
	Item* holy_sword = new Item("Lightning", "The holy sword.", chapel_chest, { WEAPON });
	holy_sword->weaponType = SHARP;
	Item* cross = new Item("Cross", "A sacred cross placed on the wall", chapel, {WEAPON});
	cross->weaponType = BLUNT;
	ex5->key = cross;
	//Vestry
	Item* shelf = new Item("Shelf", "An oxidized shelf", vestry, {IMMOVABLE,CONTAINER});
	shelf->maxItems = 5;
	Item* bible = new Item("Bible", "One of the first copys of the bible", shelf, {BOOK});
	bible->bookText = "\nThe bible is too long to be shown here,but you can still pray some of its chapter.\n";
	Item* phial = new Item("Phial", "A wonderful glass phial", vestry, {LIQUID_CONTAINER});
	phial->maxItems = 1;
	Item* wine = new Item("Wine", "The wine representing the blood of our saviour", phial, {LIQUID});
	Item* mace = new Item("Mace", "An iron mace", vestry,{WEAPON});
	mace->weaponType = BLUNT;

	// Player ----
	player = new Player("Player", "You are investigating a church with your friend", gardens);
	player->hit_points = 25;
	entities.push_back(player);

	//Npc -----
	Npc* luis = new Npc("Luis","Your allways trustable friend",gardens);
	luis->hit_points = 1;
	entities.push_back(luis);
}

// ----------------------------------------------------
World::~World()
{
	for (list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
		delete *it;

	entities.clear();
}

// ----------------------------------------------------
bool World::Turn(vector<string>& args)
{
	bool ret = true;

	if (args.size() > 0 && args[0].length() > 0)
		ret = ParseCommand(args);

	GameLoop();

	return ret;
}

// ----------------------------------------------------
void World::GameLoop()
{

	for (list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
		(*it)->Turn();

}

// ----------------------------------------------------
bool World::ParseCommand(vector<string>& args)
{
	bool ret = true;

	switch (args.size())
	{
	case 1: // commands with no arguments ------------------------------
	{
		if (Same(args[0], "look") || Same(args[0], "l"))
		{
			player->Look(args);
		}
		else if (Same(args[0], "north") || Same(args[0], "n"))
		{
			(args.size() == 1) ? args.push_back("north") : args[1] = "north";
			player->Go(args);
		}
		else if (Same(args[0], "south") || Same(args[0], "s"))
		{
			(args.size() == 1) ? args.push_back("south") : args[1] = "south";
			player->Go(args);
		}
		else if (Same(args[0], "east") || Same(args[0], "e"))
		{
			(args.size() == 1) ? args.push_back("east") : args[1] = "east";
			player->Go(args);
		}
		else if (Same(args[0], "west") || Same(args[0], "w"))
		{
			(args.size() == 1) ? args.push_back("west") : args[1] = "west";
			player->Go(args);
		}
		else if (Same(args[0], "up") || Same(args[0], "u"))
		{
			(args.size() == 1) ? args.push_back("up") : args[1] = "up";
			player->Go(args);
		}
		else if (Same(args[0], "down") || Same(args[0], "d"))
		{
			(args.size() == 1) ? args.push_back("down") : args[1] = "down";
			player->Go(args);
		}
		else if (Same(args[0], "inventory") || Same(args[0], "i"))
		{
			player->Inventory();
		}
		else
			ret = false;
		break;
	}
	case 2: // commands with one argument ------------------------------
	{
		if (Same(args[0], "look") || Same(args[0], "l"))
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

		else if (Same(args[0], "attack") )
		{
			player->Attack(args);
		}
		else if (Same(args[0],"talk") || Same(args[0],"speak")) {
			player->Talk(args);
		}
		else if (Same(args[0], "read")) {
			player->Read(args);
		}
		else
			ret = false;
		break;
	}
	case 3: // commands with two arguments ------------------------------
	{
		break;
	}
	case 4: // commands with three arguments ------------------------------
	{
		if (Same(args[0], "unlock") || Same(args[0], "break") || Same(args[0], "open"))
		{
			player->UnLock(args);
		}
		else if (Same(args[0], "lock") )
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