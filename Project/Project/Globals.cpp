#include "globals.h"
#include <string>
#include <conio.h>
#include <iostream>
#include "Main.h"

using namespace std;

void End() {
	endGame = true;
}

// -------------------------------------------------
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
