/*******************

 .d8888b.                   888      d8b 888
d88P  Y88b                  888      Y8P 888
888    888                  888          888
888         .d88b.  888d888 88888b.  888 888888
888        d88""88b 888P"   888 "88b 888 888
888    888 888  888 888     888  888 888 888
Y88b  d88P Y88..88P 888     888 d88P 888 Y88b.
 "Y8888P"   "Y88P"  888     88888P"  888  "Y888

Designed by Patrick Melanson (patrick.melanstone@gmail.com)

Started on 23/03/2012


This program provides a realistic space flight simulation
The player's ship_t is called the "Hab"
Forces are as realistic as possible
Each pixel at camera.actualZoom() == 0 level is equivalent the distance light travels in a vacumn in 1/299,792,458th of a second (one meter)
See changelog.txt for changelog past 31/03/2012

*******************/

/*******************

       CREDITS

-reddit and stackoverflow
For helping me with programming structure, practices, and methods
This project has definitely benefited from them,
I don't know how I could've made this as cool as it was without 'em

-zzyzzyxx
Technically also redditor, but was the most deserving of my upvotes.
Helped me with some neat programming tricks (isstream reading was his suggestion)
Definitely one of the most constructive members that I've been able to work with.

-Max J.
One of the bestest programmers I know IRL, and also a pretty good spacesimmer.
Helped me with 'good programming practice', whatever that is (next project I'll implement calling functions and classes, promise!)
Also a pretty good guy to bounce ideas off of. My int main is only 9 lines because of him.

-Mrs. Odecki
One of the most chill teachers I've had.
Gotten me through some of my silly moments (eg array[x][y] array[y][x] mixups)
This project wouldn't've been able to be started without her

-Dr. Admiral Magwood
The Admiral of OCESS. Can't get much better than that. Maybe Robert Thirsk.
Made an orbit simulator in Quick Basic, which this is based off of (the concept, more than the code)
Good with orbital physics. Orbital projection algorithms wouldn't exist in corbit without him.

-OCESS/Smamule Blatz/OCESS people
Baltz specifically, for helping me put gravity calculations into maths.
Just, be cool spacesim. Don't lose your swagger.
Without spacesim, there would be no space simulation, and thus this project would be, like, a 2D platformer game or something like that.

*******************/

/*******************

       CONTROLS

    CAMERA CONTROLS

ARROW KEYS     NUMPAD +/-
move camera    zoom in/out


     SHIP CONTROLS

W/S
engine up/down

A/D            ENTER
spin ccw/cw    set engines to 100

BACKSPACE      SHIFT+BACKSPACE
cut engines    stop turning

*******************/

/*******************

Copyright (C) 2012 Patrick Melanson <patrick.melanstone@gmail.com>

This software is licensed under the WTFPL license 2012, as follows:




            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
                    Version 2, December 2004

 Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>

 Everyone is permitted to copy and distribute verbatim or modified
 copies of this license document, and changing it is allowed as long
 as the name is changed.

            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

  0. You just DO WHAT THE FUCK YOU WANT TO.




Isn't that an awesome license? I like it.

*******************/

/*******************

	  TODO LIST

-Implement atmospheric resistance
-Do more math stuff
-Add SRBs
-Add more objects (AYSE, ISS)
-Make framerate close up to planets acceptable
-Greater time acceleration with fewer CPU cycles (i.e. not calculating every step, only every 60th, then extrapolating)
-Background stars

*******************/

/*******************

     CHANGELOG

23/03/2012: Started program, we'll have some fun times with this yet
24/03/2012: Added momentum physics with WASD keys, and bouncy collision detection off sides. Collision is a bit glitchy
25/03/2012: Added turning physics, looked at http://www.helixsoft.nl/articles/circle/sincos.htm <--- SO COOL, fixed collision glitchiness
26/03/2012: Rewrote to include fixeds and ints, and tidy up code
29/03/2012: Rewrote to take out fixeds, they are silly
31/03/2012: Built project with AutoVersioning, will autogenerate changelog now, into changelog.txt

*******************/

#include <allegro.h>
#include <math.h>
#include <vector>
//#include <memory>
#include "version.h"
//#include <boost/shared_ptr.hpp>
//#include <boost/make_shared.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include "display.h"
#include "entity.h"
#include "initialization.h"
using namespace std;

