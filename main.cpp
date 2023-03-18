#include <iostream>
#include "wordle.h"

#define CLEAR "\033[2J"
#define HOME "\033[;1H"

using namespace std;

int main(int argc, char* argv[])
{
    int choice = getMenuItem(); // <- Before the loop, to get the initial selection

    while (choice != 3)
    {
        if (choice == 1)
        {
            cout << CLEAR << HOME;
            playWordle();
        }
        else if (choice == 2)
        {
            cout << CLEAR << HOME;
            showStatistics();
        }

        choice = getMenuItem(); // <- At the end of the loop, to get the next
    }

    return 0;
}