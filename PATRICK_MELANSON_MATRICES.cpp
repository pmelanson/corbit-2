#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <fstream>
#define ARRAY_SIZE 5
#define CELL_SIZE 4
#define ARRAY_COUNT 2
using namespace std;

int matrix_operate (int iResult[ARRAY_SIZE][ARRAY_SIZE], char cMO, int y, int x);

void print_matrix (int n);

void print_result (int iResult[ARRAY_SIZE][ARRAY_SIZE]);

int iMatrix[ARRAY_COUNT][ARRAY_SIZE][ARRAY_SIZE] = {0};
int iRows[ARRAY_SIZE] = {0};   //iRows[3] is the number of rows in the resultant matrix
int iColumns[ARRAY_SIZE] = {0};//iColumns[3] ''

int main() {

    int iResult[ARRAY_SIZE][ARRAY_SIZE] = {0};

	int n = 0;  //I only realized that these variable names spelt out "Nyx" once I'd finished the program, I swear
	int y = 0;
	int x = 0;
	char cMO;   //MO being Method of Operation's abbreviation
	char strFile[21] = "";

	ifstream file;


	cout << "Filename (example.txt): ";
	cin.getline (strFile, 21);
	file.open (strFile);

	for (n = 0; n < ARRAY_COUNT; n++) {
		file >> iRows[n];
		file >> iColumns[n];

		for (y = 0; y < iRows[n]; y++)

			for (x = 0; x < iColumns[n]; x++)
				file >> iMatrix[n][y][x];

		print_matrix (n);

	}

	if ( (iRows[0] != iRows[1]) || (iColumns[0] != iColumns[1]) ) {
		cout << "\n\nThe matrices are not compatible. ";
		system ("pause");
		return (1);
	}

	cout << "\n\n";
	system ("pause");

	file.close();
	system ("cls");

	cout << "Input modus operandi (+ - *): ";
	cin >> cMO;


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

	cout << endl;
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

void print_result (int iResult[ARRAY_SIZE][ARRAY_SIZE]) {    //because you can't print a 3D matrix, e.g. iMatrix, like a 2D array, e.g. iResult

	int y = 0;
	int x = 0;


	cout << endl << endl;

	for (y = 0; y < iRows[2]; y++) {

		for (x = 0; x < iColumns[2]; x++)
			cout << setw (CELL_SIZE) << iResult[y][x] << "|";

		cout << endl;
	}


}
