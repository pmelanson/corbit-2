/*******************
Auditorium Program

Designed by Patrick Melanson

Last updated on 22/03/2012


This program creates an auditorium full of seats (30 rows, with 50 seats per row)
It also stores the price and availability of each seat in a matrix

*******************/
#include <stdlib.h>
#include <iostream>
using namespace std;

//global declarations
struct auditorium {
    int price;  //can be HIGH ($20), MED ($15), or LOW ($10)
    bool sold;  //whether the seat has been sold
};

auditorium seat[30][50];


void DisplayAuditorium();   //prints out the auditorium, with the stage being at the top, and whether a seat is available
int NumSold();              //finds out how many seats have been sold
int TotalValue();           //finds the value of all seats in the auditorium
int SoldValue();            //finds the value of all sold seats in the auditorium
void SellBlock (int y, int n, int x);   //sells a block of seats. n is the number of seats to sell
int BlockPrice (int y, int n, int x);   //finds price of a block of seats. n is the number of seats to sell

int main() {

    int iSold;
    int iTotalValue;
    int iSoldValue;

    int x;  //x indicates the seat
    int y;  //y indicates the row

    enum cost {LOW = 10, MED = 15, HIGH = 20};

    //initializing seat values, first 5 rows cost $20, next 15 rows cost $15, last 10 rows cost $10
    for (x = 0; x < 50; x++) {
        for (y = 0; y < 5; y++)
            seat[y][x].price = HIGH;

        for (y = 5; y < 20; y++)
            seat[y][x].price = MED;

        for (y = 20; y < 30; y++)
            seat[y][x].price = LOW;
    }



    /**SCREEN 1

    -shows an unedited auditorium
    -shows the price of certain blocks of seats
    -sells the aforementioned blocks of seats
    -calculates and stores the number of seats sold
    -calculates unsold seats

    **/
    DisplayAuditorium();

    SellBlock (2, 50, 1);
    SellBlock (12, 50, 1);
    SellBlock (22, 50, 1);
    SellBlock (10, 6, 25);
    SellBlock (11, 6, 25);

    iSold = NumSold();  //stored in a variable, so NumSold need not be recalled
    printf ("The total number of seats sold is: %4d\n", iSold );
    printf ("the number of seats unsold is: %4d\n\n", (50 * 30) - iSold);

    system ("pause");
    system ("cls");



    /**SCREEN 2

    -shows an edited auditorium, with sold seats
    -redisplays information from previous screen
    -calculates and displays percentage of seats sold
    -calculates and displays percentage of the value sold

    **/
    DisplayAuditorium();

    printf ("The total number of seats sold is: %4d\n", iSold );
    printf ("the number of seats unsold is: %4d\n\n", (50 * 30) - iSold);


    //new information begins here
    iTotalValue = TotalValue();
    iSoldValue = SoldValue();

    printf ("The total value of all seats in the auditorium is: $%d\n", iTotalValue );
    printf ("The total value of all sold seats in the auditorium is: $%d\n", iSoldValue );

    //Calculate the percentage of seats sold
    printf ("Seats sold in auditorium: %.1f%%\n", float (iSold * 100) / (50 * 30) );
    //Calculate the percentage of the value sold
    printf ("Value of seats sold in auditorium: %.1f%%\n\n", float (iSoldValue * 100) / iTotalValue);

    system ("pause");
    return (0);
}

void DisplayAuditorium() {

    int y;
    int x;

    printf ("%54s", "STAGE\n\n");

    for (y = 0; y < 30; y++) {
        printf ("Row %2d |", y + 1);

        for (x = 0; x < 50; x++)
            if (seat[y][x].sold == true)
                printf (" X");
            else if (seat[y][x].sold == false)
                printf (" _");

        printf ("\n");
    }

    printf ("\nThe cost of 50 seats in row 2, starting at seat 1 is:  $%4d\n", BlockPrice (2, 50, 1) );

    printf ("The cost of 50 seats in row 12, starting at seat 1 is: $%4d\n", BlockPrice (12, 50, 1) );

    printf ("The cost of 50 seats in row 22, starting at seat 1 is: $%4d\n", BlockPrice (22, 50, 1) );

    printf ("The cost of 6 seats in row 10, starting at seat 25 is: $%4d\n", BlockPrice (10, 6, 25) );

    printf ("The cost of 6 seats in row 11, starting at seat 25 is: $%4d\n", BlockPrice (11, 6, 25) );
}

int NumSold() {

    int iSold = 0;

    int y;
    int x;

    for (y = 0; y < 30; y++)
        for (x = 0; x < 50; x++)
            if (seat[y][x].sold == true)
                iSold++;

    return (iSold);
}

int TotalValue() {

    int iValue = 0;

    int y;

    for (y = 0; y < 30; y++)
        iValue += seat[y][0].price * 50; //the first seat will have the same value as other seats in that row, so (seat 1) * 50 will equal the value for that row

    return (iValue);
}

int SoldValue() {

    int iSold = 0;

    int y;
    int x;

    for (y = 0; y < 30; y++)
        for (x = 0; x < 50; x++)
            if (seat[y][x].sold == true)
                iSold += seat[y][x].price;

    return (iSold);
}

void SellBlock (int y, int n, int x) {

    int i;
    x--;
    y--;

    for (i = 0; i < n; i++)
        seat[y][x + i].sold = true;
}

int BlockPrice (int y, int n, int x) {

    int iValue = 0;

    int i;
    x--;
    y--;

    for (i = 0; i < n; i++)
        iValue += seat[y][x + i].price;

    return (iValue);
}
