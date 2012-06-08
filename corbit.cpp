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
Each pixel at camera.actualzoomLevel() = 0 level is equivalent the distance light travels in a vacumn in 1/299,792,458th of a second (one meter)
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

     CHANGELOG

23/03/2012: Started program, we'll have some fun times with this yet
24/03/2012: Added momentum physics with WASD keys, and bouncy collision detection off sides. Collision is a bit glitchy
25/03/2012: Added turning physics, looked at http://www.helixsoft.nl/articles/circle/sincos.htm <--- SO COOL, fixed collision glitchiness
26/03/2012: Rewrote to include fixeds and ints, and tidy up code
29/03/2012: Rewrote to take out fixeds, they are silly
31/03/2012: Built project with AutoVersioning, will autogenerate changelog now, into changelog.txt. Smell ya later!

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
#include <string.h>
using namespace std;

///global and constant declarations///
const unsigned short int screenWidth = 1280;    //my computer's resolution
//const unsigned short int screenWidth = 1140;    //my computer's resolution
const unsigned short int screenHeight = 980;    //my computer's resolution
//const unsigned short int screenHeight = 800;    //my computer's resolution
bool printDebug = true;

BITMAP *buffer = NULL;
volatile unsigned int cycle = 0, cps = 0, cycleCounter = 0, fps = 0, fpsCounter = 0, inputTimer = 0;
unsigned long long cycleRate = 60;   //how many times per second all calculations are performed
const unsigned short int FPSCOUNTBPS = 2,   //how many times per second FPS and CPS (cycles per second) are calculated. Larger numbers result in faster refresh rates, but also larger extrapolations
                                       INPUTBPS = 15;  //how many times per second keyboard is checked for input

const long double AU = 1495978707e2, G = 6.673e-11, PI = 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844609550582231725359408128481117450284102701938521105559644622948954930381964428810975665933446128475648233786783165271201909145648566923460348610454326648213393607260249141273724587006606315588174881520920962829254091715364367892590360011330530548820466521384146951941511609433057270365759591953092186117381932611793105118548074462379962749567351885752724891227938183011949129833673362440656643086021394946395224737190702179860943702770539217176293176752384674818467669405132000568127145263560827785771342757789609173637178721468440901224953430146549585371050792279689258923542019956112129021960864034418159813629774771309960518707211349999998372978049951059731732816096318595024459455346908302642522308253344685035261931188171010003137838752886587533208381420617177669147303598253490428755468731159562863882353787593751957781857780532171226806613001927876611195909216420198938095257201065485863278865936153381827968230301952035301852968995773622599413891249721775283479131515574857242454150695950829533116861727855889075098381754637464939319255060400927701671139009848824012858361603563707660104710181942955596198946767;

//enum craft_enum {HAB, CRAFTMAX};
//enum body_enum {SUN, MERCURY, VENUS, EARTH, MARS, JUPITER, SATURN, URANUS, NEPTUNE, PLUTO, BODYMAX};
enum entity_enum {SUN, MERCURY, VENUS, EARTH, MARS, JUPITER, SATURN, URANUS, NEPTUNE, PLUTO, HAB, ENTITYMAX};
enum navMode_enum {MAN, APP_TARG, DEPART_REF, CCW, CW, NAVMAX};

///class declarations///
class viewpoint_t {

	const int zoomMagnitude;  //when zooming out, actual zoom level = camera.zoom ^ zoomMagnitude, therefore is an exponential zoom
	const float zoomStep; //rate at which cameras zoom out
	const float maxZoom;  //the smaller this is, the further you can zoom in
	const double minZoom; //the smaller this is, the farther you can zoom out
	const unsigned short int panSpeed;

public:
	long double x;
	long double y;

	long double zoomLevel;
	long double actualZoom();

	struct physical_t *target;
	struct physical_t *reference;
	bool track;
	void shift();
	void autoZoom();

	void zoom (short int direction);
	void panX (short int direction);
	void panY (short int direction);

	viewpoint_t (int _zoomMagnitude, float _zoomStep, float _maxZoom, double _minZoom, unsigned short int _panSpeed, long double _zoomLevel) :
		zoomMagnitude (_zoomMagnitude), zoomStep (_zoomStep), maxZoom (_maxZoom), minZoom (_minZoom), panSpeed (_panSpeed), x (0), y (0), zoomLevel (_zoomLevel), track (true)
	{}
} camera (  22,				0.05,		0.5,	1e-11,	10,			0);   //constructor initializes consts in the order they are declared, which is...
//          zoomMagnitude   zoomStep    maxZoom minZoom panSpeed    zoomLevel

class display_t {

	const unsigned short int gridSpace, lineSpace;
	const unsigned short int targVectorLength, vVectorLength;
	const unsigned short int craftX, craftY;    //the x and y position of the craft displayed on the HUD

public:
	struct physical_t *target;
	struct physical_t *reference;
	void drawHUD();
	void drawGrid();

