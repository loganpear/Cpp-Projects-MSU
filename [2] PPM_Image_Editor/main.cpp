// Example run from the terminal (inversion + contrast + mirror): 
// g++ -std=c++20 -Wall main.cpp ppmio.cpp && ./a.out car.ppm out.ppm -i -x -m 

#include "ppmio.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

void interpret_input(int const &argc, char *const argv[],
                     std::string &input_file, std::string &output_file,
                     std::vector<std::string> &flags_vec);

void execute_all_flags(std::vector<std::vector<RGB>> &image_matrix,
                       std::vector<std::string> const &flags_vec);

void apply_grayscale(std::vector<std::vector<RGB>> &image_matrix);

void apply_inversion(std::vector<std::vector<RGB>> &image_matrix);

void apply_contrast(std::vector<std::vector<RGB>> &image_matrix);

void apply_blur(std::vector<std::vector<RGB>> &image_matrix);

void apply_mirror(std::vector<std::vector<RGB>> &image_matrix);

void apply_compression(std::vector<std::vector<RGB>> &image_matrix);

int main(int argc, char *argv[]) {

    // initialize variables to store i/o files and flags
    std::string input_file, output_file;
    std::vector<std::string> flags_vec;

    // test for errors in command line input. If valid, store appropriate values
    try {
        interpret_input(argc, argv, input_file, output_file, flags_vec);
    } catch (std::invalid_argument const &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    // read the input ppm file and store it in a matrix
    std::vector<std::vector<RGB>> image_matrix;
    try {
        image_matrix = readPPM(input_file);
    } catch (std::exception const &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    // Loop through all flags from flags_vec and apply to image_matrix
    try {
        execute_all_flags(image_matrix, flags_vec);
    } catch (std::exception const &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    // write the modified image_matrix to the output file
    try {
        writePPM(output_file, image_matrix);
    } catch (std::exception const &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}

// Function to interpret the command line input by testing for errors and
// storing the input file names and flags in the appropriate variables
void interpret_input(int const &argc, char *const argv[],
                     std::string &input_file, std::string &output_file,
                     std::vector<std::string> &flags_vec) {

    // number of arguments from the command line excluding the executable
    int num_args = argc - 1;

    // check if the number of arguments is less than 2 (input and output files)
    if (num_args < 2) {
        throw std::invalid_argument(
            "Error: One input file and one output file are required.");
    }

    std::string const possible_flag_chars = "gixbmc";
    int count_non_flags = 0; // later we check to confirm there are 2 non-flags

    // skip the executable by starting at i = 1
    for (int i = 1; i < argc; ++i) {
        std::string const &arg = argv[i];
        int arg_len = arg.length();

        // if the argument doesnt start with a '-' then this first occurance is
        // the input file, then the next is the output file
        if (arg.at(0) != '-') {
            count_non_flags++;
            if (count_non_flags == 1) {
                input_file = arg;
            } else if (count_non_flags == 2) {
                output_file = arg;
            } else {
                throw std::invalid_argument(
                    "Error: Too many non-flag arguments. Only 2 are allowed: "
                    "input and output files.");
            }
            // continues so that only flags are assessed in the rest of the loop
            continue;
        }

        // the following conditions know that the argument is a flag
        if (arg_len != 2) { // make sure flag is of length 2
            throw std::invalid_argument(
                "Error: Invalid flag: '" + arg +
                "'. Flags must be a hyphen(-) followed by 1 valid character."
                " Examples: -g, -i, -x, -b, -m, -c");
        }
        // make sure flag is in the list of possible flags
        if (possible_flag_chars.find(arg.at(1)) == std::string::npos) {
            throw std::invalid_argument(
                "Error: Invalid flag: '" + arg +
                "'. Allowed flags: -g, -i, -x, -b, -m, -c");
        }
        // if none of the errors are triggered then we push_back the flag
        flags_vec.push_back(argv[i]);
    }

    // make sure that there are exactly 2 non-flag arguments aka files
    if (count_non_flags != 2) {
        throw std::invalid_argument(
            "Error: One input file and one output file are required.");
    }
}

// Loops through every flag in flags_vec and calls the appropriate functions to
// alter image_matrix by reference in the order that flags appear
void execute_all_flags(std::vector<std::vector<RGB>> &image_matrix,
                       std::vector<std::string> const &flags_vec) {

    for (std::string const &flag : flags_vec) {

        // Make sure there is some amount of image to work with
        if (image_matrix.empty()) {
            throw std::invalid_argument(
                "Error: No image to apply flags to. Image is empty.");
        }

        if (flag == "-g") {
            apply_grayscale(image_matrix);
        } else if (flag == "-i") {
            apply_inversion(image_matrix);
        } else if (flag == "-x") {
            apply_contrast(image_matrix);
        } else if (flag == "-b") {
            apply_blur(image_matrix);
        } else if (flag == "-m") {
            apply_mirror(image_matrix);
        } else if (flag == "-c") {
            apply_compression(image_matrix);
        }
    }
}

// Converts image_matrix to grayscale
void apply_grayscale(std::vector<std::vector<RGB>> &image_matrix) {
    int num_rows = image_matrix.size();
    int num_cols = image_matrix[0].size();

    // iterates through each pixel (row and col) of the image_matrix
    for (int row = 0; row < num_rows; row++) {
        for (int col = 0; col < num_cols; col++) {

            // gets the pixel's current RGB values and averages them
            RGB &pixel = image_matrix.at(row).at(col);
            unsigned char gray = (pixel.r + pixel.g + pixel.b) / 3;
            pixel = {gray, gray, gray};
        }
    }
}

// inverts the colors of the image_matrix
void apply_inversion(std::vector<std::vector<RGB>> &image_matrix) {
    int num_rows = image_matrix.size();
    int num_cols = image_matrix.at(0).size();

    for (int row = 0; row < num_rows; ++row) {
        for (int col = 0; col < num_cols; ++col) {

            // changes each pixel in place (reference) to its inverse
            RGB &pixel = image_matrix.at(row).at(col);
            pixel.r = 255 - pixel.r;
            pixel.g = 255 - pixel.g;
            pixel.b = 255 - pixel.b;
        }
    }
}

// applies a contrast factor of 1.2 to the image_matrix
void apply_contrast(std::vector<std::vector<RGB>> &image_matrix) {
    int num_rows = image_matrix.size();
    int num_cols = image_matrix.at(0).size();

    for (int row = 0; row < num_rows; ++row) {
        for (int col = 0; col < num_cols; ++col) {
            RGB &pixel = image_matrix.at(row).at(col);

            int new_r = (pixel.r - 128) * 1.2 + 128;
            int new_g = (pixel.g - 128) * 1.2 + 128;
            int new_b = (pixel.b - 128) * 1.2 + 128;

            // Make sure values floor at 0 and roof at 255
            if (new_r < 0)
                new_r = 0;
            if (new_r > 255)
                new_r = 255;

            if (new_g < 0)
                new_g = 0;
            if (new_g > 255)
                new_g = 255;

            if (new_b < 0)
                new_b = 0;
            if (new_b > 255)
                new_b = 255;

            // assign new values casted to unsigned chars
            pixel.r = static_cast<unsigned char>(new_r);
            pixel.g = static_cast<unsigned char>(new_g);
            pixel.b = static_cast<unsigned char>(new_b);
        }
    }
}

// applies a blur effect to the image_matrix
void apply_blur(std::vector<std::vector<RGB>> &image_matrix) {

    int num_rows = image_matrix.size();
    int num_cols = image_matrix.at(0).size();

    // address the edge case that the image is too small to blur
    if (num_rows < 3 || num_cols < 3) {
        std::cerr << ("Warning: Blur had no effect. Image must be at least 3x3 "
                      "to blur.\n");
        // Now it continues main() without bluring as this isnt a reason to
        // terminate the program, it just tells the user that nothing will blur
        return;
    }

    // make a copy of the original image_matrix to reference
    std::vector<std::vector<RGB>> original_matrix = image_matrix;

    // Iterate over each non-edge pixel in the image_matrix
    for (int row = 1; row < num_rows - 1; ++row) {
        for (int col = 1; col < num_cols - 1; ++col) {

            // initializes sums of r, g and b
            int sum_r = 0, sum_g = 0, sum_b = 0;

            // iterate over each neighbor pixel including the current pixel
            for (int i = -1; i <= 1; ++i) {     // for each neighbor row (i)
                for (int j = -1; j <= 1; ++j) { // for each neighbor col (j)
                    RGB const &neighbor =
                        original_matrix.at(row + i).at(col + j);
                    sum_r += neighbor.r;
                    sum_g += neighbor.g;
                    sum_b += neighbor.b;
                }
            }
            // average the sums and assign them by reference to the pixel
            image_matrix.at(row).at(col) = {
                static_cast<unsigned char>(sum_r / 9),
                static_cast<unsigned char>(sum_g / 9),
                static_cast<unsigned char>(sum_b / 9)};
        }
    }
}

// mirrors the image_matrix horizontally
void apply_mirror(std::vector<std::vector<RGB>> &image_matrix) {

    int num_rows = image_matrix.size();
    int num_cols = image_matrix.at(0).size();

    // handle edge case that num_cols is 1 (theres nothing to flip)
    if (num_cols == 1) {
        std::cerr << "Warning: Mirror had no effect. Image needs more than 1 "
                     "column to mirror.\n";
        return;
    }

    // make a copy of the original image_matrix to reference
    std::vector<std::vector<RGB>> original_matrix = image_matrix;

    // iterate over each row and col in the image_matrix normally
    for (int row = 0; row < num_rows; ++row) {
        for (int col = 0; col < num_cols; ++col) {

            // get the mirrored column's pixel to replace the current pixel
            int mirror_col = (num_cols - 1) - col;
            image_matrix.at(row).at(col) =
                original_matrix.at(row).at(mirror_col);
        }
    }
}

// removes every even row and column from the image_matrix to compress it
void apply_compression(std::vector<std::vector<RGB>> &image_matrix) {

    int num_rows = image_matrix.size();
    int num_cols = image_matrix.at(0).size();

    // Case: num_rows or num_cols is 1 (compressing would return nothing)
    if (num_rows == 1 || num_cols == 1) {
        // this is a terminating error becuase the user is instructing the
        // program to produce nothing which deems the continuation of this
        // program useless
        throw std::invalid_argument(
            "Error: You have compressed the image too far. "
            "Image must be at least 2x2 for compression. Terminating "
            "program since the current image is nothing after compression.");
    }

    // declare a new matrix to store the compressed image_matrix
    std::vector<std::vector<RGB>> new_matrix;

    // iterate through odd rows and cols and store in new_matrix
    for (int row = 1; row < num_rows; row += 2) {

        // append a blank new row to new_matrix for each odd row
        new_matrix.push_back(std::vector<RGB>{});

        for (int col = 1; col < num_cols; col += 2) {
            new_matrix.back().push_back(image_matrix.at(row).at(col));
        }
    }
    // replace image_matrix with the compressed new_matrix
    image_matrix = new_matrix;
}
