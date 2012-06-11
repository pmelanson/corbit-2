#include <iostream>
#include <sstream>
#include <fstream>
#include <allegro.h>
#include <vector>

#ifndef ENTITY_H
#include "entity.h"
#endif

#include "initialization.h"
using namespace std;
const long double au = 1495978707e2;

void initializeFromFile(vector <physical_t*> &entity) {

	//file initialization
	ifstream datafile;
	datafile.open ("entities.txt");
	if (datafile.is_open())
		cout << "datafile open\n";
	if (datafile.good())
		cout << "datafile good\n";

	//data initializations
	string container (""), name ("");
	long double x = 1337, y = 1337, Vx = 0, Vy = 0;
	long double mass = 1337, radius = 1337;
	unsigned int fillColor = 255, specialColor = 255, specialRadius = 413;
	short unsigned int R = 255, G = 255, B = 255;
	float specialFloat = 612;
	string line ("");

	datafile.ignore (4096, '!');
	cout << uppercase;

	while (getline (datafile, line)) { //each loop through this reads in an entity

		string container (""), name ("");
		x = 1337, y = 1337, Vx = 0, Vy = 0;
		mass = 1337, radius = 1337, specialRadius = 413;
		specialFloat = 612;
		fillColor = 255, specialColor = 255;

		stringstream iss (line);
		iss >> container;
		cout << endl << container;


		if (parse (datafile, name));
		else {
			name = "N/A";
			cout << "could not determine name, set to " << name << endl;
		}

		if (parse (datafile, x)) {
			x *= au;
			if (x == 0)
				x = 1;
		} else
			cout << "x read fail for " << name << endl;

		if (parse (datafile, y)) {
			y *= au;
			if (y == 0)
				y = 1;
		} else
			cout << "y read fail for " << name << endl;

		if (parse (datafile, Vx));
		else
			cout << "Vx read fail for " << name << endl;

		if (parse (datafile, Vy));
		else
			cout << "Vy read fail for " << name << endl;

		if (parse (datafile, mass));
		else
			cout << "mass read fail for " << name << endl;

		if (parse (datafile, radius))
			radius *= 2;
		else
			cout << "radius read fail for " << name << endl;

		if (parseColor (datafile, R, G, B))
			fillColor = makecol (R, G, B);
		else
			cout << "fillColor read fail for " << name << endl;

		if (parseColor (datafile, R, G, B))
			specialColor = makecol (R, G, B);
		else
			cout << "specialColor read fail for " << name << endl;

		if (parse (datafile, specialRadius));
		else
			cout << "specialRadius read fail for " << name << endl;

		if (parse (datafile, specialFloat));
		else
			cout << "specialFloat read fail for " << name << endl;

		if (container == "solarBody") {
			specialRadius *= 100;

//            body.push_back (new solarBody_t (name, x, y, Vx, Vy, mass, radius, fillColor, specialColor, specialRadius, specialFloat) );
			entity.push_back (new solarBody_t (name, x, y, Vx, Vy, mass, radius, fillColor, specialColor, specialRadius, specialFloat) );
//			body_eg.push_back (boost::make_shared <solarBody_t> (name, x, y, Vx, Vy, mass, radius, fillColor, specialColor, specialRadius, specialFloat) );
		}

		if (container == "craft")
			if (name == "Habitat") {
				specialRadius *= 2;

//				craft.push_back (new habitat_t (name, x, y, Vx, Vy, mass, radius, fillColor, specialColor, specialRadius, specialFloat) );
				entity.push_back (new habitat_t (name, x, y, Vx, Vy, mass, radius, fillColor, specialColor, specialRadius, specialFloat) );
//				body_eg.push_back (boost::make_shared<habitat_t>(name, x, y, Vx, Vy, mass, radius, fillColor, specialColor, specialRadius, specialFloat) );
			}

		if (container == "N/A") {

			cout << endl << name << " was not constructed with data of\n";
			cout << "x = " << x << endl;
			cout << "y = " << y << endl;
			cout << "Vx = " << Vx << endl;
			cout << "Vy = " << Vy << endl;
			cout << "mass = " << mass << endl;
			cout << "radius = " << radius << endl;
			cout << hex << "fillColor = " << hex << fillColor << endl;
			cout << hex << "specialColor = " << specialColor << endl;
			cout << "specialRadius = " << dec << specialRadius << endl;
			cout << "specialFloat = " << specialFloat << endl;
		}

		datafile.ignore (4096, '!');
	}


	/*long double xPos = 0, yPos = 0;

	datafile.ignore (4096, '<');

	while (getline (datafile, line, '>')) { //loops, getting the position of each background star

		istringstream iss (line);
	}*/
	datafile.close();
	cout << nouppercase;
}
