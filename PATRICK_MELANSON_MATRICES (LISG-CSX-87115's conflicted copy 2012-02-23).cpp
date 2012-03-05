#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <fstream>
#define ARRAY_SIZE 5
#define CELL_SIZE 4
#define ARRAY_COUNT 2
using namespace std;

void print_matrix (int iMatrix[ARRAY_COUNT][ARRAY_SIZE][ARRAY_SIZE], int n, int iRows, int iColumns) {

	int y = 0;
	int x = 0;


	cout << endl << endl;

	for (y = 0; y < iRows; y++) {

		for (x = 0; x < iColumns; x++)
			cout << setw (CELL_SIZE) << iMatrix[n][y][x] << "|";

		cout << endl;
	}

}

int matrix_operate (int iMatrix[ARRAY_COUNT][ARRAY_SIZE][ARRAY_SIZE], char cMO, int y, int x) {

	int n = 0;
	int iResult[ARRAY_SIZE][ARRAY_SIZE] = {0};

	switch (cMO) {
	case '+':
		iResult[y][x] = iMatrix[0][y][x] + iMatrix[1][y][x];
		break;
	case '-':
		iResult[y][x] = iMatrix[0][y][x] - iMatrix[1][y][x];
		break;
	case '*':
		iResult[y][x] += iMatrix[0][y][x] * iMatrix[1][x][y];
		break;

	}



return iResult[ARRAY_SIZE][ARRAY_SIZE];

       }

void print_result (int iResult[ARRAY_SIZE][ARRAY_SIZE], int iRows, int iColumns) {

	int y = 0;
	int x = 0;


	cout << endl << endl;

	for (y = 0; y < iRows; y++) {

		for (x = 0; x < iColumns; x++)
			cout << setw (CELL_SIZE) << iResult[y][x] << "|";

		cout << endl;
	}


}

int main() {

	int iMatrix[ARRAY_COUNT][ARRAY_SIZE][ARRAY_SIZE] = {0};
	int iResult[ARRAY_SIZE][ARRAY_SIZE] = {0};

	int n = 0;  //I only realized that these variable names spelt out "Nyx" once I'd finished the program, I swear
	int y = 0;
	int x = 0;
	char cMO;   //MO being Method of Operation's abbreviation
	char cCont = 'Y';
	int iValid = 0;
	char strFile[21] = "";
	int iRows = 0;
	int iColumns = 0;

	ifstream file;

	//Where the actual code begins
	while (cCont == 'Y') {

		cout << "Filename (example.txt): ";
		cin.getline (strFile, 21);
		file.open (strFile);

		for (n = 0; n < ARRAY_COUNT; n++) {
			file >> iRows;
			file >> iColumns;

			for (y = 0; y < iRows; y++)

				for (x = 0; x < iColumns; x++)
					file >> iMatrix[n][y][x];

			print_matrix (iMatrix, n, iRows, iColumns);
		}

		file.close();


		while (iValid == 0) {       //asks the user if they want to run again
			cout << endl << "\nRead another matrix (Y/N)? ";
			cin >> cCont;
			cCont = toupper (cCont);

			/*switch (cCont) {
				case 'Y':
				case 'N': {
						iValid = 1;
						system ("cls");
					}
					break;
				default: {
						system ("cls");
						cout << "Read another matrix (Y/N)? " << cCont << endl
						     << "\nInvalid input";
					}     //detects bad input
			}
					}
				}*/

			iValid = 0;
			system ("cls");

			cout << "Input modus operandi (+ - *): ";
			cin >> cMO;


			for (y = 0; y < iRows; y++) {
				for (x = 0; x < iRows; x++) {
					iResult[y][x] = matrix_operate (iMatrix, cMO, y, x);
				}
			}

			for (n = 0; n < ARRAY_COUNT; n++) {
				print_matrix (iMatrix, n, iRows, iColumns);

				if (n == (ARRAY_COUNT - 1) )
					cout << "\n\n=";
				else
					cout << endl << cMO;

			}

			print_result (iResult, iRows, iColumns);

		}
	}


}
