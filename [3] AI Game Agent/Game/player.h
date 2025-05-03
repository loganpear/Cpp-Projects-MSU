#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <iostream>
#include <array>

class Player
{
    std::array<int, 2> pos{-1, -1}; // Player position (h, w)
    char direction{' '};            // Direction the player is facing

public:
    Player() = default;
    Player(int, int, char);                                                      // Constructor
    void setPos(int, int);                                                       // Set the player's position
    void getPos(int &, int &) const;                                             // Get the player's position
    void setDirection(char d) { direction = d; }                                 // Set the player's direction
    char getDirection() const { return direction; }                              // Get the player's direction
    int getW() { return pos[1]; }                                                // Get the player's w coordinate
    int getH() { return pos[0]; }                                                // Get the player's h coordinate
    void respawn(std::vector<std::vector<char>> &map, const std::vector<int> &); // Respawn the player
    int getStage(int w, const std::vector<int> &stage_indices);                  // Get the stage number based on w coordinate
};

#endif // PLAYER_H