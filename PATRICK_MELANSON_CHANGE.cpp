/*
"EXAMPLE TITLE" program designed on DD/02/2012 by Patrick Melanson, Ms Odecki's class
This program is descriptive
*/

#include <iostream>
#include <stdlib.h>
#include <iomanip>
using namespace std;

int quarters (int &money);
int dimes (int &money);
int nickels (int &money);

int main() {

	int money;

    cout << "How many cents do you have (integer)? ";
	cin >> money;
	cout << "You have " << quarters (money) << " quarters, ";
	cout << dimes (money) << " dimes, ";
	cout << nickels (money) << " nickels, and you would have ";
	cout << money << " pennies left, were the Canadian mint still going to make them." << endl;

	system ("pause");
	return (0);
}

int quarters (int &money) {

    int iRemain;
    int iQuart;

    iQuart = money / 25;
    iRemain = money % 25;

    money %= 25;

    return (iQuart);

}
int dimes (int &money) {

    int iRemain;
    int iDime;

    iDime = money / 10;
    iRemain = money % 10;

    money %= 10;

    return (iDime);

}
int nickels (int &money) {

    int iRemain;
    int iNick;

    iNick = money / 5;
    iRemain = money % 5;

    money %= 5;

    return (iNick);

}
