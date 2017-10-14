#include <iostream>
#include "globals.h"
#include "room.h"
#include "exit.h"
#include "item.h"
#include "player.h"
#include "Npc.h"


// ----------------------------------------------------
Player::Player(const char* title, const char* description, Room* room) :
	Creature(title, description, room)
{
	type = PLAYER;
	phase = 0;
	maxItems = 5;
}

// ----------------------------------------------------
Player::~Player()
{
}

// ----------------------------------------------------
void Player::Look(const vector<string>& args) const
{
	if (args.size() > 1)
	{	//Look over the room 
		for (list<Entity*>::const_iterator it = parent->container.begin(); it != parent->container.cend(); ++it)
		{
			if (Same((*it)->name, args[1])  )
			{
				(*it)->Look();
				return;
			}
			if((*it)->type == EXIT && Same(args[1], ((Exit*)(*it))->description)) {
				((Exit*)(*it))->LookDetails();
				return;
			}
		}
		//Look in inventory
		for (list<Entity*>::const_iterator it = container.begin(); it != container.cend(); ++it) {
			if (Same((*it)->name, args[1]))
			{
				(*it)->Look();
				return;
			}
		}

		//Look for player
		if (Same(args[1], "me"))
		{
			cout << "\n" << name << "\n";
			cout << description << "\n";
		}
	}
	else
	{
		parent->Look();
	}
}

// ----------------------------------------------------
void Player::Go(const vector<string>& args)
{
	list<Exit*> exits = GetRoom()->GetExitsByDirection(args[1]);

	if (exits.size() <= 0)
	{
		cout << "\nThere is no exit at '" << args[1] << "'.\n";
		return;
	}

	for (list<Exit*>::iterator it = exits.begin(); it != exits.end(); ++it) {
		Exit* ex = (Exit*)*it;
		if (!ex->locked) {
			cout << "\nYou take direction " << ex->GetDirectionFrom((Room*)parent) << "...\n";
			ChangeParentTo(ex->GetDestinationFrom((Room*)parent));
			parent->Look();

			return;
		}
	}

	cout << "\nThat exit is locked.\n";
}


// ----------------------------------------------------
void Player::Take(const vector<string>& args)
{
	if (args.size()==6) {
		Item* item = (Item*)parent->Find(args[3], ITEM);

		// we could pick something from a container in our inventory ...
		if (item == nullptr)
			item = (Item*)Find(args[3], ITEM);

		if (item == nullptr)
		{
			cout << "\nCannot find '" << args[3] << "' in this room or in your inventory.\n";
			return;
		}

		Item* subitem = (Item*)item->Find(args[1], ITEM);

		if (subitem == nullptr)
		{
			cout << "\n" << item->name << " does not contain '" << args[1] << "'.\n";
			return;
		}

		Item* tool = (Item*)parent->Find(args[5],ITEM);

		if (tool == nullptr) {
			tool = (Item*)Find(args[5], ITEM);
		}

		if (tool == nullptr) {
			cout << "\n Cannot find '" << args[5] << "' in this room or in your inventory.\n";
		}


		if (subitem->Contains(IMMOVABLE)) {
			cout << "\n You cant take this item from the room. \n";
			return;
		}
		if (subitem->Contains(CURSED)) {
			Die();
			return;
		}

		if (subitem->Contains(LIQUID) && !tool->Contains(LIQUID_CONTAINER)){
			cout << "\n You can't take '" << subitem->name << "' with '" << tool->name << "'.\n";
			return;
		}
		if (!tool->Contains(CONTAINER) && !tool->Contains(LIQUID_CONTAINER)) {
			cout << "\n You cant put '" << subitem->name << "' in '" << tool->name << "'.\n";
			return;
		}

		if (tool->maxItems == tool->container.size()) {
			cout << "\n" << tool->name << " is out of space.\n";
			return;
		}

		cout << "\nYou take " << subitem->name << " from " << item->name << ".\n";
		subitem->ChangeParentTo(tool);
	}
	else if (args.size() == 4)
	{
		Item* item = (Item*)parent->Find(args[3], ITEM);

		// we could pick something from a container in our inventory ...
		if (item == nullptr)
			item = (Item*)Find(args[3], ITEM);

		if (item == nullptr)
		{
			cout << "\nCannot find '" << args[3] << "' in this room or in your inventory.\n";
			return;
		}

		Item* subitem = (Item*)item->Find(args[1], ITEM);

		if (subitem == nullptr)
		{
			cout << "\n" << item->name << " does not contain '" << args[1] << "'.\n";
			return;
		}
		if (subitem->Contains(IMMOVABLE)) {
			cout << "\n You cant take this item from the room. \n";
			return;
		}
		if (subitem->Contains(CURSED)) {
			Die();
			return;
		}

		if (subitem->Contains(LIQUID)) {
			cout << "\n You cant take  this item with your hands. \n";
			return;
		}

		if (maxItems == container.size()) {
			cout << "\n Your inventory is out of space.\n";
			return;
		}

		cout << "\nYou take " << subitem->name << " from " << item->name << ".\n";
		subitem->ChangeParentTo(this);
	}
	else if (args.size() == 2)
	{
		Item* item = (Item*)parent->Find(args[1], ITEM);

		if (item == nullptr)
		{
			cout << "\nThere is no item here with that name.\n";
			return;
		}

		if(item->Contains(CURSED)) {
			Die();
			return;
		}

		if (item->Contains(IMMOVABLE)) {
			cout << "\n You cant take this item from the room. \n";
			return;
		}
		if (item->Contains(LIQUID)) {
			cout << "\n You cant take  this item with your hands. \n";
			return;
		}

		if (maxItems == container.size()) {
			cout << "\n Your inventory is out of space.\n";
			return;
		}

		cout << "\nYou take " << item->name << ".\n";
		item->ChangeParentTo(this);
	}
}

