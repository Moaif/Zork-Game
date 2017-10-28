#include <iostream>
#include "globals.h"
#include "room.h"
#include "exit.h"
#include "item.h"
#include "player.h"
#include "Npc.h"

using namespace std;

Player::Player(const char* title, const char* description, Room* room,float hitpoints,float basicDmg) :
	Creature(title, description, room,hitpoints)
{
	this->basicDmg = basicDmg;
	type = PLAYER;
	phase = 0;
	maxItems = 6;
}


Player::~Player()
{
}


void Player::Look(const vector<string>& args) const
{
	if (args.size() > 1)
	{	//Look over the room 
		for (vector<Entity*>::const_iterator it = parent->container.cbegin(); it != parent->container.cend(); ++it)
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
		for (vector<Entity*>::const_iterator it = container.cbegin(); it != container.cend(); ++it) {
			if (Same((*it)->name, args[1]))
			{
				(*it)->Look();
				return;
			}
		}

		//Look for player
		if (Same(args[1], "me"))
		{
			Cout("\n" + name + "\n");
			Cout(description + "\n");
		}

		Cout("\nThere is no '" + args[1] + "' in this room.\n");
	}
	else
	{
		parent->Look();
	}
}


void Player::Go(const vector<string>& args)
{
	list<Exit*> exits = GetRoom()->GetExitsByDirection(args[1]);

	if (exits.size() <= 0)
	{
		Cout("\nThere is no exit at '" + args[1] + "'.\n");
		return;
	}

	for (list<Exit*>::iterator it = exits.begin(); it != exits.end(); ++it) {
		Exit* ex = (Exit*)*it;
		if (!ex->locked) {
			Cout("\nYou take direction " + ex->GetDirectionFrom((Room*)parent) + "...\n");
			ChangeParentTo(ex->GetDestinationFrom((Room*)parent));
			parent->Look();

			return;
		}
	}

	Cout("\nThat direction is locked.\n");
}


