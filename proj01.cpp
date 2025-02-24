#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>       // for pow()
#include <iomanip>     // for rounding
#include <set>         // for getting unique containers

// Function declarations
void load_dictionary(const std::string &filename,
                     std::vector<std::string> &positiveWords,
                     std::vector<std::string> &negativeWords);

void load_dataset(const std::string &filename, std::vector<std::string> &titles,
                  std::vector<int> &years, std::vector<double> &ratings,
                  std::vector<std::string> &reviews);

double calculate_mean(std::vector<double> const &ratings_vector, int const &n);

double calculate_stdev(std::vector<double> const &ratings_vector,
                       double const &mean, int const &n);

int find_the_minimums_index(std::vector<double> const &ratings_vector);

int find_the_maximums_index(std::vector<double> const &ratings_vector);

std::vector<std::string> split_string_to_words(std::string const &review);

std::vector<int> sentiment_analysis(
    std::vector<std::string> const &reviews_vector,
    std::vector<std::string> const &positive_words,
    std::vector<std::string> const &negative_words);

std::vector<int> find_multi_review_indexes(
    std::string const &title_in_question,
    std::vector<std::string> const &titles_vector);

void get_unique_titles_with_average_rating(
    std::vector<std::string> &unique_titles,
    std::vector<double> &averaged_ratings);

void get_best_and_worst_titles_overall(
    std::string &best_titles_output, std::string &worst_titles_output,
    std::vector<std::string> const &unique_titles1,
    std::vector<double> const &average_ratings1,
    std::vector<std::string> const &unique_titles2,
    std::vector<double> const &average_ratings2);


int main() {
    // File paths
    const std::string dataset1_path{"set1.csv"};
    const std::string dataset2_path{"set2.csv"};
    const std::string dictionary_path{"dictionary.txt"};

    // Load the word dictionary
    std::vector<std::string> positiveWords, negativeWords;
    load_dictionary(dictionary_path, positiveWords, negativeWords);

    // Load the datasets
    std::vector<std::string> titles1, titles2;
    std::vector<int> years1, years2;
    std::vector<double> ratings1, ratings2;
    std::vector<std::string> reviews1, reviews2;

    load_dataset(dataset1_path, titles1, years1, ratings1, reviews1);
    load_dataset(dataset2_path, titles2, years2, ratings2, reviews2);

    // Edge case: if one or more data set is empty
    if (titles1.empty() || titles2.empty()) {
        std::cout << "Error: One or more datasets are empty. Exiting program.\n";
        return 1;
    }

    // Edge case: if the dictionary file is empty
    if (positiveWords.empty() || negativeWords.empty()) {
        std::cout << "Error: The dictionary file is empty. Exiting program.\n";
        return 1;
    }

    // Edge case: multiple reviews of the same movie
    std::vector<std::string> unique_titles1 = titles1;
    std::vector<double> average_ratings1 = ratings1;

    std::vector<std::string> unique_titles2 = titles2;
    std::vector<double> average_ratings2 = ratings2;

    get_unique_titles_with_average_rating(unique_titles1, average_ratings1);
    get_unique_titles_with_average_rating(unique_titles2, average_ratings2);

    int count_set1 = titles1.size(), count_set2 = titles2.size();

    double mean1 = calculate_mean(ratings1, count_set1),
           mean2 = calculate_mean(ratings2, count_set2);

    double standard_dev1 = calculate_stdev(ratings1, mean1, count_set1),
           standard_dev2 = calculate_stdev(ratings2, mean2, count_set2);

    int minimum_index_set_1 = find_the_minimums_index(ratings1),
        minimum_index_set_2 = find_the_minimums_index(ratings2);

    double minimum_value_set1 = ratings1.at(minimum_index_set_1),
           minimum_value_set2 = ratings2.at(minimum_index_set_2);

    int maximum_index_set_1 = find_the_maximums_index(ratings1),
        maximum_index_set_2 = find_the_maximums_index(ratings2);

    double maximum_value_set1 = ratings1.at(maximum_index_set_1),
           maximum_value_set2 = ratings2.at(maximum_index_set_2);

    std::vector<int> analysis_results1 =
        sentiment_analysis(reviews1, positiveWords, negativeWords);
    int pos_count1{analysis_results1.at(0)}, neg_count1{analysis_results1.at(1)},
        inc_count1{analysis_results1.at(2)};

    std::vector<int> analysis_results2 =
        sentiment_analysis(reviews2, positiveWords, negativeWords);
    int pos_count2{analysis_results2.at(0)}, neg_count2{analysis_results2.at(1)},
        inc_count2{analysis_results2.at(2)};

    std::string best_titles_output, worst_titles_output;

    get_best_and_worst_titles_overall(best_titles_output, worst_titles_output,
                                      unique_titles1, average_ratings1,
                                      unique_titles2, average_ratings2);

    std::cout << std::fixed << std::setprecision(2);

    std::cout << " Set 1 Set 2\n";
    std::cout << "------------------------------------------------\n";
    std::cout << "Count:          " << count_set1 << std::setw(16) << count_set2 << '\n';
    std::cout << "Mean:           " << mean1 << std::setw(16) << mean2 << '\n';
    std::cout << "STDV:           " << standard_dev1 << std::setw(16) << standard_dev2 << '\n';
    std::cout << "Min:            " << minimum_value_set1 << std::setw(16) << minimum_value_set2 << '\n';
    std::cout << "Max:            " << maximum_value_set1 << std::setw(16) << maximum_value_set2 << '\n';
    std::cout << "Pos:            " << pos_count1 << std::setw(16) << pos_count2 << '\n';
    std::cout << "Neg:            " << neg_count1 << std::setw(16) << neg_count2 << '\n';
    std::cout << "Inc:            " << inc_count1 << std::setw(16) << inc_count2 << '\n';
    std::cout << best_titles_output << '\n';
    std::cout << worst_titles_output << '\n';

    return 0;
}

