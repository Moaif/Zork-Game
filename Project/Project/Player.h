#ifndef __Player__
#define __Player__

#include "creature.h"
#include <string>
#include <vector>


class Player : public Creature
{
public:
	Player(const char* name, const char* description, Room* room);
	~Player();

	bool Go(const vector<string>& args);
	void Look(const vector<string>& args) const;
	bool Take(const vector<string>& args);
	bool Drop(const vector<string>& args);
	void Inventory() const;
	bool Attack(const vector<string>& args);
	bool Lock(const vector<string>& args);
	bool UnLock(const vector<string>& args);
	void Talk(const vector<string>& args);
	void Stun(const vector<string>& args);
	void Read(const vector<string>& args);
	void TieUp(const vector<string>& args);

private:
	int maxItems;//Numero e items maximo en inventario
};

#endif //__Player__