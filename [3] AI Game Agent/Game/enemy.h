#ifndef ENEMY_H
#define ENEMY_H

#include <string>
#include <vector>
#include "player.h"

class Enemy
{
    std::string type;
    int pos[2]; // Position of the enemy (h, w)
    char direction{'>'};

private:
public:
    Enemy(int, int, std::string);                                                    // Constructor
    void move(std::vector<std::vector<char>> &, Player &, const std::vector<int> &); // Move the enemy in the map
};

#endif // ENEMY_H