	display_t (short unsigned int _gridSpace, unsigned short int _lineSpace, unsigned short int _targVectorLength, unsigned short int _vVectorLength, unsigned short int _craftX, unsigned short int _craftY) :
		gridSpace (_gridSpace), lineSpace (_lineSpace), targVectorLength (_targVectorLength), vVectorLength (_vVectorLength), craftX (_craftX), craftY (_craftY * lineSpace)
	{}
} HUD (   18,		15,			110,				65,				140,	22);    //constructor initializes consts in the order they are declared, which is...
//        gridSpace	lineSpace	targVectorLength	vVectorLength	craftX	craftY

struct physical_t { //stores data about any physical physical, such as mass and radius, acceleration, velocity, and angle from right

	const string name;  //I love C++ over C so much for this

	const unsigned long int radius;   //radius of physical
	const long double mass;	//mass of physical
	long double x, y; //the center of the physical, compared to the origin (center of solar system)
	long int a(), b();  //on-screen position of physical
	float turnRadians;
	long double distance (long double x, long double y), thetaToObject (physical_t &target);
	virtual void move();   //moves physical

	long double acceleration;    //total acceleration, no calculations are actually performed on this, just for printing purposes
	virtual void acc (long double radians, long double acc);	//accelerates the physical, but really just calls accX and accY. acc here is m/s/s, not m/s/ms
	void accX (long double radians, long double acc), accY (long double radians, long double acc);
	long double Vx, Vy;   //the physical's speed (m/s) along each axis
	long double Vcen (physical_t &targ), Vtan (physical_t &targ), thetaV();	//gets the vcen and vtan relative to a target, as well as the theta of the velocity vector
	long double gravity (long double _x, long double _y, long double _mass);  //calculate gravitational acceleration to point
	void gravitate (physical_t &targ), detectCollision (physical_t &targ);  //gravitate towards/detect collision with target

	void turn();   //turns the physical
	long double turnRate; //rate at which the physical turns
	float turnRateStep, maxTurnRate;    //values are in degrees per second

	const unsigned int fillColor;
	virtual void draw (unsigned int A, unsigned int B, long double zoom);	//draws physical

	struct AI_t {

		navMode_enum navmode;       //enum for which navmode to use, e.g. MAN, CCW
		string descriptor[NAVMAX];   //string describing the current nav mode

		AI_t () :
			navmode (MAN)
		{

			descriptor[MAN] = "Manual";
			descriptor[APP_TARG] = "App Targ";
			descriptor[DEPART_REF] = "Depart Ref";
			descriptor[CCW] = "CCW Prograde";
			descriptor[CW] = "CW Retrograde";
		}
	} AI;

	union { //declares either engine or atmosphereHeight

		float engine;
		const unsigned int atmosphereHeight;
	};

	union { //declares either fuel or atmosphereDrag

		unsigned long int fuel;
		const float atmosphereDrag;
	};

	physical_t (string _name, long double _x, long double _y, long double _Vx, long double _Vy, long double _mass, unsigned long int _radius, unsigned int _fillColor, float _engine_atmosphereHeight, float _fuel_atmosphereDrag) :
		name (_name), x (_x), y (_y), Vx (_Vx), Vy (_Vy), mass (_mass), radius (_radius), fillColor (_fillColor), turnRate (0), turnRadians (0), atmosphereHeight (_engine_atmosphereHeight), atmosphereDrag (_fuel_atmosphereDrag), turnRateStep (0.5 * PI / 180), maxTurnRate (10 * PI / 180) {
	}
};

struct solarBody_t : physical_t {   //stores information about an astronomical body, in addition to information already stored by an physical

	const unsigned int atmosphereColor;

	virtual void draw(unsigned int A, unsigned int B, long double zoom);

	solarBody_t (string _name, long double _x, long double _y, long double _Vx, long double _Vy, long double _mass, long unsigned int _radius, unsigned int _fillColor, unsigned int _atmosphereColor, float _engine_atmosphereHeight, float _fuel_atmosphereDrag) :
		physical_t (_name, _x, _y, _Vx, _Vy, _mass, _radius, _fillColor, _engine_atmosphereHeight, _fuel_atmosphereDrag),
		atmosphereColor (_atmosphereColor) {

		cout << endl << name << " initialized, with data of\n";
		cout << "x = " << x << endl;
		cout << "y = " << y << endl;
		cout << "Vx = " << Vx << endl;
		cout << "Vy = " << Vy << endl;
		cout << "mass = " << mass << endl;
		cout << "radius = " << radius << endl;
		cout << hex << "fillColor = " << hex << fillColor << endl;
		cout << hex << "atmosphereColor = " << atmosphereColor << endl;
		cout << "atmosphereHeight = " << dec << atmosphereHeight << endl;
		cout << "atmosphereDrag = " << atmosphereDrag << endl;
	}
};

struct ship_t : physical_t {  //stores information about a pilotable ship, in addition to information already stored by an physical

	void move();
	void acc(long double radians, long double acc);
	void fireEngine();

