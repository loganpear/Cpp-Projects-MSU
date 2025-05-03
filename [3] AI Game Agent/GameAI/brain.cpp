#include "brain.h"

#include <fstream>
#include <iostream>
#include <set>

// test if the next cell based on the current direction is possible
bool Brain::testNextCell(
    int dir, int row, int col,
    std::vector<char> const &walls) const  // walls could also have 'B' or 'T'
{
    if (dir == -1 || dir == 0) return false;  // -1 means there was no prev_move

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

    char cell = mem_map.at(row + d_row).at(col + d_col);

    // check to see if the next move would be into a well Ex: + B T
    for (char wall : walls) {
        if (cell == wall) return false;
    }

    return true;  // if no walls were found in the next cell
}

// random direction but makes sure its possible and not wasting a move
int Brain::randomFallback(int row, int col, std::vector<int> directions,
                          std::vector<char> const &walls) const {
    while (!directions.empty()) {
        int index = std::rand() % directions.size();
        int dir = directions.at(index);

        if (testNextCell(dir, row, col, walls)) {
            return dir;
        }

        directions.erase(directions.begin() + index);
    }

    return 0;  // if no options then do nothing
}

// get what integer backwards would be based on the prev_step's direction
int Brain::getBackwardsDirection(int dir) const {
    if (dir == 1) return 3;
    if (dir == 2) return 4;
    if (dir == 3) return 1;
    if (dir == 4) return 2;
    return 0;  // if not valid direction from prev_step
}

// test if a target char is in a direct path to entity
// also return its distance by reference
int Brain::getMoveIfDirect(char target, int entity_row, int entity_col,
                           int &distance) const {
    // calc the bounds so you dont index out of range
    int max_rows = mem_map.size();
    int max_cols = mem_map.at(0).size();

    // up direction
    distance = 0;  // always set distance to 0 so prev checks dont disrupt
    for (int test_row = entity_row - 1; test_row >= 0; --test_row) {
        char cell = mem_map.at(test_row).at(entity_col);
        distance++;  // its simple to increment distance regardless
        if (cell == target) return 1;
        if (cell != ' ') break;
    }

    // left
    distance = 0;
    for (int test_col = entity_col - 1; test_col >= 0; --test_col) {
        char cell = mem_map.at(entity_row).at(test_col);
        distance++;
        if (cell == target) return 2;
        if (cell != ' ') break;
    }

    // down
    distance = 0;
    for (int test_row = entity_row + 1; test_row < max_rows; ++test_row) {
        char cell = mem_map.at(test_row).at(entity_col);
        distance++;
        if (cell == target) return 3;
        if (cell != ' ') break;
    }

    // right
    distance = 0;
    for (int test_col = entity_col + 1; test_col < max_cols; ++test_col) {
        char cell = mem_map.at(entity_row).at(test_col);
        distance++;
        if (cell == target) return 4;
        if (cell != ' ') break;
    }

    distance = 0;
    return 0;  // no direct path to target
}

// test if a certain item is visable in the viz
bool Brain::itemInViz(char const &item,
                      std::vector<std::vector<char>> const &viz) const {
    for (size_t row = 0; row < viz.size(); ++row) {
        for (size_t col = 0; col < viz.at(row).size(); ++col) {
            if (viz.at(row).at(col) == item) {
                return true;
            }
        }
    }
    return false;
}

// determine the stage, options: zeros, flags, mines, or maze
std::string Brain::updateGuessStage(
    std::vector<std::vector<char>> const &viz) const {
    if (itemInViz('0', viz)) {
        return "zeros";

    } else if (itemInViz('A', viz) || itemInViz('B', viz)) {
        return "flags";

    } else if (itemInViz('T', viz)) {
        return "mines";

    } else {
        return "maze";  // either maze
    }
    // Xgames test is in a different func
}

