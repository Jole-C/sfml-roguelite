// Assignment 1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Game.h"

int main()
{
    
    //Create the game controller
    Game game;
    game.Initialise();

    while (true)
    {
        //Run
        game.Run();
    }

    //return
    return 0;

}
