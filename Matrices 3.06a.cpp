#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <fstream>
#define ARRAY_SIZE 5
#define CELL_SIZE 2
#define ARRAY_COUNT 2
using namespace std;

void print_matrix (int iMatrix[ARRAY_COUNT][ARRAY_SIZE][ARRAY_SIZE], int n) {

	int y = 0;
	int x = 0;

	cout << endl << endl;

	for (x = 0; x < ARRAY_SIZE; x++) {

		for (y = 0; y < ARRAY_SIZE; y++)
			cout << setw (CELL_SIZE) << iMatrix[n][x][y] << "|";

		cout << endl;
	}

}

void print_result (int iResult[ARRAY_SIZE][ARRAY_SIZE]) {
//because print_matrix only works with 3D arrays
	int y = 0;
	int x = 0;

	for (x = 0; x < iColumns; x++) {

		for (y = 0; y < iRows; y++)
			cout << setw (CELL_SIZE) << iResult[y][x] << "|";

		cout << endl;
	}


}

int operate_matrix (int iMatrix[ARRAY_COUNT][ARRAY_SIZE][ARRAY_SIZE], char cMO, int n, int x, int y, int iColumns, int iRows) {

	int iResult[ARRAY_SIZE][ARRAY_SIZE] = {0};

	switch (cMO) {
		case '+':
			iResult[x][y] += iMatrix[n][x][y];
			break;

		case '-':
			iResult[x][y] -= iMatrix[n][x][y];
			break;

		/*case '*':   //This is assuming there are only two matrices, which is simpler
			iResult[x][y] = iMatrix[0][x][y] * iMatrix[1][x][y] +
			                iMatrix[0][x][y] * iMatrix[1][x][y] +
			                iMatrix[0][x][y] * iMatrix[1][x][y];
			break;*/
	}

	return iResult[x][y];

}

int main() {

	int iMatrix[ARRAY_COUNT][ARRAY_SIZE][ARRAY_SIZE] = {0};
	int iResult[ARRAY_SIZE][ARRAY_SIZE] = {0};

	int x = 0;
	int y = 0;
	int n = 0;
	int iRows = 0;
	int iColumns = 0;
	int iMatrices = 0;
	char cMO;
	char cCont;
	int iValid = 0;
	char strFile[21] = "";



	cout << "Would you like to input values Manually or read from a File (M/F)? ";
	cin >> cCont;
	cCont = toupper (cCont);
	system ("cls");

	/*switch (cCont) {
	    case 'M':
            iRows = ARRAY_SIZE;
            iColumns = ARRAY_SIZE;
        break;

        case 'F':
            cout << "Input filename, with extension (20 chars): ";
            cin >> strFile;

            int iRows = 0;
            int iColumns = 0;

            ifstream file;
            file.open (strFile);

            int get (iRows);
    //        int get (iColumns);
            cout << iRows << endl << iColumns << endl;
        break;

	}*/


	cCont = 'Y';

	while (cCont == 'Y') {

        for (n = 0; n < iMatrices; n++)
			print_matrix (iMatrix, n);


		cout << "Enter matrix you wish to modify (1/2) ";
		cin >> n;
		n -= 1;
		cout << "Enter column of cell you wish to modify (1-3): ";
		cin >> y;
		y -= 1;
		cout << "Enter row of cell you wish to modify (1-3): ";
		cin >> x;
		x -= 1;

		cout << "Enter new value: ";
		cin >> iMatrix[n][y][x];

		n = 0;
		x = 0;
		y = 0;

		for (n = 0; n < iMatrices; n++)
			print_matrix (iMatrix, n);

		while (iValid == 0) {       //asks the user if they want to run again
			cout << endl << "\nEnter another value (Y/N)? ";
			cin >> cCont;
			cCont = toupper (cCont);

			switch (cCont) {
				case 'Y':
				case 'N': {
						iValid = 1;
						system ("cls");
					}
					break;
				default: {
						system ("cls");
						cout << "Enter another value (Y/N)? " << cCont << endl
						     << "\nInvalid input";
					}     //detects bad input
			}
		}

		iValid = 0;

	}

	system ("cls");

	cout << "Modus Operandum (+ - *): ";
	cin >> cMO;

	n = 0;
	x = 0;
	y = 0;

	for (n = 0; n < ARRAY_COUNT; n++) {

		for (x = 0; x < iColumns; x++) {

			for (y = 0; y < iRows; y++)
				iResult[x][y] = operate_matrix (iMatrix, cMO, n, x, y, iRows, iColumns);

		}

	}


	for (n = 0; n < ARRAY_COUNT; n++) {
		print_matrix (iMatrix, n);

		if ( ! (n == (ARRAY_COUNT - 1) ) )   //if the last matrix has yet to be printed
			cout << endl << cMO;
		else
			cout << endl << "=" << endl << endl;
	}

	print_result (iResult, iRows, iColumns);



	return (0);
}
