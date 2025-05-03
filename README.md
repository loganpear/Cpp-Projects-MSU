# C++ Projects  

This repository contains a collection of C++ projects completed as part of coursework through Michigan State University - CSE 232.  

## Project 1: Movie Reviews Classification - Sentiment Analysis

### Overview  
This project implements a sentiment analysis tool for movie reviews. It processes two datasets of movie reviews (`set1.csv` and `set2.csv`) and analyzes them using a dictionary of positive and negative words (`dictionary.txt`). The program calculates statistical metrics such as mean, standard deviation, and sentiment counts (positive, negative, and inconclusive reviews). It also identifies the best and worst-rated movies across the datasets.  

### Key Features  
- **Data Loading**: Reads movie review data from CSV files and a dictionary of sentiment words.  
- **Statistical Analysis**: Computes mean, standard deviation, and identifies minimum/maximum ratings.  
- **Sentiment Analysis**: Determines the sentiment of each review based on the presence of positive and negative words.  
- **Duplicate Handling**: Aggregates ratings for movies with multiple reviews.  
- **Output**: Displays results in a formatted table, including overall best and worst movies.  

### Attached Files  
- `main.cpp`: The main C++ program implementing the sentiment analysis.  
- `set1.csv`: Dataset 1 containing movie titles, years, ratings, and reviews.  
- `set2.csv`: Dataset 2 containing additional movie data.
- `dictionary.txt`: Contains two comma-separated lists of words, where the first line represents positive words and the second line represents negative words, used for sentiment analysis of movie reviews.
- `example_output.txt`: Sample output of the program for reference.  

This project demonstrates proficiency in file I/O, data manipulation, and statistical analysis in C++.

---

## Project 2: PPM Image Editor – Image Processing Application  

### Overview  
This project implements a full-featured image processing application that manipulates images in the Portable Pixmap (PPM) format. The application supports a variety of common image manipulation techniques, applied through command-line arguments. Users specify an input image, an output image, and desired transformations like color inversion, grayscale conversion, contrast enhancement, blurring, mirroring, and compression.

The program reads the input `.ppm` file into a structured 2D vector of RGB pixels, applies the specified operations sequentially, and then outputs the modified image.  

### Key Features  
- **PPM File Handling**: Efficiently reads from and writes to binary `.ppm` (P6) files.
- **Command-Line Interface**: Flexible parsing of input/output paths and transformation flags.
- **Color Manipulations**: Supports grayscale, color inversion, and contrast enhancement.
- **Geometric Transformations**: Implements horizontal mirroring and lossy compression.
- **Filtering**: Applies a box-blur filter for smoothing effects.
- **Robust Error Handling**: Provides clear and informative error messages for invalid inputs or file I/O issues.

### Supported Flags  
| Flag | Description                                       |
|------|---------------------------------------------------|
| `-g` | Convert image to grayscale                        |
| `-i` | Invert image colors                               |
| `-x` | Adjust contrast (factor of 1.2)                   |
| `-b` | Blur image using a box-blur filter                |
| `-m` | Mirror image horizontally                         |
| `-c` | Compress image by subsampling (skipping pixels)   |

### Example Command-Line Usage  
```bash
# Apply inversion, contrast, and mirror transformations:
g++ -std=c++20 -Wall main.cpp ppmio.cpp && ./a.out car.ppm out.ppm -i -x -m
```

- Input: `car.ppm`  
- Output: `out.ppm` (inverted colors, enhanced contrast, and mirrored horizontally)

### Attached Files  
- `main.cpp`: Core application logic and transformation implementations.
- `ppmio.h`: Header file defining the RGB struct and I/O function prototypes for `.ppm` files.
- `ppmio.cpp`: Implementation of `.ppm` file reading and writing functions.
- `car.ppm`: Sample input image file used for demonstration.
- `out.ppm`: Example output after applying inversion, contrast enhancement, and mirroring.

### Technical Details  
- **RGB Struct Definition**:
  ```cpp
  struct RGB {
      unsigned char r, g, b;  // red, green, and blue pixel values
  };
  ```

- **PPM Image Reading** (`readPPM` function):  
  Reads binary PPM files into a 2D vector structure, ensuring correct format verification and error handling.

- **PPM Image Writing** (`writePPM` function):  
  Outputs the processed image data into a properly formatted binary `.ppm` file.

This project demonstrates proficiency in advanced file I/O, 2D vector manipulations, command-line interface parsing, and algorithmic image processing techniques in C++.

---

## Project 3: AI Game Agent – Autonomous Decision-Making in a Grid-Based Game  

### Overview  
This project centers on implementing an AI agent that autonomously navigates a grid-based game environment using strategic decision-making logic. The game includes various stage types such as mazes, open fields, and trap zones. The agent must make real-time movement decisions based on a limited visibility window, internal memory, and environment cues.

Only the AI logic was developed by the student (see `brain.cpp` and `brain.h`), while all other supporting files—including the game engine, map files, and interface headers—were provided by Michigan State University's CSE 232 course.  

### Key Features  
- **Autonomous Agent Behavior**: Implements logic to navigate the map, avoid traps, and pursue goals using local observations.
- **Environment Awareness**: Leverages a memory map to recall previously seen locations and adjust strategy accordingly.
- **Custom Movement Logic**: Includes specialized logic (`xGamesMoveAlg`) to handle complex stage-specific movement behavior.
- **Minimal Input Interface**: Uses the provided `GameState` structure to interpret current surroundings and decide on the next move.

### Attached Files  
- `brain.cpp`: Contains the AI movement logic and decision algorithms.
- `brain.h`: Declares the `Brain` class interface and movement-related utilities.
- All other files (e.g., `main.cpp`, `game.h`, `enemy.cpp`, `.map` files): Provided by CSE 232 instructional staff and not authored by the student.

### Technical Highlights  
- Decision-making is based on visibility constraints and spatial reasoning.
- AI logic is stage-aware, adapting movement strategy depending on map configuration.
- Demonstrates real-time interaction between AI behavior and game state updates.

This project demonstrates proficiency in AI agent modeling, decision logic under partial observability, and integration with pre-built game systems in C++.  
