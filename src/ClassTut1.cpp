#include <iostream>
#include <cmath>
using namespace std;

// Define Class as "class" followed by class name
class cplexNum {
public: //Access Modifier. Works until another access modifier is used. Public allows any part of the program to access contents.
    // Constructor call:
    // Class name with brackets. Bracket arguments represent attributes that are initialised. Order matters for when user defines object of class.
    explicit cplexNum(float real, float imaginary): real(real), imaginary(imaginary) //After colon is where you can initialise default values for variables/attributes.
    {
        // You can define initial variables/attributes here instead. Useful for if you want to do validation checks.
    }

    // Class Behavior functions. Should be in public.
    void setComplex(float numR, float numI) {
        real = numR;
        imaginary = numI;
    }
    void displayReal() {
        cout << real << endl;
    }
    void displayImaginary() {
        cout << imaginary << "j" << endl;
    }
    void displayComplex() {
        if (imaginary >= 0) {
            cout << real << "+" << imaginary << "j" << endl;
        }
        else {
            cout << real << imaginary << "j" << endl;
        }

    }
    void displayConjugate() {
        float tempImag = imaginary * -1;
        if (tempImag >= 0) {
            cout << real << "+" << tempImag << "j" << endl;
        }
        else {
            cout << real << tempImag << "j" << endl;
        }
    }
    void displayMagnitude() {
        float magnitude = sqrt((real*real)+(imaginary*imaginary));
        cout << magnitude << endl;
    }


private: //Access Modifier. Restricts access of following code to only functions within current class. Valid Until another access modifier is called.
    float real;
    float imaginary;
}; // End with semicolon


class PolarNum {
public:
    explicit PolarNum(float R, float theta): R(R), theta(theta) {}

    void setPolar(float numRM, float numTheta) {
        R = numRM;
        theta = numTheta;
    }
    void displayR() {
        cout << R << endl;
    }
    void displayTheta() {
        cout << theta << endl;
    }
    void displayPolar() {
        cout << R << "(cos(" << theta << ") + jsin(" << theta << "))" << endl;
    }
    void displayComplexEquiv() {
        float a = R * cos(theta);
        float b = R * sin(theta);

        if (b >= 0) {
            cout << a << "+" << b << "j" << endl;
        }
        else {
            cout << a << b << "j" << endl;
        }
    }


private:
    float R;
    float theta;
};

int main() {
    //Defining object of class
    //Calling class functions to test functionality
    cplexNum cplex(0, 0);
    cplex.displayReal();
    cplex.displayImaginary();
    cplex.displayComplex();
    cplex.displayConjugate();
    cplex.displayMagnitude();

    cplex.setComplex(4, -5);
    cplex.displayReal();
    cplex.displayImaginary();
    cplex.displayComplex();
    cplex.displayConjugate();
    cplex.displayMagnitude();

    cplex.setComplex(-4, 5);
    cplex.displayReal();
    cplex.displayImaginary();
    cplex.displayComplex();
    cplex.displayConjugate();
    cplex.displayMagnitude();

    PolarNum polar(0, 0);
    polar.displayR();
    polar.displayTheta();
    polar.displayPolar();
    polar.displayComplexEquiv();

    polar.setPolar(7, 1.5);
    polar.displayR();
    polar.displayTheta();
    polar.displayPolar();
    polar.displayComplexEquiv();
}