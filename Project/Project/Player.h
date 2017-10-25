#ifndef __Player__
#define __Player__

#include "creature.h"
#include <string>
#include <vector>


class Player : public Creature
{
public:
	Player(const char* name, const char* description, Room* room,float hitpoints,float basicDmg);
	~Player();

	void Go(const std::vector<std::string>& args) override;
	void Look(const std::vector<std::string>& args) const override;
	void Take(const std::vector<std::string>& args);
	void Drop(const std::vector<std::string>& args);
	void Inventory() const;
	void Lock(const std::vector<std::string>& args);
	void UnLock(const std::vector<std::string>& args);
	void Break(const std::vector<std::string>& args);
	void Attack(const std::vector<std::string>& args);
	void Dodge() override;
	void Talk(const std::vector<std::string>& args);
	void Stun(const std::vector<std::string>& args);
	void Read(const std::vector<std::string>& args) const;
	void TieUp(const std::vector<std::string>& args);
	void Pour(const std::vector<std::string>& args);
	void Pray(const std::vector<std::string>& args,int phase);
	void Touch(const std::vector<std::string>& args,int phase);
	void Pierce(const std::vector<std::string>& args) const;
	void Die() override;

	int GetPhase() const;
	void SetPhase(int phase);
	void SetNpcG(Npc* n);

private:
	int phase;
	Npc* npcG;
	int maxItems;//Numero e items maximo en inventario
};

#endif //__Player__