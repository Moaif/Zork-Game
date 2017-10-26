#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include "globals.h"
#include "world.h"
#include <windows.h>

using namespace std;

int main()
{
	char key;
	string player_input;
	vector<string> args;
	args.reserve(10);

	//Get max console buffer for string delimitation
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
	{
		// an error occourred
		cerr << "Cannot determine console size." << endl;
	}
	else
	{
		SetMaxConsoleBuffer(csbi.srWindow.Right - csbi.srWindow.Left);
	}
	Cout("Welcome to Adrian's Zork\n");
	Cout("----------------\n");
	Cout("\nThis journey starts with you and your friend Luis proving who is more brave by entering a haunted church. Now you are in the church gardens trying to enter.\n");

	Cout("\n----------------\n");
	World my_world;

	args.push_back("look");
	

	while (1)
	{

		if (_kbhit() != 0)
		{
			key = _getch();
			if (key == '\b')
			{
				if (player_input.length() > 0)
				{
					player_input.pop_back();
					cout << '\b';
					cout << " ";
					cout << '\b';
				}
			}
			else if (key != '\r')
			{
				player_input += key;
				cout << key;
			}
			else
			{	
				const char* str = player_input.c_str();

				do
				{
					const char *begin = str;

					while (*str != ' ' && *str)
						str++;
					if (string(begin, str) != "") {
						args.push_back(string(begin, str));
					}
				} while (0 != *str++);
			}
		}

		if (args.size() > 0) {
			player_input = "";
		}
		SetPlayerInput(player_input);

		if (my_world.Turn(args) == false) {
			Cout("\nSorry, I do not understand your command.\n");
		}

		if (args.size() > 0 && Same(args[0], "quit"))
			break;

		
		if (GetEndGame())
			break;

		if (args.size() > 0)
		{
			if (!(args[0].length() > 0)) {
				cout << "\n> ";
			}
			else
			{
				cout << "> ";
			}
			args.clear();
		}
	}

	cout << endl;
	Cout("\nThanks for playing, Bye!\n");
	cout << endl;
	system("pause");
	args.clear();//Autodelete vector when empty and not in use
	return 0;
}