// true if there are at least 2 blank cells in the specified direction
bool Brain::testForTwoSpaces(int const &direction, int const &row,
                             int const &col) const {
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
    int max_rows = mem_map.size();
    int max_cols = mem_map.at(0).size();

    for (int step = 1; step <= 2; ++step) {
        int new_row = row + step * d_row;
        int new_col = col + step * d_col;

        // bounds check
        if (new_row < 0 || new_row >= max_rows || new_col < 0 ||
            new_col >= max_cols) {
            return false;
        }
        char cell = mem_map.at(new_row).at(new_col);

        if (cell != ' ') {
            return false;
        }
    }
    return true;
}

// this is the main direction deciding algorithm
int Brain::defaultMoveAlg(int const &map_row, int const &map_col,
                          std::vector<char> const &walls) const {
    std::vector<int> options = {1, 2, 3, 4};  // default all directions

    // Find what the side directions are
    std::vector<int> side_dirs;
    int chance = 3;  // default 1/3 chance of takng a side dir change

    // if prev was up/down
    if (prev_move == 1 || prev_move == 3) {
        side_dirs.push_back(4);  // add right twice to weight
        side_dirs.push_back(4);  // movig right more likely
        side_dirs.push_back(2);  // left

        // weigh right even heavier if
        if (stage_guess == "mines" || (stage_guess == "flags" && B_taken)) {
            side_dirs.push_back(4);
            chance = 2;  // 1/2 change we go for the side
        }

        // if prev was left/right
    } else if (prev_move == 2 || prev_move == 4) {
        side_dirs.push_back(1);  // up
        side_dirs.push_back(3);  // down

    } else {  // if no prev move or prev_move = 0
        return randomFallback(map_row, map_col, options, walls);
    }
    // get a random side
    int rand_index = std::rand() % side_dirs.size();

    // 1. If theres two open spaces on a that side, take a 1/3 change of going
    if (testForTwoSpaces(side_dirs.at(rand_index), map_row, map_col)) {
        if (std::rand() % chance == 0) {  // roll the dice for if we turn
            return side_dirs.at(rand_index);
        }  // note we only take this chance on one side, not both
    }

    // 2. try continuing in the same direction as prev_move
    if (testNextCell(prev_move, map_row, map_col, walls)) {
        return prev_move;
    }

    // 3. try the side directions (backtracking is the last resort)
    int rand_side = randomFallback(map_row, map_col, side_dirs, walls);

    if (rand_side != 0) {  // if no other options, go backwards
        return rand_side;
    } else {
        return getBackwardsDirection(prev_move);
    }
}

// returns a vector of each safe direction option in X games mode
std::vector<int> Brain::findXgamesMoves(int const &map_row,
                                        int const &map_col) const {
    // 3x3 grid where the entity 'v' is in the center
    // columns A, B, C
    // rows 0  0A,0B,0C
    //      1  1A,1B,1C
    //      2  2A, v,2C
    //      3  3A,3B,3C
    //      4  4A,4B,4C

    std::vector<int> safe_moves;

    // check right
    char oneC = mem_map.at(map_row - 1).at(map_col + 1);
    char twoC = mem_map.at(map_row).at(map_col + 1);
    char threeC = mem_map.at(map_row + 1).at(map_col + 1);
    if (oneC != 'X' && twoC == ' ' && threeC != 'X') {
        // weight moving right more heavily
        safe_moves.push_back(4);
        safe_moves.push_back(4);
        safe_moves.push_back(4);
    }

    // check left
    char oneA = mem_map.at(map_row - 1).at(map_col - 1);
    char twoA = mem_map.at(map_row).at(map_col - 1);
    char threeA = mem_map.at(map_row + 1).at(map_col - 1);
    if (oneA != 'X' && twoA == ' ' && threeA != 'X') {
        safe_moves.push_back(2);
    }

    // check up
    if (map_row - 2 >= 0) {
        char oneB = mem_map.at(map_row - 1).at(map_col);
        char zeroB = mem_map.at(map_row - 2).at(map_col);
        if (oneB == ' ' && zeroB != 'X') {
            safe_moves.push_back(1);
        }
    }

    // check down
    if (map_row + 2 < static_cast<int>(mem_map.size())) {
        char threeB = mem_map.at(map_row + 1).at(map_col);
        char fourB = mem_map.at(map_row + 2).at(map_col);
        if (threeB == ' ' && fourB != 'X') {
            safe_moves.push_back(3);
        }
    }

    std::set<int> unique_dirs(safe_moves.begin(), safe_moves.end());

    // if backwards dir is in unique_dirs && unique_dirs.size() > 1
    int back_dir = getBackwardsDirection(prev_move);
    if (unique_dirs.count(back_dir) && unique_dirs.size() > 1) {
        // remove the backwords option
        unique_dirs.erase(back_dir);
    }

    for (int move : unique_dirs) {
        // weigh going in a straight lie more likely
        if (move == 4 && prev_move == 4) {
            // only once for right
            safe_moves.push_back(4);

            // append thrice for not right straightline
        } else if (move == prev_move) {
            safe_moves.push_back(prev_move);
            safe_moves.push_back(prev_move);
            safe_moves.push_back(prev_move);
        }
    }
    return safe_moves;
}

