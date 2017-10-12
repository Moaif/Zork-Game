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
	void Read(const vector<string>& args) const;
	void TieUp(const vector<string>& args) const;
	void Pour(const vector<string>& args);
	void Pray(const vector<string>& args,int phase);
	void Touch(const vector<string>& args,int phase);
	void Pierce(const vector<string>& args) const;

	int GetPhase() const;
	void SetPhase(int phase);

private:
	int phase;
	int maxItems;//Numero e items maximo en inventario
};

#endif //__Player__