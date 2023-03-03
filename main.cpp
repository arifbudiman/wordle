#include <iostream>
#include "wordle.h"

#define CLEAR_SCREEN "\033[2J"
#define HOME "\033[H"
#define WORDLIST "wordlist.txt"
#define ANSWERLIST "answerlist.txt"
#define PLAYLOG "play.log"

using namespace std;

int main()
{
    int choice;

    do
    {
        cout << CLEAR_SCREEN << HOME << endl
             << " 1 - Play Wordle\n"
             << " 2 - Display Statistics\n"
             << " 3 - Exit\n\n"
             << " Enter your choice and press return: ";
        cin >> choice;
        cin.clear();
        cin.get();

        switch (choice)
        {
        case 1:
            playWordle();
            break;
        case 2:
            showStatistics();
            break;
        case 3:
            cout << "End of Program.\n";
            break;
        default:
            cout << "Not a Valid Choice. \n"
                 << "Choose again.\n";
            break;
        }
    } while (choice != 3);

    return 0;
}