// ----------------------------------------------------
void Player::Inventory() const
{
	list<Entity*> items;
	FindAll(ITEM, items);

	if (items.size() == 0)
	{
		cout << "\nYou do not own any item.\n";
		return;
	}

	for (list<Entity*>::const_iterator it = items.begin(); it != items.cend(); ++it)
	{
			cout << "\n" << (*it)->name;
	}

	cout << "\n";
}

// ----------------------------------------------------
void Player::Drop(const vector<string>& args)
{
	if (args.size() == 2)
	{
		Item* item = (Item*)Find(args[1], ITEM);

		if (item == nullptr)
		{
			cout << "\nThere is no item on you with that name.\n";
			return;
		}

		cout << "\nYou drop " << item->name << "...\n";
		item->ChangeParentTo(parent);
	}
	else if (args.size() == 4)
	{
		Item* item = (Item*)Find(args[1], ITEM);

		if (item == nullptr)
		{
			cout << "\nCan not find '" << args[1] << "' in your inventory.\n";
			return;
		}

		Item* container = (Item*)parent->Find(args[3], ITEM);

		if (container == nullptr) {
			container = (Item*)Find(args[3], ITEM);
		}

		if (container == nullptr)
		{
			cout << "\nCan not find '" << args[3] << "' in your inventory or in the room.\n";
			return;
		}
		if (!container->Contains(CONTAINER)) {
			cout << "\n You cant put '" << item->name << "' in '" << container->name << "'.\n";
			return;
		}

		if (container->maxItems == container->container.size()) {
			cout << "\n" << container->name << " is out of space.\n";
			return;
		}

		cout << "\nYou put " << item->name << " into " << container->name << ".\n";
		item->ChangeParentTo(container);
	}
	if (args.size() == 6) {
	
		Item* actualContainer = (Item*)parent->Find(args[3], ITEM);

		if (actualContainer == nullptr) {
			actualContainer = (Item*)Find(args[3], ITEM);
		}

		if (actualContainer == nullptr)
		{
			cout << "\nCan not find '" << args[3] << "' in your inventory or in the room.\n";
			return;
		}

		Item* item = (Item*)actualContainer->Find(args[1], ITEM);

		if (item == nullptr)
		{
			cout << "\nCan not find '" << args[1] << "' in '"<< args[3]<<"'.\n";
			return;
		}

		Item* container = (Item*)parent->Find(args[5], ITEM);

		if (container == nullptr) {
			container = (Item*)Find(args[5], ITEM);
		}

		if (container == nullptr) {
			cout << "\nCan not find '" << args[5] << "' in your inventory or in the room.\n";
			return;
		}

		if (container->maxItems == container->container.size()) {
			cout << "\n" << container->name << " is out of space.\n";
			return;
		}

		if (container->Contains(CONTAINER) && !item->Contains(LIQUID)) {
			cout << "\nYou put " << item->name << " into " << container->name <<" from " << actualContainer->name << ".\n";
			item->ChangeParentTo(container);
			return;
		}
		if (container->Contains(LIQUID_CONTAINER) && item->Contains(LIQUID)) {
			cout << "\nYou put " << item->name << " into " << container->name << " from " << actualContainer->name << ".\n";
			item->ChangeParentTo(container);
			return;
		}

		cout << "\n You cant put '" << item->name << "' in '" << container->name << "'.\n";
	}
}