	const unsigned int engineColor;
	const unsigned short int engineRadius;

	float burnRate;

	virtual void draw(unsigned int A, unsigned int B, long double zoom);

	ship_t (string _name, long double _x, long double _y, long double _Vx, long double _Vy, long double _mass, long unsigned int _radius, unsigned int _fillColor, unsigned int _engineColor, unsigned short int _engineRadius, float _fuel_atmosphereDrag) :
		physical_t (_name, _x, _y, _Vx, _Vy, _mass, _radius, _fillColor, 0, _fuel_atmosphereDrag),
		engineColor (_engineColor), engineRadius (_engineRadius), burnRate (10) {
	}
};

struct habitat_t : ship_t {

	void draw(unsigned int A, unsigned int B, long double zoom);

	habitat_t (string _name, long double _x, long double _y, long double _Vx, long double _Vy, long double _mass, long unsigned int _radius, unsigned int _fillColor, unsigned int _engineColor, unsigned short int _engineRadius, float _fuel_atmosphereDrag) :
		ship_t (_name, _x, _y, _Vx, _Vy, _mass, _radius, _fillColor, _engineColor, _engineRadius, _fuel_atmosphereDrag) {

		cout << endl << name << " initialized, with data of\n";
		cout << "x = " << x << endl;
		cout << "y = " << y << endl;
		cout << "Vx = " << Vx << endl;
		cout << "Vy = " << Vy << endl;
		cout << "mass = " << mass << endl;
		cout << "radius = " << radius << endl;
		cout << hex << "fillColor = " << hex << fillColor << endl;
		cout << hex << "engineColor = " << engineColor << endl;
		cout << "engineRadius = " << dec << engineRadius << endl;
		cout << "fuel = " << fuel << endl;
	}
};

vector <physical_t*> entity;

//vector iterators are initialized here, so they don't have to be redeclared literally hundreds of times a second
vector <physical_t*>::iterator it, itX, itY;	//it stands for iterator

///prototypes///
void CYCLE(), FPS(), INPUT();   //timer functions for running calculations, getting FPS, and getting input, respectively
void input(), drawBuffer(), debug(), drawGrid();
void changeFrameRate (short int step);
void initializeFromFile(), initializeAllegro(), initialize();
void calculate(), drawDisplay();
void cleanup();
bool parse (istream &stream, long double &data), parse (istream &stream, float &data), parse (istream &stream, unsigned int &data), parse (istream &stream, unsigned long int &data), parse (istream &stream, unsigned long long int &data), parseColor (istream &stream, unsigned int &color), parse (istream &stream, char &data), parse (istream &stream, string &data);


void CYCLE () {

	cycle++;
}
END_OF_FUNCTION (CYCLE);

void FPS () {

	cps = cycleCounter * FPSCOUNTBPS;
	cycleCounter = 0;

	fps = fpsCounter * FPSCOUNTBPS;
	fpsCounter = 0;
}
END_OF_FUNCTION (FPS);

void INPUT () {

	inputTimer++;
};
END_OF_FUNCTION (INPUT);

void drawBuffer () {

	textprintf_ex (buffer, font, 0, SCREEN_H - 10, makecol (255, 255, 255), -1, "Corbit v%d.%d%d.%d", AutoVersion::MAJOR, AutoVersion::MINOR, AutoVersion::REVISION, AutoVersion::BUILD);

	draw_sprite (buffer, screen, SCREEN_H, SCREEN_W); // Draw the buffer to the screen
	draw_sprite (screen, buffer, 0, 0);
	clear_bitmap (buffer); // Clear the contents of the buffer bitmap
}

