#include "game.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include <cmath>
#include <algorithm>

using std::cerr;
using std::cout;
using std::endl;
using std::getline;
using std::ifstream;
using std::isdigit;
using std::string;
using std::stringstream;
using std::unordered_map;
using std::vector;

Game::Game(const string &path_to_map, int visual)
    : path_to_map(path_to_map), visual(visual)
{
}

vector<int> Game::getStageIndices(const string &line)
{
    vector<int> temp_stage_indices; // Vector to store stage indices
    for (size_t i{0}; i < line.size(); i++)
    {
        if (line.at(i) != ' ' && isdigit(line.at(i))) // Check for digits
        {
            temp_stage_indices.push_back(i); // Store the stage indices
        }
    }

    return temp_stage_indices;
}

int Game::getStage(int w)
{
    // Get the stage number based on coordinates
    for (size_t i{0}; i < stage_indices.size(); i++)
    {
        if (w >= stage_indices.at(i) && (i + 1 >= stage_indices.size() || w < stage_indices.at(i + 1)))
        {
            return i; // Return the stage number
        }
    }
    throw std::runtime_error("Invalid stage index: " + std::to_string(w)); // Error if no valid stage found
}

vector<vector<char>> Game::getVision()
{
    vector<vector<char>> vision;
    int p_w = player.getW();
    int p_h = player.getH();
    char direction = player.getDirection();

    int min_row, max_row, min_col, max_col;

    // Compute the vision box (6 cells deep including player's cell, 5 cells wide)
    switch (direction)
    {
    case 'v': // facing down
        min_row = p_h - 1;
        max_row = p_h + 5;
        min_col = p_w - 2;
        max_col = p_w + 2;
        break;
    case '^': // facing up
        min_row = p_h - 5;
        max_row = p_h + 1;
        min_col = p_w - 2;
        max_col = p_w + 2;
        break;
    case '>': // facing right
        min_col = p_w - 1;
        max_col = p_w + 5;
        min_row = p_h - 2;
        max_row = p_h + 2;
        break;
    case '<': // facing left
        min_col = p_w - 5;
        max_col = p_w + 1;
        min_row = p_h - 2;
        max_row = p_h + 2;
        break;
    default:
        return vision; // return an empty vector if no valid direction
    }

    // Ensure our bounds don't exceed the map's limits.
    min_row = std::max(min_row, 0);
    min_col = std::max(min_col, 0);
    max_row = std::min(max_row, static_cast<int>(map.size()) - 1);
    if (!map.empty())
        max_col = std::min(max_col, static_cast<int>(map[0].size()) - 1);

    // Create the vision vector by copying the visible part of the map.
    for (int i = min_row; i <= max_row; ++i)
    {
        vector<char> row;
        for (int j = min_col; j <= max_col; ++j)
        {
            row.push_back(map[i][j]);
        }
        vision.push_back(row);
    }
    return vision;
}

void Game::createMap(const vector<string> &map_lines)
{
    // Create the map from the given lines
    cout << "Creating map..." << endl;

    for (size_t h{1}; h < map_lines.size(); h++)
    {
        for (size_t w{0}; w < map_lines.at(h).size(); w++)
        {
            this->map.at(h - 1).at(w) = map_lines.at(h).at(w); // Fill the map with characters
            if (map_lines.at(h).at(w) == 'v' || map_lines.at(h).at(w) == '>' || map_lines.at(h).at(w) == '<' || map_lines.at(h).at(w) == '^')
            {
                // found a player character
                player = Player(h - 1, w, map_lines.at(h).at(w)); // Ensure Player class has a matching constructor
            }
            else if (map_lines.at(h).at(w) == '0') // Check for end position
            {
                // found a food entity
                int temp_stage = getStage(w); // Get the stage number
                // cout << "stage of: " << w << " is : " << temp_stage << endl;

                if (food_count.find(temp_stage) != food_count.end()) // Check if the key exists in the map
                {
                    food_count[temp_stage]++; // Increment the key value
                }
                else
                {
                    food_count[temp_stage] = 1; // Initialize the key value
                }
            }
            else if (map_lines.at(h).at(w) == 'A' || map_lines.at(h).at(w) == 'B') // Check for food entity
            {
                stage_flag_picked[getStage(w)] = false; // Mark the stage as picked
                stage_flag_placed[getStage(w)] = false; // Mark the stage as placed
            }
            else if (map_lines.at(h).at(w) == 'X')
            {
                enemies.push_back(Enemy(h - 1, w, "vertical")); // Create an enemy object
            }
            else if (map_lines.at(h).at(w) == 'D')
            {
                int stage = getStage(w);
                doors[stage] = false;
            }
        }
    }
}