///global and constant declarations///
BITMAP* buffer = NULL;

bool printDebug = true;

enum entity_enum {SUN, MERCURY, VENUS, EARTH, MARS, JUPITER, SATURN, URANUS, NEPTUNE, PLUTO, HAB, ENTITYMAX};

unsigned long long cycleRate = 60;	//how many times per second all calculations are performed
unsigned int cycle = 0, cps = 0, cycleCounter = 0,	//used for running calculation loop and getting calculations performed per second
                                     fps = 0, fpsCounter = 0,	//used for getting frames per second
                                             inputTimer = 0;	//used for timing input

vector <physical_t*> entity;	//the vector of ALL entities. It's public because otherwise I'd just end up passing dozens of references, to achieve the same result

//vector iterators are
vector <physical_t*>::iterator it = entity.begin(), itX = entity.begin(), itY = entity.begin();	//it stands for iterator. Initialized here, so they don't have to be redeclared literally hundreds of times a second

///prototypes///
void input(), drawBuffer(), drawDisplay(), debug();	//gets input, draws buffer to screen, draw stuff to buffer, draws debug information, respectively
void changeTimeStep (float step);	//changes how many calculations are performed per second (60 default)
void calculate();	//calculates velocities, accelerations, new positions, turn, etc. for all objects

void drawBuffer() {

	textprintf_ex (buffer, font, 0, SCREEN_H - 10, makecol (255, 255, 255), -1, "Corbit v%d.%d%d.%d", AutoVersion::MAJOR, AutoVersion::MINOR, AutoVersion::REVISION, AutoVersion::BUILD);	//prints version information

	draw_sprite (buffer, screen, SCREEN_H, SCREEN_W); // Draw the buffer to the screen
	draw_sprite (screen, buffer, 0, 0);
	clear_bitmap (buffer); // Clear the contents of the buffer bitmap
}

