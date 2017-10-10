#ifndef __Item__
#define __Item__


#include <vector>
#include "entity.h"

class Room;

using namespace std;

enum ItemType
{
	COMMON,
	WEAPON,
	IMMOVABLE,
	CURSED,
	CONTAINER,
	LIQUID_CONTAINER,
	LIQUID,
	BOOK
};

enum WeaponType
{
	SHARP,
	BLUNT
};

class Item : public Entity
{
public:
	Item(const char* name, const char* description, Entity* parent, vector<ItemType> item_type = {COMMON});
	~Item();

	void Look() const;
	void ChangeItemType(ItemType old, ItemType remplace);
	bool Contains(ItemType type);

public:
	vector<ItemType> item_type;
	WeaponType weaponType;//Solo se usa si es un arma
	string bookText;//Solo se usa si es un libro
};

#endif //__Item__