// ----------------------------------------------------
void Player::Lock(const vector<string>& args)
{
	if (!IsAlive())
		return;

	Exit* exit = GetRoom()->GetExit(args[1]);

	if (exit == nullptr)
	{
		cout << "\nThere is no exit at '" << args[1] << "'.\n";
		return;
	}

	if (exit->locked == true)
	{
		cout << "\nThat exit is already locked.\n";
		return;
	}

	Item* item = (Item*)Find(args[3], ITEM);

	if (item == nullptr)
	{
		cout << "\nItem '" << args[3] << "' not found in your inventory.\n";
		return;
	}

	if (exit->key != item)
	{
		cout << "\nItem '" << item->name << "' is not the key for " << exit->GetDirectionFrom((Room*)parent) << ".\n";
		return;
	}

	cout << "\nYou lock " << exit->GetDirectionFrom((Room*)parent) << "...\n";

	exit->locked = true;
}

// ----------------------------------------------------
void Player::UnLock(const vector<string>& args)
{
	Exit* exit = GetRoom()->GetExit(args[1]);

	if (exit == nullptr)
	{
		cout << "\nThere is no '" << args[1] << "'.\n";
		return;
	}

	if (exit->locked == false)
	{
		cout << "\nThat exit is not locked.\n";
		return;
	}

	Item* item = (Item*)Find(args[3], ITEM);

	if (item == nullptr)
	{
		cout << "\n'" << args[3] << "' not found in your inventory.\n";
		return;
	}
	if (exit->key == nullptr) {
		if (item->Contains(WEAPON)) {
			cout << "\nYou break the " << exit->description << "...\n";
			exit->locked = false;
			exit->description = "Broken " + exit->description;
			return;
		}
		else
		{
			cout << "\n" << item->name << " is not enough to break " << exit->description << "\n";
			return;
		}
	}
	if (exit->key != item)
	{
		cout << "\nYou cant unlock '" << exit-> description << "' with " << item->name << ".\n";
		return;
	}

	cout << "\nYou unlock " << exit->GetDirectionFrom((Room*)parent) << "...\n";

	exit->locked = false;
}

void Player::Talk(const vector<string>& args) {
	Creature* creature = (Creature*)parent->Find(args[1],CREATURE);
	if (creature == nullptr) {
		cout << "\n'" << args[1] << "' is not in this room.\n";
		return;
	}
	creature->Talk();

}

void Player::Stun(const vector<string>& args) {
	Creature* npc = (Creature*)parent->Find(args[1],CREATURE);
	if (npc == nullptr) {
		cout << "\n'" << args[1] << "' is not in this room.\n";
		return;
	}
	Item* item = (Item*)Find(args[3],ITEM);
	if (item == nullptr) {
		cout << "\n'" << args[3] << "' is not in your inventory.\n";
	}
	if (item->weaponType == BLUNT && item->Contains(WEAPON)) {
		cout << "\n You try to hit " << npc->name << " with " << item->name << ".\n";
		npc->Stun(item);
		return;
	}

	cout << "\n You can't do that with '"<< item->name <<"'\n";

}

void Player::Read(const vector<string>& args) const{
	if (args.size() == 2) {
		Item* item = (Item*)parent->Find(args[1], ITEM);
		if (item == nullptr) {
			item = (Item*)Find(args[1], ITEM);
		}
		if (item == nullptr) {
			cout << "\n'" << args[1] << "' is not in this room or inventory.\n";
			return;
		}

		if (item->Contains(BOOK)) {
			cout << item->bookText;
			return;
		}

		cout << "\n" << item->name << "is not readable.\n";
	}
	else if (args.size() == 4) {
		Item* cont = (Item*)parent->Find(args[3], ITEM);
		if (cont == nullptr) {
			cont = (Item*)Find(args[3],ITEM);
		}

		if (cont == nullptr) {
			cout << "\n'" << args[3] << "' is not in this room or inventory";
			return;
		}

		Item* item = (Item*)cont->Find(args[1], ITEM);
	
		if (item == nullptr) {
			cout << "\n'" << args[1] << "' is not in "<< cont->name <<".\n";
			return;
		}

		if (item->Contains(BOOK)) {
			cout << item->bookText;
			return;
		}

		cout << "\n" << item->name << "is not readable.\n";
	}
}

void Player::TieUp(const vector<string>& args) const {
	Creature* npc = (Creature*)parent->Find(args[1], CREATURE);
	if (npc == nullptr) {
		cout << "\n'" << args[1] << "' is not in the room.\n";
		return;
	}

	Item* item = (Item*)Find(args[3],ITEM);
	if (item == nullptr) {
		cout << "\n'" << args[3] << "' is not in your inventory.\n";
		return;
	}

	if (Same(item->name , "Rope") && npc->IsStuned()) {
		npc->TieUp();
		cout << "\nYou tie up " << npc->name << " with " << item->name << ".\n";
		return;
	}
	if (!Same(item->name,"Rope")) {
		cout << "\nYou can't use " << item->name << " for this.\n";
		return;
	}
	cout << "\n" << npc->name << " evade your action.\n";
}

