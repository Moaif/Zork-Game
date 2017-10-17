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


Npc* npcG;//variable global definida en Npc.h, mas info alli

World::World()
{
	clock_t timer = clock();
	turnFrec = INIT_TurnFrec;

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
	rock->weapondDmg = 3;
	Item* gargoyle = new Item("Gargoyle", "A gargoyle statue with a hole inside", gardens, { IMMOVABLE,CONTAINER });
	gargoyle->maxItems = 1;
	Item* key = new Item("Key", "Old iron key.", gargoyle);
	ex2->key = key;

	entities.push_back(rock);
	entities.push_back(gargoyle);
	entities.push_back(key);

	//Lobby
	Item* chalice = new Item("Chalice", "An empty gold chalice", lobby, { CURSED,LIQUID_CONTAINER });
	chalice->maxItems = 1;
	chalice->cursedText = "You have been possessed by a ghost.";
	Item* cabinet = new Item("Cabinet", "An old wooden cabinet", lobby, {IMMOVABLE,CONTAINER});
	cabinet->maxItems = 3;
	Item* rope = new Item("Rope","A simple rope",cabinet);
	Item* showcase = new Item("Showcase", "A dirty glass showcase", lobby, {IMMOVABLE,CONTAINER});
	showcase->maxItems = 2;
	Item* dagger = new Item("Dagger", "An iron dagger", showcase, {WEAPON});
	dagger->weaponType = SHARP;
	dagger->weapondDmg = 7;

	entities.push_back(chalice);
	entities.push_back(cabinet);
	entities.push_back(rope);
	entities.push_back(showcase);
	entities.push_back(dagger);

	//Central_Nave
	Item* altar = new Item("Altar", "A well furnished medium height altar", central_nave, { IMMOVABLE,CONTAINER });
	altar->maxItems = 3;
	Item* eBook = new Item("Book", "An old book with mysterious drawings", altar, {BOOK});
	eBook->bookText = "\n\t\t\t\t\tThe exorcism Book\n \n\tIn this book you will discover the ritual to exorcise any kind of evil.\n \n\t1 You need to pour holy water over the afected.\n\t2 You must pray chapter 7 from a bible.\n\t3 Finally you have to touch him with a sacred cross.\n";
	Item* stoup = new Item("Stoup", "A stone stoup setter in the wall", central_nave, { IMMOVABLE,LIQUID_CONTAINER });
	stoup->maxItems = 1;
	Item* hwater = new Item("Holy_water", "Water purified by goods", stoup, {LIQUID});

	entities.push_back(altar);
	entities.push_back(eBook);
	entities.push_back(stoup);
	entities.push_back(hwater);

	//Chapel
	Item* chapel_chest = new Item("Chest", "A big beautiful chest", chapel, {IMMOVABLE,CONTAINER});
	chapel_chest->maxItems = 2;
	Item* holy_sword = new Item("Lightning", "The holy sword.", chapel_chest, { WEAPON });
	holy_sword->weaponType = SHARP;
	holy_sword->weapondDmg = 15;
	Item* cross = new Item("Cross", "A sacred cross", chapel, {WEAPON});
	cross->weaponType = BLUNT;
	cross->weapondDmg = 5;
	ex5->key = cross;

	entities.push_back(chapel_chest);
	entities.push_back(holy_sword);
	entities.push_back(cross);

	//Vestry
	Item* shelf = new Item("Shelf", "An oxidized shelf", vestry, {IMMOVABLE,CONTAINER});
	shelf->maxItems = 5;
	Item* bible = new Item("Bible", "An old bible book, with only some of it's chapters", shelf, {CONTAINER});
	bible->maxItems = 5;
	Item* ch1 = new Item("Chapter1", "Genesis", bible, {BOOK});
	ch1->bookText = "\nThe bible is too long to be shown here,but you can still pray the chapter.\n";
	Item* ch4 = new Item("Chapter4", "The forth chapter of the bible", bible, {BOOK});
	ch4->bookText = "\nThe bible is too long to be shown here,but you can still pray the chapter.\n";
	Item* ch7 = new Item("Chapter7", "The seventh chapter of the bible", bible, { BOOK });
	ch7->bookText = "\nThe bible is too long to be shown here,but you can still pray the chapter.\n";
	Item* ch10 = new Item("Chapter10", "The tenth chapter of the bible", bible, { BOOK });
	ch10->bookText = "\nThe bible is too long to be shown here,but you can still pray the chapter.\n";
	Item* ch15 = new Item("Chapter15", "The fifteen chapter of the bible", bible, { BOOK });
	ch15->bookText = "\nThe bible is too long to be shown here,but you can still pray the chapter.\n";
	Item* phial = new Item("Phial", "A wonderful glass phial", vestry, {LIQUID_CONTAINER});
	phial->maxItems = 1;
	Item* wine = new Item("Wine", "The wine representing the blood of our saviour", phial, {LIQUID});
	Item* mace = new Item("Mace", "An iron mace", vestry,{WEAPON});
	mace->weaponType = BLUNT;
	mace->weapondDmg = 10;

	entities.push_back(shelf);
	entities.push_back(bible);
	entities.push_back(ch1);
	entities.push_back(ch4);
	entities.push_back(ch7);
	entities.push_back(ch10);
	entities.push_back(ch15);
	entities.push_back(phial);
	entities.push_back(wine);
	entities.push_back(mace);

	// Player ----
	player = new Player("Player", "You are investigating a church with your friend", gardens);
	player->hit_points = 25;
	player->basicDmg = 1;

	entities.push_back(player);

	//Npc -----
	Npc* luis = new Npc("Luis", "Your allways trustable friend", gardens, {hwater,bible,ch7});
	npcG = luis;
	luis->basicDmg = 1;
	luis->hit_points = 1;

	entities.push_back(luis);

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
			(args.size() == 1) ? args.push_back("north") : args[1] = "north";
			player->Go(args);
		}
		else if (Same(args[0], "south"))
		{
			(args.size() == 1) ? args.push_back("south") : args[1] = "south";
			player->Go(args);
		}
		else if (Same(args[0], "east"))
		{
			(args.size() == 1) ? args.push_back("east") : args[1] = "east";
			player->Go(args);
		}
		else if (Same(args[0], "west"))
		{
			(args.size() == 1) ? args.push_back("west") : args[1] = "west";
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
		else if (Same(args[0],"pray")) {
			player->Pray(args,phase);
		}
		else
			ret = false;
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
		else if (Same(args[0], "pray")) {
			player->Pray(args,phase);
		}
		else if (Same(args[0],"touch")) {
			player->Touch(args,phase);
		}
		else if (Same(args[0], "attack")) {
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