#include "LetterColour.h"
#include "Solver.h"
#include <iostream>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <random>
#include <format>
#include <algorithm>

std::unordered_set<std::string> load_words_from_file(std::string path)
{
    std::unordered_set<std::string> words{};
    std::ifstream file_stream(path, std::ios::binary);

    if (!file_stream.is_open())
        std::cout << " failed to open " << path << '\n';
    else
    {
        std::string new_word;
        while (file_stream >> new_word)
            words.insert(new_word);
    }

    return words;
}

std::string pick_random_word(std::unordered_set<std::string> &words)
{
    auto word_iter = words.begin();

    // Use uniform_int_distribution to get a word index from 0 to words.size() - 1
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, words.size() - 1);

    // Advance word_iter by a random value
    std::advance(word_iter, distrib(gen));

    return *word_iter;
}

void debug_cout(int debug, std::string output)
{
    if (debug)
        std::cout << output;
}

// A guess is valid if:
//  1. The guess is exactly 5 characters long
//  2. The guess exists in the overall set of words
int check_valid_guess(std::unordered_set<std::string> &words, std::string guess)
{
    if (guess.size() != 5)
    {
        std::cout << guess << " is not 5 characters long.\n";
        return 0;
    }

    if (!words.contains(guess))
    {
        std::cout << guess << " is not in word list.\n";
        return 0;
    }

    return 1;
}

std::vector<LetterColour> get_letter_colours(std::string guess, std::string random_word)
{
    std::vector<LetterColour> letters{};

    // 5x5 check
    for (std::string::size_type i = 0; i < guess.length(); ++i)
    {
        // Check if the letter matches
        if (guess[i] == random_word[i])
        {
            letters.push_back(LetterColour(guess[i], Colour::GREEN));
            continue;
        }

        // Check if the letter is in the rest of the word
        if (std::find(random_word.begin(), random_word.end(), guess[i]) != random_word.end())
        {
            letters.push_back(LetterColour(guess[i], Colour::YELLOW));
            continue;
        }

        // No colour
        letters.push_back(LetterColour(guess[i]));
    }

    return letters;
}

std::string string_letter_colours(std::vector<LetterColour> letter_colours)
{
    std::string colours{};

    for (LetterColour let : letter_colours) 
    {
        colours.append(let.get_ansi_colour());
    }

    return colours;
}

void clear_screen_ansi()
{
    std::cout << "\x1B[2J\x1B[H";
}

std::string get_guess_from_user(std::unordered_set<std::string> &words)
{
    std::string guess;
    // Get input from user
    do {
        // Ask user for guess
        std::cout << "Guess: ";
        std::cin >> guess;

        // Convert guess to lowercase
        std::transform(guess.begin(), guess.end(), guess.begin(), 
        [](unsigned char c) {
            return std::tolower(c);
        });

    } while (!check_valid_guess(words, guess));

    return guess;
}

std::string get_guess_from_computer(Solver &solver, const std::vector<LetterColour> &previous_guess)
{
    return solver.get_best_word(previous_guess);
}

// Loops through a game of wordle with random_word as the chosen word
// Returns:
//  0 if the user loses, 1 if the user wins
int wordle(std::unordered_set<std::string> &words, std::string random_word, int computer)
{
    // Instantiate a new solver entity
    Solver solver;
    if (computer)
        solver.set_filter_words(words);

    // Give user 6 guesses
    int guesses = 6;
    std::string guess;
    std::vector<std::string> previous_guesses{};
    std::vector<LetterColour> previous_guess{};

    while (guesses > 0)
    {
        // Print info
        clear_screen_ansi();
        std::cout << "Guesses Remaining: " << guesses << '\n';
        for (std::string guess : previous_guesses)
            std::cout << guess << '\n';

        if (!computer)
            guess = get_guess_from_user(words);
        else
        {
            guess = get_guess_from_computer(solver, previous_guess);
            std::cout << "Computer guess: " << guess << '\n';
        }

        // Check if the guess is correct
        if (guess == random_word)
            return 1;

        // Deduct for incorrect guess
        --guesses;

        previous_guess = get_letter_colours(guess, random_word);
        std::string letter_colours = string_letter_colours(previous_guess);

        // Set previous_guess to the letter_colours
        // Add the guess to the previous_guesses vector
        previous_guesses.push_back(letter_colours);
    }

    return 0;
}

int main(int argc, char *argv[]) 
{
    // Flags
    int debug = 0;
    int computer = 0;
    // 
    for (int i = 0; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "-d")
            debug = 1;

        if (arg == "-c")
            computer = 1;
    }

    // Load set of words into memory set
    std::unordered_set<std::string> words = load_words_from_file("words.txt");
    int word_count = words.size();    

    if (word_count <= 0)
    {
        std::cout << "No words loaded!\n";
        return -1;
    }

    debug_cout(debug, std::format("{} words loaded!\n", word_count));

    // Pick random word from the memory set to be the word to be played
    std::string random_word = pick_random_word(words);

    debug_cout(debug, std::format("Random word picked: {}\n", random_word));


    // Print intro text
    std::cout << "Welcome to Wordle!\nPress enter to continue...";
    std::cin.get();

    // Loop through game
    int is_win = 0;
    is_win = wordle(words, random_word, computer);

    // Print out results
    std::cout << "The word was " << random_word << ".\n";
    if (is_win)
        std::cout << "You win!\n";
    else
        std::cout << "You lose.\n";

    return 0;
}