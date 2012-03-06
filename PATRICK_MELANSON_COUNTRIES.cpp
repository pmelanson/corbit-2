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
    long lArea = 0;
    long lGDP = 0;  //in trillions

    int n;
    char s[11];

    ifstream file;
    file.open ("Textfile Data.txt");     //opens file, wherein data is formatted as [Name]//[Population]//[Area]//[GDP $USD million)]//

    cout << setw (20) << "Country" << setw (20) << "Population" << setw (20) << "Area (sqkm)" << setw (20) << "GDP ($USD trillions" << setw (20) << "Capita per sqkm" << setw (30) << "GDP/Capita ($USD million)\n";

    while (!file.eof() )
    {

        file.getline (strName, 21);

        file.getline (s, 11);

        file.getline (s, 11);
        lArea = atoi (s);

        file.getline (s, 11);
        lGDP = atoi (s);

        cout << setw (20) << strName << setw (20) << lPop << setw (20) << lArea << setw (20) << lGDP << setw (20) << lPop / lArea << setw (30) << (double(lGDP) / 1000000) / lPop << endl;

    }


    cout << endl;
    system ("pause");
    return (0);
}
