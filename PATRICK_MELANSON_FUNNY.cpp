/*Patrick Melanson, Funny Headline assignment, Ms Odecki, 8th of February
This program takes input from the user and rearranges it into a mad-lib-esque headline*/

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <iomanip>
using namespace std;
int main(){
 
 int iItems;
 int iKeys;
 char strMeasure[16];
 float fMathNum;
 char strNoun[21];
 char strOS[16];
 char strOccupation[21];
 
 int n = 0;
 
 
 
 cout<<"Alright, it's a slow news day. You've got to spice things up a little"<<endl;
 Sleep(2000);
 cout<<"Hmmm";
 while (n<4){    //adds thinking dots
       Sleep(500);
       cout<<".";
       n++;}
 n=0;
 
 cout<<endl<<"I have an idea! I'll take some stuff I already know, like";
 while (n<4){    //adds thinking dots
       Sleep(500);
       cout<<".";
       n++;} 
 n=0;
 
 //gets all inputs from user
 cout<<endl<<endl<<"The number of items on my desk (volumes greater than 1 cm cubed): "; cin>>iItems; 
 cout<<endl<<"Additional keys my keyboard has (in addition to standard keys): "; cin>>iKeys; 
 cout<<endl<<"My favorite system of measurement (e.g. Centigrade, Metric): "; cin>>strMeasure; 
 cout<<endl<<"The most important number in math (real number): "; cin>>fMathNum; 
 cout<<endl<<"A random noun that is on my person (common noun): "; cin>>strNoun; 
 cout<<endl<<"The line of OS I use at home (e.g. Windows, Linux distros): "; cin>>strOS; 
 
 cout<<endl<<endl<<"Seems good. What would be a good headline? Well";
 while (n<4){    //adds thinking dots
       Sleep(500);
       cout<<".";
       n++;}
 n=0; 
 
 cout<<endl<<endl<<"Aha!";
 
 Sleep(1000);
 system("cls");
 
 
 cout<<setprecision(5)<<fMathNum<<" is closer to "<<iKeys<<" than "<<iItems<<" on the "
 <<strMeasure<<" scale, says "<<strOS<<" "<<strNoun<<" expert\n\n";
 
 Sleep(4000);
 
 cout<<"you know what, that just doesn't make sense.";
 
 if (iKeys == iItems) cout<<" Probably because "<<iKeys<<" and "<<iItems<<" are the same.";
 
 Sleep(2500);
 
 cout<<"\n\nI'll just come up with a random occupation (e.g. Builder, no spaces): ";
 cin>>strOccupation;
 
 if (iKeys != 1) strcat(strOccupation, "s");           //Makes occupation plural when appropriate
 if (iItems != 1) strcat(strNoun, "s");                //Makes noun plural when appropriate
 
 system("cls");
 
 
 //prints both headlines, for easier comparison
 cout<<setprecision(5)<<fMathNum<<" is closer to "<<iKeys<<" than "<<iItems<<" on the "
 <<strMeasure<<" scale, says "<<strOS<<" "<<strNoun<<" expert\n\n";
 
 cout<<iKeys<<" "<<strOccupation<<" successfully implant "<<strOS<<" into "
 <<iItems<<" "<<strNoun<<", using "<<strMeasure<<" system\n\n";
 
 Sleep(2000);
 
 cout<<"\n\nYou know what, good enough. I'll call it a day.\n\n\n"; 
 
 
 system("pause");
 return(0);
}
