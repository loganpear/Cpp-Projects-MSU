// g++ -std=c++20 -Wall main.cpp Game/game.cpp Game/player.cpp GameAI/brain.cpp Game/enemy.cpp && ./a.out
#include <iostream>
#include <string>

#include "Game/game.h"
#include "GameAI/brain.h"
#include "manual_interface.h"

using std::cout;
using std::endl;
using std::string;

int main(int argc, char **argv)
{
    string path_to_map = "Maps/L1.map"; // Path to the defaukl map file
    //string path_to_map = "Maps/Test3.map"; // CUSTOM MAP 
    int visual = 0;                     // Flag for visual mode (0 = no visual)
    bool human = false;

    for (int i{1}; i < argc; i++)
    {
        if (string(argv[i]) == "-map")
        {
            if (i + 1 < argc && argv[i + 1][0] != '-')
            {
                path_to_map = string(argv[i + 1]); // Get the map file path from command line argument
                i++;
            }
            else
            {
                std::cerr << "Error: No map file provided after -map option." << std::endl;
                return 1; // Exit with error if no map file is provided
            }
        }
        if (string(argv[i]) == "-visual")
        {
            visual = 1; // visual with delay between moves
        }
        else if (string(argv[i]) == "-human")
        {
            visual = 2; // no delay visual
            human = true;
        }
        else if (string(argv[i]) == "-testhuman")
        {
            visual = 3; // no delay visual and no fog
            human = true;
        }
        else if (string(argv[i]) == "-testvisual")
        {
            visual = 4; // visual with delay and no fog
        }
    }

    // Ensure that the student functions match expectations
    Game game = Game(path_to_map, visual); // Create a new game object
    Brain brain = Brain();                 // Create a new brain object

    game.initGame(); // Start the game

    while (!game.isGameOver()) // Loop until the game is over
    {
        GameState game_state = game.getGameState(); // Get the current game state

        int action;
        if (human)
        {
            action = getAction();
        }
        else
        {
            action = brain.getNextMove(game_state); // Get the next move from the AI brain
        }
        game.advanceGameCycle(action); // Advance the game by one cycle
    }

    cout << "======================================================\nGame Over! \n";
    cout << "Final Score: " << game.getScore() << endl; // Display the final score
    if (game.getScore() > 100 && game.getScore() < 1000)
    {
        cout << "Nicely done. You achieved the minimum requirements." << endl; // Display game over message
    }
    else if (game.getScore() > 1000)
    {
        cout << "You won! Fantastic performance!" << endl; // Display game lost message
    }
    else
    {
        cout << "Nice try. Maybe you'll get it next time." << endl; // Display game won message
    }
    cout << "\n";
    return 0;
}