// Function definitions below main()

double calculate_mean(std::vector<double> const &ratings_vector, int const &n) {
    double total_of_ratings{0.0}; // The sum of every rating in the list

    for (double rating : ratings_vector) {
        total_of_ratings += rating;
    }
    return total_of_ratings / n; // This returns the mean
}

double calculate_stdev(std::vector<double> const &ratings_vector,
                       double const &mean, int const &n) {
    double numerator{0.0};

    for (double rating : ratings_vector) {
        numerator += pow((rating - mean), 2);
    }
    return pow((numerator / n), 0.5); // exponent 0.5 is square root
}

int find_the_minimums_index(std::vector<double> const &ratings_vector) {
    double current_minimum{ratings_vector.at(0)};
    int minimums_index = 0;
    int i = 0;

    for (double rating : ratings_vector) {
        if (rating < current_minimum) {
            current_minimum = rating;
            minimums_index = i;
        }
        i++;
    }
    return minimums_index;
}

int find_the_maximums_index(std::vector<double> const &ratings_vector) {
    double current_maximum{ratings_vector.at(0)};
    int maximums_index = 0;
    int i = 0;

    for (double rating : ratings_vector) {
        if (rating > current_maximum) {
            current_maximum = rating;
            maximums_index = i;
        }
        i++;
    }
    return maximums_index;
}

std::vector<std::string> split_string_to_words(std::string const &review) {
    std::string word = "";
    std::vector<std::string> words_vector;

    for (char ch : review) {
        if (ch == ' ') {
            if (word != "") {
                words_vector.push_back(word);
            }
            word = "";

        } else if (ch == ',' or ch == '.') {
            continue;

        } else {
            word.push_back(std::tolower(static_cast<unsigned char>(ch)));
        }
    }

    if (word != "") {
        words_vector.push_back(word);
    }
    return words_vector;
}

std::vector<int> sentiment_analysis(
    std::vector<std::string> const &reviews_vector,
    std::vector<std::string> const &positive_words,
    std::vector<std::string> const &negative_words) {
    std::vector<int> output_vector;
    int dataset_positives_count{0}, dataset_negatives_count{0},
        dataset_inconclusives_count{0};

    for (std::string const &review : reviews_vector) {
        std::vector<std::string> words_vector = split_string_to_words(review);

        int review_positives_count{0}, review_negatives_count{0};

        for (std::string const &review_word : words_vector) {
            for (std::string const &pos_word : positive_words) {
                if (review_word == pos_word) {
                    review_positives_count++;
                    break;
                }
            }
            for (std::string const &neg_word : negative_words) {
                if (review_word == neg_word) {
                    review_negatives_count++;
                    break;
                }
            }
        }

        if (review_positives_count > review_negatives_count) {
            dataset_positives_count++;
        } else if (review_negatives_count > review_positives_count) {
            dataset_negatives_count++;
        } else {
            dataset_inconclusives_count++;
        }
    }

    output_vector.push_back(dataset_positives_count);
    output_vector.push_back(dataset_negatives_count);
    output_vector.push_back(dataset_inconclusives_count);
    return output_vector;
}

std::vector<int> find_multi_review_indexes(
    std::string const &title_in_question,
    std::vector<std::string> const &titles_vector) {
    std::vector<int> indexs_vector;
    int i = 0;

    for (std::string title_from_vector : titles_vector) {
        if (title_in_question == title_from_vector) {
            indexs_vector.push_back(i);
        }
        i++;
    }
    return indexs_vector;
}