void input () {

	if (key[KEY_Z])
		printDebug = !printDebug;

	if (key[KEY_A] && entity[HAB]->turnRate > -entity[HAB]->maxTurnRate && entity[HAB]->AI.navmode == MAN)
		entity[HAB]->turnRate -= entity[HAB]->turnRateStep;

	if (key[KEY_D] && entity[HAB]->turnRate < entity[HAB]->maxTurnRate && entity[HAB]->AI.navmode == MAN)
		entity[HAB]->turnRate += entity[HAB]->turnRateStep;

	if (key[KEY_W])
		if (key_shifts & KB_SHIFT_FLAG)
			entity[HAB]->engine += 0.5;
		else if (key_shifts & KB_CTRL_FLAG)
			entity[HAB]->engine += 0.1;
		else
			entity[HAB]->engine += 1;

	if (key[KEY_S])
		if (key_shifts & KB_SHIFT_FLAG)
			entity[HAB]->engine -= 0.5;
		else if (key_shifts & KB_CTRL_FLAG)
			entity[HAB]->engine += 0.1;
		else
			entity[HAB]->engine -= 1;

	if (key[KEY_BACKSPACE]) {
		if (key_shifts & KB_SHIFT_FLAG) {   //all those if statements there just slow the turnRate gradually
			if (fabs (entity[HAB]->turnRate) < entity[HAB]->turnRateStep)
				entity[HAB]->turnRate = 0;
			else if (entity[HAB]->turnRate > 0 && !key[KEY_A])   //you can't turn in the opposite direction of turning, and use the stop key at the same time
				entity[HAB]->turnRate -= entity[HAB]->turnRateStep;
			else if (entity[HAB]->turnRate < 0 && !key[KEY_D])   //ditto, but for the other key
				entity[HAB]->turnRate += entity[HAB]->turnRateStep;
		} else
			entity[HAB]->engine = 0;
	}

	if (key[KEY_ENTER])
		entity[HAB]->engine = 100;

	if (key[KEY_LEFT])
		camera.panX (-1);

	if (key[KEY_RIGHT])
		camera.panX (1);

	if (key[KEY_UP])
		camera.panY (-1);

	if (key[KEY_DOWN])
		camera.panY (1);

	if (key[KEY_PLUS_PAD])
		if (key_shifts & KB_ALT_FLAG)
			changeFrameRate (1);
		else
			camera.zoom (1);

	if (key[KEY_MINUS_PAD])
		if (key_shifts & KB_ALT_FLAG)
			changeFrameRate (-1);
		else
			camera.zoom (-1);

	if (key[KEY_1])
		if (key_shifts & KB_SHIFT_FLAG)
			HUD.target = entity[MERCURY];
		else if (key_shifts & KB_CTRL_FLAG)
			HUD.reference = entity[MERCURY];
		else if (key_shifts & KB_ALT_FLAG)
			entity[HAB]->AI.navmode = MAN;
		else
			camera.target = entity[MERCURY];

	if (key[KEY_2])
		if (key_shifts & KB_SHIFT_FLAG)
			HUD.target = entity[VENUS];
		else if (key_shifts & KB_CTRL_FLAG)
			HUD.reference = entity[VENUS];
		else if (key_shifts & KB_ALT_FLAG)
			entity[HAB]->AI.navmode = APP_TARG;
		else
			camera.target = entity[VENUS];

	if (key[KEY_3])
		if (key_shifts & KB_SHIFT_FLAG)
			HUD.target = entity[EARTH];
		else if (key_shifts & KB_CTRL_FLAG)
			HUD.reference = entity[EARTH];
		else if (key_shifts & KB_ALT_FLAG)
			entity[HAB]->AI.navmode = DEPART_REF;
		else
			camera.target = entity[EARTH];

	if (key[KEY_4])
		if (key_shifts & KB_SHIFT_FLAG)
			HUD.target = entity[MARS];
		else if (key_shifts & KB_CTRL_FLAG)
			HUD.reference = entity[MARS];
		else if (key_shifts & KB_ALT_FLAG)
			entity[HAB]->AI.navmode = CCW;
		else
			camera.target = entity[MARS];

	if (key[KEY_5])
		if (key_shifts & KB_SHIFT_FLAG)
			HUD.target = entity[JUPITER];
		else if (key_shifts & KB_CTRL_FLAG)
			HUD.reference = entity[JUPITER];
		else if (key_shifts & KB_ALT_FLAG)
			entity[HAB]->AI.navmode = CW;
		else
			camera.target = entity[JUPITER];

	if (key[KEY_6])
		if (key_shifts & KB_SHIFT_FLAG)
			HUD.target = entity[SATURN];
		else if (key_shifts & KB_CTRL_FLAG)
			HUD.reference = entity[SATURN];
		else if (key_shifts & KB_ALT_FLAG)
			entity[HAB]->AI.navmode = MAN;
		else
			camera.target = entity[SATURN];

	if (key[KEY_7])
		if (key_shifts & KB_SHIFT_FLAG)
			HUD.target = entity[URANUS];
		else if (key_shifts & KB_CTRL_FLAG)
			HUD.reference = entity[URANUS];
		else if (key_shifts & KB_ALT_FLAG)
			entity[HAB]->AI.navmode = MAN;
		else
			camera.target = entity[URANUS];

	if (key[KEY_8])
		if (key_shifts & KB_SHIFT_FLAG)
			HUD.target = entity[NEPTUNE];
		else if (key_shifts & KB_CTRL_FLAG)
			HUD.reference = entity[NEPTUNE];
		else if (key_shifts & KB_ALT_FLAG)
			entity[HAB]->AI.navmode = MAN;
		else
			camera.target = entity[NEPTUNE];

	if (key[KEY_9])
		if (key_shifts & KB_SHIFT_FLAG)
			HUD.target = entity[PLUTO];
		else if (key_shifts & KB_CTRL_FLAG)
			HUD.reference = entity[PLUTO];
		else if (key_shifts & KB_ALT_FLAG)
			entity[HAB]->AI.navmode = MAN;
		else
			camera.target = entity[PLUTO];

	if (key[KEY_0])
		camera.target = entity[HAB];


	inputTimer--;
}

