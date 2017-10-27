#ifndef __Globals__
#define __Globals__

#include <string>

class World;
class Npc;

#ifdef _DEBUG
	#define RAND() getPseudoRand()
#else
	#define RAND() getRand()
#endif



void End();
void Win();
bool Same(const std::string& a, const std::string& b);
bool Same(const char* a, const std::string& b);
bool Same(const std::string& a, const char* b);
void turnCout(const std::string& a);
void turnCout(const char* a);
int getPseudoRand();
int getRand();
void worldSubscribe(World*);
void SetTurnFrec(double);
void SetPlayerInput(const std::string& s);
bool GetEndGame();
void SetMaxConsoleBuffer(int);
void Cout(const char*);
void Cout(const std::string&);



#endif //__Globals__
