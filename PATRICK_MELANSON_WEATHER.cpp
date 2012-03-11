/*
"Weather" program designed on 10/03/2012 by Patrick Melanson, Ms Odecki's class
This program takes user input to find the temperature for each day of the week,
then uses functions to find the average, high and low for the week, and which day they occur on
*/

#include <iostream>
#include <stdlib.h>
using namespace std;

float ave (float fDay[]);
void HiLo (float fDay[], float &fLo, float &fHi, int &iHiDay, int &iLoDay);

int main() {

	float fDay[7] = {};//{1, 2, 3, 4, 5, 6, 7}
	char strDay[7][10] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
	float fLo;
	float fHi;
	int iHiDay = 0;
	int iLoDay = 0;

	int i;

	for (i = 0; i < 7; i++) {
		cout << "Temperature for " << strDay[i] << ": ";
		cin >> fDay[i];
	}

	HiLo (fDay, fLo, fHi, iHiDay, iLoDay);
	cout << "\nLow for this week was " << fLo << ", which first occured on " << strDay[iLoDay];
	cout << "\nHigh for this week was " << fHi << ", which first occured on " << strDay[iHiDay];
	cout << "\nAverage for this week was " << ave (fDay);

	cout << endl;
	system ("pause");
	return (0);
}

float ave (float fDay[]) {

	int i;
	float fAve;

	for (i = 0; i < 7; i++)
		fAve += fDay[i];

	fAve /= 7;

	return (fAve);

}

void HiLo (float fDay[], float &fLo, float &fHi, int &iHiDay, int &iLoDay) {

    fLo = fDay[0];
	fHi = fDay[0];

	int i;

	for (i = 0; i < 7; i++) {

		if (fDay[i] > fHi) {
			iHiDay = i;
			fHi = fDay[i];
		}

		if (fDay[i] < fLo) {
			iLoDay = i;
			fLo = fDay[i];
		}
	}

}