void Player::Take(const vector<string>& args)
{
	Npc* npc =(Npc*) parent->Find("Mordecai",CREATURE);//We want to see if the npc is in the room
	if (args.size()==6) {
		Item* item = (Item*)parent->Find(args[3], ITEM);

		// we could pick something from a container in our inventory ...
		if (item == nullptr)
			item = (Item*)Find(args[3], ITEM);

		if (item == nullptr)
		{
			Cout("\nCannot find '" + args[3] + "' in this room or in your inventory.\n");
			return;
		}

		if (item->locked) {
			Cout("\n" + item->name + " is locked.\n");
			return;
		}

		Item* subitem = (Item*)item->Find(args[1], ITEM);

		if (subitem == nullptr)
		{
			Cout("\n" + item->name + " does not contain '" + args[1] + "'.\n");
			return;
		}

		Item* tool = (Item*)parent->Find(args[5],ITEM);

		if (tool == nullptr) {
			tool = (Item*)Find(args[5], ITEM);
		}

		if (tool == nullptr) {
			Cout("\nCannot find '" + args[5] + "' in this room or in your inventory.\n");
		}


		if (subitem->Contains(ItemType::IMMOVABLE)) {
			Cout("\nYou cant take this item from the room. \n");
			return;
		}
		if (subitem->Contains(ItemType::CURSED)) {
			Cout("\n" + subitem->cursedText + "\n");
			Die();
			return;
		}

		if (subitem->Contains(ItemType::LIQUID) && !tool->Contains(ItemType::LIQUID_CONTAINER)){
			Cout("\nYou can't take '" + subitem->name + "' with '" + tool->name + "'.\n");
			return;
		}
		if (!tool->Contains(ItemType::CONTAINER) && !tool->Contains(ItemType::LIQUID_CONTAINER)) {
			Cout("\nYou cant put '" + subitem->name + "' in '" + tool->name + "'.\n");
			return;
		}

		if (tool->maxItems == tool->container.size()) {
			Cout("\n" + tool->name + " is out of space.\n");
			return;
		}

		Cout("\nYou take " + subitem->name + " from " + item->name + ".\n");
		subitem->ChangeParentTo(tool);

		if (npc != nullptr) {
			npc->Observe(subitem);
		}
	}
	else if (args.size() == 4)
	{
		Item* item = (Item*)parent->Find(args[3], ITEM);

		// we could pick something from a container in our inventory ...
		if (item == nullptr)
			item = (Item*)Find(args[3], ITEM);

		if (item == nullptr)
		{
			Cout("\nCannot find '" + args[3] + "' in this room or in your inventory.\n");
			return;
		}

		if (item->locked) {
			Cout("\n" + item->name + " is locked.\n");
			return;
		}

		Item* subitem = (Item*)item->Find(args[1], ITEM);

		if (subitem == nullptr)
		{
			Cout("\n" + item->name + " does not contain '" + args[1] + "'.\n");
			return;
		}
		if (subitem->Contains(ItemType::IMMOVABLE)) {
			Cout("\nYou cant take this item from the room. \n");
			return;
		}
		if (subitem->Contains(ItemType::CURSED)) {
			Cout("\n" + subitem->cursedText + "\n");
			Die();
			return;
		}

		if (subitem->Contains(ItemType::LIQUID)) {
			Cout("\nYou cant take  this item with your hands. \n");
			return;
		}

		if (maxItems == container.size()) {
			Cout("\nYour inventory is out of space.\n");
			return;
		}

		Cout("\nYou take " + subitem->name + " from " + item->name + ".\n");
		subitem->ChangeParentTo(this);

		if (npc != nullptr) {
			npc->Observe(subitem);
		}
	}
	else if (args.size() == 2)
	{
		Item* item = (Item*)parent->Find(args[1], ITEM);

		if (item == nullptr)
		{
			Cout("\nThere is no item here with that name.\n");
			return;
		}

		if(item->Contains(ItemType::CURSED)) {
			Cout("\n" + item->cursedText + "\n");
			Die();
			return;
		}

		if (item->Contains(ItemType::IMMOVABLE)) {
			Cout("\nYou cant take this item from the room. \n");
			return;
		}
		if (item->Contains(ItemType::LIQUID)) {
			Cout("\nYou cant take  this item with your hands. \n");
			return;
		}

		if (maxItems == container.size()) {
			Cout("\nYour inventory is out of space.\n");
			return;
		}

		Cout("\nYou take " + item->name + ".\n");
		item->ChangeParentTo(this);

		if (npc != nullptr) {
			npc->Observe(item);
		}
	}
}


void Player::Inventory() const
{
	list<Entity*> items;
	FindAll(ITEM, items);

	if (items.size() == 0)
	{
		Cout("\nYou do not own any item.\n");
		return;
	}

	for (list<Entity*>::const_iterator it = items.begin(); it != items.cend(); ++it)
	{
			Cout("\n" + (*it)->name);
	}

	cout << endl;
}


