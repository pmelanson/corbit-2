/*
"YAHTZEE" program designed on 22/02/2012 by Patrick Melanson, Ms Odecki's class
This program is descriptive
*/

#include <iostream>
#include <stdlib.h>
#include <time.h>
#define DICE 5
using namespace std;
int main() {

	int iDie[DICE] = {0};
	int iRolls = 0;

	int n = 0;
	int iDone = 0;

	srand (time (0) );

	while (iDone == 0) {

		iRolls++;

		for (n = 0; n < DICE; n++) {
			iDie[n] = rand() % 6 + 1;
			cout << iDie[n] << " ";
		}

		cout << endl;

		if (iDie[0] == iDie[1] &&
		        iDie[1] == iDie[2] &&
		        iDie[2] == iDie[3] &&
		        iDie[3] == iDie[4]) {
			cout << "\nYou rolled a Yahtzee in " << iRolls << " rolls!";
			iDone = 1;
		}


		//system ("cls");
	}




	cout << endl << endl;
	system ("pause");
	return (0);
}
