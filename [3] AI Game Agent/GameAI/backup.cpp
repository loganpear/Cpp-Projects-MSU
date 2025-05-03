/*#include "brain.h"

#include <fstream>
#include <iostream>

// test if the next cell based on the current direction is possible
bool testNextCell(
    int dir, const std::vector<std::vector<char>> &map, int row, int col,
    std::vector<char> walls = {'+'})  // walls couls also have 'B' or 'T'
{
    if (dir == -1) return false;  // -1 means there was no prev_move

    // change in row/col to find the value of the next cell
    int d_row = 0, d_col = 0;

    // find the corresponding directions in int
    if (dir == 1)
        d_row = -1;
    else if (dir == 2)
        d_col = -1;
    else if (dir == 3)
        d_row = 1;
    else if (dir == 4)
        d_col = 1;

    char cell = map.at(row + d_row).at(col + d_col);

    // check to see if the next move would be into a well Ex: + B T
    for (char wall : walls) {
        if (cell == wall) return false;
    }

    return true; // if no walls were found in the next cell
}

// random direction but makes sure its possible and not wasting a move
int randomFallback(const std::vector<std::vector<char>> &map, int row, int col,
                   std::vector<int> directions = {1, 2, 3, 4},
                   std::vector<char> walls = {'+'}) {
    while (!directions.empty()) {
        int index = std::rand() % directions.size();
        int dir = directions.at(index);

        if (testNextCell(dir, map, row, col, walls)) {
            return dir;
        }

        directions.erase(directions.begin() + index);
    }

    return 0;  // if no options then do nothing 
}

// get what integer backwards would be based on the prev_step's direction
int getBackwardsDirection(int dir) {
    if (dir == 1) return 3;
    if (dir == 2) return 4;
    if (dir == 3) return 1;
    if (dir == 4) return 2;
    return -1; // if not valid direction from prev_step
}

// test if a target char is right next to entity
int getMoveIfProximity(char target, const std::vector<std::vector<char>> &map,
                       int row, int col) {
    // return the direction it needs to go to hit the target
    if (map.at(row - 1).at(col) == target) return 1;
    if (map.at(row).at(col - 1) == target) return 2;
    if (map.at(row + 1).at(col) == target) return 3;
    if (map.at(row).at(col + 1) == target) return 4;
    return 0;  // not found
}

// test if a certain item is visable in the viz
bool itemInViz(char const &item, std::vector<std::vector<char>> const &viz){
    for (size_t row=0; row < viz.size(); ++row){
        for (size_t col=0; col < viz.at(row).size(); ++col){
            if(viz.at(row).at(col) == item){
                return true;
            }
        }
    }
    return false;
}

// determine the stage, options: zeros, flags, or maze (either type 1 or 2)
std::string updateGuessStage(std::vector<std::vector<char>> const &viz){
    if (itemInViz('0', viz) ) {
        return "zeros";

    } else if (itemInViz('A', viz) || itemInViz('B', viz)) {
        return "flags";

    } else {
        return "maze";
    }
}

Brain::Brain() {
    // initialize mem_map to assume dimensions <= (35 x 175)
    mem_map = std::vector<std::vector<char>>(35, std::vector<char>(175, '?'));
    std::srand(std::time(nullptr));  // makes sure random
}

// update the memory map based on what the new visual sees
void Brain::updateMemMap(const std::vector<std::vector<char>> &viz,
                         int const &map_row, int const &map_col) {

    // find where the entity is inside the vision grid
    int viz_row = -1, viz_col = -1;  // initialize to -1

    // search every row and column of the vic to location the entity
    for (size_t i = 0; i < viz.size(); ++i) {
        for (size_t j = 0; j < viz.at(i).size(); ++j) {
            char ch = viz.at(i).at(j);
            // test if the current char is our entitie character
            if (ch == 'v' || ch == '^' || ch == '<' || ch == '>') {
                viz_row = static_cast<int>(i);
                viz_col = static_cast<int>(j);
                break;
            }
        }
        if (viz_row != -1) break; // quit outer loop once we find him
    }

    // copy each cell from vision into mem_map
    for (size_t i = 0; i < viz.size(); ++i) {
        for (size_t j = 0; j < viz.at(i).size(); ++j) {

            int global_row = map_row - viz_row + static_cast<int>(i);
            int global_col = map_col - viz_col + static_cast<int>(j);

            // set mem map at (i,j) to the corresponding vision value
            mem_map.at(global_row).at(global_col) = viz.at(i).at(j);
        }
    }
}

void Brain::printMemoryMap(std::ofstream &ofile) const {
    ofile << "\nMEMORY MAP:\n";
    // assuming map dimensions are <= (35 x 175)
    for (int i = 0; i < 35; ++i) {
        for (int j = 0; j < 175; ++j) {
            ofile << mem_map.at(i).at(j);
        }
        ofile << '\n';
    }
}

// AI logic to determine the next move based on the gamestate
int Brain::getNextMove(GameState &gamestate) {
      Return a number 0-4:
        0: nothing
        1: up
        2: left
        3: down
        4: right
     // short name for vision
    std::vector<std::vector<char>> const &viz = gamestate.vision;  

    // in the mem_map where entity '<v>^' is located
    int map_row = gamestate.pos.at(0);
    int map_col = gamestate.pos.at(1);

    updateMemMap(viz, map_row, map_col);

    // output file for seeing variables and debugging
    std::ofstream ofile("output.txt");
    ofile << "Stage    : " << gamestate.stage << '\n';

    // stortcut for frequently accessed cells
    char right_cell = mem_map.at(map_row).at(map_col + 1);

    // initialize next_move to be default random
    int next_move = randomFallback(mem_map, map_row, map_col);  

    // detect if stage has changed then make a stage guess
    if (prev_stage != gamestate.stage) {
        // set a col to not go backwards from to prevent backtracking
        backtrack_col = map_col;
        stage_guess = updateGuessStage(viz); // make stage guess

    } else if (stage_guess == "") {  // first move edge case
        stage_guess = updateGuessStage(viz); // make stage guess
    }
    ofile << "backtrack col: " << backtrack_col << '\n';
    ofile << "stage guess: " << stage_guess << '\n';


    // prevent backtracking
    if (map_col == backtrack_col) {
        next_move = 4;  // move right, away from the completed section

    } else if (gamestate.stage == 0) {
        // if right space != wall then move right
        if (right_cell != '+') {
            next_move = 4;
        }
        // else test the direction its pointing at, if != + then move the same
        // direction
        else {
            if (testNextCell(prev_move, mem_map, map_row, map_col)) {
                next_move = prev_move;
            } else {
                next_move = randomFallback(mem_map, map_row, map_col, {1, 3});
            }
        }

    } else if (gamestate.stage == 1) {
        bool search_above = false;
        bool search_below = false;
        int map_size = mem_map.size();

        // make sure we find the top and bottom walls of a col before moving on
        bool top_wall_found = false;
        bool bottom_wall_found = false;

        // Check all rows in the current column for '0'
        for (int r = 0; r < map_size; ++r) {
            char cell = mem_map.at(r).at(map_col);
            if (cell == '0') {
                if (r < map_row)
                    search_above = true;
                else if (r > map_row)
                    search_below = true;

            // test if we've found the top and bottom walls
            } else if (r < map_row && cell == '+') {
                top_wall_found = true;
            } else if (r > map_row && cell == '+') {
                bottom_wall_found = true;
            }
        }
        if (!bottom_wall_found) search_below = true;
        if (!top_wall_found) search_above = true;

        if (search_above) {
            next_move = 1; // go up
        } else if (search_below) {
            next_move = 3; // go down
        } else {  // if there are certainly no more '0's in col 
            if (right_cell == '+') {
                // if we previously went up or down aka 1 or 3
                if ((prev_move == 1 || prev_move == 3) &&
                    // see if we can continue in that verticle direction
                    testNextCell(prev_move, mem_map, map_row, map_col)) {
                    next_move = prev_move;
                } else { // if not, go the other verticle direction
                    next_move =
                        randomFallback(mem_map, map_row, map_col, {1, 3});
                }

            } else { // if you can go right, just go right 
                next_move = 4;
            }
        }

    } else if (gamestate.stage == 2) {
        // continute the direction youre going if possible
        if (testNextCell(prev_move, mem_map, map_row, map_col)) {
            next_move = prev_move;
        } else { // if not, go anywhere (last choice backwards)
            std::vector<int> options{1, 2, 3, 4};
            // now remove the backwards option from options
            int backwards_direction = getBackwardsDirection(prev_move);
            options.erase(std::remove(options.begin(), options.end(),
                                      backwards_direction),
                          options.end());

            int result = randomFallback(mem_map, map_row, map_col, options);
            if (result != 0) {
                next_move = result;
            } else {  // if no other possible moves go backwards
                next_move = backwards_direction;
            }
        }

    } else if (gamestate.stage == 3) {
        ofile << "A_taken: " << A_taken << '\n';
        if (!A_taken) { // we have not yet picked up flag 'A'
        
            // treat B as a wall for now since we dont have A yet
            std::vector<char> stage3_walls{'+','B'};  

            // first test if 'A' is touching any side of the entity
            int prox_result =
                getMoveIfProximity('A', mem_map, map_row, map_col);
            if (prox_result != 0) {  // this means we found 'A' next to us
                next_move = prox_result;
                A_taken = true;

            // else try to continue in the direction youre going 
            } else if (testNextCell(prev_move, mem_map, map_row, map_col,
                                    stage3_walls)) {
                next_move = prev_move;

            } else {  // go randomly but dont go backwards unless you have to
                std::vector<int> options{1, 2, 3, 4};
                // now remove the backwards option from options
                int backwards_direction = getBackwardsDirection(prev_move);
                options.erase(std::remove(options.begin(), options.end(),
                                          backwards_direction),
                              options.end());

                int result = randomFallback(mem_map, map_row, map_col, options,
                                            stage3_walls);
                if (result != 0) {
                    next_move = result;
                } else {  // if no other possible moves go backwards
                    next_move = backwards_direction;
                }
            }

        } else if (!B_taken) {
            // first test if 'B' is touching any side of the entity
            int prox_result =
                getMoveIfProximity('B', mem_map, map_row, map_col);

            if (prox_result != 0) {  // this means we found 'B' next to us
                next_move = prox_result;
                B_taken = true;

            // go straight if posible
            } else if (testNextCell(prev_move, mem_map, map_row, map_col)) {
                next_move = prev_move;
            } else { // if not the turn randomly but try not to go backwards
                std::vector<int> options{1, 2, 3, 4};
                // now remove the backwards option from options
                int backwards_direction = getBackwardsDirection(prev_move);
                options.erase(std::remove(options.begin(), options.end(),
                                          backwards_direction),
                              options.end());

                int result = randomFallback(mem_map, map_row, map_col, options);
                if (result != 0) {
                    next_move = result;
                } else {  // if no other possible moves go backwards
                    next_move = backwards_direction;
                }
            }

        } else {  // this is after B has been taken
            // if right space != wall then move right
            if (right_cell != '+') {
                next_move = 4;
            }
            // else test the direction its pointing at, if != + then move the
            // same direction
            else {
                if (testNextCell(prev_move, mem_map, map_row, map_col)) {
                    next_move = prev_move;
                } else {
                    next_move =
                        randomFallback(mem_map, map_row, map_col, {1, 3});
                }
            }
        }
    }
    printMemoryMap(ofile); // for debugging in a new file
    prev_move = next_move;
    prev_stage = gamestate.stage;
    return next_move;
}*/

