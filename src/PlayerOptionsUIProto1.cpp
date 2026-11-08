#include <iostream>
using namespace std;

// Player Options interface. Will be at the bottom.
// I think separating this from actions makes sense so added an option to go to action menu.
// This UI will get replaced by actions menu if actions menu is selected with everything else staying the same.
// All options will go to their own interface which will have an option to go back to main interface (With this menu).


// This file is intended for visualising and so is easy to edit.
int main() {
    cout << "============================================" << endl;
    cout << "\n";
    cout << "Options: \n" << endl;
    cout << "1: View Hand" << endl;
    cout << "2: View Player Info" << endl;
    cout << "3: View Enemy Info" << endl;
    cout << "4: View Player Discard Pile" << endl;
    cout << "5: View Enemy Discard Pile" << endl;
    cout << "6: Go to Actions menu" << endl;
    cout << "\n";
    cout << "============================================" << endl;
    cout << "\n";
    cout << "Please select an option by typing the corresponding number" << endl;
    int pick = 0;
    cin >> pick;
    cout << pick; //For debugging.

}