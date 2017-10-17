#ifndef __Globals__
#define __Globals__

#include <string>

class World;

#ifdef _DEBUG
	#define RAND() getPseudoRand()
#else
	#define RAND() getRand()
#endif

using namespace std;

void End();
void Win();
bool Same(const string& a, const string& b);
bool Same(const char* a, const string& b);
bool Same(const string& a, const char* b);
void turnCout(const string& a);
void turnCout(const char* a);
int getPseudoRand();
int getRand();
void worldSubscribe(World*);
void SetTurnFrec(double);


#endif //__Globals__
