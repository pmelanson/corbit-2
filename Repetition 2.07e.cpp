#include <iostream>
#include <stdlib.h>
#include <string.h>
using namespace std;
int main() {

	char strIn[21];
	char cCont = 'Y';
	int iValid = 0;

	while (cCont == 'Y') {

		system ("cls");
		int i = 0;

		cout << "Input string you wish to modify: ";
		cin >> strIn;
		cout << endl;

		for (i = 0; i < strlen (strIn); i++) {
			if (i > 0)
				cout << "*";

			cout << strIn[i];
		}

		/*if (i == (strlen (strIn) - 2))
			i = strlen (strIn);
				}*/

		cout << "\n\nRun again (Y/N)? ";
		cin >> cCont;
		cCont = toupper (cCont);


	}

	cout << endl;
	return (0);
}
