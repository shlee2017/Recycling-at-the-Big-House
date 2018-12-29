//
//  BigHouseOpt.cpp
//  FinalProject
//
//  Created by Sangheon Lee on 12/6/18.
//  Copyright © 2018 Sangheon Lee. All righmatrix reserved.
//

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <vector>
using namespace std;
size_t P1,P2;
size_t numCon = 4;
size_t numVar = 4;
bool solution = true;

vector<vector<double>> matrix = {
    //       paper bag poster attend.  bins
    {0, 0,       1,     2,     3,      4},
    {0, 0,       30,    15,    240,    310},//objective function
    {5, 100000, -2000, -600,  -12000, -19000},//budget
    {6, 10000,  -50,   -15,   -140,   -230},//employees
    {7, 112252, -9000, -5000, -16000, -20000},//ppl to reach
    {8, 336,    -3,    -3,    -5,     -7}//time
};

int program[4] = {20000, 15, 50, 30};
string progName[4] = {"paper bag", "poster", "attendants", "recycle bins" };
void Data() {
    char YoN;
    cout << "Welcome to Big House Waste Optimization Program\n";
    cout << "Are there any known conditions? (Y/N) ";
    cin >> YoN;
    cout << "\n";
    if(YoN == 'Y' || YoN == 'y'){
        cout << "If unknown enter in -1\n";
        int choice;
        cout << "Who are we going against?(input corresponding number)\n";
        cout << " 1: Middle Tennessee\n 2: Army\n 3: Rutgers\n 4: Iowa \n 5: Notre Dame\n"
        << " 6: Michigan State\n 7: Ohio\n";
        cin >> choice;
        if(choice == 2 || choice == 3){
            for(size_t i = 2; i < 6; ++i){
                matrix[1][i] *= .9;
            }
        }
        else if(choice == 4 || choice == 6 || choice == 7){
            for(size_t i = 2; i < 6; ++i){
                matrix[1][i] *= 1.12;
            }
        }
        cout << "What is the weather like?\n";
        cout << " 1: Clear\n 2: Rainy \n 3: Snowy\n";
        cin >> choice;
        if(choice == 2){
            for(size_t i = 2; i < 6; ++i){
                matrix[1][i] *= .85;
            }
        }
        else if(choice == 3){
            for(size_t i = 2; i < 6; ++i){
                matrix[1][i] *= .93;
            }
        }
        cout << "What time?\n";
        cout << " 1: Morning \n 2: Midday\n 3: Night\n";
        cin >> choice;
        if(choice == 1){
            for(size_t i = 2; i < 6; ++i){
                matrix[1][i] *= .94;
            }
        }
        else if(choice == 3){
            for(size_t i = 2; i < 6; ++i){
                matrix[1][i] *= 1.05;
            }
        }
        cout << "What is the budget?\n";
        cin >> choice;
        if(choice != -1){
            matrix[2][1] = choice;
        }
        cout << "How many employees?\n";
        cin >> choice;
        if(choice != -1){
            matrix[3][1] = choice;
        }
        cout << "How much time do we have to put into these programs?\n";
        cin >> choice;
        if(choice != -1){
            matrix[5][1] = choice;
        }
        cout << "\n";
    }
}
void findPivot();
void Prref();
bool isOptimal();

void Simplex() {
    while(!isOptimal()){
        findPivot();
        Prref();
    }
}

void findPivot() {
    double max = 0.0;
    double min = 999999999.0;
    //find the col of the pivot
    for(size_t j = 2; j <= numVar + 1; ++j) {
        if (matrix[1][j] > max) {
            max = matrix[1][j];
            P2 = j;
        }
    }
    //find the row of the pivot
    for (size_t i = 2; i <= numCon + 1; ++i) {
        if (matrix[i][P2] < 0.0) {
            if (abs(matrix[i][1] / matrix[i][P2]) < min) {
                min = abs(matrix[i][1] / matrix[i][P2]);
                P1 = i;
            }
        }
    }
    double temp = matrix[0][P2];
    matrix[0][P2] = matrix[P1][0];
    matrix[P1][0] = temp;
}

void Prref() {
    //row reduces the matrix
    for (size_t i = 1; i <= numCon + 1; ++i) {
        if (i != P1) {
            for (size_t j = 1; j <= numVar + 1; ++j) {
                if (j != P2){
                    matrix[i][j] -= matrix[P1][j] * matrix[i][P2] / matrix[P1][P2];
                }
            }
        }
    }
    matrix[P1][P2] = 1.0 / matrix[P1][P2];
    //readjust the matrix so that the variables would have the proper coefficient
    for (size_t j = 1; j <= numVar + 1; ++j) {
        if (j != P2) {
            matrix[P1][j] *= abs(matrix[P1][P2]);
        }
    }
    for (size_t i = 1; i <= numCon + 1; i++) {
        if (i != P1) {
            matrix[i][P2] *= matrix[P1][P2];
        }
    }
}

bool isOptimal() {
    //checks whether if finding a solution for the matrix is possible
    for (size_t i = 2; i <= numCon + 1; ++i) {
        if (matrix[i][1] < 0.0) {
            solution = false;
            return true;
        }
    }
    //checks the if there are any coefficients that are greater than 0
    //to see if the matrix needs to be further optimized or not
    for (size_t j = 2; j <= numVar + 1; ++j){
        if (matrix[1][j] > 0.0) {
            return false;
        }
    }
    return true;
}


void Result() {
    if (!solution) {
        cout << " NO SOLUTION.\n";
        return;
    }
    for (size_t i = 1; i <= numVar; ++i) {
        for (size_t j = 2; j <= numCon + 1; ++j) {
            if (matrix[j][0] == 1.0 * i) {
                cout << "Program " << i << ": " << matrix[j][1] << "\n";
                cout << int(program[i - 1] * matrix[j][1]) << ' ' << progName[i -1] << "\n";
            }
        }
    }
    cout << "\nHow much to recycle(gallons): " << matrix[1][1] << "\n";
}
int main() {
    Data();
    Simplex();
    Result();
    return 10;
}