void input() {

	if (key[KEY_TAB]) {	//toggles camera tracking
		camera.track = !camera.track;
		camera.x = camera.target->x;
		camera.y = camera.target->y;
	}

	if (key[KEY_TILDE])
		HUD.display = !HUD.display;

	if (key[KEY_Z])	//toggles printing of debug information
		printDebug = !printDebug;

	if (key[KEY_G])	//toggles grid drawing
		HUD.displayGrid = !HUD.displayGrid;

	if (key[KEY_A] && entity[HAB]->turnRate > -entity[HAB]->maxTurnRate && entity[HAB]->AI.navmode == MAN)	//turns hab, if hab is under the maximum turn rate, and the navmode is set to manual
		entity[HAB]->turnRate -= entity[HAB]->turnRateStep;

	if (key[KEY_D] && entity[HAB]->turnRate < entity[HAB]->maxTurnRate && entity[HAB]->AI.navmode == MAN)	//ditto, but for CW turning
		entity[HAB]->turnRate += entity[HAB]->turnRateStep;

	if (key[KEY_W])	//throttle up, hold shift for fine adjustments. There is no cap on how high you can go, like in original version of orbit
		if (key_shifts & KB_SHIFT_FLAG)
			entity[HAB]->engine += 0.1;
		else
			entity[HAB]->engine += 1;

	if (key[KEY_S])	//ditto, for throttle down. Yes, I know you can go negative engines, in the original orbit version you can as well
		if (key_shifts & KB_SHIFT_FLAG)
			entity[HAB]->engine -= 0.1;
		else
			entity[HAB]->engine -= 1;


	if (key[KEY_BACKSPACE]) {	//cuts engines, holding down ctrl stops turning
		if (key_shifts & KB_CTRL_FLAG) {   //all those if statements there just slow the turnRate gradually
			if (fabs (entity[HAB]->turnRate) < entity[HAB]->turnRateStep)
				entity[HAB]->turnRate = 0;
			else if (entity[HAB]->turnRate > 0 && !key[KEY_A])   //you can't turn in the opposite direction of turning, and use the stop key at the same time
				entity[HAB]->turnRate -= entity[HAB]->turnRateStep;
			else if (entity[HAB]->turnRate < 0 && !key[KEY_D])   //ditto, but for the other key
				entity[HAB]->turnRate += entity[HAB]->turnRateStep;
		} else
			entity[HAB]->engine = 0;
	}

	if (key[KEY_ENTER])	//engines to 100%, RAMMING SPEED!
		entity[HAB]->engine = 100;

	if (key[KEY_LEFT])	//pans camera left
		camera.panX (-1);

	if (key[KEY_RIGHT])	//pans camera right
		camera.panX (1);

	if (key[KEY_UP])	//pans camera up
		camera.panY (1);

	if (key[KEY_DOWN])	//you get the idea
		camera.panY (-1);

	if (key[KEY_PLUS_PAD])	//zooms camera in, holding alt increases calculations performed per second
		if (key_shifts & KB_ALT_FLAG)
			changeTimeStep (1.1);
		else
			camera.zoom (1);

	if (key[KEY_MINUS_PAD])	//ditto, zooming out, decreases calculations
		if (key_shifts & KB_ALT_FLAG)
			changeTimeStep (0.9);
		else
			camera.zoom (-1);

	///set targ/ref to nth planet from sun when holding down shift/ctrl, alt sets navmode for hab
	if (key[KEY_1])	//AI to manual
		if (key_shifts & KB_SHIFT_FLAG)
			HUD.target = entity[MERCURY];
		else if (key_shifts & KB_CTRL_FLAG)
			HUD.reference = entity[MERCURY];
		else if (key_shifts & KB_ALT_FLAG)
			entity[HAB]->AI.navmode = MAN;
		else {
			camera.target = entity[MERCURY];
			camera.x = camera.target->x;
			camera.y = camera.target->y;
		}

	if (key[KEY_2])	//AI to approach targ
		if (key_shifts & KB_SHIFT_FLAG)
			HUD.target = entity[VENUS];
		else if (key_shifts & KB_CTRL_FLAG)
			HUD.reference = entity[VENUS];
		else if (key_shifts & KB_ALT_FLAG)
			entity[HAB]->AI.navmode = APP_TARG;
		else {
			camera.target = entity[VENUS];
			camera.x = camera.target->x;
			camera.y = camera.target->y;
		}

	if (key[KEY_3])	//AI to depart ref
		if (key_shifts & KB_SHIFT_FLAG)
			HUD.target = entity[EARTH];
		else if (key_shifts & KB_CTRL_FLAG)
			HUD.reference = entity[EARTH];
		else if (key_shifts & KB_ALT_FLAG)
			entity[HAB]->AI.navmode = DEPART_REF;
		else {
			camera.target = entity[EARTH];
			camera.x = camera.target->x;
			camera.y = camera.target->y;
		}

	if (key[KEY_4])	//AI to ccw orientation around target
		if (key_shifts & KB_SHIFT_FLAG)
			HUD.target = entity[MARS];
		else if (key_shifts & KB_CTRL_FLAG)
			HUD.reference = entity[MARS];
		else if (key_shifts & KB_ALT_FLAG)
			entity[HAB]->AI.navmode = CCW;
		else {
			camera.target = entity[MARS];
			camera.x = camera.target->x;
			camera.y = camera.target->y;
		}

	if (key[KEY_5])	//AI to cw orientation around target
		if (key_shifts & KB_SHIFT_FLAG)
			HUD.target = entity[JUPITER];
		else if (key_shifts & KB_CTRL_FLAG)
			HUD.reference = entity[JUPITER];
		else if (key_shifts & KB_ALT_FLAG)
			entity[HAB]->AI.navmode = CW;
		else {
			camera.target = entity[JUPITER];
			camera.x = camera.target->x;
			camera.y = camera.target->y;
		}

	if (key[KEY_6])
		if (key_shifts & KB_SHIFT_FLAG)
			HUD.target = entity[SATURN];
		else if (key_shifts & KB_CTRL_FLAG)
			HUD.reference = entity[SATURN];
		else {
			camera.target = entity[SATURN];
			camera.x = camera.target->x;
			camera.y = camera.target->y;
		}

	if (key[KEY_7])
		if (key_shifts & KB_SHIFT_FLAG)
			HUD.target = entity[URANUS];
		else if (key_shifts & KB_CTRL_FLAG)
			HUD.reference = entity[URANUS];
		else {
			camera.target = entity[URANUS];
			camera.x = camera.target->x;
			camera.y = camera.target->y;
		}

	if (key[KEY_8])
		if (key_shifts & KB_SHIFT_FLAG)
			HUD.target = entity[NEPTUNE];
		else if (key_shifts & KB_CTRL_FLAG)
			HUD.reference = entity[NEPTUNE];
		else {
			camera.target = entity[NEPTUNE];
			camera.x = camera.target->x;
			camera.y = camera.target->y;
		}

	if (key[KEY_9])
		if (key_shifts & KB_SHIFT_FLAG)
			HUD.target = entity[PLUTO];
		else if (key_shifts & KB_CTRL_FLAG)
			HUD.reference = entity[PLUTO];
		else {
			camera.target = entity[PLUTO];
			camera.x = camera.target->x;
			camera.y = camera.target->y;
		}

	if (key[KEY_0]) {	//sets center to hab
		camera.target = entity[HAB];
		camera.x = camera.target->x;
		camera.y = camera.target->y;
	}


	inputTimer--;
}

