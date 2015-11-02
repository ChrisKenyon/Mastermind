// main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MastermindGame.h"
#include <stdlib.h>
#include <windows.h>

int main()
{
	cout << "Welcome To Mastermind!\n    by Chris Kenyon\n";
	MastermindGame game = MastermindGame();

	try
	{
		game.playGame();
	}
	catch (exception ex)
	{
		cout << "Error!\n" << ex.what() << "\n Exiting!";
	}

	Sleep(1000);

    return 0;
}

