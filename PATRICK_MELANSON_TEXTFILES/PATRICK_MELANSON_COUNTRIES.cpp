/*
"COUNTRIES" program designed on 14/02/2012 by Patrick Melanson, Ms Odecki's class
This program takes input from a text file about countries and outputs that, including some post-read values, onto the screen
*/

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <iomanip>
using namespace std;
int main()
{

    char strName[21];
    long lPop = 0;
    long lPopTotal = 0;
    long lArea = 0;
    long lAreaTotal = 0;
    float fGDP = 0;  //in trillions
    float fGDPTotal = 0;

    int n = 0;
    char s[11] = "";

    ifstream file;
    file.open ("PATRICK_MELANSON_DATA.txt");     //opens file, wherein data is formatted as [Name]//[Population]//[Area]//[GDP ($USD trillion)]

    cout << setw (20) << "Country" << setw (20) << "Population" << setw (20) << "Area (sqkm)" << setw (20) << "GDP ($USD trillions)" << setw (20) << "Capita per sqkm" << setw (30) << "GDP/Capita ($USD thousand)\n";

    while (!file.eof() )
    {

        file.getline (strName, 21);

        file.getline (s, 11);
        lPop = atoi (s);
        lPopTotal += lPop;

        file.getline (s, 11);
        lArea = atoi (s);
        lAreaTotal += lArea;

        file.getline (s, 11);
        fGDP = atof (s);
        fGDPTotal += fGDP;

        cout << setw (20) << strName << setw (20) << lPop << setw (20) << lArea << setw (20) << fGDP << setw (20) << lPop / lArea << setw (30) << (fGDP / lPop) * 1000000000 << endl;
        
        n++;

    }
    
    lPopTotal /= n;
    lAreaTotal /= n;
    fGDPTotal /= n;
    
    cout << endl << setw (20) << "Average:" << setw (20) << lPopTotal << setw (20) << lAreaTotal << setw (20) << fGDPTotal << setw (20) << lPopTotal / lAreaTotal << setw (30) << (fGDPTotal / lPopTotal) * 1000000000 << endl;

    file.close();

    cout << endl;
    system ("pause");
    return (0);
}
