#include <iostream>
#include "wordle.h"

#define CLEAR_SCREEN "\033[2J"
#define HOME "\033[H"
#define WORDLIST "wordlist.txt"
#define ANSWERLIST "answerlist.txt"

using namespace std;

int main()
{
    /*
    string answer = pickRandomWord(ANSWERLIST);
    string guess = "crown";

    if (isValidGuess(guess, WORDLIST))
    {
        // cout << guess << " is a valid guess." << endl;
        printHint(guess, answer);
        printHint("paced", answer);
        printHint("acute", answer);
        printHint("enact", answer);
        printHint("trace", answer);
        printHint("teach", answer);
    }
    else
    {
        cout << guess << " is NOT a valid guess." << endl;
    }
    */
    cout << CLEAR_SCREEN << HOME;
    
    playWordle();

    return 0;
}