void changeFrameRate (short int step) {

	if (cycleRate + step <= 18446744073709551615LL && cycleRate + step > 0) {   //making sure that cycleRate does not go beyond the limits of an unsigned long long int (but frankly, you'd better have a crazy computer to do this many cycles per second, you silly person
		cycleRate += step;
		install_int_ex (CYCLE, BPS_TO_TIMER (cycleRate) );
	}
}

void debug() {

	const unsigned short int spacing = SCREEN_H - SCREEN_H / 4;

	textprintf_ex (buffer, font, 0, 0 + spacing, makecol (200, 200, 200), -1, "DEBUG: center.x: %Lf", camera.target->x);
	textprintf_ex (buffer, font, 0, 10 + spacing, makecol (200, 200, 200), -1, "DEBUG: center.y = %Lf", camera.target->y );
	textprintf_ex (buffer, font, 0, 20 + spacing, makecol (200, 200, 200), -1, "DEBUG: center.a: %Li", camera.target->a() );
	textprintf_ex (buffer, font, 0, 30 + spacing, makecol (200, 200, 200), -1, "DEBUG: center.b: %Li", camera.target->b() );
	textprintf_ex (buffer, font, 0, 40 + spacing, makecol (200, 200, 200), -1, "DEBUG: center.Vx: %Lf", camera.target->Vx);
	textprintf_ex (buffer, font, 0, 50 + spacing, makecol (200, 200, 200), -1, "DEBUG: center.Vy: %Lf", camera.target->Vy);
	textprintf_ex (buffer, font, 0, 60 + spacing, makecol (200, 200, 200), -1, "DEBUG: center.turnRate: %Lf", camera.target->turnRate);
	textprintf_ex (buffer, font, 0, 70 + spacing, makecol (200, 200, 200), -1, "DEBUG: center.radius: %lu", camera.target->radius);
	textprintf_ex (buffer, font, 0, 80 + spacing, makecol (200, 200, 200), -1, "DEBUG: center.mass: %lu", camera.target->mass);
	textprintf_ex (buffer, font, 0, 90 + spacing, makecol (200, 200, 200), -1, "DEBUG: arc tan: %Lf", atan2f (entity[HAB]->x - entity[EARTH]->x, entity[HAB]->y - entity[EARTH]->y) + PI * 0.5 );
	textprintf_ex (buffer, font, 0, 100 + spacing, makecol (200, 200, 200), -1, "DEBUG: Actual zoom: %Lf", camera.actualZoom() );
	textprintf_ex (buffer, font, 0, 110 + spacing, makecol (200, 200, 200), -1, "DEBUG: camera X: %Ld", camera.x);
	textprintf_ex (buffer, font, 0, 120 + spacing, makecol (200, 200, 200), -1, "DEBUG: camera Y: %Ld", camera.y);
	textprintf_ex (buffer, font, 0, 150 + spacing, makecol (200, 200, 200), -1, "DEBUG: earth atmosphere: %u", entity[EARTH]->atmosphereHeight);
}

void physical_t::move() {

	x += Vx;
	y += Vy;
}

void ship_t::move() {

	fireEngine();

	x += Vx;
	y += Vy;
}

void ship_t::fireEngine() {

	if (fuel > 0) {
		acc (turnRadians, engine * burnRate);
		fuel -= engine / 60;
	}
}

void physical_t::turn () {

	turnRadians += turnRate / 60;

	if (turnRadians < 0)
		turnRadians += 2 * PI;

	else if (turnRadians > 2 * PI)
		turnRadians -= 2 * PI;

	if (AI.navmode == APP_TARG) {

//		if (entity[HAB]->thetaToObject (HUD.target) == PI);
	}
}

void physical_t::acc (long double radians, long double acc) {

	accX (radians, acc / mass / 60);
	accY (radians, acc / mass / 60);
	acceleration += acc / mass / 60;
}

void ship_t::acc (long double radians, long double acc) {

	accX (radians, acc / (mass + fuel) / 60);
	accY (radians, acc / (mass + fuel) / 60);
	acceleration += acc / (mass + fuel) / 60;
}

void physical_t::accX (long double radians, long double acc) {	//takes angle at which to accelerate, and takes F/m for acc

	Vx += cos (radians) * acc;
}

void physical_t::accY (long double radians, long double acc) {	//takes angle at which to accelerate, and takes F/m for acc

	Vx += sin (radians) * acc;
}

long double physical_t::distance (long double targX, long double targY) {	//finds distance from physical to target

	return (sqrtf( ((x - targX) * (x - targX)) + ((y - targY) * (y - targY)) ));	//finds the distance between two entities, using d = sqrt ( (x1 - x2)^2 + (y1 - y2) )
}

long double physical_t::gravity(long double targX, long double targY, long double targMass) {

	return (G * mass * targMass / distance (targX, targY) );	//G * mass1 * mass2 / r^2
}

long int physical_t::a() { //on-screen x position of physical

	return ( (x - camera.x) * camera.actualZoom() + SCREEN_W / 2);
}

long int physical_t::b() { //on-screen y position of physical

	return ( (y - camera.y) * camera.actualZoom() + SCREEN_H / 2);
}

