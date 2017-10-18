#ifndef __Item__
#define __Item__


#include <vector>
#include "entity.h"

class Room;


enum ItemType
{
	COMMON,
	WEAPON,
	IMMOVABLE,
	CURSED,
	CONTAINER,
	LIQUID_CONTAINER,
	LIQUID,
	BOOK,
	DESTRUCTIBLE
};

enum WeaponType
{
	SHARP,
	BLUNT
};

class Item : public Entity
{
public:
	Item(const char* name, const char* description, Entity* parent,vector<ItemType> item_type = { COMMON }, bool locked=false, Entity* key=nullptr);
	~Item();

	void Look() const override;
	void ChangeItemType(ItemType old, ItemType remplace);
	bool Contains(ItemType type)const;
	void DropItems();

public:
	vector<ItemType> item_type;
	bool locked;
	Entity* key;
	WeaponType weaponType;//Solo se usa si es un arma
	float weapondDmg;//Solo si es arma
	string bookText;//Solo se usa si es un libro
	int maxItems; //Solo se usa si es un container
	string cursedText; // Solo se usa si es cursed
	float dmgResistance;//Solo se usa si es destructible
	string keyDescription;//Solo se usa si el item tiene llave
};

#endif //__Item__