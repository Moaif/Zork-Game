#ifndef _Npc_
#define _Npc_

#include "creature.h"
#include <vector>
#include <string>

#define COMBATFrec 20.0


enum class NpcForms
{
	HIGH,
	MEDIUM,
	LOW,
	NONE
};

class Npc : public Creature {

public:
	Npc(const char* name, const char* description, Room* room, std::vector<Item*> items);
	~Npc();

	void Dodge()override;
	void Combat();
	void ReceiveAttack(float)override;
	void ReceiveStun(float)override;
	void Turn()override;
	void Talk()override;
	void Observe(Item*);
	void Stab();
	void Exorciced();
	void Killed();
	void FinalForm(Room*);
	void Die()override;
private:
	void RandomMove();
private:
	bool inLobby;
	bool posessed;
	bool chargedAttack;
	int phase;
	std::vector<Item*> prohibitedItems;
	NpcForms form=NpcForms::NONE;
};

extern Npc* npcG;//Hasta tener una idea mejor, se usa para llamarlo cuando se pasa a la ultima fase, 
				//ya que no se si estara en la mimsma sala del juegador en dicho momento

#endif // _Npc_

