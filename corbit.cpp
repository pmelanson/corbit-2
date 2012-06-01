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
Each pixel at camera.actualzoomLevel() = 0 level is equivalent the distance light travels in a vacumn in 1/299,792,458th of a second (one metre)
See changelog.txt for changelog past 31/03/2012

*******************/

/*******************

       CONTROLS

    CAMERA CONTROLS

ARROW KEYS     NUMPAD +/-
move camera    zoom in/out


     SHIP CONTROLS

W/S
throttle up/down

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
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
using namespace std;

//globals
const unsigned short int screenWidth = 1280;    //my computer's resolution
const unsigned short int screenHeight = 980;    //my computer's resolution
bool printDebug = false;

BITMAP *buffer = NULL;
volatile unsigned short int timer = 0, cycle = 0;
unsigned long int frameRate = 60;   //used as the base in frameRate exponential calculations NOT ACTUAL FRAMERATE (see changeFrameRate())

const long double PI = 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844609550582231725359408128481117450284102701938521105559644622948954930381964428810975665933446128475648233786783165271201909145648566923460348610454326648213393607260249141273724587006606315588174881520920962829254091715364367892590360011330530548820466521384146951941511609433057270365759591953092186117381932611793105118548074462379962749567351885752724891227938183011949129833673362440656643086021394946395224737190702179860943702770539217176293176752384674818467669405132000568127145263560827785771342757789609173637178721468440901224953430146549585371050792279689258923542019956112129021960864034418159813629774771309960518707211349999998372978049951059731732816096318595024459455346908302642522308253344685035261931188171010003137838752886587533208381420617177669147303598253490428755468731159562863882353787593751957781857780532171226806613001927876611195909216420198938095257201065485863278865936153381827968230301952035301852968995773622599413891249721775283479131515574857242454150695950829533116861727855889075098381754637464939319255060400927701671139009848824012858361603563707660104710181942955596198946767;
const long double G = 6.673e-11;
const long double AU = 1495978707e2;

enum craft_enum {HAB, CRAFTMAX};
enum body_enum {SUN, MERCURY, VENUS, EARTH, MARS, JUPITER, SATURN, URANUS, NEPTUNE, PLUTO, BODYMAX};
enum navMode_enum {MAN, APP_TARG, DEPART_REF, CCW, CW, NAVMAX};


//prototypes
void nextFrame(), nextCycle();
void input(), drawBuffer(), debug(), drawGrid();
void detectCollision(), gravitate();
void changeFrameRate(short int step);

//beginning of class declarations
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

	viewpoint_t (const int _zoomMagnitude, const float _zoomStep, const float _maxZoom, const double _minZoom, const unsigned short int _panSpeed, const long double _zoomLevel) :
		zoomMagnitude (_zoomMagnitude), zoomStep (_zoomStep), maxZoom (_maxZoom), minZoom (_minZoom), panSpeed (_panSpeed), x (0), y (0), zoomLevel (_zoomLevel), track (true)
	{}
};

class display_t {

	const short unsigned int gridSpace;
	const unsigned short int lineSpace;

public:
	struct physical_t *target;
	struct physical_t *reference;
	void drawHUD();
	void drawGrid();

	display_t (const short unsigned int _gridSpace, const unsigned short int _lineSpace) :
		gridSpace (_gridSpace), lineSpace (_lineSpace)
	{}
};

struct physical_t { //stores data about any physical physical, such as mass and radius, acceleration, velocity, and angle from right

	const string name;    //I love C++ over C so much for this

	const long double mass;
	const long double radius;   //mass of physical, to be used in calculation F=ma, and radius of physical
	long double x, y; //the center of the physical
	long int a();
	long int b();
	float turnRadians;
	long double distance (long double x, long double y);
	void move();   //moves physical

	long double acc;    //total acceleration, no calculations are actually performed on this, just for printing purposes
	virtual long double accX (long double radians, long double acc); //the physical's acceleration (m/s/s) along the x axis
	virtual long double accY (long double radians, long double acc); //''
	long double Vx, Vy;   //the physical's speed (m/s) along each axis
	long double gravity (long double _x, long double _y, long double _mass);

	void turn();   //turns the physical
	long double turnRate; //rate at which the physical turns

	virtual void draw();    //draws physical
	const unsigned int fillColor;

	physical_t (string _name, long double _x, long double _y, long double _Vx, long double _Vy, long int _mass, unsigned int _radius, unsigned int _fillColor) :
		name (_name), x (_x), y (_y), Vx (_Vx), Vy (_Vy), mass (_mass), radius (_radius), fillColor (_fillColor), turnRate (0), turnRadians (0)
	{}
};

struct solarBody_t : physical_t {   //stores information about an astronomical body, in addition to information already stored by an physical

	const unsigned short int atmosphereDrag;
	const unsigned int atmosphereColor;
	const unsigned short int atmosphereHeight;

	void draw();

	solarBody_t (string _name, long double _x, long double _y, long double _Vx, long double _Vy, long int _mass, unsigned int _radius, unsigned int _fillColor, unsigned int _atmosphereColor, unsigned short int _atmosphereHeight) :
		physical_t (_name, _x, _y, _Vx, _Vy, _mass, _radius, _fillColor),
		atmosphereColor (_atmosphereColor), atmosphereHeight (_atmosphereHeight), atmosphereDrag (42)
	{}
};

struct ship_t : physical_t {  //stores information about a pilotable ship, in addition to information already stored by an physical

	void fireEngine();
	float engine;
	const unsigned int engineColor;
	const unsigned short int engineRadius;
	unsigned long int fuel;
	float burnRate;

	long double accX (long double radians, long double acc); //the physical's acceleration (m/s/s) along the x axis
	long double accY (long double radians, long double acc); //''

	struct autopilot_t {

		navMode_enum navmode;       //enum for which navmode to use, e.g. MAN, CCW
		string descriptor[NAVMAX];   //string describing the current nav mode

		autopilot_t () :
			navmode (MAN)
		{}
	} autopilot;

	virtual void draw();

	ship_t (const string _name, const long double _x, const long double _y, const long double _Vx, const long double _Vy, long int _mass, unsigned int _radius, unsigned int _fillColor, unsigned int _engineColor, unsigned short int _engineRadius) :
		physical_t (_name, _x, _y, _Vx, _Vy, _mass, _radius, _fillColor),
		engineColor (_engineColor), engineRadius (_engineRadius), engine (0), fuel (1e6), burnRate (10)
	{}
};

struct habitat_t : ship_t {

	void draw();

	habitat_t (const string _name, const long double _x, const long double _y, const long double _Vx, const long double _Vy, long int _mass, unsigned int _radius, unsigned int _fillColor, unsigned int _engineColor, unsigned short int _engineRadius) :
		ship_t (_name, _x, _y, _Vx, _Vy, _mass, _radius, _fillColor, _engineColor, _engineRadius)
	{}
};

viewpoint_t camera (  22,             0.01,       0.5,    0.8e-10, 10,         0);   //constructor initializes consts in the order they are declared, which is...
//                  zoomMagnitude   zoomStep    maxZoom minZoom panSpeed    zoomLevel

display_t HUD (   18,         15);    //constructor initializes consts in the order they are declared, which is...
//              gridSpace   lineSpace

vector <ship_t*> craft;
vector <solarBody_t*> body;


int main () {

	//looping variable initialization
	vector <ship_t*>::iterator spaceship;
	vector <solarBody_t*>::iterator rock;

	//allegro initializations
	allegro_init();
	install_keyboard();
	set_color_depth (desktop_color_depth());
	set_gfx_mode (GFX_AUTODETECT_WINDOWED, screenWidth, screenHeight, 0, 0);

	LOCK_VARIABLE (timer);
	LOCK_FUNCTION (timestep);
	changeFrameRate (0);
	buffer = create_bitmap (SCREEN_W, SCREEN_H);

	//file initialization
	ifstream datafile;
	datafile.open ("entities.txt");
	if (datafile.is_open())
		cout << "datafile open\n";
	if (datafile.good())
		cout << "datafile good\n";

	//data initializations
	string line = "";

	datafile.ignore (4096, '!');

	while (getline (datafile, line)) { //each loop through this reads in an entity

		string container = "", name = "";
		long double x = 1337, y = 1337, Vx = 0, Vy = 0;
		long double mass = 1337, radius = 1337, specialRadius = 413;
		unsigned short int R = 10, G = 11, B = 12;
		unsigned short int R2 = 20, G2 = 21, B2 = 22;
		unsigned int fillColor = 50, specialColor = 50;

		cout << endl << line;
		istringstream iss (line);
		iss >> container;


		if (getline (datafile, line)) { // was able to read a line
			istringstream iss (line);

			if (iss >> name); // was able to parse the data
			if (name == "")
				name = "Blank";
			cout << " " << name;
		}

		if (getline (datafile, line)) { // was able to read a line
			istringstream iss (line);

			if (iss >> x) // was able to parse the number
				x *= AU;
		}

		if (getline (datafile, line)) { // was able to read a line
			istringstream iss (line);

			if (iss >> y) // was able to parse the number
				y *= AU;
		}

		if (getline (datafile, line)) { // was able to read a line
			istringstream iss (line);

			if (iss >> Vx); // was able to parse the number
		}

		if (getline (datafile, line)) { // was able to read a line
			istringstream iss (line);

			if (iss >> Vy); // was able to parse the number
		}

		if (getline (datafile, line)) { // was able to read a line
			istringstream iss (line);

			if (iss >> mass); // was able to parse the number

			if (mass <= 0)
				mass = 1337;
		}

		if (getline (datafile, line)) { // was able to read a line
			istringstream iss (line);

			if (iss >> radius) // was able to parse the number
				radius *= 2;

			if (radius <= 0)
				radius = 1337;
		}

		if (getline (datafile, line)) {
			istringstream iss (line);

			if (iss >> R) {
				iss.ignore (2, ',');
				if (iss >> G) {
					iss.ignore (2, ',');
					if (iss >> B)
						fillColor = makecol (R, G, B);
				}
			}
		}

		if (getline (datafile, line)) {
			istringstream iss (line);

			if (iss >> R2) {
				iss.ignore (2, ',');
				if (iss >> G2) {
					iss.ignore (2, ',');
					if (iss >> B2)
						specialColor = makecol (R2, G2, B2);
				}
			}
		}

		if (getline (datafile, line)) { // was able to read a line
			istringstream iss (line);

			if (iss >> specialRadius); // was able to parse the number

			if (specialRadius <= 0)
				specialRadius = 1337;
		}

		if (container == "solarBody") {
			body.push_back (new solarBody_t (name, x, y, Vx, Vy, mass, radius, fillColor, specialColor, specialRadius) );

			cout << "\nBody initialized, with data of\nx = " << x << endl;
			cout << "y = " << y << endl;
			cout << "Vx = " << Vx << endl;
			cout << "Vy = " << Vy << endl;
			cout << "mass = " << mass << endl;
			cout << "radius = " << radius << endl;
			cout << "fillColor = " << R << ", " << G << ", " << B << ", " << endl;;
			cout << "atmosphereColor = " << R2 << ", " << G2 << ", " << B2 << ", " << endl;
			cout << "atmosphereHeight = " << specialRadius << endl;
		}

		if (container == "ship")
			if (name == "Habitat") {
				craft.push_back (new habitat_t (name, x, y, Vx, Vy, mass, radius, fillColor, specialColor, specialRadius) );
				cout << "\nHabitat initialized, with data of\nx = " << x << endl;
				cout << "y = " << y << endl;
				cout << "Vx = " << Vx << endl;
				cout << "Vy = " << Vy << endl;
				cout << "mass = " << mass << endl;
				cout << "radius = " << radius << endl;
				cout << "fillColor = " << R << ", " << G << ", " << B << ", " << endl;
				cout << "engine = " << 0 << endl;
				cout << "engineColor = " << R2 << ", " << G2 << ", " << B2 << ", " << endl;
				cout << "engineRadius = " << specialRadius << endl;
			}

		datafile.ignore (4096, '!');
	}

	for (spaceship = craft.begin(); spaceship != craft.end(); ++spaceship) {
		(*spaceship)->autopilot.descriptor[MAN] = "Manual";
		(*spaceship)->autopilot.descriptor[APP_TARG] = "App Targ";
		(*spaceship)->autopilot.descriptor[DEPART_REF] = "Depart Ref";
		(*spaceship)->autopilot.descriptor[CCW] = "CCW Prograde";
		(*spaceship)->autopilot.descriptor[CW] = "CW Retrograde";
	}

	camera.target = craft[HAB];
//	camera.target = body[EARTH];
	camera.reference = body[EARTH];
	HUD.target = body[EARTH];
	HUD.reference = body[MARS];

///PROGRAM STARTS HERE///
	while (!key[KEY_ESC]) {

		while (timer > 0) {

			craft[HAB]->acc = 0;

			input();

			gravitate();
			detectCollision();

			for (rock = body.begin(); rock != body.end(); ++rock)
				(*rock)->move();

			for (spaceship = craft.begin(); spaceship != craft.end(); ++spaceship) {
				(*spaceship)->turn();
				(*spaceship)->fireEngine();
				(*spaceship)->move();
			}

			camera.autoZoom();

			if (camera.track == true)
				camera.shift();

			timer--;

		}

		HUD.drawGrid();

		for (rock = body.begin(); rock != body.end(); ++rock)
			(*rock)->draw();

		for (spaceship = craft.begin(); spaceship != craft.end(); ++spaceship) {
			(*spaceship)->draw();
		}

		HUD.drawHUD();

		if (printDebug)
			debug();

		drawBuffer();
	}

//end of program
	destroy_bitmap (buffer);
	release_screen();

	for (rock = body.begin(); rock != body.end(); ++rock)
		delete *rock;
	body.clear();

	for (spaceship = craft.begin(); spaceship != craft.end(); ++spaceship)
		delete *spaceship;
	craft.clear();

	return 0;
}
END_OF_MAIN();

void nextFrame() {

	timer++;
}
END_OF_FUNCTION (nextFrame);

void drawBuffer () {

	textprintf_ex (buffer, font, 0, SCREEN_H - 10, makecol (255, 255, 255), -1, "Corbit v%d.%d%d.%d", AutoVersion::MAJOR, AutoVersion::MINOR, AutoVersion::REVISION, AutoVersion::BUILD);

	draw_sprite (buffer, screen, SCREEN_H, SCREEN_W); // Draw the buffer to the screen
	draw_sprite (screen, buffer, 0, 0);
	clear_bitmap (buffer); // Clear the contents of the buffer bitmap
}

void input () {

    if (key[KEY_Z])
        printDebug = !printDebug;

	if (key[KEY_A] && craft[HAB]->turnRate < 0.05)
		craft[HAB]->turnRate -= 0.005 * PI / 180;

	if (key[KEY_D] && craft[HAB]->turnRate < 0.05)
		craft[HAB]->turnRate += 0.005 * PI / 180;

	if (key[KEY_W])
		if (key_shifts & KB_SHIFT_FLAG)
			craft[HAB]->engine += 0.5;
		else
			craft[HAB]->engine += 0.1;

	if (key[KEY_S])
		if (key_shifts & KB_SHIFT_FLAG)
			craft[HAB]->engine -= 0.5;
		else
			craft[HAB]->engine -= 0.1;

	if (key[KEY_BACKSPACE]) {
		if (key_shifts & KB_SHIFT_FLAG) {   //all those if statements there just slow the turnRate gradually
			if (fabs (craft[HAB]->turnRate) < 0.005 * PI / 180)
				craft[HAB]->turnRate = 0;
			else if (craft[HAB]->turnRate > 0)
				craft[HAB]->turnRate -= 0.005 * PI / 180;
			else
				craft[HAB]->turnRate += 0.005 * PI / 180;
		} else
			craft[HAB]->engine = 0;
	}

	if (key[KEY_ENTER])
		craft[HAB]->engine = 100;

	if (key[KEY_LEFT])
		camera.panX (-1);

	if (key[KEY_RIGHT])
		camera.panX (1);

	if (key[KEY_UP])
		camera.panY (-1);

	if (key[KEY_DOWN])
		camera.panY (1);

	if (key[KEY_PLUS_PAD])
		if (key_shifts & KB_SHIFT_FLAG)
			changeFrameRate (1);
		else
			camera.zoom (1);

	if (key[KEY_MINUS_PAD])
		if (key_shifts & KB_SHIFT_FLAG)
			changeFrameRate (-1);
		else
			camera.zoom (-1);

	if (key[KEY_1])
		if (key_shifts & KB_SHIFT_FLAG)
			HUD.target = body[MERCURY];
		else if (key_shifts & KB_CTRL_FLAG)
			HUD.reference = body[MERCURY];
		else if (key_shifts & KB_ALT_FLAG)
			craft[HAB]->autopilot.navmode = MAN;
		else
			camera.target = body[MERCURY];

	if (key[KEY_2])
		if (key_shifts & KB_SHIFT_FLAG)
			HUD.target = body[VENUS];
		else if (key_shifts & KB_CTRL_FLAG)
			HUD.reference = body[VENUS];
		else if (key_shifts & KB_ALT_FLAG)
			craft[HAB]->autopilot.navmode = APP_TARG;
		else
			camera.target = body[VENUS];

	if (key[KEY_3])
		if (key_shifts & KB_SHIFT_FLAG)
			HUD.target = body[EARTH];
		else if (key_shifts & KB_CTRL_FLAG)
			HUD.reference = body[EARTH];
		else if (key_shifts & KB_ALT_FLAG)
			craft[HAB]->autopilot.navmode = DEPART_REF;
		else
			camera.target = body[EARTH];

	if (key[KEY_4])
		if (key_shifts & KB_SHIFT_FLAG)
			HUD.target = body[MARS];
		else if (key_shifts & KB_CTRL_FLAG)
			HUD.reference = body[MARS];
		else if (key_shifts & KB_ALT_FLAG)
			craft[HAB]->autopilot.navmode = CCW;
		else
			camera.target = body[MARS];

	if (key[KEY_5])
		if (key_shifts & KB_SHIFT_FLAG)
			HUD.target = body[JUPITER];
		else if (key_shifts & KB_CTRL_FLAG)
			HUD.reference = body[JUPITER];
		else if (key_shifts & KB_ALT_FLAG)
			craft[HAB]->autopilot.navmode = CW;
		else
			camera.target = body[JUPITER];

	if (key[KEY_6])
		if (key_shifts & KB_SHIFT_FLAG)
			HUD.target = body[SATURN];
		else if (key_shifts & KB_CTRL_FLAG)
			HUD.reference = body[SATURN];
		else if (key_shifts & KB_ALT_FLAG)
			craft[HAB]->autopilot.navmode = MAN;
		else
			camera.target = body[SATURN];

	if (key[KEY_7])
		if (key_shifts & KB_SHIFT_FLAG)
			HUD.target = body[URANUS];
		else if (key_shifts & KB_CTRL_FLAG)
			HUD.reference = body[URANUS];
		else if (key_shifts & KB_ALT_FLAG)
			craft[HAB]->autopilot.navmode = MAN;
		else
			camera.target = body[URANUS];

	if (key[KEY_8])
		if (key_shifts & KB_SHIFT_FLAG)
			HUD.target = body[NEPTUNE];
		else if (key_shifts & KB_CTRL_FLAG)
			HUD.reference = body[NEPTUNE];
		else if (key_shifts & KB_ALT_FLAG)
			craft[HAB]->autopilot.navmode = MAN;
		else
			camera.target = body[NEPTUNE];

	if (key[KEY_9])
		if (key_shifts & KB_SHIFT_FLAG)
			HUD.target = body[PLUTO];
		else if (key_shifts & KB_CTRL_FLAG)
			HUD.reference = body[PLUTO];
		else if (key_shifts & KB_ALT_FLAG)
			craft[HAB]->autopilot.navmode = MAN;
		else
			camera.target = body[PLUTO];

	if (key[KEY_0])
		camera.target = craft[HAB];
}

void changeFrameRate(short int step) {

	if (frameRate + step <= 4294967295 && frameRate + step > 0) {
		frameRate += step;
		install_int_ex (nextFrame, BPS_TO_TIMER (frameRate + 1) );
	}
}

void debug() {

	const unsigned short int spacing = 700;

	textprintf_ex (buffer, font, 0, 0 + spacing, makecol (200, 200, 200), -1, "DEBUG: hab.x: %Lf", craft[HAB]->x);
	textprintf_ex (buffer, font, 0, 10 + spacing, makecol (200, 200, 200), -1, "DEBUG: hab.y = %Lf", craft[HAB]->y );
	textprintf_ex (buffer, font, 0, 20 + spacing, makecol (200, 200, 200), -1, "DEBUG: hab a: %Li", craft[HAB]->a() );
	textprintf_ex (buffer, font, 0, 30 + spacing, makecol (200, 200, 200), -1, "DEBUG: hab b: %Li", craft[HAB]->b() );
	textprintf_ex (buffer, font, 0, 40 + spacing, makecol (200, 200, 200), -1, "DEBUG: Vx: %Lf", craft[HAB]->Vx);
	textprintf_ex (buffer, font, 0, 50 + spacing, makecol (200, 200, 200), -1, "DEBUG: Vy: %Lf", craft[HAB]->Vy);
	textprintf_ex (buffer, font, 0, 60 + spacing, makecol (200, 200, 200), -1, "DEBUG: Venus.a: %Li", body[VENUS]->a() );
	textprintf_ex (buffer, font, 0, 70 + spacing, makecol (200, 200, 200), -1, "DEBUG: Venus.b: %Li", body[VENUS]->b() );
	textprintf_ex (buffer, font, 0, 80 + spacing, makecol (200, 200, 200), -1, "DEBUG: Earth.a: %Li", body[EARTH]->a() );
	textprintf_ex (buffer, font, 0, 90 + spacing, makecol (200, 200, 200), -1, "DEBUG: Earth.b: %Li", body[EARTH]->b() );
	textprintf_ex (buffer, font, 0, 100 + spacing, makecol (200, 200, 200), -1, "DEBUG: Venus.x: %Lf", body[VENUS]->x);
	textprintf_ex (buffer, font, 0, 110 + spacing, makecol (200, 200, 200), -1, "DEBUG: Venus.y: %Lf", body[VENUS]->y);
	textprintf_ex (buffer, font, 0, 120 + spacing, makecol (200, 200, 200), -1, "DEBUG: arc tan: %Lf", atan2f (craft[HAB]->x - body[EARTH]->x, craft[HAB]->y - body[EARTH]->y) + PI * 0.5 );
	textprintf_ex (buffer, font, 0, 130 + spacing, makecol (200, 200, 200), -1, "DEBUG: Actual zoom: %Lf", camera.actualZoom() );
	textprintf_ex (buffer, font, 0, 140 + spacing, makecol (200, 200, 200), -1, "DEBUG: camera X: %Li", camera.x);
	textprintf_ex (buffer, font, 0, 150 + spacing, makecol (200, 200, 200), -1, "DEBUG: camera Y: %Li", camera.y);
	textprintf_ex (buffer, font, 0, 160 + spacing, makecol (200, 200, 200), -1, "DEBUG: turnRate: %Lf", craft[HAB]->turnRate);
	textprintf_ex (buffer, font, SCREEN_W/2, SCREEN_H/2, makecol (200, 200, 200), -1, "poop");
}

void physical_t::move() {

	x += Vx;
	y += Vy;
}

void ship_t::fireEngine() {

	if (fuel > 0) {
		acc += accX (turnRadians, (engine * 1000000) / frameRate );
		acc += accY (turnRadians, (engine * 1000000) / frameRate );
		fuel -= fabs (engine) / frameRate;
	}
}

void physical_t::turn () {

	turnRadians += turnRate;

	if (turnRadians < 0)
		turnRadians += 2 * PI;

	if (turnRadians > 2 * PI)
		turnRadians -= 2 * PI;
}

long double ship_t::accX (long double radians, long double acc) {

	Vx += ( (cos (radians) * acc) / (mass + fuel) ) / frameRate;
	return (( (sin (radians) * acc) / (mass + fuel) ) / frameRate);
}

long double ship_t::accY (long double radians, long double acc) {

	Vy += ( (sin (radians) * acc) / (mass + fuel) ) / frameRate;
	return (( (sin (radians) * acc) / (mass + fuel) ) / frameRate);
}

long double physical_t::accX (long double radians, long double acc) {

	Vx += ( (cos (radians) * acc) / mass ) / frameRate;
	return (( (cos (radians) * acc) / mass ) / frameRate);
}

long double physical_t::accY (long double radians, long double acc) {

	Vy += ( (sin (radians) * acc) / mass ) / frameRate;
	return (( (sin (radians) * acc) / mass ) / frameRate);
}

long double physical_t::distance (const long double _x, const long double _y) { //finds distance from physical to target

	return (sqrtf( ((x - _x) * (x - _x)) + ((y - _y) * (y - _y)) )); //finds the distance between two entities, using d = sqrt ( (x1 - x2)^2 + (y1 - y2) )
}

long double physical_t::gravity(long double _x, long double _y, long double _mass) {

	return (G * mass * _mass / distance (_x, _y) );    //G * mass1 * mass2 / r^2
}

long int physical_t::a() { //on-screen x position of physical

	return ( (x - camera.x) * camera.actualZoom() + SCREEN_W / 2);
}

long int physical_t::b() { //on-screen y position of physical

	return ( (y - camera.y) * camera.actualZoom() + SCREEN_H / 2);
}

void physical_t::draw() {

	circlefill (buffer, a(), b(), radius * camera.actualZoom(), fillColor); //draws the physical to the buffer
}

void solarBody_t::draw() {

	circlefill (buffer, a(), b(), camera.actualZoom() * (radius + atmosphereHeight), atmosphereColor);   //draws the atmosphere to the buffer

	circlefill (buffer, a(), b(), radius * camera.actualZoom(), fillColor); //draws the planet body to the buffer

	textprintf_ex (buffer, font, a(), b(), makecol (200, 200, 200), -1, "%s", name.c_str() );
}

void ship_t::draw() {

	circlefill (buffer, a(), b(), radius * camera.actualZoom(), fillColor); //draws the picture to the buffer
	line (buffer, a(), b(), //draws the 'engine'
		  a() + radius * cos (turnRadians) * camera.actualZoom(),
		  b() + radius * sin (turnRadians) * camera.actualZoom(),
		  engineColor);
}

void habitat_t::draw() {

	circlefill (buffer, a(), b(), radius * camera.actualZoom(), fillColor); //draws the hull to the buffer

	if (engine == 0) {

		circlefill (buffer, //draws the center 'engine'
					a() + (radius - engineRadius) * cos (turnRadians - (PI) ) * camera.actualZoom(),
					b() + (radius - engineRadius) * sin (turnRadians - (PI) ) * camera.actualZoom(),
					engineRadius * camera.actualZoom(),
					fillColor - 1052688);   //the inactive engine color is fillColor - hex(101010)
		circlefill (buffer, //draws the left 'engine'
					a() + radius * cos (turnRadians - (PI * .75) ) * camera.actualZoom(),
					b() + radius * sin (turnRadians - (PI * .75) ) * camera.actualZoom(),
					engineRadius * camera.actualZoom(),
					fillColor - 1052688);   //the inactive engine color is fillColor - hex(101010)
		circlefill (buffer, //draws the right 'engine'
					a() + radius * cos (turnRadians - (PI * 1.25) ) * camera.actualZoom(),
					b() + radius * sin (turnRadians - (PI * 1.25) ) * camera.actualZoom(),
					engineRadius * camera.actualZoom(),
					fillColor - 1052688);   //the inactive engine color is fillColor - hex(101010)
	}

	else {

		circlefill (buffer, //draws the center 'engine'
					a() + (radius - engineRadius) * cos (turnRadians - (PI) ) * camera.actualZoom(),
					b() + (radius - engineRadius) * sin (turnRadians - (PI) ) * camera.actualZoom(),
					engineRadius * camera.actualZoom(),
					engineColor);
		circlefill (buffer, //draws the left 'engine'
					a() + radius * cos (turnRadians - (PI * .75) ) * camera.actualZoom(),
					b() + radius * sin (turnRadians - (PI * .75) ) * camera.actualZoom(),
					engineRadius * camera.actualZoom(),
					engineColor);
		circlefill (buffer, //draws the right 'engine'
					a() + radius * cos (turnRadians - (PI * 1.25) ) * camera.actualZoom(),
					b() + radius * sin (turnRadians - (PI * 1.25) ) * camera.actualZoom(),
					engineRadius * camera.actualZoom(),
					engineColor);
	}
}

void display_t::drawGrid () {  //draws a grid to the screen, later on I will be making gravity distort it

	unsigned short int x, y;

	for (x = 0; x < SCREEN_W; x += gridSpace)
		for (y = 0; y < SCREEN_H; y += gridSpace)
			putpixel (buffer, x, y, makecol (255, 255, 255));
}

void display_t::drawHUD () {

	float thetaV = atan2f (-craft[HAB]->Vy, craft[HAB]->Vx);
	float thetaTarg = PI;

	rectfill (buffer, 0, 0, 300, 35 * lineSpace, 0);
	rect (buffer, -1, -1, 300, 35 * lineSpace, makecol (255, 255, 255));

	textprintf_ex (buffer, font, lineSpace, 1 * lineSpace, makecol (200, 200, 200), -1, "Orbit V (m/s):"), textprintf_ex (buffer, font, 200, 1 * lineSpace, makecol (255, 255, 255), -1, "1337");
	textprintf_ex (buffer, font, lineSpace, 2 * lineSpace, makecol (200, 200, 200), -1, "Hab/Targ V diff:"), textprintf_ex (buffer, font, 200, 2 * lineSpace, makecol (255, 255, 255), -1, "%-10.7Lg",
			(craft[HAB]->Vx + craft[HAB]->Vy) - (target->Vx + target->Vy));
	textprintf_ex (buffer, font, lineSpace, 3 * lineSpace, makecol (200, 200, 200), -1, "Centrifugal V (m/s):");
	textprintf_ex (buffer, font, lineSpace, 4 * lineSpace, makecol (200, 200, 200), -1, "Tangential V (m/s):");
	textprintf_ex (buffer, font, lineSpace, 6 * lineSpace, makecol (200, 200, 200), -1, "Fuel (kg):"), textprintf_ex (buffer, font, 200, 6 * lineSpace, makecol (255, 255, 255), -1, "%li", craft[HAB]->fuel);
	textprintf_ex (buffer, font, lineSpace, 7 * lineSpace, makecol (200, 200, 200), -1, "Engines (kg/s):"), textprintf_ex (buffer, font, 200, 7 * lineSpace, makecol (255, 255, 255), -1, "%-10.1f", craft[HAB]->engine);
	textprintf_ex (buffer, font, lineSpace, 8 * lineSpace, makecol (200, 200, 200), -1, "Acc (m/s/s):"), textprintf_ex (buffer, font, 200, 8 * lineSpace, makecol (255, 255, 255), -1, "%-10.5Lf", craft[HAB]->acc);
	textprintf_ex (buffer, font, lineSpace, 10 * lineSpace, makecol (200, 200, 200), -1, "Altitude (m):"), textprintf_ex (buffer, font, 200, 10 * lineSpace, makecol (255, 255, 255), -1, "%-10.5Lg", craft[HAB]->distance (target->x, target->y));
	textprintf_ex (buffer, font, lineSpace, 11 * lineSpace, makecol (200, 200, 200), -1, "Pitch (radians):");
	textprintf_ex (buffer, font, lineSpace, 12 * lineSpace, makecol (200, 200, 200), -1, "Stopping Acc:"), textprintf_ex (buffer, font, 200, 12 * lineSpace, makecol (255, 255, 255), -1, "%-10.5Lf",
			craft[HAB]->distance (target->x, target->y) / (2 * craft[HAB]->distance (target->x, target->y) - target->radius) * cos (thetaV - thetaTarg));
	textprintf_ex (buffer, font, lineSpace, 13 * lineSpace, makecol (200, 200, 200), -1, "Periapsis (m):");
	textprintf_ex (buffer, font, lineSpace, 14 * lineSpace, makecol (200, 200, 200), -1, "Apoapsis (m):");

	circlefill (buffer, 140, 22 * lineSpace, craft[HAB]->radius, craft[HAB]->fillColor); //draws the habitat onto the HUD at a constant size, along with velocity vector and position related to reference

	if (craft[HAB]->engine == 0) {

		circlefill (buffer, //draws the center 'engine'
					140 + (craft[HAB]->radius - craft[HAB]->engineRadius) * cos (craft[HAB]->turnRadians - PI ),
					22 * lineSpace + (craft[HAB]->radius - craft[HAB]->engineRadius) * sin (craft[HAB]->turnRadians - PI ),
					craft[HAB]->engineRadius,
					craft[HAB]->fillColor - 1052688);   //the inactive engine color is fillColor - hex(101010)
		circlefill (buffer, //draws the left 'engine'
					140 + craft[HAB]->radius * cos (craft[HAB]->turnRadians - (PI * .75) ),
					22 * lineSpace + craft[HAB]->radius * sin (craft[HAB]->turnRadians - (PI * .75) ),
					craft[HAB]->engineRadius,
					craft[HAB]->fillColor - 1052688);   //the inactive engine color is fillColor - hex(101010)
		circlefill (buffer, //draws the right 'engine'
					140 + craft[HAB]->radius * cos (craft[HAB]->turnRadians - (PI * 1.25) ),
					22 * lineSpace + craft[HAB]->radius * sin (craft[HAB]->turnRadians - (PI * 1.25) ),
					craft[HAB]->engineRadius,
					craft[HAB]->fillColor - 1052688);   //the inactive engine color is fillColor - hex(101010)
	}

	else {

		circlefill (buffer, //draws the center 'engine'
					140 + (craft[HAB]->radius - craft[HAB]->engineRadius) * cos (craft[HAB]->turnRadians - (PI) ),
					22 * lineSpace + (craft[HAB]->radius - craft[HAB]->engineRadius) * sin (craft[HAB]->turnRadians - (PI) ),
					craft[HAB]->engineRadius,
					craft[HAB]->engineColor);
		circlefill (buffer, //draws the left 'engine'
					140 + craft[HAB]->radius * cos (craft[HAB]->turnRadians - (PI * .75) ),
					22 * lineSpace + craft[HAB]->radius * sin (craft[HAB]->turnRadians - (PI * .75) ),
					craft[HAB]->engineRadius,
					craft[HAB]->engineColor);
		circlefill (buffer, //draws the right 'engine'
					140 + craft[HAB]->radius * cos (craft[HAB]->turnRadians - (PI * 1.25) ),
					22 * lineSpace + craft[HAB]->radius * sin (craft[HAB]->turnRadians - (PI * 1.25) ),
					craft[HAB]->engineRadius,
					craft[HAB]->engineColor);
	}

	line (buffer, 140, 22 * lineSpace, (140) + (craft[HAB]->radius * 1.2) * cos (thetaV), (22 * lineSpace) + (craft[HAB]->radius * 1.2) * sin (thetaV), makecol (255, 0, 0));
//	textprintf_ex (buffer, font, (140) + (craft[HAB]->radius * 2) * cos (thetaTarg), (22 * lineSpace) + (craft[HAB]->radius * 2) * sin (thetaTarg), makecol (255, 255, 255), -1, "%s", target->name.c_str());
	line (buffer, 140, 22 * lineSpace, (140) + (craft[HAB]->radius * 1.2) * cos (thetaTarg), (22 * lineSpace) + (craft[HAB]->radius * 1.2) * sin (thetaTarg), makecol (0, 0, 255));

	textprintf_ex (buffer, font, lineSpace, 30 * lineSpace, makecol (200, 200, 200), -1, "Center:"), textprintf_ex (buffer, font, 200, 30 * lineSpace, makecol (255, 255, 255), -1, "%s", camera.target->name.c_str());
	textprintf_ex (buffer, font, lineSpace, 31 * lineSpace, makecol (200, 200, 200), -1, "Target:"), textprintf_ex (buffer, font, 200, 31 * lineSpace, makecol (255, 255, 255), -1, "%s", target->name.c_str());
	textprintf_ex (buffer, font, lineSpace, 32 * lineSpace, makecol (200, 200, 200), -1, "Reference:"), textprintf_ex (buffer, font, 200, 32 * lineSpace, makecol (255, 255, 255), -1, "%s", reference->name.c_str());
	textprintf_ex (buffer, font, lineSpace, 33 * lineSpace, makecol (200, 200, 200), -1, "Autopilot:"), textprintf_ex (buffer, font, 200, 33 * lineSpace, makecol (255, 255, 255), -1, "%s", craft[HAB]->autopilot.descriptor[craft[HAB]->autopilot.navmode].c_str());
	textprintf_ex (buffer, font, lineSpace, 34 * lineSpace, makecol (200, 200, 200), -1, "FPS:"), textprintf_ex (buffer, font, 200, 34 * lineSpace, makecol (255, 255, 255), -1, "%d", frameRate);
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

void viewpoint_t::autoZoom() {

}

void detectCollision () {

}

void gravitate () { //calculates gravitational forces, and accelerates, between two entities

}

void iterate (void transform() ) {

}