// move generator algorithm for Xgames stage
int Brain::xGamesMoveAlg(int const &map_row, int const &map_col) const {
    std::vector<int> moves = findXgamesMoves(map_row, map_col);

    if (moves.size() == 0)
        return 0;
    else if (moves.size() == 1)
        return moves.at(0);
    else {
        return moves.at(std::rand() % (moves.size()));
    }
}

// find the mem_map colunm of an item
int Brain::findItemColumn(char const &item) const {
    // this assumns the map has equal length rows which it does as of now
    for (size_t col = 0; col < mem_map.at(0).size(); ++col) {
        for (size_t row = 0; row < mem_map.size(); ++row) {
            if (mem_map.at(row).at(col) == item) {
                return col;
            }
        }
    }
    return -1;  // this should never be returned
}

Brain::Brain() {
    std::srand(std::time(nullptr));  // makes sure random
}

// test for the Xgames obsticle stage
void Brain::testForXgames(int const &map_col,
                          std::vector<std::vector<char>> const &viz) {
    // update fartherst col if necessary
    if (map_col > farthest_col_reached) {
        farthest_col_reached = map_col;
    }
    // if we see an X and have reached the closest column we've seen an X, then
    // we know we're in Xgames mode
    if (itemInViz('X', viz)) {
        int X_col = findItemColumn('X');
        if (X_col < closestX_col) {
            closestX_col = X_col;
        }
        if (closestX_col <= farthest_col_reached) {
            stage_guess = "Xgames";
            Xgames_happened = true;
        }
    }
}
// expands the mem_map dimensions if necessary
void Brain::expandMemMap() {
    // check if theres a non '?' on the bottom border
    if (!bottom_found) {
        std::vector<char> const &bottom = mem_map.at(mem_map.size() - 1);
        for (char const &ch : bottom) {
            if (ch != '\0') {
                if (ch != '?') {
                    // append a row of question marks to the bottom
                    mem_map.push_back(std::vector<char>(bottom.size(), '?'));
                    break;
                }
            } else {  // this means we found a null char (the real bottom)
                bottom_found = true;
            }
        }
    }
    // test the right end for non ?
    bool append_col = false;
    for (std::vector<char> const &row : mem_map) {
        if (row.back() != '?') {
            append_col = true;
            break;
        }
    }
    if (append_col) {
        for (std::vector<char> &row : mem_map) {
            row.push_back('?');
        }
    }
}

