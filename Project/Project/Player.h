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

	void Go(const vector<string>& args);
	void Look(const vector<string>& args) const;
	void Take(const vector<string>& args);
	void Drop(const vector<string>& args);
	void Inventory() const;
	void Lock(const vector<string>& args);
	void UnLock(const vector<string>& args);
	void Attack(const vector<string>& args);
	void Talk(const vector<string>& args);
	void Stun(const vector<string>& args);
	void Read(const vector<string>& args) const;
	void TieUp(const vector<string>& args);
	void Pour(const vector<string>& args);
	void Pray(const vector<string>& args,int phase);
	void Touch(const vector<string>& args,int phase);
	void Pierce(const vector<string>& args) const;
	void Die();

	int GetPhase() const;
	void SetPhase(int phase);

private:
	int phase;
	int maxItems;//Numero e items maximo en inventario
};

#endif //__Player__