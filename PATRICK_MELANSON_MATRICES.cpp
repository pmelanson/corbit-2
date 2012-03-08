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
	cout << "Filename (e.g. textfile date, no extension): ";
	cin.getline (strFile, 21);
	strcat (strFile, ".txt");
	file.open (strFile);

	if (!file.is_open() || !file.good() )
		cout << "File couldn't be opened or read, continue";

	for (n = 0; n < ARRAY_COUNT; n++) { //reads and prints matrices
		file >> iRows[n];
		file >> iColumns[n];

		for (y = 0; y < iRows[n]; y++)

			for (x = 0; x < iColumns[n]; x++)
				file >> iMatrix[n][y][x];

		print_matrix (n);

	}

    cout << endl;
	system ("pause");
	file.close();
	system ("cls");


	//performs operations on matrices, prints out result

	cout << "Input modus operandi (";

	if ( (iRows[0] != iRows[1]) || (iColumns[0] != iColumns[1]) ) {

		cout << "only '*' because matrices are incompatible): ";
		cin >> cMO;
		cMO = '*';
	} else {
		cout << "+ - *): ";
		cin >> cMO;
	}

	if (iRows[0] <= iRows[1])    //The resultant matrix will be the size of the smallest matrix
		iRows[2] = iRows[0];
	else if (iRows[1] < iRows[0])
		iRows[2] = iRows[1];

	if (iColumns[0] <= iColumns[1])    //The resultant matrix will be the size of the smallest matrix
		iColumns[2] = iColumns[0];
	else if (iColumns[1] < iColumns[0])
		iColumns[2] = iColumns[1];

	n = 0;
	y = 0;
	x = 0;

	for (y = 0; y < iRows[2]; y++) {
		for (x = 0; x < iRows[2]; x++) {
			iResult[y][x] = matrix_operate (iResult, cMO, y, x);
		}
	}

	for (n = 0; n < ARRAY_COUNT; n++) {
		print_matrix (n);

		if (n == (ARRAY_COUNT - 1) )
			cout << "\n=";
		else
			cout << endl << cMO;

	}

	print_result (iResult);

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
