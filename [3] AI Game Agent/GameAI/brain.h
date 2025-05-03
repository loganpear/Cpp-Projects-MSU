#ifndef BRAIN_H
#define BRAIN_H

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <vector>

#include "../Game/game.h"

class Brain {
   public:
    Brain();

    int getNextMove(GameState &gamestate);
    bool testNextCell(int dir, int row, int col,
                      std::vector<char> const &walls = {'+'}) const;
    int findItemColumn(char const &item) const;
    int randomFallback(int row, int col,
                       std::vector<int> directions = {1, 2, 3, 4},
                       std::vector<char> const &walls = {'+'}) const;
    int getBackwardsDirection(int dir) const;
    int getMoveIfDirect(char target, int entity_row, int entity_col,
                        int &distance) const;
    bool itemInViz(char const &item,
                   std::vector<std::vector<char>> const &viz) const;
    std::string updateGuessStage(
        std::vector<std::vector<char>> const &viz) const;
    bool testForTwoSpaces(int const &direction, int const &row,
                          int const &col) const;
    int defaultMoveAlg(int const &map_row, int const &map_col,
                       std::vector<char> const &walls = {'+'}) const;
    void updateMemMap(const std::vector<std::vector<char>> &viz,
                      int const &map_row, int const &map_col);
    void expandMemMap();
    void printMemoryMap(std::ofstream &ofile) const;
    void testForXgames(int const &map_col,
                       std::vector<std::vector<char>> const &viz);
    int xGamesMoveAlg(int const &map_row, int const &map_col) const;
    std::vector<int> findXgamesMoves(int const &map_row,
                                     int const &map_col) const;

    std::string stage_guess = "";  // initialized as empty (no guess)
    int prev_move = -1;
    int prev_stage = 0;
    int backtrack_col = 0;
    std::vector<std::vector<char>> mem_map;

    bool A_taken = false;
    bool B_taken = false;
    int closestX_col = 99999;  // initialize to a high number
    int farthest_col_reached = 0;
    bool Xgames_happened = false;

    // saves compute so we dont keep testing if we need to expand
    bool bottom_found = false; // the vertical dimension of the mem_map
                
};

#endif  // BRAIN_H