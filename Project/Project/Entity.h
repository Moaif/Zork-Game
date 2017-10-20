#ifndef __Entity__
#define __Entity__

#include <string>
#include <list>
#include <iostream>
#include <vector>

enum EntityType
{
	ENTITY,
	ROOM,
	EXIT,
	ITEM,
	CREATURE,
	PLAYER,
};

class Entity
{
public:
	Entity(const char* name, const char* description, Entity* parent);
	virtual ~Entity();

	virtual void Look() const;
	virtual void Turn();

	void	ChangeParentTo(Entity* new_parent);
	bool    Find(Entity* entity) const;
	Entity* Find(EntityType type) const;
	Entity* Find(const std::string& name, EntityType type) const;
	void	FindAll(EntityType type, std::list<Entity*>& list_to_fill) const;

public:
	EntityType type;
	std::string name;
	std::string description;

	Entity* parent;
	std::vector<Entity*> container;
};

#endif //__Entity__