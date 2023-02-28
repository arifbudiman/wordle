#include <iostream>
#include "wordle.h"

#define CLEAR_SCREEN "\033[2J"
#define HOME "\033[H"
#define WORDLIST "wordlist.txt"
#define ANSWERLIST "answerlist.txt"

using namespace std;

int main()
{
    string answer = pickRandomWord(ANSWERLIST);
    string guess = "crown";

    cout << CLEAR_SCREEN << HOME;

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
    
    return 0;
}