#include <iostream>
#include "globals.h"
#include "Entity.h"

using namespace std;

Entity::Entity(const char* name, const char* description, Entity* parent = NULL):
	name(name),description(description),parent(parent)
{

	type = ENTITY;

	if (parent != NULL)
		parent->container.push_back(this);
}


Entity::~Entity()
{}


void Entity::Look() const
{
	cout << "\n" << name << "\n";
	cout << description << "\n";
}


void Entity::Turn()
{}


void Entity::ChangeParentTo(Entity* new_parent)
{

	bool found = false;
	if (parent != nullptr) {
		parent->container.erase(find(parent->container.begin(),parent->container.end(),this));
	}

	parent = new_parent;

	if (parent != nullptr)
		parent->container.push_back(this);
}


bool Entity::Find(Entity* entity) const
{
	for (vector<Entity*>::const_iterator it = container.cbegin(); it != container.cend(); ++it)
	{
		if ((*it) == entity)
			return true;
	}

	return false;
}


Entity* Entity::Find(EntityType type) const
{
	for (vector<Entity*>::const_iterator it = container.cbegin(); it != container.cend(); ++it)
	{
		if ((*it)->type == type)
			return *it;
	}

	return NULL;
}


Entity* Entity::Find(const string& name, EntityType type) const
{
	for (vector<Entity*>::const_iterator it = container.cbegin(); it != container.cend(); ++it)
	{
		if ((*it)->type == type)
		{
			if (Same((*it)->name, name))
				return *it;
		}
	}

	return NULL;
}


void Entity::FindAll(EntityType type, list<Entity*>& list_to_fill) const
{
	for (vector<Entity*>::const_iterator it = container.cbegin(); it != container.cend(); ++it)
	{
		if ((*it)->type == type)
			list_to_fill.push_back(*it);
	}
}