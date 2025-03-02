Here's a short README file based on your explanation:

---

# Spiral Matrix Solutions

## Overview

This repository contains solutions to the **Spiral Matrix** problem, which is the task of filling a square matrix of size `n` with numbers in a spiral pattern, starting from the bottom-right corner and working towards the center.

### Problem Description:
Given a size `n` (which is always an odd number), the goal is to fill an `n x n` matrix with values starting from the highest number (n^2 - 1) at the bottom-right corner, spiraling around, and ending with the value 0 at the center. The matrix should fill in such a way that each number is placed in the next available position according to the spiral pattern: left, up, right, and down.

---

## Solutions

### 1. **Original Solution (Linear Approach)**

The **original solution** was created as part of a lab assignment for my **CSE 232** course at MSU. This solution uses a straightforward linear/functional approach to solve the spiral matrix problem. The main goal was to quickly produce a working solution with minimal complexity and code. The program uses a single loop to iteratively fill the matrix while managing direction changes and edge cases.

### 2. **Object-Oriented Solution (GridClass)**

After completing the original solution, I wanted to improve my skills with object-oriented programming (OOP) in C++. In this **second solution**, I refactored the problem into an object-oriented design by creating a `GridClass`. This class encapsulates the attributes and methods of the grid, making the solution more modular and flexible. By using a class, the code becomes easier to manage, extend, and apply to different problems beyond just the spiral matrix task.

---

## File Descriptions

1. **spiral_linear_original.cpp**  
   This is the first solution, where the matrix is filled using a functional approach. The code solves the problem by iterating through the matrix and checking conditions to change direction as needed.

2. **spiral_object_oriented.cpp**  
   The second solution, which implements the spiral matrix problem using object-oriented programming with a `GridClass`. This version is more modular and flexible and demonstrates a better understanding of C++ classes and OOP principles.

3. **output_size7.txt**  
   This file contains an example output of the spiral matrix generated with a matrix size of 7. It shows how the numbers are arranged in a spiral pattern in a 7x7 matrix.

---

## Conclusion

These two approaches represent different problem-solving strategies, one focused on functional programming and the other on object-oriented design. Both solutions provide a working implementation of the spiral matrix problem, showcasing different styles of coding in C++.