#include "brain.h"

#include <fstream>
#include <iostream>

// test if the next cell based on the current direction is possible
bool testNextCell(
    int dir, const std::vector<std::vector<char>> &map, int row, int col,
    std::vector<char> walls = {'+'})  // walls couls also have 'B' or 'T'
{
    if (dir == -1 || 0) return false;  // -1 means there was no prev_move

    // change in row/col to find the value of the next cell
    int d_row = 0, d_col = 0;

    // find the corresponding directions in int
    if (dir == 1)
        d_row = -1;
    else if (dir == 2)
        d_col = -1;
    else if (dir == 3)
        d_row = 1;
    else if (dir == 4)
        d_col = 1;

    char cell = map.at(row + d_row).at(col + d_col);

    // check to see if the next move would be into a well Ex: + B T
    for (char wall : walls) {
        if (cell == wall) return false;
    }

    return true;  // if no walls were found in the next cell
}

// random direction but makes sure its possible and not wasting a move
int randomFallback(const std::vector<std::vector<char>> &map, int row, int col,
                   std::vector<int> directions = {1, 2, 3, 4},
                   std::vector<char> walls = {'+'}) {
    while (!directions.empty()) {
        int index = std::rand() % directions.size();
        int dir = directions.at(index);

        if (testNextCell(dir, map, row, col, walls)) {
            return dir;
        }

        directions.erase(directions.begin() + index);
    }

    return 0;  // if no options then do nothing
}

