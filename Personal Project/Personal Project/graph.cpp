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

// getPositionOnRoot takes in the root of the team tree and a team name, and returns the index of the team on the roots "next" vector
int getPositionOnRoot(ListNode* root, string teamName) {
    // loop over every child of the root
    for (int n = 0; n < root->next.size(); n++) {
        // if the team name matches the node we are on, return the index
        if (teamName == root->next[n]->data) {
            return n;
        }
    }
    error("team not found!");
}

// isChild takes in a parent ListNode* and a child string, and returns true if the string is one of the children of the parent
// node, false otherwise
bool isChild(ListNode* parent, string child) {
    // loop over every child node of the given parent node
    for (ListNode* teamNode : parent->next) {
        // if the child team is found, return true
        if (teamNode->data == child) return true;
    }
    return false;
}

// deallocates the graph to avoid leaks in test cases
void deallocateGraph(ListNode* root) {
    for (ListNode* node : root->next) {
        delete node;
    }

    delete root;
}


// fillGraph takes in a vector with each of the lines from the teams file and creates a graph with each team as node connected to the
// root and its children are every other team which it considers to be a rival
ListNode* fillGraph(Vector<string> teams) {
    ListNode* root = new ListNode("root");

    // populate the root with all fourteen teams first, each as its own node. We do this first so that we have all nodes make rival
    // connections to!
    for (string line : teams) {
        // find the first comma and use the substring function to find the first team name in the line
        string headTeam = line.substr(0, stringIndexOf(line, ","));
        // create new node that contains the team name and attach it to the root
        ListNode* newTeam = new ListNode(headTeam);
        root->next.add(newTeam);
    }

    // instead of using the getPositionOnRoot function each time, we simply create a variable called headTeamPosition which we set
    // to zero, and then increase by one each line we move down. We know that this will be correct index.
    int headTeamPosition = 0;
    for (string line : teams) {
        // break each team in the line into a new item in the vector
        Vector<string> workingTeams = stringSplit(line, ',');
        string headTeam = workingTeams[0];

        // loop over every RIVAL now. So i starts at one, and increases until we have hit every rival on the line
        for (int i = 1; i < workingTeams.size(); i++) {
            string currentRival = workingTeams[i];
            // use given function to get the index of the rivalTeam on the root's next vector
            int rivalPosition = getPositionOnRoot(root, currentRival);
            // add a pointer that points from the headTeam to the rivalTeam
            root->next[headTeamPosition]->next.add(root->next[rivalPosition]);
        }
        // once we have added every rival for one team, increase the index and go to the next line
        headTeamPosition++;
    }
    return root;
}

// areRivals takes in the filled graph root, and the user input vector of the two current teams. Returns true if both teams consider the
// other to be a rival, false otherwise.
bool areRivals(ListNode* root, Vector<string> currentTeams) {
    // use given function to find the position of the user inputted teams on the root
    int team1Position = getPositionOnRoot(root, currentTeams[0]);
    int team2Position = getPositionOnRoot(root, currentTeams[1]);

    // use the given isChild function to check if the second user inputted team is a childe of the first, and vice-versa. If so,
    // return true.
    if (isChild(root->next[team1Position], currentTeams[1])
            && isChild(root->next[team2Position], currentTeams[0])) {
        return true;
    } else {
        return false;
    }
}

// provided function takes care of the framework operations
void teamSearchGraph(string filepath) {
    // The proivded code opens the file with the given name
    // and then reads the lines of that file into a vector.
    ifstream in;
    Vector<string> teams;

    if (openFile(in, filepath)) {
        readEntireFile(in, teams);
    }
    cout << "Read file " << filepath << ", "
         << teams.size() << " teams found." << endl;

    // uses student function to populate the graph
    ListNode* root = fillGraph(teams);

    // lists the teams so the user can see their options
    for (ListNode* team : root->next) {
        cout << team->data << endl;
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

        // uses the student function to check the graph if the two teams are rivals
        if (areRivals(root, currentTeams)) {
            cout << currentTeams[0] << " and " << currentTeams[1] << " are rivals!" << endl;
        } else {
            cout << currentTeams[0] << " and " << currentTeams[1] << " are not rivals." << endl;
        }
    }
}

PROVIDED_TEST("fillGraph and areRivals tests") {
    Vector<string> testVector = {"red,blue,green",
                                "blue,green",
                                "yellow,red,green",
                                "green,blue,red,yellow"};

    Vector<string> entry1 = {"red", "green"};
    Vector<string> entry2 = {"red", "blue"};
    Vector<string> entry3 = {"red", "yellow"};
    Vector<string> entry4 = {"yellow", "green"};
    Vector<string> entry5 = {"green", "green"};

    ListNode* testRoot = fillGraph(testVector);

    EXPECT(areRivals(testRoot, entry1));
    EXPECT(!areRivals(testRoot, entry2));
    EXPECT(!areRivals(testRoot, entry3));
    EXPECT(areRivals(testRoot, entry4));
    EXPECT(!areRivals(testRoot, entry5));

    deallocateGraph(testRoot);
}

PROVIDED_TEST("fillGraph and areRivals tests 2") {
    Vector<string> testVector = {"Stanford,Berkeley,UCLA",
                                "Berkeley,Stanford,UCLA",
                                "UCLA,Stanford,Berkeley",
                                "Oregon"};

    Vector<string> entry1 = {"Stanford", "Berkeley"};
    Vector<string> entry2 = {"Stanford", "UCLA"};
    Vector<string> entry3 = {"Berkeley", "UCLA"};
    Vector<string> entry4 = {"Oregon", "Stanford"};

    ListNode* testRoot = fillGraph(testVector);

    EXPECT(areRivals(testRoot, entry1));
    EXPECT(areRivals(testRoot, entry2));
    EXPECT(areRivals(testRoot, entry3));
    EXPECT(!areRivals(testRoot, entry4));

    deallocateGraph(testRoot);
}

PROVIDED_TEST("fillGraph and areRivals tests 3") {
    Vector<string> testVector = {"Raptors",
                                "Lakers",
                                "Jazz",
                                "Hawks"};

    Vector<string> entry1 = {"Raptors", "Jazz"};
    Vector<string> entry2 = {"Raptors", "Hawks"};
    Vector<string> entry3 = {"Raptors", "Lakers"};
    Vector<string> entry4 = {"Jazz", "Lakers"};

    ListNode* testRoot = fillGraph(testVector);

    EXPECT(!areRivals(testRoot, entry1));
    EXPECT(!areRivals(testRoot, entry2));
    EXPECT(!areRivals(testRoot, entry3));
    EXPECT(!areRivals(testRoot, entry4));

    deallocateGraph(testRoot);
}

