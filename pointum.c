int main() {

	int iNum;
	int *ptrInt = &iNum;			// declares a pointer to an Integer.

	iNum = 7;
	printf ("%d\t", iNum);  //7
	printf ("%d\t",  *ptrInt);  //9  // prints the value at address

	*ptrInt = 9;			// assigns value to address
	printf ("%d\t",  iNum);     //9          // Notice that iNum is now 9
	printf ("%d\t", ptrInt);    //address		// prints what the address is.
	printf ("%d\t", &iNum);

	return 0;
}