void changeTimeStep (float step) {

	if (cycleRate * step <= 18446744073709551615LL && cycleRate * step > 1) {   //making sure that cycleRate does not go too small, or beyond the limits of an unsigned long long int (but frankly, you'd better have a crazy computer to do this many cycles per second, you silly person)
		cycleRate *= step;	//in/decrements calculations performed per second
		install_int_ex (CYCLE, BPS_TO_TIMER (cycleRate) );	//applies changes
	}
}

void debug() {

	const unsigned short int spacing = SCREEN_H - SCREEN_H / 4;	//how low on the screen to print debug information

	textprintf_ex (buffer, font, 0, 0 + spacing, makecol (200, 200, 200), -1, "DEBUG: center.x: %Lf", camera.target->x);
	textprintf_ex (buffer, font, 0, 10 + spacing, makecol (200, 200, 200), -1, "DEBUG: center.y = %Lf", camera.target->y );
	textprintf_ex (buffer, font, 0, 20 + spacing, makecol (200, 200, 200), -1, "DEBUG: center.a: %li", camera.target->a() );
	textprintf_ex (buffer, font, 0, 30 + spacing, makecol (200, 200, 200), -1, "DEBUG: center.b: %li", camera.target->b() );
	textprintf_ex (buffer, font, 0, 40 + spacing, makecol (200, 200, 200), -1, "DEBUG: center.Vx: %Lf", camera.target->Vx);
	textprintf_ex (buffer, font, 0, 50 + spacing, makecol (200, 200, 200), -1, "DEBUG: center.Vy: %Lf", camera.target->Vy);
	textprintf_ex (buffer, font, 0, 60 + spacing, makecol (200, 200, 200), -1, "DEBUG: center.turnRate: %Lf", camera.target->turnRate);
	textprintf_ex (buffer, font, 0, 70 + spacing, makecol (200, 200, 200), -1, "DEBUG: center.radius: %lu", camera.target->radius);
	textprintf_ex (buffer, font, 0, 80 + spacing, makecol (200, 200, 200), -1, "DEBUG: center.mass: %lu", camera.target->mass);
	textprintf_ex (buffer, font, 0, 90 + spacing, makecol (200, 200, 200), -1, "DEBUG: arc tan: %Lf", atan2f (entity[HAB]->x - entity[EARTH]->x, entity[HAB]->y - entity[EARTH]->y));
	textprintf_ex (buffer, font, 0, 100 + spacing, makecol (200, 200, 200), -1, "DEBUG: Actual zoom: %Lf", camera.actualZoom() );
	textprintf_ex (buffer, font, 0, 110 + spacing, makecol (200, 200, 200), -1, "DEBUG: camera X: %Ld", camera.x);
	textprintf_ex (buffer, font, 0, 120 + spacing, makecol (200, 200, 200), -1, "DEBUG: camera Y: %Ld", camera.y);
	textprintf_ex (buffer, font, 0, 150 + spacing, makecol (200, 200, 200), -1, "DEBUG: earth atmosphere: %u", entity[EARTH]->atmosphereHeight);
	textprintf_ex (buffer, font, 0, 160 + spacing, makecol (200, 200, 200), -1, "DEBUG: camera.x: %Lf", camera.x);
	textprintf_ex (buffer, font, 0, 170 + spacing, makecol (200, 200, 200), -1, "DEBUG: camera.y: %Lf", camera.y);
	textprintf_ex (buffer, font, 0, 180 + spacing, makecol (200, 200, 200), -1, "DEBUG: camera.Vx: %li", camera.Vx);
	textprintf_ex (buffer, font, 0, 190 + spacing, makecol (200, 200, 200), -1, "DEBUG: camera.Vy: %li", camera.Vy);
}

