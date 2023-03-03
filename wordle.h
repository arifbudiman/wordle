#ifndef WORDLE_H
#define WORDLE_H

#define GREEN "\033[92m"
#define YELLOW "\033[93m"
#define GREY "\033[90m"
#define RESET "\033[0m"
#define CLEAR_SCREEN "\033[2J"
#define HOME "\033[H"

#define CURSOR_DOWN_ONE_LEFT_FIVE "\033[1B\033[5D"
#define CURSOR_UP_TWO "\033[2A"

#define WORDLIST "wordlist.txt"
#define ANSWERLIST "answerlist.txt"
#define PLAYLOG "play.log"

#include <algorithm>
#include <string>
#include <fstream>
#include <cctype>
#include <vector>
#include <iostream>

void showStatistics(std::string filename)
{
    int totalPlayed = 0;
    int totalWin = 0;
    int percentWin = 0;
    int currentStreak = 0;
    int maxStreak = 0;

    std::ifstream ifstream(filename);
    std::string line;
    while (getline(ifstream, line))
    {
        totalPlayed++;
        if (line.find("true", 0) != std::string::npos)
        {
            totalWin++;
            currentStreak++;
            if (currentStreak > maxStreak)
            {
                maxStreak = currentStreak;
            }
        }
        else
        {
            currentStreak = 0;
        }
    }

    percentWin = totalWin / (double)totalPlayed * 100;

    std::cout << "Played: " << totalPlayed << std::endl;
    std::cout << "Win: " << percentWin << "%" << std::endl;
    std::cout << "Current streak: " << currentStreak << std::endl;
    std::cout << "Max streak: " << maxStreak << std::endl;

    ifstream.close();
}

void showStatistics()
{
    showStatistics(PLAYLOG);
}

void logThePlay(bool isWinning, int numOfTries, std::string filename)
{
    // open a file for appending
    std::ofstream logfile(filename, std::ios_base::app);

    if (logfile.is_open())
    {
        logfile << (isWinning ? "true" : "false") << "," << numOfTries << std::endl;
        logfile.close();
    }
}

void logThePlay(bool isWinning, int numOfTries)
{
    logThePlay(isWinning, numOfTries, PLAYLOG);
}

std::string pickRandomWord(std::string filename)
{
    std::ifstream ifstream(filename);

    std::string line;

    // count how many lines are in the file
    int totalNumberOfLines = 0;
    while (getline(ifstream, line))
    {
        totalNumberOfLines++;
    }

    // initialize random seed
    srand (time(NULL));
    // get a random number between 1 and totalNumberOfLines
    int randomLineNumber = rand() % totalNumberOfLines + 1;

    // clear any error states and set position in input sequence to 0 (the beginning of the file)
    ifstream.clear();
    ifstream.seekg(0);

    // get the word in randomLineNumber
    int currentLineNumber = 0;
    while (getline(ifstream, line))
    {
        currentLineNumber++;
        if (currentLineNumber == randomLineNumber)
        {
            // just in case: transform the word to lowercase
            std::transform(line.begin(), line.end(), line.begin(), ::tolower);
            // close the file
            ifstream.close();
            // return the word
            return line;
        }
    }

    // if we get here, that means there's something wrong (e.g. the random line number exceeds the max, etc.)
    return "";
}

bool isValidGuess(std::string word, std::string fileName)
{
    // open the file
    std::ifstream ifstream(fileName);

    std::string line;
    int currentLineNumber = 0;

    // transform to lowercase
    std::transform(word.begin(), word.end(), word.begin(), ::tolower);

    // go line by line
    while (getline(ifstream, line))
    {
        currentLineNumber++;

        // if the word is in this line
        if (line.find(word, 0) != std::string::npos)
        {
            // close the file
            ifstream.close();
            // the word is a valid guess
            return true;
        }
    }

    // close the file
    ifstream.close();

    // the word is not a valid guess
    return false;
}

bool isValidGuess(std::string word)
{
    return isValidGuess(word, WORDLIST);
}

void printHint(std::string guess, std::string answer)
{
    std::string color;

    std::vector<char> charactersNotUsed;

    for (int i = 0; i < guess.length(); i++)
    {
        // letter placed in the correct position
        if (guess[i] == answer[i])
        {
            color = GREEN;
        }
        // letter exists but in different position
        else if (answer.find(guess[i]) != std::string::npos)
        {
            color = YELLOW;
        }
        // letter doesn't exist in the answer
        else
        {
            color = GREY;
            charactersNotUsed.push_back(guess[i]);
        }

        std::cout << color << " --- "  << CURSOR_DOWN_ONE_LEFT_FIVE;
        std::cout << "| " << guess[i] << " |"  << CURSOR_DOWN_ONE_LEFT_FIVE;
        std::cout << " --- " << RESET;

        // if this is not the last character, put the cursor up 2 lines
        if (i < guess.length() - 1)
        {
            std::cout << CURSOR_UP_TWO;
        }
        // otherwise (if this is the last character), put the cursor in the next line (put a line break)
        else
        {
            std::cout << std::endl << std::endl;
        }
    }
}

void printHint(std::vector<std::string> guesses, std::string answer)
{
    for (int i = 0; i < guesses.size(); i++)
    {
        printHint(guesses[i], answer);
    }
}

void playWordle()
{
    int numberOfTry = 0;

    std::string answer = pickRandomWord(ANSWERLIST);

    std::vector<std::string> guesses;

    while (numberOfTry < 6)
    {
        std::string guess;
        std::cout << "Please enter your guess: ";
        std::cin >> guess;

        // transform guess to lowercase
        std::transform(guess.begin(), guess.end(), guess.begin(), ::tolower);

        if (guess.length() != 5)
        {
            std::cout << "Your guess must contain 5 letters." << std::endl;
        }
        else if (!isValidGuess(guess))
        {
            std::cout << "Your guess is not a valid word." << std::endl;
        }
        else
        {
            numberOfTry++;

            //printHint(guess, answer);

            std::cout << CLEAR_SCREEN << HOME;
            guesses.push_back(guess);
            printHint(guesses, answer);

            // TODO: log the guess in a temporary file so that we know what letters have been used.

            if (guess.compare(answer) == 0)
            {
                // log the play statistics in a file
                logThePlay(true, guesses.size());

                std::cout << "Guessed in " << numberOfTry << " tries." << std::endl;
                
                break;
            }
        }
    }

    
}

#endif