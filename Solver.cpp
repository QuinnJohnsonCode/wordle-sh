#include "Solver.h"
#include "LetterColour.h"
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <algorithm>

Solver::Solver() {}

Solver::Solver(const std::unordered_set<std::string> &words)
{
    // Clone words into a set to be filtered down
    this->filter_words = words;
}

void Solver::set_filter_words(const std::unordered_set<std::string> &words)
{
    this->filter_words = words;
}

std::string Solver::get_best_word(const std::vector<LetterColour> &previous_guess)
{   
    std::string best_word;

    // Filter the word set
    if (previous_guess.size() > 0)
        filter_word_set(previous_guess);

    // Get frequencies of letters (sorted)
    std::unordered_map<char, int> frequencies_dict = get_character_frequencies();
    
    std::vector<std::pair<char, int>> sorted_frequencies(frequencies_dict.begin(), frequencies_dict.end());
    std::sort(sorted_frequencies.begin(), sorted_frequencies.end(), 
        [] (std::pair<char, int> a, std::pair<char, int> b) 
        {
            return a.second > b.second;
        }
    );

    // Determine best_word based on frequencies
    int max_score = 0;
    int temp_score = 0;
    for (const std::string &word: filter_words)
    {
        temp_score = determine_score(sorted_frequencies, word);
        if (temp_score > max_score)
        {
            max_score = temp_score;
            best_word = word;
        }
    }

    return best_word;
}

int Solver::determine_score(const std::vector<std::pair<char, int>> &sorted_frequencies, const std::string &word)
{
    std::unordered_set<char> seen{};
    int score = 0;
    for (char c : word)
    {
        auto it = std::find_if(sorted_frequencies.begin(), sorted_frequencies.end(),
                            [c](const std::pair<char, int>& p) { return p.first == c; });
        if (it != sorted_frequencies.end() && !seen.contains(it->first))
        {
            seen.insert(it->first);
            score += it->second;  // Add the frequency score for this letter
        }
    }

    return score;
}

std::unordered_map<char, int> Solver::get_character_frequencies()
{
    std::unordered_map<char, int> frequencies_dict;
    for (char c = 'a'; c <= 'z'; ++c) 
    {
        frequencies_dict[c] = 0;
    }

    for (auto &word : filter_words)
        add_character_frequencies(word, frequencies_dict);
    
    return frequencies_dict;
}

void Solver::add_character_frequencies(const std::string &word, std::unordered_map<char, int> &frequencies)
{
    for (char c : word)
        frequencies[c] += 1;
}

void Solver::filter_word_set(const std::vector<LetterColour> &previous_guess)
{
    std::unordered_set<std::string> valid_words;
    for (const auto &word : filter_words)
    {
        if (passes_word_filter(word, previous_guess))
        {
            valid_words.insert(word);
        }
    }
    filter_words = std::move(valid_words);
}

// 0 to filter, 1 to not filter (i.e. passes the filter)
int Solver::passes_word_filter(const std::string &word, const std::vector<LetterColour> &previous_guess)
{
    std::unordered_set<char> unique_chars;
    std::unordered_set<int> yellow_positions;

    for (std::vector<LetterColour>::size_type i = 0; i < previous_guess.size(); ++i)
    {
        char letter = previous_guess[i].letter;
        if (previous_guess[i].colour == Colour::NO_COLOUR)
        {
            if (word.find(letter) != std::string::npos)
                return 0;  // fails filter if the letter is in the word
        }
        else if (previous_guess[i].colour == Colour::YELLOW)
        {
            // Track that the letter is yellow
            yellow_positions.insert(i);

            // The letter must be in the word, but not in the same position
            if (word[i] == letter)
                return 0;  // fails filter if the letter is in the same position
            unique_chars.insert(letter);
        }
        else if (previous_guess[i].colour == Colour::GREEN)
        {
            // The letter must be in the same position
            if (word[i] != letter)
                return 0;  // fails filter if the letter is not in the same position
        }
    }

    // Check for YELLOW letters that must exist elsewhere in the word
    for (auto letter : unique_chars)
    {
        if (word.find(letter) == std::string::npos)
            return 0;  // fails filter if the YELLOW letter is missing
    }

    // Check that no letter marked NO_COLOUR appears in any position
    for (std::vector<LetterColour>::size_type i = 0; i < previous_guess.size(); ++i)
    {
        if (previous_guess[i].colour == Colour::NO_COLOUR)
        {
            char letter = previous_guess[i].letter;
            if (word[i] == letter)
                return 0;  // fails filter if the letter appears at the same position
        }
    }

    return 1;
}