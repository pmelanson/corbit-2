#include <iostream>
#include <stdlib.h>
#include <iomanip>
using namespace std;

void printQuad (int w, int h, char cBlock);

int main() {
	int w = 0;
	int h = 0;
	int i = 0;
	char cBlock;

	cout << "Width of rectangle (integer): ";
	cin >> w;
	w *= 2; //cell for each monospace fonts is twice as high as it is wide, therefore the rectangle must be printed twice as wide
	cout << "Height of rectangle (integer): ";
	cin >> h;
	cout << "Charcater to make up lines (character): ";
	cin >> cBlock;

	system ("cls");

	if (w < 1 || h < 1)
		cout << "Invalid size";
	else if (h == 1) {
		for (i = 0; i < w; i++)
			cout << cBlock;
		cout << "\n\nWarning: Cannot display top row, height too small";   //if the rectangle is only 1 unit high (i.e. two cells wide and one cell high) displaying a top row will make the rectangle twice as high.
	} else
		printQuad (w, h, cBlock);

	cout <<  "\n\n";
	system ("pause");
}

void printQuad (int w, int h, char cBlock) {

	int i;

	for (i = 0; i < w; i++)
		cout << cBlock; //creates top line

	for (i = 2; i < h; i++)
		cout << endl << cBlock << setw (w - 1) << cBlock; //prints sides

	cout << endl;

	for (i = 0; i < w; i++)
		cout << cBlock; //creates base

}
