#ifndef LETTER_COLOUR_H
#define LETTER_COLOUR_H

#include <string>

enum class Colour { NO_COLOUR, YELLOW, GREEN };

class LetterColour
{
public:
    char letter;
    Colour colour;

    LetterColour(char letter);

    LetterColour(char letter, Colour colour);

    std::string get_ansi_colour();
};

#endif