#include <iostream>
#include <stdlib.h>
#include <iomanip>
using namespace std;

void printQuad (int w, int h, char cBlock);

int main(){
    int w = 0;
    int h = 0;
    char cBlock;

    cout << "Width of rectangle (integer): ";
    cin >> w;
    w--;    //otherwise the rectangle will be one character too long
    cout << "Height of rectangle (integer): ";
    cin >> h;
    h -= 2; //otherwise the rectangle will be two rows too high
    cout << "Charcater to make up lines (character): ";
    cin >> cBlock;

    system("cls");

    printQuad(w, h, cBlock);

    cout << "\n\n";
    system("pause");
}

void printQuad (int w, int h, char cBlock) {

int i;

for (i=0;i<w;i++)
    cout << cBlock; //creates top row
for (i=0;i<h;i++) {
    cout << cBlock; //prints left border
    cout << setw(w) << cBlock;  //prints right border
}

}