void Game::loadMap(const string &path)
{
    // Load the map from the file
    // Implementation of map loading goes here
    cout << "Loading map from: " << path << endl;
    ifstream map_file(path);
    string line;
    int h_counter{0}; // For calculating the map height
    int s_counter{0}; // For calculating the number of stages
    int w_counter{0}; // For calculating the map width
    vector<string> map_lines;

    if (!map_file.is_open())
    {
        cerr << "Error: Could not open map file: " << path << endl;
        return;
    }

    while (getline(map_file, line))
    {
        // Process each line of the map file
        // cout << line << endl;
        if (line == "")
            throw std::runtime_error("Empty line in map file");
        if (h_counter == 0)
        {
            w_counter = line.length(); // Set width on first line
        }
        else if (line.length() != static_cast<size_t>(w_counter))
        {
            throw std::runtime_error("Inconsistent line length in map file");
        }
        h_counter++;               // Increment height counter for each line
        map_lines.push_back(line); // Store the line in the map
    }

    this->map.resize(h_counter, std::vector<char>(w_counter)); // Resize the map

    this->stage_indices = getStageIndices(map_lines.at(0)); // Get stage indices from the first line
    s_counter = stage_indices.size();                       // Count the number of stages

    if (s_counter > 10)
    {
        throw std::runtime_error("Too many stages in map file: " + path + " Modify getStageIndices() \
             to provide more stages"); // Error if too many stages
    }

    cout << "Map Size: " << w_counter << "," << h_counter << endl;
    cout << "Number of stages: " << s_counter << endl;

    createMap(map_lines); // Create the map from the lines
}

void Game::initGame()
{
    cout << "======================================================\nStarting CSE232 Maze-Game (Project 3)\n"
         << "======================================================" << endl;
    loadMap(path_to_map); // Load the map
}

void Game::advanceGameCycle(int action)
{
    // Advance game by one cycle implementation
    if (action == 0)
    {
        // do nothing
    }
    else if (action == 1 || action == 2 || action == 3 || action == 4)
    {
        movePlayer(action); // Move the player based on the action
    }
    else
    {
        throw std::runtime_error("Invalid player action: " + std::to_string(action)); // Error if invalid action
    }
    if (getStage(player.getW()) > max_crossed_stage)
    {
        max_crossed_stage = getStage(player.getW()); // Update the maximum stage crossed
        score += max_crossed_stage * 10;
    }
    checkEnemies(); // Check the enemies in the game
    cycle++;
}

bool Game::isGameOver() const
{
    // Check if game has ended
    if (cycle > MAX_CYCLE || game_won)
        return true;
    return false;
}

int Game::getScore() const
{
    // Return current score
    return score;
}