// get what integer backwards would be based on the prev_step's direction
int getBackwardsDirection(int dir) {
    if (dir == 1) return 3;
    if (dir == 2) return 4;
    if (dir == 3) return 1;
    if (dir == 4) return 2;
    return -1;  // if not valid direction from prev_step
}

// test if a target char is in a direct path to entity
int getMoveIfDirect(char target, const std::vector<std::vector<char>> &map,
                    int entity_row, int entity_col, int &distance) {
    // know the bounds so you dont index out of range
    int max_rows = map.size();
    int max_cols = map.at(0).size();

    // up direction
    distance = 0;  // always set distance to 0 so prev checks dont disrupt
    for (int test_row = entity_row - 1; test_row >= 0; --test_row) {
        char cell = map.at(test_row).at(entity_col);
        distance++;  // its simple to increment distance regardless
        if (cell == target) return 1;
        if (cell != ' ') break;
    }

    // left direction
    distance = 0;
    for (int test_col = entity_col - 1; test_col >= 0; --test_col) {
        char cell = map.at(entity_row).at(test_col);
        distance++;
        if (cell == target) return 2;
        if (cell != ' ') break;
    }

    // down direction
    distance = 0;
    for (int test_row = entity_row + 1; test_row < max_rows; ++test_row) {
        char cell = map.at(test_row).at(entity_col);
        distance++;
        if (cell == target) return 3;
        if (cell != ' ') break;
    }

    // right direction
    distance = 0;
    for (int test_col = entity_col + 1; test_col < max_cols; ++test_col) {
        char cell = map.at(entity_row).at(test_col);
        distance++;
        if (cell == target) return 4;
        if (cell != ' ') break;
    }

    distance = 0;
    return 0;  // no direct path to target
}

