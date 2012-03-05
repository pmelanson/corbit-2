#include <iostream>
#include <stdlib.h>
using namespace std;
int main(){
 
 int iAge = 0;
 int iWork = 0;
 
 cout<<"Input age (integer): ";
 cin>>iAge;
 cout<<"Input years of service (integer): ";
 cin>>iWork;
 
 if (iWork > iAge - 14) cout<<"You can't have that many years of service in your life, think about that again\n";
 else {
      if ((iAge >= 65) || (iWork + iAge >= 85)) cout<<"Alright, I guess you can take a break now\n";
      else if (iAge >= 14) cout<<"Keep working!\n";
      else if ((iAge < 14) && (iAge > 0)) cout<<"Alright, I'm pretty sure that the minimum working age is 14. You're going to have to ";    //the next command is system("pause")
      else cout<<"Now that's just plain silly.\n";
 }
 
 
 system("pause");
 return(0);
}
