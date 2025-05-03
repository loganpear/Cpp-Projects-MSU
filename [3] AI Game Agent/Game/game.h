#ifndef GAME_H
#define GAME_H

#include <string>
#include <cctype>
#include <unordered_map>
#include <vector>
#include <array>

#include "player.h"
#include "enemy.h"

struct GameState
{
    int stage{0};                          // Current stage
    int score{0};                          // Current score
    int cycle{0};                          // Current cycle
    std::vector<std::vector<char>> vision; // Vision of the player
    std::array<int, 2> pos;                // Player position (h, w)
};

class Game
{
    std::string path_to_map;
    const int MAX_CYCLE{1000};
    int score{0};
    int cycle{0};
    int visual{0}; // Flag for visual mode
    std::vector<std::vector<char>> map;
    std::vector<int> stage_indices;                  // Indices of stages in the map
    std::unordered_map<int, int> food_count;         // Count of food items per stage (key: stage, value: count)
    std::unordered_map<int, bool> stage_flag_picked; // Flags for stages (picked)
    std::unordered_map<int, bool> stage_flag_placed; // Flags for stages (placed)
    std::unordered_map<int, bool> doors;             // Door states for stages
    std::vector<Enemy> enemies;                      // Vector of enemies
    Player player;
    int max_crossed_stage{0}; // Maximum stage crossed by the player
    bool game_won{false};     // Flag for game won state

private:
    void loadMap(const std::string &);                     // Loads the map from a file
    void createMap(const std::vector<std::string> &);      // Creates a map of given lines
    std::vector<int> getStageIndices(const std::string &); // Gets stage indices from a line
    int getStage(int);                                     // Gets the stage number based on col value (w value)
    std::vector<std::vector<char>> getVision();            // Gets the vision of the player based on position and direction
    void displayGame();
    bool isInVision(int, int);
    void movePlayer(int direction);
    void checkEnemies();      // Checks the enemies in the game
    void openDoor(int stage); // Opens the door for the given stage

public:
    Game(const std::string &, int); // Constructor
    void initGame();                // Initializes and starts the game
    void advanceGameCycle(int);     // Advances the game state by one cycle
    bool isGameOver() const;        // Checks if the game is over
    int getScore() const;           // Gets current score
    GameState getGameState();       // Gets the current game state
};

#endif // GAME_H