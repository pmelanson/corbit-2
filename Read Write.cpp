/*
"Read Write" program designed on 13/02/2012 by Patrick Melanson, Ms Odecki's class
Messing around with input/output to files
*/

#include <iostream>
#include <stdlib.h>
#include <fstream>
using namespace std;
int main() {

	fstream myfile;

	myfile.open ("ZoosmellPooplord.txt");
	myfile << "Jeggbert Poopsmell lol is a barnstink fartlass what a newb";
	myfile.close();

	char lolz[51];

	cin.getline (lolz, 21);
	cout << lolz << endl;

	myfile.open ("ZoosmellPooplord.txt");
	myfile >> lolz;
	cout << lolz << endl;
	myfile.getline (lolz, 21);
	cout << endl << lolz << endl;

	while (myfile.eof() ) {
		cout << lolz;
		myfile.getline (lolz, 100);
	}

	cout<<"all done";

	myfile.close();

	cout << endl;
	system ("pause");
	return (0);
}
