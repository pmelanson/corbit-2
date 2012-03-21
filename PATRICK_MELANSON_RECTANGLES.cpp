#include <iostream>
#include <stdlib.h>
#include <iomanip>
using namespace std;

void printQuad (int w, int h, int x, int y, char cBlock);

int main() {
	int w = 0;
	int h = 0;
	int x = 0;
	int y = 0;
	char cBlock;

	int i = 0;

	cout << "Width of rectangle (integer): ";
	cin >> w;
	w *= 2; //cell for each monospace fonts is twice as high as it is wide, therefore the rectangle must be printed twice as wide

	cout << "Height of rectangle (integer): ";
	cin >> h;

	cout << "X offset of top left corner (integer): ";
	cin >> x;
	x *= 2;
	if (x == 2) //due to the nature of setw(), an x value of 2 will only offset the rectangle by 1
		x++;

	cout << "Y offset of top left corner (integer): ";
	cin >> y;

	cout << "Charcater to make up lines (character): ";
	cin >> cBlock;

	system ("cls");


	if (w < 1 || h < 1 || x < 0 || y < 0)
		cout << "Values are invalid";
	else
		printQuad (w, h, x, y, cBlock);

	cout <<  "\n\n";
	system ("pause");
}

void printQuad (int w, int h, int x, int y, char cBlock) {

	int i;

	for (i = 0; i < y; i++)     //offsets down by y
		cout << endl;

	if (h > 1) {
		cout << setw (x);           //offsets to right by x
		for (i = 0; i < w; i++)     //prints top
			cout << cBlock;
	}

	for (i = 2; i < h; i++)     //prints sides, offset to right by x
		cout << setw (x) << endl << cBlock << setw (w - 1) << cBlock;

	cout << endl << setw (x);   //offsets to right by x
	for (i = 0; i < w; i++)     //prints bottom
		cout << cBlock;

}
