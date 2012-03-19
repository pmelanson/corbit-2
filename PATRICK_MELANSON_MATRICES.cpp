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

int cont (char strMessage[31]);   //To ask the user if they want to continue, includes error checking

//globals
int iMatrix[ARRAY_COUNT][ARRAY_SIZE][ARRAY_SIZE] = {0};
int iRows[ARRAY_SIZE] = {0};   //iRows[3] is the number of rows in the resultant matrix
int iColumns[ARRAY_SIZE] = {0};//iColumns[3] ''

int main() {

	int iResult[ARRAY_SIZE][ARRAY_SIZE] = {0};

	int n = 0;  //I only realized that these variable names spelt out "Nyx" once I'd finished the program, I swear
	int y = 0;
	int x = 0;
	char strFile[21] = "";
	char cMO;   //MO being Method of Operation's abbreviation
	char strMessage[46] = "Is this the matrix you wanted";
	char cCont = 'N';

	ifstream file;


	//asks user for filename and gets data, prints out results, checks for errors
	cout << "Filename (TextFileExample, no extension): ";
	cin.getline (strFile, 21);
	strcat (strFile, ".txt");
	file.open (strFile);

	if (!file.is_open() || !file.good() ) {
		cout << "\nFile couldn't be opened or read\n\n";
		system ("pause");
		return (1);
    }

    //reads and prints matrices
	for (n = 0; n < ARRAY_COUNT; n++) { //does operation on each matrix
		file >> iRows[n];               //reads number of rows
		file >> iColumns[n];            //reads number of columns

		for (y = 0; y < iRows[n]; y++)  //loops for each row
			for (x = 0; x < iColumns[n]; x++)//loops for each column
				file >> iMatrix[n][y][x];    //reads cell

		print_matrix (n);               //prints the current matrix

	}

    cout << endl;
	system ("pause");
	file.close();
	system ("cls");


	//performs operations on matrices, prints out result
	if ( (iRows[0] != iRows[1]) || (iColumns[0] != iColumns[1]) ) {

		cout << "Matrices will be multiplied, cannot add or subtract incompatible matrices";
		cin >> cMO;
		cMO = '*';
	} else {
		cout << "Input modus operandi (+ - *): ";
		cin >> cMO;
	}

    //The resultant matrix will have the same numbers of rows as the smallest matrix
	if (iRows[0] <= iRows[1])
		iRows[2] = iRows[0];
	else if (iRows[1] < iRows[0])
		iRows[2] = iRows[1];

    //The resultant matrix will have the same numbers of columns as the smallest matrix
	if (iColumns[0] <= iColumns[1])
		iColumns[2] = iColumns[0];
	else if (iColumns[1] < iColumns[0])
		iColumns[2] = iColumns[1];

    //operates on matrices
	for (y = 0; y < iRows[2]; y++) {
		for (x = 0; x < iRows[2]; x++) {
			iResult[y][x] = matrix_operate (iResult, cMO, y, x);
		}
	}

    //prints matrices
	print_matrix (0);
	cout << endl << cMO;
    print_matrix (1);
	cout << "\n=";
	print_result (iResult);

    //end of program
	cout << "\n\n";
	system ("pause");
}

int matrix_operate (int iResult[ARRAY_SIZE][ARRAY_SIZE], char cMO, int y, int x) {

	int i = 0;

	switch (cMO) {
		case '+':
			iResult[y][x] = iMatrix[0][y][x] + iMatrix[1][y][x];
			break;
		case '-':
			iResult[y][x] = iMatrix[0][y][x] - iMatrix[1][y][x];
			break;
		case '*':

			for (i = 0; i < iRows[2]; i++)
				iResult[y][x] += iMatrix[0][y][i] * iMatrix[1][i][x];

			break;

	}

	return iResult[y][x];

}

void print_matrix (int n) {

	int y = 0;
	int x = 0;


	cout << endl << endl;

	for (y = 0; y < iRows[n]; y++) {

		for (x = 0; x < iColumns[n]; x++)
			cout << setw (CELL_SIZE) << iMatrix[n][y][x] << "|";

		cout << endl;
	}

}

void print_result (int iResult[ARRAY_SIZE][ARRAY_SIZE]) {

	int y = 0;
	int x = 0;


	cout << endl << endl;

	for (y = 0; y < iRows[2]; y++) {

		for (x = 0; x < iColumns[2]; x++)
			cout << setw (CELL_SIZE) << iResult[y][x] << "|";

		cout << endl;
	}


}

int cont (char strMessage[31]) {

	int iValid = 0;
	char cCont = 'Y';

	while (iValid == 0) {       //asks the user if they want to run again
		cout << "\n\n" << strMessage << " (Y/N)? ";
		cin >> cCont;
		cCont = toupper (cCont);

		switch (cCont) {    //detects invalid input
			case 'Y':
			case 'N': {
					iValid = 1;
					system ("cls");
				}
				break;

			default: {
					system ("cls");
					cout << strMessage << " (Y/N)? " << cCont << endl
					     << "\nInvalid input\n\n";
				}
		}
	}

	fflush (stdin);
	iValid = 0;

	return cCont;

}
