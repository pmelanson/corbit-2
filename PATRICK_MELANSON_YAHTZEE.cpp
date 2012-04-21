/*
"YAHTZEE" program designed on 22/02/2012 by Patrick Melanson, Ms Odecki's class
This program rolls 6 'dice', outputting their rolls, until all dice roll the same roll.
At this point it stops, and prints out how many rolls it took to score a Yahtzee
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

	srand (time (0) );

	for (iRolls = 0; 0 == 0; iRolls++) {

		for (n = 0; n < DICE; n++) {
			iDie[n] = rand() % 6 + 1;
			cout << iDie[n] << " ";
		}

		cout << endl;

		if (iDie[0] == iDie[1] &&
		        iDie[1] == iDie[2] &&
		        iDie[2] == iDie[3] &&
		        iDie[3] == iDie[4]) {
			cout << "\nYou rolled a Yahtzee in " << iRolls << " rolls!\n\n";
			system ("pause");
			return 0;
		}

		//system ("cls");
	}

}
