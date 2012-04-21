#include <iostream>
#include <stdlib.h>
using namespace std;
int main(){
 
 char cKey;
 char cCont = 'Y';
 int iValid = 0;
 
 while (cCont=='Y'){
       
       cout<<"Input letter (one letter): ";
       cin>>cKey;
       system("cls");
       cKey=toupper(cKey);
       
       switch (cKey){
              case '&':
              case '\'':
              case '(': cout<<"Letter: "<<cKey<<endl<<"Number: 1";
                   break;
              case 'A':
              case 'B':
              case 'C': cout<<"Letter: "<<cKey<<endl<<"Number: 2";
                   break;
              case 'D':
              case 'E':
              case 'F': cout<<"Letter: "<<cKey<<endl<<"Number: 3";
                   break;
              case 'G':
              case 'H':
              case 'I': cout<<"Letter: "<<cKey<<endl<<"Number: 4";
                   break;
              case 'J':
              case 'K':
              case 'L': cout<<"Letter: "<<cKey<<endl<<"Number: 5";
                   break;
              case 'M':
              case 'N':
              case 'O': cout<<"Letter: "<<cKey<<endl<<"Number: 6";
                   break;
              case 'P':
              case 'Q':
              case 'R':
              case 'S': cout<<"Letter: "<<cKey<<endl<<"Number: 7";
                   break;
              case 'T':
              case 'U':
              case 'V': cout<<"Letter: "<<cKey<<endl<<"Number: 8";
                   break;
              case 'W':
              case 'X':
              case 'Y':
              case 'Z': cout<<"Letter: "<<cKey<<endl<<"Number: 9";
                   break;
              default: cout<<"that is just silly, please re-enter";
       }
              
       while (iValid == 0){        //asks the user if they want to run again
              cout<<endl<<"\nRun again (Y/N)? ";
              cin>>cCont;
              cCont = toupper(cCont);
                     
                   switch (cCont){
                          case 'Y':
                          case 'N': {iValid = 1; system("cls");}
                               break;
                          default: {system("cls"); cout<<"Run again (Y/N)? "<<cCont<<endl
                          <<"\nInvalid input";}     //detects bad input
                   }
       }
                   iValid = 0;
                   
 }
              
 
 return(0);
}
