#ifndef __World__
#define __World__

#include <list>
#include <vector>
#include <string>
#include <ctime>
#define TURN_FREC 5

class Entity;
class Player;

class World
{
public:

	World();
	~World();

	bool Turn(vector<string>& args);
	bool ParseCommand(vector<string>& args);
	void GameLoop();

private:

	list<Entity*> entities;
	Player* player;
	clock_t timer;
};

#endif //__World__