// test if a certain item is visable in the viz
bool itemInViz(char const &item, std::vector<std::vector<char>> const &viz) {
    for (size_t row = 0; row < viz.size(); ++row) {
        for (size_t col = 0; col < viz.at(row).size(); ++col) {
            if (viz.at(row).at(col) == item) {
                return true;
            }
        }
    }
    return false;
}

// determine the stage, options: zeros, flags, or maze (either type 1 or 2)
std::string updateGuessStage(std::vector<std::vector<char>> const &viz) {
    if (itemInViz('0', viz)) {
        return "zeros";

    } else if (itemInViz('A', viz) || itemInViz('B', viz)) {
        return "flags";

    } else {
        return "maze";
    }
}

// true if there are at least 2 blank cells in the specified direction
bool testForTwoSpaces(int const &direction,
                      std::vector<std::vector<char>> const &map, int const &row,
                      int const &col, std::vector<char> const &walls = {'+'}) {
    // change in row
    int d_row = 0;
    int d_col = 0;

    if (direction == 1)
        d_row = -1;  // up
    else if (direction == 2)
        d_col = -1;  // left
    else if (direction == 3)
        d_row = 1;  // down
    else if (direction == 4)
        d_col = 1;  // right
    else
        return false;

    // know map boundries to prevent out of bounds indexing
    int max_rows = map.size();
    int max_cols = map.at(0).size();

    for (int step = 1; step <= 2; ++step) {
        int new_row = row + step * d_row;
        int new_col = col + step * d_col;

        // make sure youre in bounds
        if (new_row < 0 || new_row >= max_rows || new_col < 0 ||
            new_col >= max_cols) {
            return false;
        }
        char cell = map.at(new_row).at(new_col);

        if (cell != ' ') {
            return false;
        }
    }
    return true;
}

