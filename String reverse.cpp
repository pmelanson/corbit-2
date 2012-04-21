#include <iostream>
#include <stdlib.h>
using namespace std;

void reverse ();

char input[21] = "";
char reversed[21] = "";

int main () {

	cin >> input;

	reverse ();
	reversed[strlen (input) ] = '\0';

	cout << reversed << endl;


	system ("pause");
	return (0);
}

void reverse () {

	int n = 0;

	for (n = 0; n < strlen (input); n++)
		reversed[n] = input[strlen (input) - 1 - n];

}
