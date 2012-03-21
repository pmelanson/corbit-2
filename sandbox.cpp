#include <iostream>
#include <stdlib.h>
using namespace std;

int pointers() {

	int iNum = 5;
	int *pPoint;
	pPoint = &iNum;

	cout << pPoint << endl << &pPoint << endl << *pPoint << endl;
	iNum = 7;
	cout << *pPoint << endl;

	return (0);
}

int enumerate() {

	enum card {ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING, JOKER};

	card outCard = QUEEN;

	cout << outCard << endl;

	return (0);
}

int main() {

	cout << "pointers\n\n";
	pointers();
	cout << endl;
	system ("pause");

	cout << "\nenum\n\n";
	enumerate();
	cout << endl;
	system ("pause");


	return (0);
}
