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
private:
	bool inLobby;
	bool posessed;
	int phase;
};

#endif // _Npc_

