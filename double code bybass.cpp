#include <stdio.h>
#include <stdlib.h>

int doubleit(int &number);

int main() {
    int num = 7;
    printf("The number returned is: %d\n",doubleit(num));
    printf("your number doubled is %d\n", num);
    system("pause");
}

int doubleit(int &number) {
      number =  number * 2;
      return(300);
}