void get_unique_titles_with_average_rating(
    std::vector<std::string> &unique_titles,
    std::vector<double> &averaged_ratings) {
    std::set<std::string> unique_titles_set(unique_titles.begin(),
                                            unique_titles.end());

    for (std::string const &title : unique_titles_set) {
        std::vector<int> indexes_of_title =
            find_multi_review_indexes(title, unique_titles);
        int num_indexes = indexes_of_title.size();

        if (num_indexes > 1) {
            double sum_all_ratings = 0.0;

            for (int index : indexes_of_title) {
                sum_all_ratings += averaged_ratings.at(index);
            }
            double average_rating = sum_all_ratings / num_indexes;

            averaged_ratings.at(indexes_of_title.at(0)) = average_rating;

            for (int i = num_indexes - 1; i > 0; --i) {
                int remove_index = indexes_of_title.at(i);
                unique_titles.erase(unique_titles.begin() + remove_index);
                averaged_ratings.erase(averaged_ratings.begin() + remove_index);
            }
        }
    }
}

void get_best_and_worst_titles_overall(
    std::string &best_titles_output, std::string &worst_titles_output,
    std::vector<std::string> const &unique_titles1,
    std::vector<double> const &average_ratings1,
    std::vector<std::string> const &unique_titles2,
    std::vector<double> const &average_ratings2) {
    std::vector<std::string> all_titles = unique_titles1;
    std::vector<double> all_ratings = average_ratings1;

    int num_records_in_set2 = unique_titles2.size();

    for (int i = 0; i < num_records_in_set2; i++) {
        std::string current_title_of_set2 = unique_titles2.at(i);
        double current_rating_of_set2 = average_ratings2.at(i);
        all_titles.push_back(current_title_of_set2);
        all_ratings.push_back(current_rating_of_set2);
    }

    get_unique_titles_with_average_rating(all_titles, all_ratings);

    double min_rating = all_ratings.at(0);
    double max_rating = all_ratings.at(0);
    int num_total_records = all_ratings.size();

    for (int i = 1; i < num_total_records; i++) {
        if (all_ratings.at(i) < min_rating) {
            min_rating = all_ratings.at(i);
        }
        if (all_ratings.at(i) > max_rating) {
            max_rating = all_ratings.at(i);
        }
    }

    std::vector<std::string> min_titles;
    std::vector<std::string> max_titles;

    for (int i = 0; i < num_total_records; i++) {
        if (all_ratings.at(i) == min_rating) {
            min_titles.push_back(all_titles.at(i));
        }
        if (all_ratings.at(i) == max_rating) {
            max_titles.push_back(all_titles.at(i));
        }
    }

    int num_best_titles = max_titles.size(), num_worst_titles = min_titles.size();

    if (num_best_titles == 1) {
        best_titles_output = "Overall Best Title: " + max_titles.at(0);
    } else {
        best_titles_output = "Overall Best Titles (tie): ";
        for (int i = 0; i < num_best_titles; i++) {
            best_titles_output += max_titles.at(i);
            if (i < num_best_titles - 1) {
                best_titles_output += ", ";
            }
        }
    }

    if (num_worst_titles == 1) {
        worst_titles_output = "Overall Worst Title: " + min_titles.at(0);
    } else {
        worst_titles_output = "Overall Worst Titles (tie): ";
        for (int i = 0; i < num_worst_titles; i++) {
            worst_titles_output += min_titles.at(i);
            if (i < num_worst_titles - 1) {
                worst_titles_output += ", ";
            }
        }
    }
}

void load_dictionary(const std::string &filename,
                     std::vector<std::string> &positiveWords,
                     std::vector<std::string> &negativeWords) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening dictionary file." << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line);
    const char delimiter{','};
    std::string token;
    std::stringstream posStream(line);
    while (std::getline(posStream, token, delimiter)) {
        positiveWords.push_back(token);
    }

    std::getline(file, line);
    std::stringstream negStream(line);
    while (std::getline(negStream, token, delimiter)) {
        negativeWords.push_back(token);
    }
}

void load_dataset(const std::string &filename, std::vector<std::string> &titles,
                  std::vector<int> &years, std::vector<double> &ratings,
                  std::vector<std::string> &reviews) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening dataset file: " << filename << std::endl;
        return;
    }

    std::string line, title, review;
    int year;
    double rating;
    std::string temp;
    getline(file, line);

    while (getline(file, line)) {
        std::stringstream ss(line);

        getline(ss, title, ',');
        getline(ss, temp, ',');
        year = std::stoi(temp);
        getline(ss, temp, ',');
        rating = std::stod(temp);
        getline(ss, review);

        titles.push_back(title);
        years.push_back(year);
        ratings.push_back(rating);
        reviews.push_back(review);
    }

    file.close();
}