void Game::displayGame()
{
    cout << "\033[2J\033[H"; // Clear screen and move cursor to top-left
    cout << "Stage: " << getStage(player.getW()) << " Score: " << score << " Moves: " << cycle << endl;
    string stage_text = "";
    stage_text.resize(map[0].size(), ' '); // Initialize stage text with spaces
    for (size_t i{0}; i < stage_indices.size(); i++)
    {
        stage_text[stage_indices[i]] = std::to_string(i + 1)[0]; // Fill the stage text with stage numbers
    }
    cout << stage_text << endl; // Display the stage text
    for (size_t h{0}; h < map.size(); h++)
    {
        for (size_t w{0}; w < map.at(h).size(); w++)
        {
            if (visual == 3 || visual == 4 || isInVision(h, w)) // Check if the position is in vision
            {
                cout << map.at(h).at(w); // Display the map
            }
            else
            {
                cout << " ";
            }
        }
        cout << endl;
    }

    if (visual == 1 || visual == 4)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

bool Game::isInVision(int h, int w)
{
    int p_w = player.getW();
    int p_h = player.getH();
    char direction = player.getDirection();

    int min_row, max_row, min_col, max_col;

    // Set vision box bounds (6 cells deep including player's cell, 5 cells wide)
    switch (direction)
    {
    case 'v': // facing down
        min_row = p_h - 1;
        max_row = p_h + 5;
        min_col = p_w - 2;
        max_col = p_w + 2;
        break;
    case '^': // facing up
        min_row = p_h - 5;
        max_row = p_h + 1;
        min_col = p_w - 2;
        max_col = p_w + 2;
        break;
    case '>': // facing right
        min_col = p_w - 1;
        max_col = p_w + 5;
        min_row = p_h - 2;
        max_row = p_h + 2;
        break;
    case '<': // facing left
        min_col = p_w - 5;
        max_col = p_w + 1;
        min_row = p_h - 2;
        max_row = p_h + 2;
        break;
    default:
        return false;
    }

    // If the cell is outside the vision box, don't show it.
    if (h < min_row || h > max_row || w < min_col || w > max_col)
        return false;

    return true;
}

void Game::movePlayer(int direction)
{
    int w, h;
    player.getPos(h, w); // Get the player's position
    int new_w, new_h;    // New position variables
    switch (direction)
    {
    case 1: // Move up
        new_h = h - 1;
        new_w = w;
        player.setDirection('^'); // Update direction to up
        break;
    case 2: // Move left
        new_h = h;
        new_w = w - 1;
        player.setDirection('<'); // Update direction to left
        break;
    case 3: // Move down
        new_h = h + 1;
        new_w = w;
        player.setDirection('v'); // Update direction to down
        break;
    case 4: // Move right
        new_h = h;
        new_w = w + 1;
        player.setDirection('>'); // Update direction to right
        break;
    default:
        throw std::runtime_error("Invalid player movement"); // Error if invalid direction
        break;                                               // Invalid direction, do nothing
    }
    if (new_h < 0 || new_h >= static_cast<int>(map.size()) || new_w < 0 || new_w >= static_cast<int>(map.at(0).size()))
    {
        throw std::runtime_error("Invalid player movement: out of bounds"); // Error if out of bounds
    }
    char target_pos = map.at(new_h).at(new_w); // Get the target position
    map.at(h).at(w) = player.getDirection();
    if (target_pos == ' ')
    {
        // Move to empty space
        map.at(h).at(w) = ' ';                           // Clear the old position
        map.at(new_h).at(new_w) = player.getDirection(); // Move to the new position
        player.setPos(new_h, new_w);                     // Update the player's position
    }
    else if (target_pos == '+')
    {
        // Do nothing. Hit a wall
    }
    else if (target_pos == '0')
    {
        map.at(h).at(w) = ' ';                           // Clear the old position
        map.at(new_h).at(new_w) = player.getDirection(); // Move to the new position
        player.setPos(new_h, new_w);                     // Update the player's position
        int stage = getStage(new_w);
        food_count[stage]--; // Decrement the food count for the stage
        if (food_count[stage] == 0)
        {
            openDoor(stage); // Open the door if all food is collected
        }
        score++; // Increment the score
    }
    else if (target_pos == 'A')
    {
        // Check if the stage is already picked or placed
        int stage = getStage(new_w);
        if (stage_flag_picked[stage] == false)
        {
            map.at(h).at(w) = ' ';                           // Clear the old position
            map.at(new_h).at(new_w) = player.getDirection(); // Move to the new position
            player.setPos(new_h, new_w);                     // Update the player's position
            stage_flag_picked[stage] = true;                 // Mark the stage as picked
            score += 10;                                     // Increment the score
        }
    }
    else if (target_pos == 'B' && stage_flag_picked[getStage(new_w)] == true)
    {
        map.at(h).at(w) = ' ';                           // Clear the old position
        map.at(new_h).at(new_w) = player.getDirection(); // Move to the new position
        player.setPos(new_h, new_w);                     // Update the player's position
        stage_flag_placed[getStage(new_w)] = true;       // Mark the stage as placed
        score += 15;                                     // Increment the score
        openDoor(getStage(new_w));                       // Open the door if all food is placed
    }
    else if (target_pos == 'D')
    {
        // Do nothing. Hit a door
    }
    else if (target_pos == 'X')
    {
        // Do nothing. Hit a door
        map.at(h).at(w) = ' ';              // Clear the old position
        player.respawn(map, stage_indices); // Respawn the player
    }
    else if (target_pos == 'T')
    {
        map.at(h).at(w) = ' ';              // Clear the old position
        player.respawn(map, stage_indices); // Respawn the player
    }
    else if (target_pos == 'w')
    {
        score += 1000;
        score += MAX_CYCLE - cycle; // Increment the score
        game_won = true;            // Mark the game as won
    }
}

void Game::checkEnemies()
{
    for (auto &enemy : enemies)
    {
        enemy.move(map, player, stage_indices); // Move the enemy
    }
}

void Game::openDoor(int stage)
{
    int w = stage_indices[stage + 1];
    for (int i{0}; i < static_cast<int>(map.size()); i++)
    {
        if (map[i][w] == 'D')
        {
            map[i][w] = ' ';     // Open the door
            doors[stage] = true; // Mark the door as open
            break;
        }
    }
}

GameState Game::getGameState()
{
    // Return current game state
    GameState game_state;

    int h, w;
    player.getPos(h, w);             // Get the player's position
    int stage = getStage(w);         // Get the stage number
    game_state.stage = stage;        // Set the stage number
    game_state.score = score;        // Set the score
    game_state.cycle = cycle;        // Set the cycle
    game_state.vision = getVision(); // Get the player's vision
    game_state.pos[0] = h;           // Set the player's height
    game_state.pos[1] = w;           // Set the player's width

    if (visual) // any value greater than 0 is true
    {
        displayGame();
    }
    return game_state;
}