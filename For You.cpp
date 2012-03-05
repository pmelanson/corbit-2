#include <iostream>
#include <stdlib.h>
using namespace std;
int main(){
 
 int i;
 int iMin;
 int iMax;
 int iStep;
 
 for (i=0; i<10; i++)cout<<i<<endl;
 
 i=0;
 cout<<endl;
 
 for (i=5; i<100; i+=4)cout<<i<<endl;
 
 i=0;
 
 cout<<endl<<"min: "; cin>>iMin;
 cout<<endl<<"max: "; cin>>iMax;
 cout<<endl<<"step: "; cin>>iStep;
 
 for (i=iMin; i<iMax; i+=iStep)cout<<endl<<i;
 
 cout<<endl;
 i=0;
 
 
 
 system("pause");
 return(0);
}
