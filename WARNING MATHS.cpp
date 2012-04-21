#include <iostream>
#include <math.h>
#include <stdlib.h>
using namespace std;
int main(){
 double i;
 int n = 0;

 i = pow(2, 3);

 i++;

 i -= i*2;

 i = sqrt(fabs(i));

 i--;

 cout<<i<<endl;
 cout<<++i<<endl;
 cout<<i++<<endl;
 cout<<endl;
 
 while (n!=5){
       cout<<n;
       n++;}
 
 cout<<endl;
       
 n = 0;
 
 while (n!=5){
       cout<<n;
       ++n;}

 cout<<endl;



 system("pause");
 return(0);
 }
