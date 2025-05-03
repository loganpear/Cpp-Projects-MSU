#include "enemy.h"
#include <stdexcept>

using std::string;
using std::vector;

Enemy::Enemy(int h, int w, string enemy_type)
    : type(enemy_type), pos{h, w}
{
    if (type == "vertical")
    {
        direction = 'v'; // Set the direction to down for vertical enemies
    }
}

void Enemy::move(vector<vector<char>> &map, Player &player, const vector<int> &stage_indices)
{
    if (type == "vertical")
    {
        int new_w, new_h; // New position variables
        if (direction == 'v')
        {
            new_w = pos[1];     // Keep the same column
            new_h = pos[0] + 1; // Move down
        }
        else if (direction == '^')
        {
            new_w = pos[1];     // Keep the same column
            new_h = pos[0] - 1; // Move down
        }
        else
        {
            throw std::runtime_error("Invalid direction for vertical enemy"); // Error if direction is invalid
        }
        if (map[new_h][new_w] == ' ') // Check if the target position is empty
        {
            map[pos[0]][pos[1]] = ' '; // Clear the old position
            map[new_h][new_w] = 'X';   // Move to the new position
            pos[0] = new_h;            // Update the enemy's position
            pos[1] = new_w;
        }
        else if (map[new_h][new_w] == '+') // Check if the target position is an enemy
        {
            if (direction == 'v')
            {
                direction = '^'; // Change direction to up
            }
            else if (direction == '^')
            {
                direction = 'v'; // Change direction to down
            }
        }
        else if (player.getW() == new_w && player.getH() == new_h) // Check if the enemy hits the player
        {
            player.respawn(map, stage_indices);
            map.at(pos[0]).at(pos[1]) = ' '; // Clear the old position
            map.at(new_h).at(new_w) = 'X';   // Move to the new position
            pos[0] = new_h;                  // Update the enemy's position
            pos[1] = new_w;
        }
        else
        {
        }
    }
}