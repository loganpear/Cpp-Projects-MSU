#include "player.h"

using std::vector;

Player::Player(int h, int w, char direction)
    : pos{h, w}, direction(direction)
{
}

void Player::setPos(int h, int w)
{
    pos[0] = h; // Set the x-coordinate of the player
    pos[1] = w; // Set the y-coordinate of the player
}

void Player::getPos(int &h, int &w) const
{
    h = pos[0]; // Get the x-coordinate of the player
    w = pos[1]; // Get the y-coordinate of the player
}

int Player::getStage(int w, const vector<int> &stage_indices)
{
    for (size_t i{0}; i < stage_indices.size(); i++)
    {
        if (w >= stage_indices.at(i) && (i + 1 >= stage_indices.size() || w < stage_indices.at(i + 1)))
        {
            return i; // Return the stage number
        }
    }
    throw std::runtime_error("Invalid stage index for w: " + std::to_string(w)); // Error if no valid stage found
}

void Player::respawn(vector<vector<char>> &map, const vector<int> &stage_indices)
{
    int w, h;
    getPos(h, w);                           // Get the player's position
    int stage = getStage(w, stage_indices); // Get the stage number
    bool respawn_flag = false;              // Flag for respawn
    for (size_t i{0}; i < map.size(); i++)
    {
        if (map.at(i).at(stage_indices[stage]) == ' ')
        {
            direction = '>';                                // Reset the direction to right
            map.at(i).at(stage_indices[stage]) = direction; // Respawn the player at the end position
            setPos(i, stage_indices[stage]);                // Update the player's position
            respawn_flag = true;
        }
    }
    if (!respawn_flag)
    {
        throw std::runtime_error("Invalid respawn position for player"); // Error if no respawn position found
    }
}
