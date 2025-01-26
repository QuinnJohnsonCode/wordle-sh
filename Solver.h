#ifndef SOLVER_H
#define SOLVER_H

#include "LetterColour.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <algorithm>

class Solver
{
public:
    std::unordered_set<std::string> filter_words;
    
    Solver();

    Solver(const std::unordered_set<std::string> &words);

    void set_filter_words(const std::unordered_set<std::string> &words);

    std::string get_best_word(const std::vector<LetterColour> &previous_guess);

private:

    int determine_score(const std::vector<std::pair<char, int>> &sorted_frequencies, const std::string &word);

    std::unordered_map<char, int> get_character_frequencies();

    void add_character_frequencies(const std::string &word, std::unordered_map<char, int> &frequencies);

    void filter_word_set(const std::vector<LetterColour> &previous_guess);

    // 0 to filter, 1 to not filter (i.e. passes the filter)
    int passes_word_filter(const std::string &word, const std::vector<LetterColour> &previous_guess);
};

#endif