#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include "globals.h"
#include "world.h"

using namespace std;

bool endGame;
string player_input;


int main()
{
	char key;
	vector<string> args;
	args.reserve(10);
	endGame = false;

	cout << "Welcome to Adrian's Zork\n";
	cout << "----------------\n";
	cout << "\nThis journey starts with you and your friend Luis proving who is more brave by entering a haunted church. Now you are in the church gardens trying to enter.\n" ;

	cout << "\n----------------\n";
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
					
					args.push_back(string(begin, str));
				} while (0 != *str++);
			}
		}

		if (args.size() > 0) {
			player_input = "";
		}

		if (my_world.Turn(args) == false) {
			cout << "\nSorry, I do not understand your command.\n";
		}

		if (args.size() > 0 && Same(args[0], "quit"))
			break;

		
		if (endGame)
			break;

		if (args.size() > 0)
		{
			if (!args[0].length() > 0) {
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
	cout << "\nThanks for playing, Bye!\n";
	cout << endl;
	system("pause");
	args.clear();//Autodelete vector when empty and not in use
	return 0;
}
