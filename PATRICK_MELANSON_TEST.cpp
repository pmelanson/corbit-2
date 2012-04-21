/*******************
Online Test Program

Designed by Patrick Melanson

Last updated on 27/03/2012


This program reads a matrix from a desired file
it then checks if it is a magic matrix

*******************/

//including libraries
#include <iostream> //for output
#include <stdlib.h> //for system("pause") and ("cls")
#include <iomanip>  //for formatting as cells
#include <fstream>  //for reading files
using namespace std;//so I don't have to type std::cout << all the time, just cout <<

//declaration of functions
int checkRow (int y);       //finds the sum of each row
int checkColumn (int x);    //finds the sum of each column
int checkDiagonal (int i);  //finds the sum of each diagonal
void printMatrix (int iRowSum[10], int iColumnSum[10], int iDiagonalSum[10]);   //draws matrix

//globals
int iMatrix[10][10] = {};   //the actual matrix, can hold up to a 10x10 matrix
int iRows = 0;              //number of rows the matrix will have
int iColumns = 0;           //number of rows the matrix will have
int iCellSize = 4;          //size of matrix cell


int main () {

	/**
	initializations
	**/
	//looping variables
	int y = 0;  //looping variable for row
	int x = 0;  //looping variable for column
	int i = 0;  //looping variable for diagonals
	//data storage variables
	int iRowSum[10] = {};       //stores sum of each row
	int iColumnSum[10] = {};    //stores sum of each column
	int iDiagonalSum[2] = {};   //stores sum of each diagonal
	//operating variables
	char strFile[21] = "";
	//initializes file reading
	ifstream file;

	/**********************
	actual code begins here
	**********************/

    cout << "input filename: ";
    cin.getline (strFile, 21);

	//opens file for reading
	file.open (strFile);
	//checks if file is open for reading
	if (!file.is_open() || !file.good()) {
		cout << "File couldn't be opened or read\n";
		system ("pause");
		return (1);
	}

	/**
	reads matrix from filefor (j = 0; j < iColumns; j++)
			iDiagonalSum += iMatrix[j][j];
	**/
	file >> iRows;                   //finds number of rows
	file >> iColumns;                //finds number of columns

	if (iRows != iColumns) {
		cout << "Matrix is not a square, terminating.\n";
		system ("pause");
		return (2);
	}

	for (y = 0; y < iRows; y++)      //reads value into appropriate cell, loops through columns and rows
		for (x = 0; x < iColumns; x++)
			file >> iMatrix[y][x];

	file.close();

	//finds sum of each row, then column, then diagonal
	for (y = 0; y < iRows; y++)
		iRowSum[y] = checkRow (y);

	for (x = 0; x < iColumns; x++)
		iColumnSum[x] = checkColumn (x);

	for (i = 0; i < 2; i++)
		iDiagonalSum[i] = checkDiagonal (i);

	printMatrix (iRowSum, iColumnSum, iDiagonalSum); //prints matrix
	cout << "\n\nCells underlined with = are sums of the row\nCells in [] brackets are sums of the column";
	cout << "\n\nTop left - Bottom right diagonal: " << iDiagonalSum[0];
	cout << "\nTop right - Bottom left diagonal: " << iDiagonalSum[1];


    /**
    Checks if everything is equal
    **/

    for (y = 0; y < iRows - 1; y++)
        if (iRowSum[y] != iRowSum[y + 1]) {
            cout << "\n\nOne of the matrix's rows is not equal to another, matrix is not magic square.\n";
            system("pause");
            return (3);
        }

    for (x = 0; x < iColumns - 1; x++)
        if (iColumnSum[x] != iColumnSum[x + 1]) {
            cout << "\n\nOne of the matrix's columns is not equal to another, matrix is not magic square.\n";
            system("pause");
            return (3);
        }

    for (i = 0; i < 2; i++)
		if (iDiagonalSum[i] != iDiagonalSum[i + 1]) {
            cout << "\n\nOne of the matrix's diagonals is not equal to another, matrix is not magic square.\n";
            system("pause");
            return (3);
        }

    cout << "\n\nThis is a magic square!";

	//end of main
	cout << endl;
	system ("pause");
	return (0);
}

int checkRow (int y) {

	int x = 0;  //looping variable for going through each cell in the row
	int iRowSum = 0;    //stores sum of row

	for (x = 0; x < iColumns; x++)  //loops through each cell in row
		iRowSum += iMatrix[y][x];

	//returns sum of all cells in row
	return (iRowSum);

	//end of checkRow
}

int checkColumn (int x) {

	int y = 0;  //looping variable for going through each cell in the column
	int iColumnSum = 0;    //stores sum of column

	for (x = 0; x < iRows; x++)  //loops through each cell in column
		iColumnSum += iMatrix[y][x];

	//returns sum of all cells in row
	return (iColumnSum);

	//end of checkColumn
}

int checkDiagonal (int i) {

	int j = 0;  //looping variable for x and y, since x and y will always be equal in diagonals
	int iDiagonalSum = 0;   //stores sum of diagonal


	if (i == 0)
		for (j = 0; j < iColumns; j++)
			iDiagonalSum += iMatrix[j][j];

    else if (i == 1)
        for (j = iColumns - 1; j >= 0; j--)
			iDiagonalSum += iMatrix[j][j];

    //returns sum of all cells in diagonal
    return (iDiagonalSum);
	//end of checkDiagonal
}

void printMatrix (int iRowSum[10], int iColumnSum[10], int iDiagonalSum[10]) {

	//looping variables
	int i;  //for printing out bottom of cells
	int y;  //row
	int x;  //column

	for (y = 0; y < iRows; y++) {        //loops through each row

		//prints out a row of cells
		cout << endl;
		for (x = 0; x < iColumns; x++)
			cout << setw (iCellSize) << iMatrix[y][x] << "|";
		cout << " " << setw (iCellSize) << iRowSum[y];  //prints out row sum cell
		//end of row of cells printing loop

		//prints out bottom line of cell
		cout << endl;
		for (x = 0; x < iColumns; x++) {
			for (i = 0; i < iCellSize; i++)
				cout << "-";
			cout << "|";
		}   //end of cell bottom loop

		//prints out bottom of row sum cell
		cout << " ";
		for (x = 0; x < iCellSize; x++)
			cout << "=";

	}   //end of loop repeating through each row

	//prints out column sum
	cout << endl << endl;
	for (x = 0; x < iColumns; x++)
		cout << "[" << iColumnSum[x] << "]";

	cout << endl;
	//end of printMatrix
}
