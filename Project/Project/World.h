#ifndef __World__
#define __World__

#include <list>
#include <vector>
#include <string>
#include <ctime>

#define INIT_TurnFrec 10

class Entity;
class Player;

class World
{
public:

	World();
	~World();

	bool Turn(std::vector<std::string>& args);
	bool ParseCommand(std::vector<std::string>& args);
	void GameLoop();
	void SetTurnFrec(double);

private:

	std::list<Entity*> entities;
	Player* player;
	clock_t timer;
	double turnFrec;
};

#endif //__World__