/* spiral_object_oriented.cpp

This is my second solution to solving the spiral matrix problem.
I created a `GridClass` to hold attributes and methods of the grid, 
leading to a short and simple `main()` function. The purpose of 
resolving this problem in an object-oriented way was to get more 
experience with classes in C++ and also produce code that can potentially
have more applications than just solving the one specific task of 
creating a normal spiral matrix. 

I also spent more time designing this solution as I prioritized
readability and flexibility in the `grid` object class and 
`main()` function. 

*/

#include <vector>
#include <iostream>
#include <string>
#include <sstream>

// Function declarations for GridClass methods
class GridClass {
public:
    int n = 0;    // size `n` of the spiral matrix (Always an odd number)
    int row = 0;  // the current row index
    int col = 0;  // the current column index

    int turn_count = 0;  // keeps track of which direction to go
    std::vector<std::vector<char>> grid;
    std::vector<std::vector<int>> directions {
      {0 ,-1},  // left 
      {-1, 0},  // up
      {0 , 1},  // right
      {1 , 0}   // down
    };

    std::vector<int> movement_values = directions.at(turn_count);

    // Constructor
    GridClass(int n_);

    // Method to print the grid to standard output
    void show_grid();

    // Method to set the current (row, col) position to the `value`
    void set_row_col(char const &value);

    // Method to check if a turn is needed based on the grid and movement
    bool check_if_turn();

    // Method to turn the grid, changing direction
    void turn_grid();

    // Method to update row and column indexes according to movement
    void update_row_col_indexes();
};

int main() {
    int n = 5; // input a size `n` of the matrix (Always an odd num)
    int initial_value = n * n - 1; // start at the highest value 
    char value;  // `value` is a single digit char one's place value

    GridClass grid = GridClass(n);
    
    // starts from the bottom right corner of the grid
    // with the max value in that corner
    for (int i = initial_value ; i >= 0 ; --i) {

        // get last digit of value and convert to char using + '0'
        value = i % 10 + '0'; 
        grid.set_row_col(value);

        // check if a turn is needed
        bool execute_turn = grid.check_if_turn();
        if (execute_turn) grid.turn_grid(); 
        
        // update row and column indices 
        grid.update_row_col_indexes();
    }

    grid.show_grid();
}

// Function definitions for GridClass methods

GridClass::GridClass(int n_) {
    n = n_;
    std::vector<std::vector<char>> grid_(n);

    // initialize the board with all 'x's
    for (int i = 0; i < n; ++i) {
        grid_.at(i) = std::vector<char>(n, 'x');
    }
    grid = grid_; 

    // initialize row and col coordinates to the bottom right corner
    row = n - 1;
    col = n - 1;
}

void GridClass::show_grid() {
    std::stringstream os;
    for (int i = 0; i < static_cast<int>(grid.size()); i++) {
        for (int j = 0; j < static_cast<int>(grid.at(i).size()); j++) {
            os << grid.at(i).at(j) << " ";
        }
        os << "\n";
    }
    std::cout << os.str();
}

void GridClass::set_row_col(char const &value) {
    grid.at(row).at(col) = value;
}

bool GridClass::check_if_turn() {
    // initializing the potential next move's coordinate
    int test_row = row + movement_values.at(0);
    int test_col = col + movement_values.at(1);

    // first check if the next move would be out of bounds
    if (test_row == n || test_row < 0 || test_col == n || test_col < 0) {
        return true;
    } else { // this happens once we KNOW the next move exists on the board
        // check if the next move in the current direction already has a value
        // aka check if the next move's value is not the initial 'x'
        char next_value = grid.at(test_row).at(test_col); 
        if (next_value != 'x') return true;
    }
    // if next move is in bounds and is not set to a value other than 'x'
    return false;  // then there is no need to turn, so return false
}

void GridClass::turn_grid() {
    // `turn_count` determines whether to go up/down or left/right
    turn_count += 1;

    // I use mod (%) 4 to keep track of the direction, so when Ex:
    // `turn_count` goes above `3`, it starts over from `left` again
    movement_values = directions.at(turn_count % 4);
}

void GridClass::update_row_col_indexes() {
    row += movement_values.at(0);
    col += movement_values.at(1);
}
