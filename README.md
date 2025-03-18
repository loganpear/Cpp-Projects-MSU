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
