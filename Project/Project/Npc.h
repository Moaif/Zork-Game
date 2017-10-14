#ifndef _Npc_
#define _Npc_

#include "creature.h"
#include <vector>
#include <string>


enum class NpcForms
{
	HIGH,
	MEDIUM,
	LOW,
	NONE
};

class Npc : public Creature {

public:
	Npc(const char* name, const char* description, Room* room);
	~Npc();

	void Combat();
	void ReceiveAttack(float);
	void Turn();
	void Talk();
	void Stun(Item*);
	void Exorciced();
	void Killed();
	void FinalForm(Room*);
	void Die();
private:
	bool inLobby;
	bool posessed;
	bool chargedAttack;
	int phase;
	NpcForms form=NpcForms::NONE;
};

extern Npc* npcG;//Hasta tener una idea mejor, se usa para llamarlo cuando se pasa a la ultima fase, 
				//ya que no se si estara en la mimsma sala del juegador en dicho momento

#endif // _Npc_

