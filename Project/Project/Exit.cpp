#include <iostream>
#include "room.h"
#include "Exit.h"
#include "globals.h"

using namespace std;

Exit::Exit(const char* direction, const char* opposite_direction, const char* description, Room* origin, Room* destination,bool locked, Entity* key,const char* details) :
	Entity(direction, description, (Entity*)origin),locked(locked),key(key),destination(destination),opposite_direction(opposite_direction),details(details)
{

	destination->container.push_back(this);

	type = EXIT;

}


Exit::~Exit()
{
}



void Exit::Look(const Room* room) const
{
	Cout("\n" + description + " to " + GetDestinationFrom(room)->name + " on the " + GetDirectionFrom(room));
}


void Exit::LookDetails() const {
	string temp = details;//cant place details directly
	Cout("\n" + temp + "\n");
}


const string& Exit::GetDirectionFrom(const Room* room) const
{
	if (room == parent)
		return name;
	if (room == destination)
		return opposite_direction;

	return name;
}


Room* Exit::GetDestinationFrom(const Room* room) const
{
	if (room == parent)
		return destination;
	if (room == destination)
		return (Room*)parent;

	return NULL;
}