void Player::Drop(const vector<string>& args)
{
	if (args.size() == 2)
	{
		Item* item = (Item*)Find(args[1], ITEM);

		if (item == nullptr)
		{
			Cout("\nThere is no item on you with that name.\n");
			return;
		}

		Cout("\nYou drop " + item->name + "...\n");
		item->ChangeParentTo(parent);
	}
	else if (args.size() == 4)
	{
		Item* item = (Item*)Find(args[1], ITEM);

		if (item == nullptr)
		{
			Cout("\nCan not find '" + args[1] + "' in your inventory.\n");
			return;
		}

		Item* container = (Item*)parent->Find(args[3], ITEM);

		if (container == nullptr) {
			container = (Item*)Find(args[3], ITEM);
		}

		if (container == nullptr)
		{
			Cout("\nCan not find '" + args[3] + "' in your inventory or in the room.\n");
			return;
		}
		if (!container->Contains(ItemType::CONTAINER)) {
			Cout("\nYou cant put '" + item->name + "' in '" + container->name + "'.\n");
			return;
		}
		if (container->locked) {
			Cout("\n" + container->name + " is locked.\n");
			return;
		}

		if (container->maxItems == container->container.size()) {
			Cout("\n" + container->name + " is out of space.\n");
			return;
		}

		Cout("\nYou put " + item->name + " into " + container->name + ".\n");
		item->ChangeParentTo(container);
	}
	if (args.size() == 6) {
	
		Item* actualContainer = (Item*)parent->Find(args[3], ITEM);

		if (actualContainer == nullptr) {
			actualContainer = (Item*)Find(args[3], ITEM);
		}

		if (actualContainer == nullptr)
		{
			Cout("\nCan not find '" + args[3] + "' in your inventory or in the room.\n");
			return;
		}

		if (actualContainer->locked) {
			Cout("\n" + actualContainer->name + " is locked.\n");
			return;
		}

		Item* item = (Item*)actualContainer->Find(args[1], ITEM);

		if (item == nullptr)
		{
			Cout("\nCan not find '" + args[1] + "' in '"+ args[3]+"'.\n");
			return;
		}

		Item* container = (Item*)parent->Find(args[5], ITEM);

		if (container == nullptr) {
			container = (Item*)Find(args[5], ITEM);
		}

		if (container == nullptr) {
			Cout("\nCan not find '" + args[5] + "' in your inventory or in the room.\n");
			return;
		}

		if (container->locked) {
			Cout("\n" + container->name + " is locked.\n");
			return;
		}

		if (container->maxItems == container->container.size()) {
			Cout("\n" + container->name + " is out of space.\n");
			return;
		}

		if (container->Contains(ItemType::CONTAINER) && !item->Contains(ItemType::LIQUID)) {
			Cout("\nYou put " + item->name + " into " + container->name + " from " + actualContainer->name + ".\n");
			item->ChangeParentTo(container);
			return;
		}
		if (container->Contains(ItemType::LIQUID_CONTAINER) && item->Contains(ItemType::LIQUID)) {
			Cout("\nYou put " + item->name + " into " + container->name + " from " + actualContainer->name + ".\n");
			item->ChangeParentTo(container);
			return;
		}

		Cout("\nYou cant put '" + item->name + "' in '" + container->name + "'.\n");
	}
}


void Player::Lock(const vector<string>& args)
{
	Exit* exit = GetRoom()->GetExit(args[1]);

	if (exit == nullptr)
	{
		Cout("\nThere is no exit at '" + args[1] + "'.\n");
		return;
	}

	if (exit->locked == true)
	{
		Cout("\nThat exit is already locked.\n");
		return;
	}

	Item* item = (Item*)Find(args[3], ITEM);

	if (item == nullptr)
	{
		Cout("\nItem '" + args[3] + "' not found in your inventory.\n");
		return;
	}

	if (exit->key != item)
	{
		Cout("\nItem '" + item->name + "' is not the key for " + exit->GetDirectionFrom((Room*)parent) + ".\n");
		return;
	}

	Cout("\nYou lock " + exit->GetDirectionFrom((Room*)parent) + "...\n");

	exit->locked = true;
}


void Player::UnLock(const vector<string>& args)
{
	Exit* exit = GetRoom()->GetExit(args[1]);
	Item* container = (Item*)parent->Find(args[1], ITEM);

	if (container == nullptr) {
		container = (Item*)Find(args[1], ITEM);
	}

	if (exit != nullptr)
	{
		if (exit->locked == false)
		{
			Cout("\nThat exit is not locked.\n");
			return;
		}

		Item* item = (Item*)Find(args[3], ITEM);

		if (item == nullptr)
		{
			Cout("\n'" + args[3] + "' not found in your inventory.\n");
			return;
		}

		if (exit->key != item)
		{
			Cout("\nYou cant " +  args[0] + " '" + exit->description + "' with " + item->name + ".\n");
			return;
		}

		Cout("\nYou " + args[0] + " " + exit->GetDirectionFrom((Room*)parent) + "...\n");

		exit->locked = false;
	}

	else if (container != nullptr) {
		if (container->locked == false) {
			Cout("\nThat " + container->name + " is not locked.\n");
			return;
		};

		Item* item = (Item*)Find(args[3], ITEM);

		if (item == nullptr)
		{
			Cout("\n'" + args[3] + "' not found in your inventory.\n");
			return;
		}
		if (container->key != item) {
			Cout("\nYou cant " + args[0] + " '" + container->name + "' with " + item->name + ".\n");
			return;
		}

		Cout("\nYou " + args[0] + " " + container->name + "...\n");

		container->locked = false;

	}

	else
	{
		Cout("\nThere is no '" + args[1] + "'.\n");
	}
}

