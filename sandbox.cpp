#include <iostream>
#include <stdlib.h>
using namespace std;

int main() {

    int iNum = 5;
    int *pPoint;
    pPoint = &iNum;

    cout << pPoint << endl << &pPoint << endl << *pPoint << endl;
    iNum = 7;
    cout << *pPoint << endl;

	system ("pause");
}
