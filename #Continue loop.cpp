	char cCont = 'Y';
	int iValid = 0;


	while (cCont == 'Y') {

	    //code
	    //moar code
	    //YOU KNOW IT

		while (iValid == 0) {       //asks the user if they want to run again
			cout << endl << "\nRun again (Y/N)? ";
			cin >> cCont;
			cCont = toupper (cCont);

			switch (cCont) {
				case 'Y':
				case 'N': {
						iValid = 1;
						system ("cls");
					}
					break;
				default: {
						system ("cls");
						cout << "Run again (Y/N)? " << cCont << endl
						<< "\nInvalid input";
					}     //detects bad input
			}
		}

		iValid = 0;

	}