void Player::Break(const vector<string>& args) {
	Exit* exit = GetRoom()->GetExit(args[1]);
	Item* container = (Item*)parent->Find(args[1],ITEM);

	if (container == nullptr) {
		container = (Item*)Find(args[1],ITEM);
	}

	if (exit != nullptr)
	{
		if (exit->locked == false)
		{
			Cout("\nThat exit is not locked.\n");
			return;
		}

		Item* item = (Item*)Find(args[3], ITEM);

		if (item == nullptr)
		{
			Cout("\n'" + args[3] + "' not found in your inventory.\n");
			return;
		}
		if (exit->key != nullptr) {
			Cout("\nThis exit is not breakeable.\n");
			return;
		}

		if (item->Contains(ItemType::WEAPON)) {
			Cout("\nYou break the " + exit->description + "...\n");
			exit->locked = false;
			exit->description = "Broken_" + exit->description;
			return;
		}
		else
		{
			Cout("\n" + item->name + " is not enough to break " + exit->description + "\n");
			return;
		}
	}

	else if (container != nullptr) {

		Item* item = (Item*)Find(args[3], ITEM);

		if (item == nullptr)
		{
			Cout("\n'" + args[3] + "' not found in your inventory.\n");
			return;
		}
		if (!container->Contains(ItemType::DESTRUCTIBLE)) {
			Cout("\n" + container->name + " is not breakable.\n");
			return;
		}

		if (item->Contains(ItemType::WEAPON)) {
			float minDmg = container->dmgResistance;
			if (item->weaponType == WeaponType::BLUNT) {
				minDmg /= 2;//If it's a blunt weapon, it just need half of the dmg
			}

			if ((item->weapondDmg) >= minDmg) {
				Cout("\nYou break the " + container->name + "...\n");
				container->DropItems();
				container->ChangeParentTo(nullptr);
				return;
			}
		}

		Cout("\n" + item->name + " is not enough to break " + container->name + "\n");
		return;

	}

	else
	{
		Cout("\nThere is no '" + args[1] + "'.\n");
	}
}

void Player::Attack(const vector<string>& args)
{
	if (!IsAlive())
		return;

	if (args.size() == 2) {
		Creature *target = (Creature*)parent->Find(args[1], CREATURE);

		if (target == nullptr) {
			Cout("\n'" + args[1] + "' is not in the room.\n");
			return;
		}

		if (!target->IsAlive()) {
			Cout("\n'" + args[1] + "' is aready dead.\n");
			return;
		}

		weapon = nullptr;
		combat_target = target;
		action = Action::ATTACK;
		turnCout(name + " prepares to attack " + target->name + " with bared hands" + "!");
	}
	else if (args.size() == 4) {
		Creature *target = (Creature*)parent->Find(args[1], CREATURE);

		if (target == nullptr) {
			Cout("\n'" + args[1] + "' is not in the room.\n");
			return;
		}

		if (!target->IsAlive()) {
			Cout("\n'" + args[1] + "' is aready dead.\n");
			return;
		}

		Item* item = (Item*)Find(args[3], ITEM);

		if (item == nullptr) {
			Cout("\n'" + args[3] + "' is not in your inventory");
			return;
		}

		if (!item->Contains(ItemType::WEAPON)) {
			Cout("\n" + item->name + " is not a weapon.\n");
			return;
		}

		weapon = item;
		combat_target = target;
		action = Action::ATTACK;
		turnCout(name + " prepares to attack " + target->name + " with " + item->name + "!");
	}
}

