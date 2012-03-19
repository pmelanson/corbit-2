/*******************
Matrix Program

Designed by Patrick Melanson

Last updated on 19/03/2012


This program reads a matrix from a desired file, then prints out these matrices.
Additionally, it checks for compatibility, and performs addition, subtraction, and multiplication

*******************/

#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <fstream>
#include <string>
#define ARRAY_SIZE 5
#define CELL_SIZE 4
#define ARRAY_COUNT 2
using namespace std;

int matrix_operate (int iResult[ARRAY_SIZE][ARRAY_SIZE], char cMO, int y, int x);   //operates on only one cell at a time

void print_matrix (int n);  //prints one matrix at a time, so it is easier to format

void print_result (int iResult[ARRAY_SIZE][ARRAY_SIZE]);    //because you can't print a 3D matrix (iMatrix) like you can a 2D array (iResult)

//globals
int iMatrix[ARRAY_COUNT][ARRAY_SIZE][ARRAY_SIZE] = {0};
int iRows[ARRAY_COUNT] = {0};   //iRows[2] is the number of rows in the resultant matrix
int iColumns[ARRAY_COUNT] = {0}; //iColumns[2] ''



int main() {

    int iResult[ARRAY_SIZE][ARRAY_SIZE] = {0};

    //I only realized that these variable names spelt out "Nyx" once I'd finished the program, I swear
    int n = 0;  //Used for indicating matrix (i.e. matrix 1 or 2)
    int y = 0;  //Used for indicating row
    int x = 0;  //Used for indicating column
    char strFile[21] = "";  //Filename
    char cMO;   //MO being Method of Operation (or Modus Operandi, whichever you prefer) abbreviated
    ifstream file;


    /**
    asks user for filename and gets data
    checks if file is open for reading
    prints out results
    **/

    cout << "Filename of text file with matrix (no extension): ";
    cin.getline (strFile, 21);
    strcat (strFile, ".txt");
    file.open (strFile);

    if (!file.is_open() || !file.good() )
        cout << "File couldn't be opened or read, continue";

    //reads and prints matrices
    for (n = 0; n < ARRAY_COUNT; n++) {     //loops through each matrix
        file >> iRows[n];                   //finds number of rows
        file >> iColumns[n];                //finds number of columns

        for (y = 0; y < iRows[n]; y++)      //reads value into appropriate cell, loops through columns and rows
            for (x = 0; x < iColumns[n]; x++)
                file >> iMatrix[n][y][x];

        print_matrix (n);                   //prints appropriate matrix

    }

    file.close();   //closes file, waits for user to go to next screen
    cout << "\n\n";
    system ("pause");
    system ("cls");



    /**
    asks user what operation to perform
    operates on matrices in this way
    **/

    //asks user which operation to perform, if matrices are compatible
    if ( (iRows[0] != iRows[1]) || (iColumns[0] != iColumns[1]) ) {
        cout << "Matrices are incompatible, cannot perform addition or subtraction. Performing multiplication. ";
        system ("pause");
        cMO = '*';
    } else {
        cout << "Input modus operandi (+ - *): ";
        cin >> cMO;
    }

    //the only time this will yield an improperly sized matrix is when addition/subtraction is performed on incompatible matrices, which will not happen
    iRows[2] = iRows[0];        //The resultant matrix has the same number of rows as the first matrix
    iColumns[2] = iColumns[1];  //The resultant matrix has the same number of columns as the second matrix

    for (y = 0; y < iRows[2]; y++) {    //performs operation, one cell at a time
        for (x = 0; x < iRows[2]; x++) {
            iResult[y][x] = matrix_operate (iResult, cMO, y, x);
        }
    }

    //prints matrices, and resultant matrix
    print_matrix (0);
    cout << endl << setw (iColumns[2] * 3 - 1) << cMO;
    print_matrix (1);
    cout << endl << setw (iColumns[2] * 3 - 1) << "=";
    print_result (iResult);

    //end of program
    cout << "\n\n";
    system ("pause");
}



int matrix_operate (int iResult[ARRAY_SIZE][ARRAY_SIZE], char cMO, int y, int x) {

    int i = 0;
    int a = 0;
    int b = 0;

    switch (cMO) {
        case '+':
            iResult[y][x] = iMatrix[0][y][x] + iMatrix[1][y][x];        //adds corresponding cells from both matrices into resultant cell
            break;
        case '-':
            iResult[y][x] = iMatrix[0][y][x] - iMatrix[1][y][x];        //subtracts corresponding cells from both matrices into resultant cell
            break;
        case '*':
            for (b = 0; b < iColumns[2]; b++)
                iResult[y][x] += iMatrix[0][y][b] * iMatrix[1][b][x];   // 1a + 2b + 3c + 4d = resultant cell (multiplies row and column into resultant cell, i.e. multiplication of matrices)
            break;
    }

    return iResult[y][x];

}

void print_matrix (int n) {

    int i = 0;
    int y = 0;
    int x = 0;

    cout << "\n\n";

    for (y = 0; y < iRows[n]; y++) {        //begins printing out a row

        for (x = 0; x < iColumns[n]; x++)   //prints out each cell
            cout << setw (CELL_SIZE) << iMatrix[n][y][x] << "|";

        //prints out bottom line of cell
        cout << endl;
        for (x = 0; x < iColumns[n]; x++) {
            for (i = 0; i < CELL_SIZE; i++)
                cout << "-";
            cout << "|";
        }
        cout << endl;

    }


}

void print_result (int iResult[ARRAY_SIZE][ARRAY_SIZE]) {

    int i = 0;
    int y = 0;
    int x = 0;

    cout << "\n\n";

    for (y = 0; y < iRows[2]; y++) {        //begins printing out a row

        for (x = 0; x < iColumns[2]; x++)   //prints out each cell
            cout << setw (CELL_SIZE) << iResult[y][x] << "|";

        //prints out bottom of cell
        cout << endl;
        for (x = 0; x < iColumns[2]; x++) {
            for (i = 0; i < CELL_SIZE; i++)
                cout << "-";
            cout << "|";
        }
        cout << endl;

    }


}
