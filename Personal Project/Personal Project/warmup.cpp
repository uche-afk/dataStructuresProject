#include "listnode.h"
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "simpio.h"
#include "strlib.h"
#include <fstream>
#include "vector.h"
#include "map.h"
#include "testing/SimpleTest.h"

using namespace std;

// fillMap takes in a vector with each of the lines from the teams file and creates a map with each team as the key and each
// of its fans' most disliked teams (rivals) in a vector as the value
Map<string, Vector<string>> fillMap(Vector<string> teams) {
    Map<string, Vector<string>> filledMap;

    for (string line : teams) {
        // break each team in the line into a new item in the vector
        Vector<string> workingTeams = stringSplit(line, ',');
        // make the name of the first team the key, and a vector of the rest of the teams (the rivals) is the value
        filledMap[workingTeams[0]] = workingTeams.subList(1);
    }

    return filledMap;
}

// areRivals takes in the filledMap, and the user input vector of the two current teams. Returns true if both teams consider the
// other to be a rival, false otherwise.
bool areRivals(Map<string, Vector<string>> filledMap, Vector<string> currentTeams) {
    if (filledMap[currentTeams[0]].contains(currentTeams[1])
            && filledMap[currentTeams[1]].contains(currentTeams[0])) {
        return true;
    } else {
        return false;
    }
}

// provided function takes care of the framework operations
void teamSearchMap(string filepath) {
    // The provided code opens the file with the given name
    // and then reads the lines of that file into a vector.
    ifstream in;
    Vector<string> teams;

    if (openFile(in, filepath)) {
        readEntireFile(in, teams);
    }
    cout << "Read file " << filepath << ", "
         << teams.size() << " teams found." << endl;

    // uses student function to populate the map
    Map<string, Vector<string>> filledMap = fillMap(teams);

    // lists the teams so the user can see their options
    for (string key : filledMap.keys()) {
        cout << key << endl;
    }

    string name = "";
    while (true) {
        cout << endl;

        // asks the user for input of two teams
        string name = getLine("Enter two team names (i.e. 'Arsenal,Chelsea' (no spaces separating teams)) (RETURN to quit):");
        // ends if user hits RETURN
        if (name == "") {
            cout << "All done!";
            break;
        }
        // splits the two inputted teams into a vector
        Vector<string> currentTeams = stringSplit(name, ',');

        // uses the student function to check the map if the two teams are rivals
        if (areRivals(filledMap, currentTeams)) {
            cout << currentTeams[0] << " and " << currentTeams[1] << " are rivals!" << endl;
        } else {
            cout << currentTeams[0] << " and " << currentTeams[1] << " are not rivals." << endl;
        }
    }
}

PROVIDED_TEST("fillMap and are rivals tests") {
    Vector<string> testVector = {"red,blue,green",
                                "blue,green",
                                "yellow,red,green",
                                "green,blue,red,yellow"};

    Vector<string> entry1 = {"red", "green"};
    Vector<string> entry2 = {"red", "blue"};
    Vector<string> entry3 = {"red", "yellow"};
    Vector<string> entry4 = {"yellow", "green"};
    Vector<string> entry5 = {"green", "green"};

    Map<string, Vector<string>> testMap = fillMap(testVector);

    EXPECT(areRivals(testMap, entry1));
    EXPECT(!areRivals(testMap, entry2));
    EXPECT(!areRivals(testMap, entry3));
    EXPECT(areRivals(testMap, entry4));
    EXPECT(!areRivals(testMap, entry5));

}

PROVIDED_TEST("fillMap and areRivals tests 2") {
    Vector<string> testVector = {"Stanford,Berkeley,UCLA",
                                "Berkeley,Stanford,UCLA",
                                "UCLA,Stanford,Berkeley",
                                "Oregon"};

    Vector<string> entry1 = {"Stanford", "Berkeley"};
    Vector<string> entry2 = {"Stanford", "UCLA"};
    Vector<string> entry3 = {"Berkeley", "UCLA"};
    Vector<string> entry4 = {"Oregon", "Stanford"};

    Map<string, Vector<string>> testMap = fillMap(testVector);

    EXPECT(areRivals(testMap, entry1));
    EXPECT(areRivals(testMap, entry2));
    EXPECT(areRivals(testMap, entry3));
    EXPECT(!areRivals(testMap, entry4));
}

PROVIDED_TEST("fillMap and areRivals tests 3") {
    Vector<string> testVector = {"Raptors",
                                "Lakers",
                                "Jazz",
                                "Hawks"};

    Vector<string> entry1 = {"Raptors", "Jazz"};
    Vector<string> entry2 = {"Raptors", "Hawks"};
    Vector<string> entry3 = {"Raptors", "Lakers"};
    Vector<string> entry4 = {"Jazz", "Lakers"};

    Map<string, Vector<string>> testMap = fillMap(testVector);

    EXPECT(!areRivals(testMap, entry1));
    EXPECT(!areRivals(testMap, entry2));
    EXPECT(!areRivals(testMap, entry3));
    EXPECT(!areRivals(testMap, entry4));
}
