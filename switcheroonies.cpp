#include <iostream>
#include <stdlib.h>
#include <stdio.h>
using namespace std;
int main(){
 
 int number;
 cin>>number;
 
/* case(num){
         case >5: cout<<"too high";
              break;
         case 5: cout<<"swag";
              break;
         case <5: cout<<"too low";
              break;
         default: cout<<"bluh bluh";
         }*/

 switch(number) {
case 1 :printf("one");
break;
case 2 :printf("two");
break;
case 3 :printf("three");
break;
default: printf("not within range");
}
 
 cout<<endl;
 system("pause");
 return(0);
}
