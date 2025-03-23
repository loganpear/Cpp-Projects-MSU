#ifndef PPMIO_H
#define PPMIO_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>

// Structure to represent an RGB pixel
struct RGB
{
    unsigned char r, g, b;
};

// Function to read a PPM (P6) file and store it in a 2D vector of RGB structs
std::vector<std::vector<RGB>> readPPM(const std::string &filename);

// Function to write a 2D vector of RGB structs to a PPM (P6) file
void writePPM(const std::string &filename, const std::vector<std::vector<RGB>> &image);

#endif // PPMIO_H
