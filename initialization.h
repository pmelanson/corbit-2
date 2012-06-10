#ifndef INITIALIZATION_H
#define INITIALIZATION_H

#include <iostream>
using namespace std;

bool parse (istream &stream, long double &data),
parse (istream &stream, float &data),
parse (istream &stream, unsigned int &data),
parse (istream &stream, unsigned long int &data),
parse (istream &stream, unsigned long long int &data),
parseColor (istream &stream, unsigned short int &R, unsigned short int &G, unsigned short int &B),
parse (istream &stream, char &data),
parse (istream &stream, string &data);	//overloadable functions for parsing data from input stream (i.e. file)

void initializeFromFile();

#endif
