/* spiral_linear_original.cpp - lab 06 of CSE 232 at MSU

This is my original solution to creating a matrix of size `n` that
fills in single-digit values starting at the bottom right corner
and spiraling all the way until the middle is reached, where it puts a 
value of zero.

This code represents my initial linear/functional problem-solving
approach as it was completed in one go, with the goal of getting a
solution very quickly.

Visit `spiral_object_oriented.cpp` to see my solution to solving this 
same spiral matrix problem using an object-oriented approach.  

*/

#include <vector>
#include <iostream>
#include <string>
#include <sstream>

std::string Two_D_Vector_To_String(std::vector<std::vector<char>> vec);

bool get_direction_change(
  std::vector<std::vector<char>> grid,
  std::vector<int> const &movement,
  int const &row,
  int const &col,
  int const &n
);

int main() {
  int n = 7;  // size of grid (Allways an odd number)

  // initialize grid to `n` size filling it with 'x's
  std::vector<std::vector<char>> grid(n);
  for (int i = 0; i < n; ++i) {
      grid[i] = std::vector<char>(n, 'x');
  }

  int multi_digit_value = n * n - 1;  // this can be a two-digit value. Ex: 24
  // but later in the grid we only want the one's place digit, '4'

  int row = n - 1;
  int col = n - 1;
  std::vector<std::vector<int>> directions {
    {0 ,-1},  // left 
    {-1, 0},  // up
    {0 , 1},  // right
    {1 , 0}   // down
  };
  int num_direction_changes = 0;
  int directions_index = 0;

  for (int i = multi_digit_value ; i >= 0 ; --i ) {
      // `value` needs to be the one's place value of type char
      char value = i % 10 + '0';
      grid.at(row).at(col) = value;

      // `movement` is a 2 value pair whose respective values move (row, col)
      // `movement` varies depending on which direction you're going
      // Ex to move left: `movement` = (0,-1)
      std::vector<int> movement = directions.at(directions_index);
      bool direction_change = get_direction_change(
        grid, movement, row, col, n);

      if (direction_change) {
          num_direction_changes += 1;
          directions_index = num_direction_changes % 4;
          movement = directions.at(directions_index);
      }  
      row += movement.at(0);
      col += movement.at(1);
  }

  std::string output = Two_D_Vector_To_String(grid);
  std::cout << output;
}

// Function definitions

// This function is given at the beginning of lab 06
// It makes a string so we can easily print our grid to standard output
std::string Two_D_Vector_To_String(std::vector<std::vector<char>> vec) {
  std::stringstream os;
  for (int i = 0; i < static_cast<int>(vec.size()); i++) {
    for (int j = 0; j < static_cast<int>(vec.at(i).size()); j++) {
      os << vec.at(i).at(j) << " ";
    }
    os << "\n";
  }
  return os.str();
}

// Find out if we need a direction change
bool get_direction_change(
  std::vector<std::vector<char>> grid,
  std::vector<int> const &movement,
  int const &row,
  int const &col,
  int const &n) {

  int test_row = row + movement.at(0);
  int test_col = col + movement.at(1);
  if (test_row == n || test_row < 0 || test_col == n || test_col < 0) {
      return true;
  } else {
      char next_value = grid.at(test_row).at(test_col); 
      if (next_value != 'x') return true;
  }
  
  return false;
}
