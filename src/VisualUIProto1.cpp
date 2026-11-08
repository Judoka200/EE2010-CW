#include <iostream>
using namespace std;

// Main interface user will see to hopefully know somewhat what is going on.
// Initial plan is to have this template in a document that the program will read from to create a new "active" document on initialisation.
// This "active" document will be where updates to game state will be written to.
// Each Turn/gamestate advance will read from the "active" document to then display below updated interface.
// Not completely sure this will work as smoothly as I hope it will.

// This file is just for visualising how we want the UI to look and so is easy to edit.

int main() {
    cout << "===================================================================" << endl;
    cout << "   Deck:     4                                       CCCCC    Enemy" << endl;
    cout << "   Discard:  1            26/26    HP: [##########################]" << endl;
    cout << "                                                         Energy: 10" << endl;
    cout << "  Enemy Actions" << endl;
    cout << "     A  A  A" << endl;
    cout << "\n";
    cout << "                                          Enemy Sprite(s)" << endl;
    cout << "\n\n";
    cout << "\n\n\n\n";
    cout << "            Player Sprite(s)" << endl;
    cout << "\n";
    cout << "                                                      A  A  A" << endl;
    cout << "                                                   Player Actions" << endl;
    cout << "Energy: 10                                         " << endl;
    cout << "HP: [#####################    ]    22/26             Discard:  1" << endl;
    cout << "Player    CCCCC                                      Deck:     4" << endl;
    cout << "===================================================================" << endl;
    cout << "\n";

    // Maybe have area here displaying user's hand in a bit more detail. Unsure rn. Maybe done in separate file.

    cout << "User Options will be displayed here below above visual" << endl;
    // Design of this area to be done in separate file maybe.
}