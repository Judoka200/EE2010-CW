#include <iostream>
using namespace std;

// Simple Hand interface that will maybe be displayed below main UI but above player options UI.
// This is intended to not be too detailed so that main screen isn't too cluttered.
// We could give player option to either access more detailed hand UI or to instead access specific card they want to see.
// Can have unique simplistic card design for each card or each card type in card box.
// Card box size is subject to change.

// This file is just for visualising and so is easy to edit.

int main() {
    cout << "==============================================================================" << endl;
    // cout << "\n";
    cout << "Your Hand:" << endl;
    cout << "\n";
    cout << " +---------+     +---------+     +---------+     +---------+     +---------+ " << endl;
    cout << " |         |     |         |     |         |     |         |     |         | " << endl;
    cout << " |         |     |         |     |         |     |         |     |         | " << endl;
    cout << " |         |     |         |     |         |     |         |     |         | " << endl;
    cout << " |         |     |         |     |         |     |         |     |         | " << endl;
    cout << " |         |     |         |     |         |     |         |     |         | " << endl;
    cout << " +---------+     +---------+     +---------+     +---------+     +---------+ " << endl;
    cout << "    Name            Name            Name            Name            Name" << endl;
    cout << "    Type            Type            Type            Type            Type" << endl;
    cout << "==============================================================================" << endl;
}