#include <iostream>
#include "console.h"
#include "warmup.cpp"
#include "graph.cpp"
#include "testing/SimpleTest.h"
using namespace std;

/*
 * You are free to edit the main in any way that works
 * for your testing/debugging purposes.
 * We will supply our own main() during grading.
 */
int main() {
    if (runSimpleTests(SELECTED_TESTS)) {
        return 0;
    }

    // this can be changed to teamSearchMap to run the other solution of the program
    teamSearchMap("teams.txt");
    cout << "All done, exiting" << endl;
    return 0;
}