void calculate() {

	for (it = entity.begin(); it != entity.end(); ++it) {
		(*it)->turn();
		(*it)->acceleration = 0;
		(*it)->move();
		if (camera.track == true)
			camera.updateSpeed();
	}

	for (itX = entity.begin(); itX != entity.end(); ++itX)
		for (itY = itX, ++itY; itY != entity.end(); ++itY) {
			(*itX)->gravitate (**itY);
			(*itX)->detectCollision (**itY);
		}

	camera.shift();

	cycle--;
	cycleCounter++;
}

void drawDisplay() {

	if (HUD.displayGrid)
		HUD.drawGrid();

	for (it = entity.begin(); it != entity.end(); ++it)
//	    if ((*it)->a() + (*it)->radius > 0 && (*it)->a() - (*it)->radius < SCREEN_W &&  //if entity is on screen
//            (*it)->b() + (*it)->radius > 0 && (*it)->b() - (*it)->radius < SCREEN_H)
		(*it)->draw((*it)->a(), (*it)->b(), camera.actualZoom());

	if (HUD.display)
		HUD.drawHUD();

	if (printDebug)
		debug();

	drawBuffer();

	fpsCounter++;
}

/*void initializeFromFile() {

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
	unsigned int fillColor = makecol (255, 255, 0), specialColor = makecol (0, 255, 255), specialRadius = 413;
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
		fillColor = makecol (255, 255, 0), specialColor = makecol (0, 255, 255);

		stringstream iss (line);
		iss >> container;
		cout << endl << container;


		if (parse (datafile, name));
		else {
			name = "N/A";
			cout << "could not determine name, set to " << name << endl;
		}

		if (parse (datafile, x)) {
			x *= AU;
			if (x == 0)
				x = 1;
		} else
			cout << "x read fail for " << name << endl;

		if (parse (datafile, y)) {
			y *= AU;
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


	long double xPos = 0, yPos = 0;

	datafile.ignore (4096, '<');

	while (getline (datafile, line, '>')) { //loops, getting the position of each background star

		istringstream iss (line);
	}
	datafile.close();
	cout << nouppercase;
}

void initializeAllegro() {

	allegro_init();
	install_keyboard();
	set_keyboard_rate(100, 100);
	set_color_depth (desktop_color_depth());
	set_gfx_mode (GFX_AUTODETECT_WINDOWED, screenWidth, screenHeight, 0, 0);
	set_display_switch_mode(SWITCH_BACKGROUND);

	LOCK_VARIABLE (timer);
	LOCK_VARIABLE (fpsCounter);
	LOCK_VARIABLE (fps);
	LOCK_VARIABLE (inputTimer);
	LOCK_FUNCTION (INPUT);
	LOCK_FUNCTION (CYCLE);
	LOCK_FUNCTION (FPS);
	install_int_ex (CYCLE, BPS_TO_TIMER (cycleRate) );
	install_int_ex (FPS, BPS_TO_TIMER (FPSCOUNTBPS));
	install_int_ex (INPUT, BPS_TO_TIMER (INPUTBPS));
	buffer = create_bitmap (SCREEN_W, SCREEN_H);
}*/

void initialize() {

	initializeAllegro();
	initializeFromFile();

	camera.target = entity[HAB];
	camera.x = camera.target->x;
	camera.y = camera.target->y;
	camera.reference = entity[EARTH];
	HUD.target = entity[EARTH];
	HUD.reference = entity[MARS];
}

void cleanup() {

	destroy_bitmap (buffer);
	release_screen();

	for (it = entity.begin(); it != entity.end(); ++it)
		delete *it;
	entity.clear();
}



int main (int argc, char *argv[]) {

	initialize();

	while (!key[KEY_ESC]) {

		while (inputTimer > 0)
			input();
		while (cycle > 0)
			calculate();
		drawDisplay();
	}

	cleanup();
	return 0;
}
END_OF_MAIN();  //5 lines of code in the main function. Now that is pretty sweet.
