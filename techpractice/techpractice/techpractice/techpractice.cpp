// techpractice.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

int main()
{
    


 

 
}


void PointerBasics() {
    //pointer practice

    int health = 5;
    int score = 0;

    //pointer to address of int variable
    int* pHealth;


    //pHealth points to health
    pHealth = &health;

    std::cout << pHealth;
    std::cout << "\n";


    //make pHealth point to score
    pHealth = &score;

    std::cout << pHealth;


    std::cout << "\n";

    //add 10 to score using pointer
    *pHealth += 10;

    std::cout << score;

    std::cout << "\n";

    int* pToInt = nullptr;

    std::cout << pToInt;
    std::cout << "\n";
    //set pointer to point to address on heap
    pToInt = new int;

    std::cout << pToInt;
    std::cout << "\n";

    std::cout << &pToInt;
    std::cout << "\n";

    //free pToInt
    delete pToInt;

    int* pToInt = nullptr;
}



/* PASSING POINTER TO A FUNCTION */
void myFunction(int* pInt) {
    
    //dereference and increment the value stored
    *pInt++;
    return;
}

void funWithPointers() {
    //create int with value 10
    int someInt = 10;
    //pointer pToInt equal address of someInt;
    int* pToInt = &someInt;

    myFunction(pToInt);
    myFunction(&someInt);

    //both functions have same effect
    //someInt is now 12;

}

void funWithPointers2() {
    //create testclass object
    PointerTestClass test;

    PointerTestClass* pTestClass = &test;

    //can access member function of object directly from pointer
    pTestClass->someTestClassFunction();

}


class PointerTestClass {
public:
    void someTestClassFunction() {
        return;
    }

};


/*
POINTERS AND ARRAYS
*/

//Array name points to first element in array
void funWithArraysandPointers() { 

    //declare array
    int arrayofInts[100];

    //declare pointer to int
    //initialie it with address of first element of array

    int* pToIntArray = arrayofInts;


    pToIntArray[0] = 100;


    //function that accepts pointer also accepts arrays of the type the pointer is pointing to
}
