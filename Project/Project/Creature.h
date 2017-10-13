#ifndef __Creature__
#define __Creature__

#include <string>
#include "entity.h"
#include <vector>

class Room;
class Item;


class Creature : public Entity
{
public:
	Creature(const char* name, const char* description, Room* room);
	~Creature();

	virtual void Go(const vector<string>& args);
	virtual void Look(const vector<string>& args) const;
	virtual void Take(const vector<string>& args);
	virtual void Drop(const vector<string>& args);
	virtual void Inventory() const;
	virtual void Lock(const vector<string>& args);
	virtual void UnLock(const vector<string>& args);
	virtual void Turn();
	virtual void Talk();
	virtual void Stun(Item*);
	virtual void TieUp();

	virtual void Attack(const vector<string>& args);
	virtual void Dodge();
	virtual void ReceiveAttack(float damage);
	virtual void Die();


	Room* GetRoom() const;
	bool PlayerInRoom() const;
	bool IsAlive() const;
	bool IsStuned()const;
	bool IsTiedUp()const;

public:
	float basicDmg;
	float hit_points;
	int stuned; //Es un int para indicar los turnos que estara stuneado, depende el arma que lo golpee
	bool tiedUp;
	Creature* combat_target;
	bool dodging;
	Item* weapon;
};

#endif //__Creature__
