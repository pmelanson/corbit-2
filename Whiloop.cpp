#include <iostream>
#include <stdlib.h>
using namespace std;
int main(){
 
 int total = 0;
 int number = 0;
 
 while (total < 1000){
       printf("Enter number: ");
       scanf("%d", &number);
       if (number < 0)
          continue;
       total += number;
 }
 
 system("pause");
 return(0);
}
