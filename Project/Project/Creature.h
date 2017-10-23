#ifndef __Creature__
#define __Creature__

#include <string>
#include "entity.h"
#include <vector>

class Room;
class Item;

enum class Action
{
	NONE,
	ATTACK,
	STUN,
	DODGE
}; 


class Creature : public Entity
{
public:
	Creature(const char* name, const char* description, Room* room);
	~Creature();

	virtual void Go(const std::vector<std::string>& args);
	virtual void Look(const std::vector<std::string>& args) const;
	virtual void Turn() override;
	virtual void Talk();
	virtual void TieUp();

	virtual void Dodge();
	virtual void ReceiveAttack(float damage);
	virtual void ReceiveStun(float duration);
	virtual void Die();
	virtual void StartCombat( Creature*);
	virtual void EndCombat( Creature*);

	Room* GetRoom() const;
	bool PlayerInRoom() const;
	virtual bool IsAlive() const;
	virtual bool IsStuned()const;
	virtual bool IsTiedUp()const;
	virtual bool IsInCombat() const;

public:
	float basicDmg;
	float hit_points;
	Creature* combat_target;
	Action action;
	Item* weapon;
	float stuned; //Es un float para indicar los turnos que estara stuneado, depende el arma que lo golpee
	bool tiedUp;
	bool inCombat;
	int attackProcs[2][2];//2 modes, 3 procs on each, the las one is allways the remaining from the 2 setted
	int stunProcs[3][2];//3 modes, 3 proc on each, the las one is allways the remaining from the 2 setted
};

#endif //__Creature__