// update the memory map based on what the new visual sees
void Brain::updateMemMap(const std::vector<std::vector<char>> &viz,
                         int const &map_row, int const &map_col) {
    // initialize mem map if empty
    if (mem_map.empty()){ 
        mem_map = std::vector<std::vector<char>>(map_row + 7, std::vector<char>(map_col + 7, '?'));
    }
    // expand dimensions if necessary
    expandMemMap();

    // find where the entity is inside the vision grid
    int viz_row = -1, viz_col = -1;  // initialize to -1

    // search every row and column of the viz to location the entity
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

    for (size_t i = 0; i < mem_map.size(); ++i) {
        for (size_t j = 0; j < mem_map.at(i).size(); ++j) {
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
    ofile << "Bottom found: " << bottom_found << '\n';

    // stortcut for frequently accessed cells
    char right_cell = mem_map.at(map_row).at(map_col + 1);
    ofile << "Right cell: " << right_cell << '\n';

    // initialize next_move to be default random
    int next_move = randomFallback(map_row, map_col);

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

    // we test for Xgames stage seperately everytime becasue its unique
    if (!Xgames_happened) {
        testForXgames(map_col, viz);
    }

    // options for determining how to geerate the next_move
    // 1. if we have a straight line to `w` and stage != Xgames
    int dist_to_target = 0;  // this means nothing for finding `w`
    int straight_dir = getMoveIfDirect('w', map_row, map_col, dist_to_target);

    if (stage_guess != "Xgames" && straight_dir != 0) {
        next_move = straight_dir;

        // 2. prevent backtracking
    } else if (map_col == backtrack_col) {
        next_move = 4;  // move right, away from the completed section

        // 3. if we identify were in either of the 2 maze stages
    } else if (stage_guess == "maze") {
        next_move = defaultMoveAlg(map_row, map_col);

        // 4. if we identify were in the food stage with the zeroes
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
                    testNextCell(prev_move, map_row, map_col)) {
                    next_move = prev_move;
                } else {  // if not, go the other verticle direction
                    next_move = randomFallback(map_row, map_col, {1, 3});
                }

            } else {  // if you can go right, just go right
                next_move = 4;
            }
        }
        // 5. if we identify we're in the flags stage
    } else if (stage_guess == "flags") {
        int dist_to_target = 0;
        ofile << "A_taken: " << A_taken << '\n';

        // treat B as a wall for now since we dont have A yet
        std::vector<char> stage3_walls{'+', 'B', 'D'};

        if (!A_taken) {  // we have not yet picked up flag 'A'

            // first test if 'A' is within a direct path of spaces
            int straight_dir =
                getMoveIfDirect('A', map_row, map_col, dist_to_target);

            if (straight_dir != 0) {       // this means we found 'A'
                next_move = straight_dir;  // so move twards 'A'

                // if we will secure the target after the move executes
                if (dist_to_target == 1) A_taken = true;

            } else {
                next_move = defaultMoveAlg(map_row, map_col, stage3_walls);
            }

        } else if (!B_taken) {  // we have pickup up A but not yet reached B
            // first test if 'B' is a direct path on any side of the entity
            int straight_dir =
                getMoveIfDirect('B', map_row, map_col, dist_to_target);

            if (straight_dir != 0) {  // this means we found 'B' next to us
                next_move = straight_dir;

                // if we will secure the target after the move executes
                if (dist_to_target == 1) B_taken = true;

            } else {
                next_move = defaultMoveAlg(map_row, map_col, stage3_walls);
            }
        } else {  // this is after B has been taken
            next_move = defaultMoveAlg(map_row, map_col);
        }

        // 6. if we identify we're in the Xgames stage
    } else if (stage_guess == "Xgames") {
        if (right_cell == 'w')
            next_move = 4;
        else
            next_move = xGamesMoveAlg(map_row, map_col);

        // 7. if we determine we're in the mines stage
    } else if (stage_guess == "mines") {
        next_move = defaultMoveAlg(map_row, map_col, {'+', 'T'});
    }

    printMemoryMap(ofile);  // for debugging in a new file
    prev_move = next_move;
    prev_stage = gamestate.stage;
    return next_move;
}