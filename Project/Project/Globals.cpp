#include "globals.h"
#include <string>
#include <conio.h>
#include <iostream>
#include <time.h>
#include "World.h"

using namespace std;

char* AdjustChars(const char*);

bool seeded = false;
bool endGame=false;
string playerInput;
World* wInstance;
int buffer;

void End() {
	endGame = true;
}


void Win() {
	Cout("\n\n\n\n\t\t\t\t\t You defeated the evil, well played!!\n\n\n\n\n");
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

void SetPlayerInput(const string& s) {
	playerInput = s;
}

bool GetEndGame() {
	return endGame;
}

void SetMaxConsoleBuffer(int value) {
	buffer = value;
}

void Cout(const char* input) {
	
	if (strlen(input) >= (unsigned)buffer) {
		cout << AdjustChars(input);
	}
	else
	{
		cout << input;
	}
}

void Cout(const string& input) {
	
	if (input.size() >= (unsigned)buffer) {
		cout << AdjustChars(input.c_str());
	}
	else
	{
		cout << input;
	}
}

void turnCout(const string& output) {
	string padding;
	int spaces = playerInput.size() - output.size();
	for (int i = 0; i <= spaces; ++i) {
		padding.push_back(' ');
	}
	if (output.size() >= (unsigned)buffer) {
		cout << "\r" << AdjustChars(output.c_str()) << padding << "\n" << ">" << playerInput;
	}
	else
	{
		cout << "\r" << output << padding << "\n" << ">" << playerInput;
	}
}


void turnCout(const char* output) {
	string padding;
	int spaces = playerInput.size() - strlen(output);
	for (int i = 0; i <= spaces; ++i) {
		padding.push_back(' ');
	}
	if (strlen(output) >= (unsigned)buffer) {
		cout << "\r" << AdjustChars(output) << padding << "\n" << ">" << playerInput;
	}
	else
	{
		cout << "\r" << output << padding << "\n" << ">" << playerInput;
	}
}

//Private function used for change char format to fit into console size
char* AdjustChars(const char* input) {
	char* output = new char[strlen(input)];
	int i = 0;
	int j = 0;
	int lastSpace = 0;
	while (*input != '\0') {
		if (*input == ' ') {
			lastSpace = i;
		}

		if (*input == '\t') {
			lastSpace = i;
			j = j + (8 - (j % 8));
		}

		if (*input == '\n') {
			j = 0;
		}
		else if (j >= buffer-1) {//Cause if its just the size, the \n wont enter and it will break the funtion
			output[lastSpace] = '\n';
			j = 0;
		}
		else
		{
			++j;
		}
		output[i] = *input;
		++input;
		++i;
	}
	output[i] = '\0';
	return output;
}