void Player::Pour(const vector<string>& args) {
	Item* container = (Item*)Find(args[5], ITEM);
	if (container == nullptr) {
		cout << "\n'" << args[5] << "' is not in your inventory.\n";
		return;
	}

	Creature* npc = (Creature*)parent->Find(args[3], CREATURE);
	if (npc == nullptr) {
		cout << "\n'" << args[3] << "' is not in the room.\n";
		return;
	}

	Item* item = (Item*)container->Find(args[1],ITEM);
	if (item == nullptr) {
		cout << "\n'" << args[1] << "' is not in " << container->name << ".\n";
		return;
	}

	if (!item->Contains(LIQUID)) {
		cout << "\n" << item->name << "is not pourable.\n";
	}

	cout << "\n Your poured " << item->name << " over " << npc->name << " from " << container->name << ".\n";

	if (Same(item->name, "Holy_water") && Same(npc->name, "Mordecai")) {
		SetPhase(phase+1);
	}
}
void Player::Pray(const vector<string>& args,int phase) {
	Creature* npc = (Creature*)parent->Find("Mordecai",CREATURE);//In order to verify if it's in the room
	if (args.size() == 2) {
		Item* item = (Item*)parent->Find(args[1], ITEM);
		if (item == nullptr) {
			item = (Item*)Find(args[1], ITEM);
		}
		if (item == nullptr) {
			cout << "\n'" << args[1] << "' is not in this room or inventory.\n";
			return;
		}

		if (!item->Contains(BOOK)) {
			cout << "\nYou can't pray the" << item->name << ".\n";
			return;
		}

		cout << "\n You pray the " << item->name << ".\n";
		if (phase == 1) {
			if (Same(item->name, "Chapter7") && npc!=nullptr) {
				SetPhase(phase + 1);
			}
		}
		
	}
	else if (args.size() == 4) {
		Item* cont = (Item*)parent->Find(args[3], ITEM);
		if (cont == nullptr) {
			cont = (Item*)Find(args[3], ITEM);
		}

		if (cont == nullptr) {
			cout << "\n'" << args[3] << "' is not in this room or inventory";
			return;
		}

		Item* item = (Item*)cont->Find(args[1], ITEM);

		if (item == nullptr) {
			cout << "\n'" << args[1] << "' is not in " << cont->name << ".\n";
			return;
		}

		if (!item->Contains(BOOK)) {
			cout << "\nYou can't pray the" << item->name << ".\n";
			return;
		}

		cout << "\n You pray the " << item->name << " from "<<cont->name << ".\n";
		if (phase == 1) {
			if (Same(item->name, "Chapter7") && npc!=nullptr) {
				SetPhase(phase + 1);
			}
		}
	}
}
void Player::Touch(const vector<string>& args,int phase) {
	Creature* npc = (Creature*)parent->Find(args[1],CREATURE);
	if (npc == nullptr) {
		cout << "\n'" << args[1] << "' is not in the room.\n";
		return;
	}

	Item* item = (Item*)Find(args[3], ITEM);
	if (item == nullptr) {
		cout << "\n'" << args[3] << "' is not in the room.\n";
		return;
	}

	cout << "\nYou touched " << npc->name << " with " << item->name << ".\n";
	if (phase == 2) {
		if (Same(item->name, "Cross") && Same(npc->name, "Mordecai")) {
			((Npc*)npc)->Exorciced();
		}
	}

}
void Player::Pierce(const vector<string>& args) const {
	Item* container = (Item*)parent->Find(args[3], ITEM);
	if (container == nullptr) {
		cout << "\n'" << args[3] << "' is not in the room";
		return;
	}

	Item* item = (Item*)container->Find(args[1],ITEM);
	if (item == nullptr) {
		cout << "\n'" << "' is not in " << container->name << ".\n";
		return;
	}

	Item* tool = (Item*)Find(args[5], ITEM);
	if (tool == nullptr) {
		cout << "\n'" << args[5] << "' is not in your inventory.\n";
		return;
	}

	Item* subItem = (Item*)item->Find("Wine", ITEM);
	if (subItem == nullptr && Same(item->name, "Chalice")) {
		cout << "\n The chalice should be filled with wine";
	}

	if (Same(tool->name, "Lightning") && Same(container->name, "Altar") && Same(item->name, "Chalice")) {
		npcG->FinalForm((Room*)parent);
	}
	else
	{
		cout << "\nSorry, you missed something in the formula.\n";
	}
}

void Player::Die() {
	cout << "\n" << name << " dies.\n";
	End();
}

int Player::GetPhase() const {
	return phase;
}

void Player::SetPhase(int phase) {
	this->phase = phase;
}