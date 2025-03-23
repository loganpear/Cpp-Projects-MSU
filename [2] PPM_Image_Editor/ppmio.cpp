#include "ppmio.h"

std::vector<std::vector<RGB>> readPPM(const std::string &filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    std::string line;
    // Read the magic number (P6)
    std::getline(file, line);
    if (line != "P6")
    {
        throw std::runtime_error("Invalid PPM format: " + line);
    }

    // Read image dimensions and maximum color value
    int width = 0, height = 0, max_val = 0;
    while (std::getline(file, line))
    {
        if (line[0] == '#')
            continue; // Skip comments
        std::istringstream iss(line);
        if (!(iss >> width >> height))
            continue;
        else
            break;
    }

    file >> max_val;
    file.ignore(256, '\n'); // Skip to the binary data

    if (max_val != 255)
    {
        throw std::runtime_error("Unsupported max value: " + std::to_string(max_val));
    }

    // Read pixel data
    std::vector<std::vector<RGB>> image(height, std::vector<RGB>(width));
    for (int i = 0; i < height; ++i)
    {
        file.read(reinterpret_cast<char *>(image[i].data()), width * 3);
    }

    return image;
}

void writePPM(const std::string &filename, const std::vector<std::vector<RGB>> &image)
{
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    int height = image.size();
    int width = (height > 0) ? image[0].size() : 0;

    if (width == 0)
    {
        throw std::runtime_error("Empty image data.");
    }

    // Write the header
    file << "P6\n"
         << width << " " << height << "\n255\n";

    // Write pixel data
    for (const auto &row : image)
    {
        file.write(reinterpret_cast<const char *>(row.data()), width * 3);
    }
}
