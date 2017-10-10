#ifndef __World__
#define __World__

#include <list>
#include <vector>
#include <string>

using namespace std;

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
};

#endif //__World__