void Player::Dodge() {
	Creature::Dodge();
	turnCout(name + " prepares to dodge.");
}

void Player::Talk(const vector<string>& args) {
	Creature* creature = (Creature*)parent->Find(args[1],CREATURE);
	if (creature == nullptr) {
		Cout("\n'" + args[1] + "' is not in this room.\n");
		return;
	}
	creature->Talk();

}


void Player::Stun(const vector<string>& args) {
	Creature* target = (Creature*)parent->Find(args[1],CREATURE);
	if (target == nullptr) {
		Cout("\n'" + args[1] + "' is not in this room.\n");
		return;
	}
	Item* item = (Item*)Find(args[3],ITEM);
	if (item == nullptr) {
		Cout("\n'" + args[3] + "' is not in your inventory.\n");
	}
	if (item->weaponType == WeaponType::BLUNT && item->Contains(ItemType::WEAPON)) {
		Cout("\nYou prepares to stun " + target->name + " with " + item->name + ".\n");
		combat_target = target;
		weapon = item;
		action = Action::STUN;
		return;
	}

	Cout("\nYou can't do that with '" + item->name + "'\n");

}


void Player::Read(const vector<string>& args) const{
	if (args.size() == 2) {
		Item* item = (Item*)parent->Find(args[1], ITEM);
		if (item == nullptr) {
			item = (Item*)Find(args[1], ITEM);
		}
		if (item == nullptr) {
			Cout("\n'" + args[1] + "' is not in this room or inventory.\n");
			return;
		}

		if (item->Contains(ItemType::BOOK)) {
			Cout(item->bookText);
			return;
		}

		Cout("\n" + item->name + "is not readable.\n");
	}
	else if (args.size() == 4) {
		Item* cont = (Item*)parent->Find(args[3], ITEM);
		if (cont == nullptr) {
			cont = (Item*)Find(args[3],ITEM);
		}

		if (cont == nullptr) {
			Cout("\n'" + args[3] + "' is not in this room or inventory");
			return;
		}

		Item* item = (Item*)cont->Find(args[1], ITEM);
	
		if (item == nullptr) {
			Cout("\n'" + args[1] + "' is not in "+ cont->name + ".\n");
			return;
		}

		if (item->Contains(ItemType::BOOK)) {
			Cout(item->bookText);
			return;
		}

		Cout("\n" + item->name + "is not readable.\n");
	}
}


void Player::TieUp(const vector<string>& args)  {
	if (IsInCombat()) {
		Cout("\nYou cant do it while in combat.\n");
		return;
	}
	Creature* npc = (Creature*)parent->Find(args[1], CREATURE);
	if (npc == nullptr) {
		Cout("\n'" + args[1] + "' is not in the room.\n");
		return;
	}

	Item* item = (Item*)Find(args[3],ITEM);
	if (item == nullptr) {
		Cout("\n'" + args[3] + "' is not in your inventory.\n");
		return;
	}

	if (Same(item->name , "Rope") && npc->IsStuned()) {
		npc->TieUp();
		Cout("\nYou tie up " + npc->name + " with " + item->name + ".\n");
		return;
	}
	if (!Same(item->name,"Rope")) {
		Cout("\nYou can't use " + item->name + " for this.\n");
		return;
	}
	Cout("\n" + npc->name + " evade your action.\n");
}


void Player::Pour(const vector<string>& args) {
	Item* container = (Item*)Find(args[5], ITEM);
	if (container == nullptr) {
		Cout("\n'" + args[5] + "' is not in your inventory.\n");
		return;
	}

	Npc* npc = (Npc*)parent->Find(args[3], CREATURE);
	if (npc == nullptr) {
		Cout("\n'" + args[3] + "' is not in the room.\n");
		return;
	}

	Item* item = (Item*)container->Find(args[1],ITEM);
	if (item == nullptr) {
		Cout("\n'" + args[1] + "' is not in " + container->name + ".\n");
		return;
	}

	if (!item->Contains(ItemType::LIQUID)) {
		Cout("\n" + item->name + "is not pourable.\n");
		return;
	}

	Cout("\n Your poured " + item->name + " over " + npc->name + " from " + container->name + ".\n");

	if (Same(item->name, "Holy_water") && Same(npc->name, "Mordecai")) {
		SetPhase(phase+1);
		npc->Stab();
	}
}


