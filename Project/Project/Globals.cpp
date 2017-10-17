#include "globals.h"
#include <string>
#include <conio.h>
#include <iostream>
#include "Main.h"
#include <time.h>
#include "World.h"

using namespace std;
bool seeded = false;
World* wInstance;

void End() {
	endGame = true;
}


void Win() {
	cout << "\n\n\n\n\t\t\t\t\t You defeated the evil, well played!!\n\n\n\n\n";
	End();
}


bool Same(const string& a, const string& b)
{
	return _stricmp(a.c_str(), b.c_str()) == 0;
}


bool Same(const char* a, const string& b)
{
	return _stricmp(a, b.c_str()) == 0;
}


bool Same(const string& a, const char* b)
{
	return _stricmp(a.c_str(), b) == 0;
}


void turnCout(const string& output) {
	string padding;
	int spaces = player_input.size() - output.size();
	for (int i = 0; i <= spaces; ++i) {
		padding.push_back(' ');
	}
	cout << "\r" << output << padding << "\n"<< ">" << player_input;
}


void turnCout(const char* output) {
	string padding;
	int spaces = player_input.size() - strlen(output);
	for (int i = 0; i <= spaces; ++i) {
		padding.push_back(' ');
	}
	cout << "\r" << output << padding << "\n"<< ">" << player_input;
}

int getPseudoRand(){
	return rand();
}
int getRand(){

	if (!seeded) {
		srand((unsigned int)time(NULL));
		seeded = true;
	}

	return rand();
}

//I made this, cause i know that i will only have 1 world
void worldSubscribe(World* world) {
	wInstance = world;
}

void SetTurnFrec(double value) {
	wInstance->SetTurnFrec(value);
}

