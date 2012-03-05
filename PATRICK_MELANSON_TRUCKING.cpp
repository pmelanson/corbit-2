/*
"Trucking cost calculator" program designed on 11/02/2012 by Patrick Melanson, Ms Odecki's class
This program takes the user's input of shipment size, dimensions and distance to transport a shipment of soup cans
*/

#include <iostream>
#include <stdlib.h>
#include <iomanip>
#include <math.h>
#define HEIGHT 60
#define WIDTH 75
#define LENGTH 100
#define LOADCOST 8
#define TRANSCOST .37
#define MINBOXES 3
#define MAXBOXES 2000
#define MINCANS 20
#define MAXCANS 10000
#define MINGIRTH 4
#define MAXGIRTH 30
#define MINHEIGHT 10
#define MAXHEIGHT 30
#define MINDIST 50
#define MAXDIST 5000000
using namespace std;
int main() {

	double dCans;
	float fGirth;
	float fHeight;
	double dDist;
	double dBoxes;
	double iContain;
	float fLoadCost;
	float fTransCost;
	char cCont = 'Y';
	int iValid = 0;

	while (cCont == 'Y') {

		system ("cls");
		int i = 0;

		cout << "Input total cans you wish to transport (integer): ";
		cin >> dCans;
		cout << "Input radius of cans you wish to transport (cm): ";
		cin >> fGirth;
		fGirth *= 2;
		cout << "Input height of cans you wish to transport (cm): ";
		cin >> fHeight;
		cout << "Input distance you wish to transport these (km): ";
		cin >> dDist;

		cout << endl;

		//the absolute value is written, so that any accidental presses of '-' or other typos won't affect the input
		dCans = fabs (dCans);
		fGirth = fabs (fGirth);
		fHeight = fabs (fHeight);
		dDist = fabs (dDist);

		iContain = int (HEIGHT / fHeight) * int (WIDTH / fGirth) * int (LENGTH / fGirth); //Cans in a box = cans that fit lengthwise * cans that fit sideways * height of stack

		if (iContain >= dCans) iContain = dCans;    //If one box fits all the cans, only exception

		dBoxes = int ( (dCans - 1) / (iContain) ) + 1; //Total cans divided by number of cans in a box, +1, which works except...

		if (dCans == 1)
			dBoxes = 1;          //...when there is only one can...
		else if (dCans == 0)  //...or when you are dividing by zero
			dBoxes = 0;

		if ( (fGirth > WIDTH) || (fHeight > HEIGHT) ) { //This warning is printed before any other operations are performed on the number of boxes
			cout << "WARNING: The dimensions of your can is larger than the box, cannot transport.\n\n";
			dBoxes = 0;
		}

		fLoadCost = LOADCOST * dBoxes;

		fTransCost = TRANSCOST * dDist * dBoxes;

		if ( (dCans <= MINCANS) || (dCans >= MAXCANS) )
			cout << "WARNING: Total number of cans seems incorrect, maybe you want to transport a more reasonable more cans?\n\n";

		if ( (dBoxes <= MINBOXES) || (dBoxes >= MAXBOXES) )
			cout << "WARNING: Total boxes seems incorrect, maybe you want to transport a more reasonable amount or size of cans?\n\n";

		if ( (fGirth <= MINGIRTH) || (fGirth >= MAXGIRTH) )
			cout << "WARNING: The diameter of the cans seems incorrect, maybe you entered in an unreasonable value?\n\n";

		if ( (fHeight <= MINHEIGHT) || (fHeight >= MAXHEIGHT) )
			cout << "WARNING: The height of the cans seems incorrect, maybe you entered in an unreasonable value?\n\n";

		if ( (dDist <= MINDIST) || (dDist >= MAXDIST) )
			cout << "WARNING: The distance seems incorrect, maybe you want to transport them a more reasonable distance?\n\n";

		cout << "Maximum cans per box:\t" << iContain << endl;
		cout << "Total boxes:\t\t" << dBoxes << endl << endl;
		cout << "\t-Subtotal-\n";
		cout << "Loading fees:\t\t$" << setprecision (2) << fLoadCost << endl;
		cout << "Transporting:\t\t$" << setprecision (2) << fTransCost << endl << endl;
		cout << "\t-Total-\n";
		cout << "Total fee:\t\t$" << setprecision (2) << fLoadCost + fTransCost;


		while (iValid == 0) {       //asks the user if they want to run again
			cout << endl << "\nRun again (Y/N)? ";
			cin >> cCont;
			system ("cls");
			cCont = toupper (cCont);

			switch (cCont) {
			case 'Y':
			case 'N':
				iValid = 1;
				break;
			default:
				cout << "Run again (Y/N)? " << cCont << "\n\nInvalid input"; //detects bad input
			}
		}

		iValid = 0;
	}

	return (0);
}
