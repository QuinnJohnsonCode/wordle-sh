# wordle-sh

A simple "wordle" clone built in C++.

## Flags
| Flag  | Usage |
| ------------- | ------------- |
| -d  | (Debug) Will display additional information in the console.  |
| -c  | (Computer) Will task a solver with guessing the answer.  |

## Usage

1. Compile the project using make.
    - `make`

2. Run the object file made from make.
    - `./main`
    - Optionally with flags: `./main -d -c`

3. Press enter to begin playing.

4. Enter five letter words into the console to play.

## Rules

1. Five letter words are acceptable.

2. Must appear in the `words.txt` file provided.

3. If a letter appears in the word, but not the position as the guess, it will highlight **yellow**.

4. If a letter appears in the word, in the same position as the guess, it will highlight **green**.

5. The objective is to deduce the randomly selected word using yellow/green hints.

6. The game ends after the user's guess is correct (win) or if the user guesses six times (loss).

## A Note on the Solver's Performance

The Solver does not guess the correct answer every time, it has simple logic that allows it to attain an **87.7%** win rate over the 14k words provided.

This suggests the Solver is not perfect but will win nearly 9/10 games.

To do this, it filters its own dictionary of words based on yellow, green and gray letters. It then creates a frequency chart of all remaining letters and then performs an analysis on the remaining words to discover which word will reduce the list of remaining words down maximally.

This word is then passed to the game and the game will then ask for another guess, but since the Solver keeps a record of its current filtered list, this filtering will only be done once per guess.

The testing was done by running the game with all **14855** words and tracking whether the Solver could guess correctly in six guesses. It managed to solve **13028** wordle games, while losing **1827** games, thus an **87.7%** win rate.

The games in which it couldn't solve were mostly words that came down to a single letter, in which the Solver had to individually go through remaining letters, which burned too many guesses to solve. 

A different implementation might have more success if it kept the letter options more open and chose to select more minimally appearing characters to have a greater spread of reduction. This, however, would be challenging to implement and would most likely be best determined in some sort of ML process.

## Contact

For any additional inquiries, please contact my [email](mailto:Quinn.Johnson99@gmail.com).