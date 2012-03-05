#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
int main() {

	int x;
	int y;
	int iData[11][11];

	printf ("For-loop times table\n\n");

	for (x = 0; x <= 10; x++) {

		for (y = 0; y <= 10; y++) printf ("%4d|", x * y);

		printf ("\n ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----\n");
	}

	printf ("\n");
	system ("pause");   //new table
	system ("cls");

	x = 0;
	y = 0;

	printf ("While-loop times table\n\n");

	while (x <= 10) {

		while (y <= 10) {
			printf ("%4d|", x * y);
			y++;
		}

		printf ("\n ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----\n");
		x++;
		y = 0;
	}

	printf ("\n");
	system ("pause");   //new table
	system ("cls");

	x = 0;
	y = 0;

	printf ("Do-loop times table\n\n");

	do {
		do {
			printf ("%4d|", x * y);
			y++;
		} while (y <= 10);

		printf ("\n ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----\n");
		x++;
		y = 0;

	} while (x <= 10);


	printf ("\n");
	system ("pause");  //new table, using some swagnasty arrays
	system ("cls");

	x = 0;
	y = 0;

	printf ("SUPER COOL DOUBLE MOBIUS REACH AROUND ARRAY table\n\n");

	for (x = 0; x <= 10; x++) {

		for (y = 0; y <= 10; y++) {
			iData[x][y] =  x * y;
			printf ("%4d|", iData[x][y]);
		}

		cout << "\n ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----\n";

	}

	return (0);
}
