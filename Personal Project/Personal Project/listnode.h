#pragma once

#include "error.h"
#include "filelib.h"
#include "map.h"
#include "queue.h"
#include "set.h"
#include "simpio.h"
#include "stack.h"
#include "strlib.h"
#include "vector.h"
#include "testing/MemoryDiagnostics.h"
using namespace std;

struct ListNode {
    string data;           /* Data stored in the node. */
    Vector<ListNode*> next;     /* Pointer to next node in the list. */

    /*
     * This is the default ListNode constructor. Ths constructor
     * gets called when the following line of code is executed.
     *
     * ListNode* node = new ListNode;
     *
     * This version of the constructor does NOT initialize either of
     * the two field values, which can lead to undefined behavior
     * if not used carefully!
     */
    ListNode() {}

    /*
     * This is the two-argument version of the ListNode constructor.
     * The constructor gets called when the following line of code is
     * executed:
     *
     * ListNode* node = new ListNode(10, nullptr);
     *
     * This version of the constructor initializes both the data and next
     * pointer field, which is a vector of listnodes, pointing to each of
     * any number of children
     */
    ListNode(string d, Vector<ListNode*> n) {
        data = d;
        next = n;
    }

    // this is the one parameter version of the constructor. The parameter becmes the data, and the children
    // becomes an empty vector, which can be added to later.
    ListNode(string d) {
        data = d;
        Vector<ListNode*> n;
        next = n;
    }


    TRACK_ALLOCATIONS_OF(ListNode); /* Track allocation counts in test cases. */
};