// this is the main direction deciding algorithm
int defaultMoveAlg(std::vector<std::vector<char>> const &map,
                   int const &prev_move, int const &map_row, int const &map_col,
                   std::vector<char> walls = {'+'}) {
    std::vector<int> options = {1, 2, 3, 4};  // default all directions

    // Find what the side directions are
    std::vector<int> side_dirs;

    // if prev was up/down
    if (prev_move == 1 || prev_move == 3) {
        side_dirs.push_back(4);  // right
        side_dirs.push_back(2);  // left

        // if prev was left/right
    } else if (prev_move == 2 || prev_move == 4) {
        side_dirs.push_back(1);  // up
        side_dirs.push_back(3);  // down

    } else {  // if no prev move or prev_move = 0
        return randomFallback(map, map_row, map_col, options, walls);
    }
    // get a random side
    int rand_index = std::rand() % 2;  // either 0 or 1

    // 1. If theres two open spaces on a that side, take a 1/3 change of going
    if (testForTwoSpaces(side_dirs.at(rand_index), map, map_row, map_col,
                         walls)) {
        if (std::rand() % 3 == 0) {  // 1/3 chance
            return side_dirs.at(rand_index);
        }  // note we only take this chance on one side, not both
    }

    // 2. try continuing in the same direction as prev_move
    if (testNextCell(prev_move, map, map_row, map_col, walls)) {
        return prev_move;
    }

    // 3. try the side directions (backtracking is the last resort)
    int rand_side = randomFallback(map, map_row, map_col, side_dirs, walls);

    if (rand_side != 0) {  // if no other options, go backwards
        return rand_side;
    } else {
        return getBackwardsDirection(prev_move);
    }
}

Brain::Brain() {
    // initialize mem_map to assume dimensions <= (35 x 175)
    mem_map = std::vector<std::vector<char>>(35, std::vector<char>(175, '?'));
    std::srand(std::time(nullptr));  // makes sure random
}

// update the memory map based on what the new visual sees
void Brain::updateMemMap(const std::vector<std::vector<char>> &viz,
                         int const &map_row, int const &map_col) {
    // find where the entity is inside the vision grid
    int viz_row = -1, viz_col = -1;  // initialize to -1

    // search every row and column of the vic to location the entity
    for (size_t i = 0; i < viz.size(); ++i) {
        for (size_t j = 0; j < viz.at(i).size(); ++j) {
            char ch = viz.at(i).at(j);
            // test if the current char is our entitie character
            if (ch == 'v' || ch == '^' || ch == '<' || ch == '>') {
                viz_row = static_cast<int>(i);
                viz_col = static_cast<int>(j);
                break;
            }
        }
        if (viz_row != -1) break;  // quit outer loop once we find him
    }

    // copy each cell from vision into mem_map
    for (size_t i = 0; i < viz.size(); ++i) {
        for (size_t j = 0; j < viz.at(i).size(); ++j) {
            int global_row = map_row - viz_row + static_cast<int>(i);
            int global_col = map_col - viz_col + static_cast<int>(j);

            // set mem map at (i,j) to the corresponding vision value
            mem_map.at(global_row).at(global_col) = viz.at(i).at(j);
        }
    }
}

void Brain::printMemoryMap(std::ofstream &ofile) const {
    ofile << "\nMEMORY MAP:\n";
    // assuming map dimensions are <= (35 x 175)
    for (int i = 0; i < 35; ++i) {
        for (int j = 0; j < 175; ++j) {
            ofile << mem_map.at(i).at(j);
        }
        ofile << '\n';
    }
}