void physical_t::draw(unsigned int A, unsigned int B, long double zoom) {

	circlefill (buffer, A, B, radius * zoom, fillColor); //draws the physical to the buffer
}

void solarBody_t::draw(unsigned int A, unsigned int B, long double zoom) {

	circlefill (buffer, A, B, zoom * (radius), atmosphereColor);	//draws the atmosphere to the buffer

	circlefill (buffer, A, B, radius * camera.actualZoom(), fillColor);	//draws the planet body to the buffer

	textprintf_ex (buffer, font, A, B, makecol (200, 200, 200), -1, "%s", name.c_str() );
}

void ship_t::draw(unsigned int A, unsigned int B, long double zoom) {

	circlefill (buffer, A, B, radius * zoom, fillColor);	//draws the picture to the buffer
	line (buffer, A, B,	//draws the 'engine'
	      A + radius * cos (turnRadians) * zoom,
	      B + radius * sin (turnRadians) * zoom,
	      engineColor);
}

void habitat_t::draw(unsigned int A, unsigned int B, long double zoom) {

	circlefill (buffer, A, B, radius * zoom, fillColor);	//draws the hull to the buffer

	if (engine == 0) {

		circlefill (buffer,	//draws the center 'engine'
		            A + (radius - engineRadius) * cos (turnRadians - (PI) ) * zoom,
		            B + (radius - engineRadius) * sin (turnRadians - (PI) ) * zoom,
		            engineRadius * zoom,
		            fillColor - 1052688);	//the inactive engine color is fillColor - hex(101010)
		circlefill (buffer,	//draws the left 'engine'
		            A + radius * cos (turnRadians - (PI * .75) ) * zoom,
		            B + radius * sin (turnRadians - (PI * .75) ) * zoom,
		            engineRadius * zoom,
		            fillColor - 1052688);	//the inactive engine color is fillColor - hex(101010)
		circlefill (buffer,	//draws the right 'engine'
		            A + radius * cos (turnRadians - (PI * 1.25) ) * zoom,
		            B + radius * sin (turnRadians - (PI * 1.25) ) * zoom,
		            engineRadius * zoom,
		            fillColor - 1052688);	//the inactive engine color is fillColor - hex(101010)
	}

	else {

		circlefill (buffer,	//draws the center 'engine'
		            A + (radius - engineRadius) * cos (turnRadians - (PI) ) * zoom,
		            B + (radius - engineRadius) * sin (turnRadians - (PI) ) * zoom,
		            engineRadius * zoom,
		            engineColor);
		circlefill (buffer,	//draws the left 'engine'
		            A + radius * cos (turnRadians - (PI * .75) ) * zoom,
		            B + radius * sin (turnRadians - (PI * .75) ) * zoom,
		            engineRadius * zoom,
		            engineColor);
		circlefill (buffer,	//draws the right 'engine'
		            A + radius * cos (turnRadians - (PI * 1.25) ) * zoom,
		            B + radius * sin (turnRadians - (PI * 1.25) ) * zoom,
		            engineRadius * zoom,
		            engineColor);
	}
}

void display_t::drawGrid () {	//draws a grid to the screen, later on I will be making gravity distort it

	unsigned short int x, y;

	for (x = 0; x < SCREEN_W; x += gridSpace)
		for (y = 0; y < SCREEN_H; y += gridSpace)
			putpixel (buffer, x, y, makecol (255, 255, 255));
}

