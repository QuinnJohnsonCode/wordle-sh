#include "LetterColour.h"
#include <string>
#include <format>

LetterColour::LetterColour(char letter)
{
    this->letter = letter;
    this->colour = Colour::NO_COLOUR;
}

LetterColour::LetterColour(char letter, Colour colour)
{
    this->letter = letter;
    this->colour = colour;
}

std::string LetterColour::get_ansi_colour() 
{
    // 32 = Green, 33 = Yellow
    std::string ansi_code{};
    switch (this->colour)
    {
        case Colour::GREEN:
            ansi_code = std::format("\x1B[1;32m{}\033[0m", this->letter);
            break;
        case Colour::YELLOW:
            ansi_code = std::format("\x1B[1;33m{}\033[0m", this->letter);
            break;
        case Colour::NO_COLOUR:
            ansi_code = std::format("{}", this->letter);
            break;
    }

    return ansi_code;
}