void Player::Pray(const vector<string>& args,int phase) {
	Npc* npc = (Npc*)parent->Find("Mordecai",CREATURE);//In order to verify if it's in the room
	if (args.size() == 2) {
		Item* item = (Item*)parent->Find(args[1], ITEM);
		if (item == nullptr) {
			item = (Item*)Find(args[1], ITEM);
		}
		if (item == nullptr) {
			Cout("\n'" + args[1] + "' is not in this room or inventory.\n");
			return;
		}

		if (!item->Contains(ItemType::BOOK)) {
			Cout("\nYou can't pray the" + item->name + ".\n");
			return;
		}

		Cout("\n You pray the " + item->name + ".\n");
		if (phase == 1) {
			if (Same(item->name, "Chapter7") && npc!=nullptr) {
				SetPhase(phase + 1);
				npc->Stab();
			}
		}
		
	}
	else if (args.size() == 4) {
		Item* cont = (Item*)parent->Find(args[3], ITEM);
		if (cont == nullptr) {
			cont = (Item*)Find(args[3], ITEM);
		}

		if (cont == nullptr) {
			Cout("\n'" + args[3] + "' is not in this room or inventory");
			return;
		}

		Item* item = (Item*)cont->Find(args[1], ITEM);

		if (item == nullptr) {
			Cout("\n'" + args[1] + "' is not in " + cont->name + ".\n");
			return;
		}

		if (!item->Contains(ItemType::BOOK)) {
			Cout("\nYou can't pray the" + item->name + ".\n");
			return;
		}

		Cout("\nYou pray the " + item->name + " from " + cont->name + ".\n");
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
		Cout("\n'" + args[1] + "' is not in the room.\n");
		return;
	}

	Item* item = (Item*)Find(args[3], ITEM);
	if (item == nullptr) {
		Cout("\n'" + args[3] + "' is not in the room.\n");
		return;
	}

	Cout("\nYou touched " + npc->name + " with " + item->name + ".\n");
	if (phase == 2) {
		if (Same(item->name, "Cross") && Same(npc->name, "Mordecai")) {
			((Npc*)npc)->Exorciced();
		}
	}

}


void Player::Pierce(const vector<string>& args) const {
	Item* container = (Item*)parent->Find(args[3], ITEM);
	if (container == nullptr) {
		Cout("\n'" + args[3] + "' is not in the room");
		return;
	}

	Item* item = (Item*)container->Find(args[1],ITEM);
	if (item == nullptr) {
		Cout("\n'" + args[1] + "\' is not in " + container->name + ".\n");
		return;
	}

	Item* tool = (Item*)Find(args[5], ITEM);
	if (tool == nullptr) {
		Cout("\n'" + args[5] + "' is not in your inventory.\n");
		return;
	}

	Item* subItem = (Item*)item->Find("Wine", ITEM);
	if (subItem == nullptr && Same(item->name, "Chalice")) {
		Cout("\nThe chalice should be filled with wine");
	}

	if (Same(tool->name, "Lightning") && Same(container->name, "Altar") && Same(item->name, "Chalice")) {
		npcG->FinalForm((Room*)parent);
	}
	else
	{
		Cout("\nSorry, you missed something in the formula.\n");
	}
}


void Player::Die() {
	Cout("\n" + name + " dies.\n");
	End();
}


int Player::GetPhase() const {
	return phase;
}


void Player::SetPhase(int phase) {
	this->phase = phase;
}

void Player::SetNpcG(Npc* n) {
	npcG = n;
}