void display_t::drawHUD () {

	rectfill (buffer, 0, 0, 300, 36 * lineSpace, 0);	//draws background for HUD
	rect (buffer, -1, -1, 300, 36 * lineSpace, makecol (255, 255, 255));	//draws outline for HUD

	textprintf_ex (buffer, font, lineSpace, 1 * lineSpace, makecol (200, 200, 200), -1, "Orbit V (m/s):"), textprintf_ex (buffer, font, 200, 1 * lineSpace, makecol (255, 255, 255), -1, "1337");
	textprintf_ex (buffer, font, lineSpace, 2 * lineSpace, makecol (200, 200, 200), -1, "Hab/Targ V diff:"), textprintf_ex (buffer, font, 200, 2 * lineSpace, makecol (255, 255, 255), -1, "%-10.7Lg",
	        (entity[HAB]->Vx + entity[HAB]->Vy) - (target->Vx + target->Vy));
	textprintf_ex (buffer, font, lineSpace, 3 * lineSpace, makecol (200, 200, 200), -1, "Centrifugal V (m/s):");
	textprintf_ex (buffer, font, lineSpace, 4 * lineSpace, makecol (200, 200, 200), -1, "Tangential V (m/s):");
	textprintf_ex (buffer, font, lineSpace, 6 * lineSpace, makecol (200, 200, 200), -1, "Fuel (kg):"), textprintf_ex (buffer, font, 200, 6 * lineSpace, makecol (255, 255, 255), -1, "%li", entity[HAB]->fuel);
	textprintf_ex (buffer, font, lineSpace, 7 * lineSpace, makecol (200, 200, 200), -1, "Engines (kg/s):"), textprintf_ex (buffer, font, 200, 7 * lineSpace, makecol (255, 255, 255), -1, "%-10.1f", entity[HAB]->engine);
	textprintf_ex (buffer, font, lineSpace, 8 * lineSpace, makecol (200, 200, 200), -1, "Acc (m/s/s):"), textprintf_ex (buffer, font, 200, 8 * lineSpace, makecol (255, 255, 255), -1, "%-10.5Lf", entity[HAB]->acceleration);
	textprintf_ex (buffer, font, lineSpace, 10 * lineSpace, makecol (200, 200, 200), -1, "Altitude (m):"), textprintf_ex (buffer, font, 200, 10 * lineSpace, makecol (255, 255, 255), -1, "%-10.5Lg", entity[HAB]->distance (target->x, target->y));
	textprintf_ex (buffer, font, lineSpace, 11 * lineSpace, makecol (200, 200, 200), -1, "Pitch (radians):");
	textprintf_ex (buffer, font, lineSpace, 12 * lineSpace, makecol (200, 200, 200), -1, "Stopping Acc:"), textprintf_ex (buffer, font, 200, 12 * lineSpace, makecol (255, 255, 255), -1, "%-10.5Lf",
	        entity[HAB]->distance (target->x, target->y) / (2 * entity[HAB]->distance (target->x, target->y) - target->radius) * cos (entity[HAB]->thetaV() - entity[HAB]->thetaToObject (*target)));
	textprintf_ex (buffer, font, lineSpace, 13 * lineSpace, makecol (200, 200, 200), -1, "Periapsis (m):");
	textprintf_ex (buffer, font, lineSpace, 14 * lineSpace, makecol (200, 200, 200), -1, "Apoapsis (m):");

	circlefill (buffer, 140, 22 * lineSpace, entity[HAB]->radius, entity[HAB]->fillColor);	//draws the habitat onto the HUD at a constant size, along with velocity vector and position related to reference

	entity[HAB]->draw (craftX, craftY, 1);	//draws the habitat onto the HUD

	line (buffer, craftX, craftY,	//draws velocity vector on habitat
	      craftX + (vVectorLength) * cos (entity[HAB]->thetaV()),
	      craftY + (vVectorLength) * sin (entity[HAB]->thetaV()),
	      makecol (255, 0, 0));
	textprintf_ex (buffer, font,	//draws target location in respect to habitat
	               craftX + (targVectorLength) * cos (entity[HAB]->thetaToObject (*target)),
	               craftY + (targVectorLength) * sin (entity[HAB]->thetaToObject (*target)),
	               makecol (255, 255, 255), -1, "%s", target->name.c_str());

	textprintf_ex (buffer, font, lineSpace, 30 * lineSpace, makecol (200, 200, 200), -1, "Center:"), textprintf_ex (buffer, font, 200, 30 * lineSpace, makecol (255, 255, 255), -1, "%s", camera.target->name.c_str());
	textprintf_ex (buffer, font, lineSpace, 31 * lineSpace, makecol (200, 200, 200), -1, "Target:"), textprintf_ex (buffer, font, 200, 31 * lineSpace, makecol (255, 255, 255), -1, "%s", target->name.c_str());
	textprintf_ex (buffer, font, lineSpace, 32 * lineSpace, makecol (200, 200, 200), -1, "Reference:"), textprintf_ex (buffer, font, 200, 32 * lineSpace, makecol (255, 255, 255), -1, "%s", reference->name.c_str());
	textprintf_ex (buffer, font, lineSpace, 33 * lineSpace, makecol (200, 200, 200), -1, "Autopilot:"), textprintf_ex (buffer, font, 200, 33 * lineSpace, makecol (255, 255, 255), -1, "%s", entity[HAB]->AI.descriptor[entity[HAB]->AI.navmode].c_str());
	textprintf_ex (buffer, font, lineSpace, 34 * lineSpace, makecol (200, 200, 200), -1, "FPS:"), textprintf_ex (buffer, font, 200, 34 * lineSpace, makecol (255, 255, 255), -1, "%d", fps);
	textprintf_ex (buffer, font, lineSpace, 35 * lineSpace, makecol (200, 200, 200), -1, "Calcs per Second:"), textprintf_ex (buffer, font, 200, 35 * lineSpace, makecol (255, 255, 255), -1, "%d", cps);
}

void viewpoint_t::zoom (short int direction) {

	if (zoomLevel < maxZoom)
		zoomLevel += zoomStep * direction;
	else
		zoomLevel -= zoomStep;
}

long double viewpoint_t::actualZoom() {

	return (pow (zoomMagnitude, zoomLevel) + minZoom);
}

void viewpoint_t::panX (short int direction) {

	x += panSpeed / actualZoom() * actualZoom() * direction;
}

void viewpoint_t::panY (short int direction) {

	y += panSpeed / actualZoom() * actualZoom() * direction;
}

void viewpoint_t::shift() {

	x = target->x;
	y = target->y;
}

