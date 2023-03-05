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
#define GUESSLOG "guess.log"

#include <algorithm>
#include <string>
#include <fstream>
#include <cctype>
#include <vector>
#include <iostream>
#include <limits>

void showStatistics(std::string filename)
{
    std::cout << CLEAR_SCREEN << HOME;

    int totalPlayed = 0;
    int totalWin = 0;
    int percentWin = 0;
    int currentStreak = 0;
    int maxStreak = 0;

    std::ifstream statFile(filename);

    if (statFile.is_open())
    {
        std::string line;
        while (getline(statFile, line))
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

        statFile.close();
    }

    percentWin = totalWin / (double)totalPlayed * 100;

    std::cout << "Played: " << totalPlayed << std::endl;
    std::cout << "Win: " << percentWin << "%" << std::endl;
    std::cout << "Current streak: " << currentStreak << std::endl;
    std::cout << "Max streak: " << maxStreak << std::endl << std::endl;

    std::cout << "Press Enter to Continue ";
    std::cin.ignore();
}

void showStatistics()
{
    showStatistics(PLAYLOG);
}

void logTheGuess(std::string guess, std::string filename, std::ios_base::openmode openMode)
{
    std::ofstream logfile(filename, openMode);

    if (logfile.is_open())
    {
        logfile << guess << std::endl;
        logfile.close();
    }
}

void logTheGuess(std::string guess, std::ios_base::openmode openMode)
{
    logTheGuess(guess, GUESSLOG, openMode);
}

void logThePlay(std::string guessWord, bool isWinning, int numOfTries, std::string filename)
{
    // open a file for appending
    std::ofstream logfile(filename, std::ios_base::app);

    if (logfile.is_open())
    {
        logfile << guessWord << "," << (isWinning ? "true" : "false") << "," << numOfTries << std::endl;
        logfile.close();
    }
}

void logThePlay(std::string guessWord, bool isWinning, int numOfTries)
{
    logThePlay(guessWord, isWinning, numOfTries, PLAYLOG);
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
    std::cout << CLEAR_SCREEN << HOME;

    for (int i = 0; i < guesses.size(); i++)
    {
        printHint(guesses[i], answer);
    }
}

std::string dedupe(std::string str)
{
    std::sort(str.begin(), str.end());
    str.erase(std::unique(str.begin(), str.end()), str.end());
    return str;
}

void printKeyboardHint(std::vector<std::string> guesses, std::string answer)
{
    std::string guessedAbsent;
    std::string guessedMisplaced;
    std::string guessedCorrect;

    // get all letters that have been guessed and deduplicate them
    std::string allGuessedLetters;
    for (int i = 0; i < guesses.size(); i++)
    {
        allGuessedLetters += guesses[i];
    }
    allGuessedLetters = dedupe(allGuessedLetters);

    // get letters that have been guessed but are absent in the answer
    for (int i = 0; i < allGuessedLetters.length(); i++)
    {
        if (answer.find(allGuessedLetters[i] == std::string::npos))
        {
            guessedAbsent.push_back(allGuessedLetters[i]);
        }
    }
    
    // get correct letters and misplaced letters
    std::string lastGuess = guesses.back();
    for (int i = 0; i < lastGuess.length(); i++)
    {
        // correct letter
        if (lastGuess[i] == answer[i])
        {
            guessedCorrect.push_back(lastGuess[i]);
        }
        // letter exist but misplaced
        else if (answer.find(lastGuess[i]) != std::string::npos)
        {
            guessedMisplaced.push_back(lastGuess[i]);
        }
    }

    std::vector<std::string> keys = {"qwertyuiop", "asdfghjkl", "zxcvbnm"};

    // std::cout << CLEAR_SCREEN << HOME;

    // loop for keyboard rows
    for (int i = 0; i < keys.size(); i++)
    {
        // if this is the 2nd row
        if (i == 1)
        {
            std::cout << "  ";
        }
        // if this is the 3rd row
        else if (i == 2)
        {
            std::cout << "       ";
        }

        // loop for the all the letters in the current row
        for (int j = 0; j < keys[i].length(); j++)
        {
            // TODO: logic to determine color
            std::string color;
            if (guessedCorrect.find(keys[i][j]) != std::string::npos)
            {
                color = GREEN;
            }
            else if (guessedMisplaced.find(keys[i][j]) != std::string::npos)
            {
                color = YELLOW;
            }
            else if (guessedAbsent.find(keys[i][j]) != std::string::npos)
            {
                color = GREY;
            }

            std::cout << color << " --- " << CURSOR_DOWN_ONE_LEFT_FIVE
                << "| " << keys[i][j] << " |" << CURSOR_DOWN_ONE_LEFT_FIVE
                << " --- " << RESET;

            // if this isn't the last charcter in the row
            if (j < keys[i].length() -1)
            {
                std::cout << CURSOR_UP_TWO;
            }
        }
        std::cout << std::endl;
    }
}

void playWordle()
{
    std::cout << CLEAR_SCREEN << HOME;

    std::string answer = pickRandomWord(ANSWERLIST);

    std::vector<std::string> guesses;

    int numberOfTry = 0;

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

            guesses.push_back(guess);

            printHint(guesses, answer);

            printKeyboardHint(guesses, answer);

            // log the guess in a temporary file so we can use it to colorize the keyboard.
            if (numberOfTry == 1)
            {
                logTheGuess(guess, std::ios_base::trunc);
            }
            else
            {
                logTheGuess(guess, std::ios_base::app);
            }

            // winner: user guessed correctly
            if (guess.compare(answer) == 0)
            {
                logThePlay(guess, true, guesses.size());

                std::cout << "Guessed in " << guesses.size() << " tries." << std::endl;

                system("pause");

                return;
            }
        }
    }

    // loser: no correct guesses after 6 tries
    logThePlay(answer, false, 0);

    std::cout << "The answer is: " << answer << std::endl << std::endl;

    system("pause");
}

#endif