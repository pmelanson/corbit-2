char cCont = 'Y';

int cont (char sMessage[31]);   //To ask the user if they want to continue, includes error checking

int cont (char sMessage[31]) {

    int iValid = 0;

    while (iValid == 0) {       //asks the user if they want to run again
        cout << endl << "Run again (Y/N)? ";
        cin >> cCont;
        cCont = toupper (cCont);

        switch (cCont) {    //detects invalid input
            case 'Y':
            case 'N': {
                    iValid = 1;
                    system ("cls");
                }
                break;

            default: {
                    system ("cls");
                    cout << "Run again (Y/N)? " << cCont << endl
                    << "\nInvalid input\n\n";
                }
        }
    }

    fflush (stdin);
    iValid = 0;

	}