bool parse (istream &stream, long double &data) {

	string line;

	if (getline (stream, line)) { // was able to read a line
		istringstream iss (line);

		if (iss >> data); // was able to parse the number
		else
			return false;
	} else
		return false;

	return true;
}

bool parse (istream &stream, float &data) {

	string line;

	if (getline (stream, line)) { // was able to read a line
		istringstream iss (line);

		if (iss >> data); // was able to parse the number
		else
			return false;
	} else
		return false;

	return true;
}

bool parse (istream &stream, string &data) {

	string line;

	if (getline (stream, line)) { // was able to read a line
		istringstream iss (line);

		if (iss >> data); // was able to parse the string
		else
			return false;
	} else
		return false;

	return true;
}

bool parse (istream &stream, unsigned int &data) {

	string line;

	if (getline (stream, line)) { // was able to read a line
		istringstream iss (line);

		if (iss >> data); // was able to parse the number
		else
			return false;
	} else
		return false;

	return true;
}

bool parse (istream &stream, unsigned long int &data) {

	string line;

	if (getline (stream, line)) { // was able to read a line
		istringstream iss (line);

		if (iss >> data); // was able to parse the number
		else
			return false;
	} else
		return false;

	return true;
}

bool parse (istream &stream, unsigned long long int &data) {

	string line;

	if (getline (stream, line)) { // was able to read a line
		istringstream iss (line);

		if (iss >> data); // was able to parse the number
		else
			return false;
	} else
		return false;

	return true;
}

bool parseColor (istream &stream, unsigned int &data) { //takes input in the form of [RRR, GGG, BBB], default color is fuschia (if color cannot be read)

	string line;
	unsigned short int R = 255, G = 0, B = 255;

	if (getline (stream, line)) {
		istringstream iss (line);

		if (iss >> R) {
			iss.ignore (2, ',');
			if (iss >> G) {
				iss.ignore (2, ',');
				if (iss >> B)
					data = makecol (R, G, B);	//if able to parse every color value
			} else
				return false;
		} else
			return false;
	} else
		return false;

	return true;
}

void initializeFromFile() {

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

		istringstream iss (line);
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

		if (parseColor (datafile, fillColor));
		else
			cout << "fillColor read fail for " << name << endl;

		if (parseColor (datafile, specialColor));
		else
			cout << "specialColor read fail for " << name << endl;

		if (parse (datafile, specialRadius));
		else
			cout << "specialRadius read fail for " << name << endl;

		if (parse (datafile, specialFloat));
		else
			cout << "fuel read fail for " << name << endl;

		if (container == "solarBody") {
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
	changeFrameRate (0);
	install_int_ex (FPS, BPS_TO_TIMER (FPSCOUNTBPS));
	install_int_ex (INPUT, BPS_TO_TIMER (INPUTBPS));
	buffer = create_bitmap (SCREEN_W, SCREEN_H);
}

void initialize() {

	initializeAllegro();
	initializeFromFile();

	camera.target = entity[HAB];
	camera.reference = entity[EARTH];
	HUD.target = entity[EARTH];
	HUD.reference = entity[MARS];
}

void calculate() {

	for (it = entity.begin(); it != entity.end(); ++it) {
		(*it)->turn();
		(*it)->move();
	}

	for (itX = entity.begin(); itX != entity.end(); ++itX)
		for (itY = itX, ++itY; itY != entity.end(); ++itY) {
			(*itX)->gravitate (**itY);
			(*itX)->detectCollision (**itY);
		}

	if (camera.track == true)
		camera.shift();

	cycle--;
	cycleCounter++;
}

void drawDisplay() {

	HUD.drawGrid();

	for (it = entity.begin(); it != entity.end(); ++it)
		(*it)->draw((*it)->a(), (*it)->b(), camera.actualZoom());

	HUD.drawHUD();

	if (printDebug)
		debug();

	drawBuffer();

	fpsCounter++;
}

void cleanup() {

	destroy_bitmap (buffer);
	release_screen();

	for (it = entity.begin(); it != entity.end(); ++it)
		delete *it;
	entity.clear();
}

void physical_t::detectCollision (physical_t &targ) {


}

void physical_t::gravitate (physical_t &targ) { //calculates gravitational acceleration, calling and target entity, then accelerates them

	long double force = (G * mass * targ.mass) / (distance (targ.x, targ.y) * distance (targ.x, targ.y));
	long double theta = atan2f( -(y - targ.y), -(x - targ.x) );

	acc (force, theta);
	targ.acc (force, theta);
}

long double physical_t::Vcen (physical_t &targ) {


}

long double physical_t::Vtan (physical_t &targ) {


}

long double physical_t::thetaV() {

	return (atan2f (-Vy, Vx) );
}

long double physical_t::thetaToObject (physical_t &targ) {

	return (atan2f( -(y - targ.y), -(x - targ.x)) );
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
END_OF_MAIN();  //9 lines of code in the main function. Now that is pretty sweet.