// AI logic to determine the next move based on the gamestate
int Brain::getNextMove(GameState &gamestate) {
    /*  Return a number 0-4:
        0: nothing
        1: up
        2: left
        3: down
        4: right
    */ // short name for vision
    std::vector<std::vector<char>> const &viz = gamestate.vision;

    // in the mem_map where entity '<v>^' is located
    int map_row = gamestate.pos.at(0);
    int map_col = gamestate.pos.at(1);

    updateMemMap(viz, map_row, map_col);

    // output file for seeing variables and debugging
    std::ofstream ofile("output.txt");
    ofile << "Stage    : " << gamestate.stage << '\n';

    // stortcut for frequently accessed cells
    char right_cell = mem_map.at(map_row).at(map_col + 1);

    // initialize next_move to be default random
    int next_move = randomFallback(mem_map, map_row, map_col);

    // detect if stage has changed then make a stage guess
    if (prev_stage != gamestate.stage) {
        // set a col to not go backwards from to prevent backtracking
        backtrack_col = map_col;
        stage_guess = updateGuessStage(viz);  // make stage guess

    } else if (stage_guess == "") {           // first move edge case
        stage_guess = updateGuessStage(viz);  // make stage guess
    }
    ofile << "backtrack col: " << backtrack_col << '\n';
    ofile << "stage guess: " << stage_guess << '\n';

    // prevent backtracking
    if (map_col == backtrack_col) {
        next_move = 4;  // move right, away from the completed section

    } else if (stage_guess == "maze") {
        next_move = defaultMoveAlg(mem_map, prev_move, map_row, map_col);

    } else if (stage_guess == "zeros") {
        bool search_above = false;
        bool search_below = false;
        int map_size = mem_map.size();

        // make sure we find the top and bottom walls of a col before moving on
        bool top_wall_found = false;
        bool bottom_wall_found = false;

        // Check all rows in the current column for '0'
        for (int r = 0; r < map_size; ++r) {
            char cell = mem_map.at(r).at(map_col);
            if (cell == '0') {
                if (r < map_row)
                    search_above = true;
                else if (r > map_row)
                    search_below = true;

                // test if we've found the top and bottom walls
            } else if (r < map_row && cell == '+') {
                top_wall_found = true;
            } else if (r > map_row && cell == '+') {
                bottom_wall_found = true;
            }
        }
        if (!bottom_wall_found) search_below = true;
        if (!top_wall_found) search_above = true;

        if (search_above) {
            next_move = 1;  // go up
        } else if (search_below) {
            next_move = 3;  // go down
        } else {            // if there are certainly no more '0's in col
            if (right_cell == '+') {
                // if we previously went up or down aka 1 or 3
                if ((prev_move == 1 || prev_move == 3) &&
                    // see if we can continue in that verticle direction
                    testNextCell(prev_move, mem_map, map_row, map_col)) {
                    next_move = prev_move;
                } else {  // if not, go the other verticle direction
                    next_move =
                        randomFallback(mem_map, map_row, map_col, {1, 3});
                }

            } else {  // if you can go right, just go right
                next_move = 4;
            }
        }

    } else if (stage_guess == "flags") {
        int dist_to_target = 0;
        ofile << "A_taken: " << A_taken << '\n';

        if (!A_taken) {  // we have not yet picked up flag 'A'

            // treat B as a wall for now since we dont have A yet
            std::vector<char> stage3_walls{'+', 'B'};

            // first test if 'A' is within a direct path of spaces
            int straight_dir =
                getMoveIfDirect('A', mem_map, map_row, map_col, dist_to_target);

            if (straight_dir != 0) {       // this means we found 'A'
                next_move = straight_dir;  // so move twards 'A'

                // if we will secure the target after the move executes
                if (dist_to_target == 1) A_taken = true;

            } else {
                next_move = defaultMoveAlg(mem_map, prev_move, map_row, map_col,
                                           stage3_walls);
            }

        } else if (!B_taken) {
            // first test if 'B' is a direct path on any side of the entity
            int straight_dir =
                getMoveIfDirect('B', mem_map, map_row, map_col, dist_to_target);

            if (straight_dir != 0) {  // this means we found 'B' next to us
                next_move = straight_dir;

                // if we will secure the target after the move executes
                if (dist_to_target == 1) B_taken = true;

            } else {
                next_move =
                    defaultMoveAlg(mem_map, prev_move, map_row, map_col);
            }
        } else {  // this is after B has been taken
            // if right space != wall then move right
            if (right_cell != '+') next_move = 4;

            // else test the direction its pointing at, if != + then move the
            // same direction
            else {
                if (testNextCell(prev_move, mem_map, map_row, map_col)) {
                    next_move = prev_move;
                } else {
                    next_move =
                        randomFallback(mem_map, map_row, map_col, {1, 3});
                }
            }
        }
    }

    printMemoryMap(ofile);  // for debugging in a new file
    prev_move = next_move;
    prev_stage = gamestate.stage;
    return next_move;
}