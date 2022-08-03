
/*
Satoru Yamamoto
OSU email: yamamsat@oregonstate.edu

********************************/


#include <string>
#include <ios>
#include <fstream>
#include <vector>
#include <string>
#include <string.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cmath>
#include "classes.h"
using namespace std;

const int ADSPACE = 32;

bool checkInt(string str);

int main(int argc, char* const argv[]) {

    // Create the index
    std::ofstream ofs;
    //remove("EmployeeIndex");
    ofs.open("EmployeeIndex", std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    //ofs.open("tempIndex", std::ofstream::out | std::ofstream::trunc);
    //ofs.close();

    /*
    // (n-1) is index this outputs are start position for increment n

    cout << (2 ^ (1 << (1 - 1))) << endl;
    cout << (3 ^ (1 << (2 - 1))) << endl;
    cout << (4 ^ (1 << (2 - 1))) << endl;
    cout << (5 ^ (1 << (3 - 1))) << endl;
    cout << (6 ^ (1 << (3 - 1))) << endl;
    cout << (7 ^ (1 << (3 - 1))) << endl;
    */

    LinearHashIndex emp_index("EmployeeIndex");
    //emp_index.createFromFile("test.csv");
    emp_index.createFromFile("Employee.csv");

    // Loop to lookup IDs until user is ready to quit

    bool isQuit = false;
    string userInput;
    cout << "----------------- Look up ------------------" << endl;
    cout << "Enter \"q\" or \"Q\" to exit" << endl;
    while(!isQuit){
        cout << "Enter Employee ID: ";
        cin >> userInput;
        if(userInput == "q" or userInput == "Q"){
            isQuit = true;
        }else if(checkInt(userInput)){
            int inputID = stoi(userInput);
            cout << "------------- Look up for id: " << inputID << " ----------------" << endl;
            emp_index.findRecordById(inputID);

        }else{
            cout << "Please enter integer." << endl;
        }
        cout << endl;

    }
    cout << "Program is executed" << endl;

    return 0;
}

bool checkInt(string str)
{
    for(int i = 0; i < str.size(); ++i){
        if(!isdigit(str[i])){
            return false;
        }
    }
    return true;
}