#ifndef _Npc_
#define _Npc_

#include "creature.h"
#include <vector>
#include <string>


class Npc : public Creature {

public:
	Npc(const char* name, const char* description, Room* room);
	~Npc();

	bool Go(const vector<string>& args);
	bool Attack(const vector<string>& args);
	void Turn();
	void Talk();
	void Stun(Item*);
	void TieUp();
	void Exorciced();
	void FinalForm();
private:
	bool inLobby;
	bool posessed;
	int phase;
};

extern Npc* npcG;//Hasta tener una idea mejor, se usa para llamarlo cuando se pasa a la ultima fase, 
				//ya que no se si estara en la mimsma sala del juegador en dicho momento

#endif // _Npc_

