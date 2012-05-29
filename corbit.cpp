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
The player's ship is called the "Hab"
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
<<<<<<< HEAD
//const unsigned short int screenWidth = 1280;    //my computer's resolution
const unsigned short int screenWidth = 1144;    //school resolution
//const unsigned short int screenHeight = 980;    //my computer's resolution
const unsigned short int screenHeight = 830;    //school resolution
=======
const unsigned short int screenWidth = 1280;    //my computer's resolution
//const unsigned short int screenWidth = 1144;    //school resolution
const unsigned short int screenHeight = 980;    //my computer's resolution
//const unsigned short int screenHeight = 830;    //school resolution
>>>>>>> b5cb9bd94248f2b566df5745d7f1d373349e0623

BITMAP *buffer = NULL;
volatile unsigned short int timer = 0;
unsigned short int frameRate = 60;   //used as the base in frameRate exponential calculations NOT ACTUAL FRAMERATE (see changeFrameRate())

const long double PI = 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844609550582231725359408128481117450284102701938521105559644622948954930381964428810975665933446128475648233786783165271201909145648566923460348610454326648213393607260249141273724587006606315588174881520920962829254091715364367892590360011330530548820466521384146951941511609433057270365759591953092186117381932611793105118548074462379962749567351885752724891227938183011949129833673362440656643086021394946395224737190702179860943702770539217176293176752384674818467669405132000568127145263560827785771342757789609173637178721468440901224953430146549585371050792279689258923542019956112129021960864034418159813629774771309960518707211349999998372978049951059731732816096318595024459455346908302642522308253344685035261931188171010003137838752886587533208381420617177669147303598253490428755468731159562863882353787593751957781857780532171226806613001927876611195909216420198938095257201065485863278865936153381827968230301952035301852968995773622599413891249721775283479131515574857242454150695950829533116861727855889075098381754637464939319255060400927701671139009848824012858361603563707660104710181942955596198946767;
const long double G = 6.673e-11;
const long double AU = 1495978707e2;

enum playerShips {HAB, CRAFTMAX};
enum solarSystem {SUN, MERCURY, VENUS, EARTH, MARS, JUPITER, SATURN, URANUS, NEPTUNE, PLUTO, BODYMAX};


//prototypes
void nextFrame();
void input();
void drawBuffer();
void debug();
void gravitate();
void drawGrid();
void detectCollision();
void changeFrameRate(short unsigned int step);

//beginning of class declarations
class viewpoint {

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

	struct physical *target;
	struct physical *reference;
	bool track;
	void shift();
	void autoZoom();

	void zoom (short int direction);
	void panX (short int direction);
	void panY (short int direction);

	viewpoint (const int _zoomMagnitude, const float _zoomStep, const float _maxZoom, const double _minZoom, const unsigned short int _panSpeed, const long double _zoomLevel) :
		zoomMagnitude (_zoomMagnitude), zoomStep (_zoomStep), maxZoom (_maxZoom), minZoom (_minZoom), panSpeed (_panSpeed), x (0), y (0), zoomLevel (_zoomLevel), track (true)
	{}
};

struct display {

	struct physical *target;
	struct physical *reference;

	const short unsigned int gridSpace;

	void drawHUD();
	void drawGrid();

	const unsigned short int lineSpace;

	display (const short unsigned int _gridSpace, const unsigned short int _lineSpace) :
		gridSpace (_gridSpace), lineSpace (_lineSpace)
	{}
};

struct physical { //stores data about any physical physical, such as mass and radius, acceleration, velocity, and angle from right

	string name;    //I love C++ over C so much for this

	const long double mass;
	const long double radius;   //mass of physical, to be used in calculation F=ma, and radius of physical
	long double x, y; //the center of the physical
	long int a();
	long int b();
	float turnRadians;
	long int distance (long double x, long double y);
	void move();   //moves physical

	long double accX (long double radians, long double acc); //the physical's acceleration (m/s/s) along the x axis
	long double accY (long double radians, long double acc); //''
	long double Vx, Vy;   //the physical's speed (m/s) along each axis
	long double gravity(const int _x, const int _y, const int _mass);

	void turn();   //turns the physical
	long double turnRate; //rate at which the physical turns

	virtual void draw();    //draws physical
	const unsigned int fillColor;

	physical (const string _name, const long double _x, const long double _y, const long double _Vx, const long double _Vy, long int _mass, unsigned int _radius, unsigned int _fillColor) :
		name (_name), x (_x), y (_y), Vx (_Vx), Vy (_Vy), mass (_mass), radius (_radius), fillColor (_fillColor)
	{}
};

struct solarBody : physical {   //stores information about an astronomical body, in addition to information already stored by an physical

	const unsigned short int atmosphereDrag;
	const unsigned int atmosphereColor;
	const unsigned short int atmosphereHeight;

	void draw();

	solarBody (const string _name, const long double _x, const long double _y, const long double _Vx, const long double _Vy, long int _mass, unsigned int _radius, unsigned int _fillColor, unsigned int _atmosphereColor, unsigned short int _atmosphereHeight) :
		physical (_name, _x, _y, _Vx, _Vy, _mass, _radius, _fillColor),
		atmosphereColor (_atmosphereColor), atmosphereHeight (_atmosphereHeight), atmosphereDrag (42)
	{}
};

struct ship : physical {  //stores information about a pilotable ship, in addition to information already stored by an physical

	void fireEngine();
	float engine;
	const unsigned int engineColor;
	const unsigned short int engineRadius;

	virtual void draw();

	ship (const string _name, const long double _x, const long double _y, const long double _Vx, const long double _Vy, long int _mass, unsigned int _radius, unsigned int _fillColor, unsigned int _engineColor, unsigned short int _engineRadius) :
		physical (_name, _x, _y, _Vx, _Vy, _mass, _radius, _fillColor),
		engine (0), engineColor (_engineColor), engineRadius (_engineRadius)
	{}
};

struct habitat : ship {

	void draw();

	habitat (const string _name, const long double _x, const long double _y, const long double _Vx, const long double _Vy, long int _mass, unsigned int _radius, unsigned int _fillColor, unsigned int _engineColor, unsigned short int _engineRadius) :
		ship (_name, _x, _y, _Vx, _Vy, _mass, _radius, _fillColor, _engineColor, _engineRadius)
	{}
};

<<<<<<< HEAD
viewpoint camera (  22,             0.01,       0.5,    1e-11, 10,         0);   //constructor initializes consts in the order they are declared, which is...
=======
viewpoint camera (  22,             0.01,       0.5,    0.8e-10, 10,         0);   //constructor initializes consts in the order they are declared, which is...
>>>>>>> b5cb9bd94248f2b566df5745d7f1d373349e0623
//                  zoomMagnitude   zoomStep    maxZoom minZoom panSpeed    zoomLevel

display HUD (   18,         15);    //constructor initializes consts in the order they are declared, which is...
//              gridSpace   lineSpace

vector <ship*> craft;
vector <solarBody*> body;


int main () {

	//looping variable initialization
	vector <ship*>::iterator spaceship;
	vector <solarBody*>::iterator rock;

	//allegro initializations
	allegro_init();
	install_keyboard();
	set_color_depth (desktop_color_depth() );
	set_gfx_mode (GFX_AUTODETECT_WINDOWED, screenWidth, screenHeight, 0, 0);

	LOCK_VARIABLE (timer);
	LOCK_FUNCTION (timestep);
//	install_int_ex (nextFrame, BPS_TO_TIMER (120) );
    changeFrameRate (0);
	buffer = create_bitmap (SCREEN_W, SCREEN_H);

	//file initialization
	ifstream datafile;
	datafile.open ("entities.txt", ios::in);
	if (datafile.is_open())
		cout << "datafile open\n";
	if (datafile.good())
		cout << "datafile good\n";

	//data initializations
	string line;
	unsigned short int R = 10, G = 11, B = 12, R2 = 20, G2 = 21, B2 = 22;
	string container = "None", name = "Default";
    long double x = 1337, y = 1337, Vx = 1, Vy = 1, mass = 1337, radius = 1337, specialRadius = 42;
    unsigned int fillColor = makecol (100, 100, 100), specialColor = makecol (100, 100, 100);

	datafile.ignore (4096, '!');
	istringstream iss (line);

	while (getline (datafile, line)) { //each loop through this reads in an entity

		cout << endl;
		cout << endl << line;
		istringstream iss (line);
		iss >> container;


		if (getline (datafile, line)) { // was able to read a line
			istringstream iss (line);

			if (iss >> name); // was able to parse the data
			cout << " " << name;
		}

		if (getline (datafile, line)) { // was able to read a line
			istringstream iss (line);

			if (iss >> x) // was able to parse the number
				x *= AU;
			if (x == 0)
				x = 1;
		}

		if (getline (datafile, line)) { // was able to read a line
			istringstream iss (line);

			if (iss >> y) // was able to parse the number
				y *= AU;

			if (y == 0)
				y = 1;
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
		}

		if (getline (datafile, line)) { // was able to read a line
			istringstream iss (line);

			if (iss >> radius) // was able to parse the number
				radius *= 2;
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
		}

		if (container == "solarBody") {
			body.push_back (new solarBody (name, x, y, Vx, Vy, mass, radius, fillColor, specialColor, specialRadius) );

			craft.push_back (new habitat (name, x, y, Vx, Vy, mass, radius, fillColor, specialColor, specialRadius) );
				cout << endl << name << " initialized, with data of" << endl;
				cout << "x = " << x << endl
				<< "y = " << y << endl
				<< "Vx = " << Vx << endl
				<< "Vy = " << Vy << endl
				<< "mass = " << mass << endl
				<< "radius = " << radius << endl
				<< "fillColor = " << R << ", " << G << ", " << B << ", " << endl
				<< "atmosphereDrag = " << 0 << endl
				<< "atmosphereColor = " << R2 << ", " << G2 << ", " << B2 << ", " << endl
				<< "atmosphereHeight = " << specialRadius << endl;
            }

		else if (container == "ship")
			if (name == "Habitat") {
				craft.push_back (new habitat (name, x, y, Vx, Vy, mass, radius, fillColor, specialColor, specialRadius) );
				cout << endl << name << " initialized, with data of" << endl;
				cout << "x = " << x << endl
				<< "y = " << y << endl
				<< "Vx = " << Vx << endl
				<< "Vy = " << Vy << endl
				<< "mass = " << mass << endl
				<< "radius = " << radius << endl
				<< "fillColor = " << R << ", " << G << ", " << B << ", " << endl
				<< "engine = " << 0 << endl
				<< "engineColor = " << R2 << ", " << G2 << ", " << B2 << ", " << endl
				<< "engineRadius = " << specialRadius << endl;
			}

        else if (container == "None") {
            craft.push_back (new habitat (name, x, y, Vx, Vy, mass, radius, fillColor, specialColor, specialRadius) );
				cout << endl << name << " initialized, with data of" << endl;
				cout << "x = " << x << endl
				<< "y = " << y << endl
				<< "Vx = " << Vx << endl
				<< "Vy = " << Vy << endl
				<< "mass = " << mass << endl
				<< "radius = " << radius << endl
				<< "fillColor = " << R << ", " << G << ", " << B << ", " << endl
				<< "special = " << 0 << endl
				<< "specialColor = " << R2 << ", " << G2 << ", " << B2 << ", " << endl
				<< "specialRadius = " << specialRadius << endl;
        }

		datafile.ignore (4096, '!');
	}

<<<<<<< HEAD
	datafile.close();

	craft[HAB]->x = body[EARTH]->x + body[EARTH]->radius + 10000;

=======
>>>>>>> b5cb9bd94248f2b566df5745d7f1d373349e0623
	camera.target = craft[HAB];
//	camera.target = body[EARTH];
	camera.reference = body[EARTH];
    HUD.target = body[EARTH];
    HUD.reference = body[MARS];

///PROGRAM STARTS HERE///
	while (!key[KEY_ESC]) {

		while (timer > 0) {

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

		debug();

		drawBuffer();

	}

//end of program
	destroy_bitmap (buffer);
	release_screen();

    datafile.close();

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

	if (key[KEY_A])
		craft[HAB]->turnRate -= 0.1 * PI / 180;

	if (key[KEY_D])
		craft[HAB]->turnRate += 0.1 * PI / 180;

	if (key[KEY_W])
		craft[HAB]->engine ++;

	if (key[KEY_S])
		craft[HAB]->engine --;

	if (key[KEY_BACKSPACE]) {
		if (key[KEY_LSHIFT] || key[KEY_RSHIFT])
			craft[HAB]->turnRate = 0;
		else
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
		if (key[KEY_LSHIFT] || key[KEY_RSHIFT])
			changeFrameRate (1);
		else
			camera.zoom (1);

	if (key[KEY_MINUS_PAD])
		if (key[KEY_LSHIFT] || key[KEY_RSHIFT])
			changeFrameRate (-1);
		else
			camera.zoom (-1);

	if (key[KEY_TAB])
		camera.track = !camera.track;

    if (key[KEY_1])
        camera.target = body[MERCURY];

    if (key[KEY_2])
        camera.target = body[VENUS];

    if (key[KEY_3])
        camera.target = body[EARTH];

    if (key[KEY_4])
        camera.target = body[MARS];

    if (key[KEY_5])
        camera.target = body[JUPITER];

    if (key[KEY_6])
        camera.target = body[SATURN];

    if (key[KEY_7])
        camera.target = body[NEPTUNE];

    if (key[KEY_8])
        camera.target = body[URANUS];

    if (key[KEY_9])
        camera.target = body[PLUTO];
<<<<<<< HEAD

    if (key[KEY_0])
        camera.target = craft[HAB];
=======
>>>>>>> b5cb9bd94248f2b566df5745d7f1d373349e0623
}

void changeFrameRate(short unsigned int step) {

    frameRate += step;
	install_int_ex (nextFrame, BPS_TO_TIMER (frameRate + 1) );
}

void debug() {

	const unsigned short int spacing = 300;

	textprintf_ex (buffer, font, SCREEN_W - spacing, 0, makecol (255, 255, 255), -1, "DEBUG: hab.x: %Lf", craft[HAB]->x);
	textprintf_ex (buffer, font, SCREEN_W - spacing, 10, makecol (255, 255, 255), -1, "DEBUG: hab.y = %Lf", craft[HAB]->y );
	textprintf_ex (buffer, font, SCREEN_W - spacing, 20, makecol (255, 255, 255), -1, "DEBUG: hab a: %Li", craft[HAB]->a() );
	textprintf_ex (buffer, font, SCREEN_W - spacing, 30, makecol (255, 255, 255), -1, "DEBUG: hab b: %Li", craft[HAB]->b() );
	textprintf_ex (buffer, font, SCREEN_W - spacing, 40, makecol (255, 255, 255), -1, "DEBUG: Vx: %Lf", craft[HAB]->Vx);
	textprintf_ex (buffer, font, SCREEN_W - spacing, 50, makecol (255, 255, 255), -1, "DEBUG: Vy: %Lf", craft[HAB]->Vy);
	textprintf_ex (buffer, font, SCREEN_W - spacing, 60, makecol (255, 255, 255), -1, "DEBUG: Venus.a: %Li", body[VENUS]->a() );
	textprintf_ex (buffer, font, SCREEN_W - spacing, 70, makecol (255, 255, 255), -1, "DEBUG: Venus.b: %Li", body[VENUS]->b() );
	textprintf_ex (buffer, font, SCREEN_W - spacing, 80, makecol (255, 255, 255), -1, "DEBUG: Earth.a: %Li", body[EARTH]->a() );
	textprintf_ex (buffer, font, SCREEN_W - spacing, 90, makecol (255, 255, 255), -1, "DEBUG: Earth.b: %Li", body[EARTH]->b() );
	textprintf_ex (buffer, font, SCREEN_W - spacing, 100, makecol (255, 255, 255), -1, "DEBUG: Venus.x: %Lf", body[VENUS]->x);
	textprintf_ex (buffer, font, SCREEN_W - spacing, 110, makecol (255, 255, 255), -1, "DEBUG: Venus.y: %Lf", body[VENUS]->y);
	textprintf_ex (buffer, font, SCREEN_W - spacing, 120, makecol (255, 255, 255), -1, "DEBUG: arc tan: %Lf", atan2f (craft[HAB]->x - body[EARTH]->x, craft[HAB]->y - body[EARTH]->y) + PI * 0.5 );
	textprintf_ex (buffer, font, SCREEN_W - spacing, 130, makecol (255, 255, 255), -1, "DEBUG: Actual zoom: %Lf", camera.actualZoom() );
	textprintf_ex (buffer, font, SCREEN_W - spacing, 140, makecol (255, 255, 255), -1, "DEBUG: camera X: %Li", camera.x);
	textprintf_ex (buffer, font, SCREEN_W - spacing, 150, makecol (255, 255, 255), -1, "DEBUG: camera Y: %Li", camera.y);
}

void physical::move() {

	x += Vx;
	y += Vy;
}

void ship::fireEngine() {

	accX (turnRadians, engine);
	accY (turnRadians, engine);
}

void physical::turn () {

	turnRadians += turnRate;

	if (turnRadians < 0)
		turnRadians += 2 * PI;

	if (turnRadians > 2 * PI)
		turnRadians -= 2 * PI;
}

long double physical::accX (long double radians, long double acc) {

	Vx += cos (radians) * (acc * 100) / mass;
}

long double physical::accY (long double radians, long double acc) {

	Vy += sin (radians) * (acc * 100) / mass;
}

long int physical::distance (long double targetX, long double targetY) { //finds distance from physical to target

	return (sqrtf ( ((targetX - x) * (targetX - x)) + ((targetY - y) * (targetY - y)) )); //finds the distance between two entities, using d = sqrt ( (x1 - x2)^2 + (y1 - y2) )
}

long double physical::gravity(const int _x, const int _y, const int _mass) {

	return (G * mass * _mass / ((x - _x) * (x - _x) + (y - _y) * (y - _y)) );    //G * mass1 * mass2 / r^2

}

long int physical::a() { //on-screen x position of physical

	return ( (x - camera.x) * camera.actualZoom() + SCREEN_W / 2);
}

long int physical::b() { //on-screen y position of physical

	return ( (y - camera.y) * camera.actualZoom() + SCREEN_H / 2);
}

void physical::draw() {

	circlefill (buffer, a(), b(), radius * camera.actualZoom(), fillColor); //draws the physical to the buffer
}

void solarBody::draw() {

	circlefill (buffer, a(), b(), camera.actualZoom() * (radius + atmosphereHeight), atmosphereColor);   //draws the atmosphere to the buffer

//	circlefill (buffer, a(), b(), radius * camera.actualZoom(), fillColor); //draws the physical to the buffer

	textprintf_ex (buffer, font, a(), b(), makecol (255, 255, 255), -1, "%s", name.c_str() );
}

void ship::draw() {

	circlefill (buffer, a(), b(), radius * camera.actualZoom(), fillColor); //draws the picture to the buffer
	line (buffer, a(), b(), //draws the 'engine'
		  a() + radius * cos (turnRadians) * camera.actualZoom(),
		  b() + radius * sin (turnRadians) * camera.actualZoom(),
		  engineColor);
}

void habitat::draw() {

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

void display::drawGrid () {  //draws a grid to the screen, later on I will be making gravity distort it

	unsigned short int x, y;

	for (x = 0; x < SCREEN_W; x += gridSpace)
		for (y = 0; y < SCREEN_H; y += gridSpace)
			putpixel (buffer, x, y, makecol (255, 255, 255));
}

void display::drawHUD () {

<<<<<<< HEAD
	/*rectfill (buffer, 0, 0, 300, 35 * lineSpace, 0);
=======
	rectfill (buffer, 0, 0, 300, 35 * lineSpace, 0);
>>>>>>> b5cb9bd94248f2b566df5745d7f1d373349e0623
	rect (buffer, -1, -1, 300, 35 * lineSpace, makecol (255, 255, 255));

	textprintf_ex (buffer, font, lineSpace, 1 * lineSpace, makecol (200, 200, 200), -1, "Orbiting Velocity:"), textprintf_ex (buffer, font, 220, 1 * lineSpace, makecol (200, 200, 200), -1, "1337");
	textprintf_ex (buffer, font, lineSpace, 2 * lineSpace, makecol (200, 200, 200), -1, "Habitat/Target V diff:"), textprintf_ex (buffer, font, 220, 2 * lineSpace, makecol (200, 200, 200), -1, "%Lf", (fabs (craft[HAB]->Vx) + fabs (craft[HAB]->Vy)) - (fabs (body[EARTH]->Vx) + fabs (body[EARTH]->Vy)) );
	textprintf_ex (buffer, font, lineSpace, 3 * lineSpace, makecol (200, 200, 200), -1, "Centrifugal Velocity:");
	textprintf_ex (buffer, font, lineSpace, 4 * lineSpace, makecol (200, 200, 200), -1, "Tangential Velocity:");
	textprintf_ex (buffer, font, lineSpace, 6 * lineSpace, makecol (200, 200, 200), -1, "Fuel:");
	textprintf_ex (buffer, font, lineSpace, 7 * lineSpace, makecol (200, 200, 200), -1, "Engines:"), textprintf_ex (buffer, font, 220, 7 * lineSpace, makecol (200, 200, 200), -1, "%.1f", craft[HAB]->engine);
	textprintf_ex (buffer, font, lineSpace, 8 * lineSpace, makecol (200, 200, 200), -1, "Acceleration:");
	textprintf_ex (buffer, font, lineSpace, 10 * lineSpace, makecol (200, 200, 200), -1, "Distance/Altitude:");
	textprintf_ex (buffer, font, lineSpace, 11 * lineSpace, makecol (200, 200, 200), -1, "Pitch Angle:");
	textprintf_ex (buffer, font, lineSpace, 12 * lineSpace, makecol (200, 200, 200), -1, "Stopping Acceleration:");
	textprintf_ex (buffer, font, lineSpace, 13 * lineSpace, makecol (200, 200, 200), -1, "Periapsis:");
	textprintf_ex (buffer, font, lineSpace, 14 * lineSpace, makecol (200, 200, 200), -1, "Apoapsis:");

	circlefill (buffer, 100, 22 * lineSpace, craft[HAB]->radius, craft[HAB]->fillColor); //draws the habitat onto the HUD at a constant size, along with velocity vector and position related to reference

	if (craft[HAB]->engine == 0) {

		circlefill (buffer, //draws the center 'engine'
					100 + (craft[HAB]->radius - craft[HAB]->engineRadius) * cos (craft[HAB]->turnRadians - PI ),
					22 * lineSpace + (craft[HAB]->radius - craft[HAB]->engineRadius) * sin (craft[HAB]->turnRadians - PI ),
					craft[HAB]->engineRadius,
					craft[HAB]->fillColor - 1052688);   //the inactive engine color is fillColor - hex(101010)
		circlefill (buffer, //draws the left 'engine'
					100 + craft[HAB]->radius * cos (craft[HAB]->turnRadians - (PI * .75) ),
					22 * lineSpace + craft[HAB]->radius * sin (craft[HAB]->turnRadians - (PI * .75) ),
					craft[HAB]->engineRadius,
					craft[HAB]->fillColor - 1052688);   //the inactive engine color is fillColor - hex(101010)
		circlefill (buffer, //draws the right 'engine'
					100 + craft[HAB]->radius * cos (craft[HAB]->turnRadians - (PI * 1.25) ),
					22 * lineSpace + craft[HAB]->radius * sin (craft[HAB]->turnRadians - (PI * 1.25) ),
					craft[HAB]->engineRadius,
					craft[HAB]->fillColor - 1052688);   //the inactive engine color is fillColor - hex(101010)
	}

	else {

		circlefill (buffer, //draws the center 'engine'
					100 + (craft[HAB]->radius - craft[HAB]->engineRadius) * cos (craft[HAB]->turnRadians - (PI) ),
					22 * lineSpace + (craft[HAB]->radius - craft[HAB]->engineRadius) * sin (craft[HAB]->turnRadians - (PI) ),
					craft[HAB]->engineRadius,
					craft[HAB]->engineColor);
		circlefill (buffer, //draws the left 'engine'
					100 + craft[HAB]->radius * cos (craft[HAB]->turnRadians - (PI * .75) ),
					22 * lineSpace + craft[HAB]->radius * sin (craft[HAB]->turnRadians - (PI * .75) ),
					craft[HAB]->engineRadius,
					craft[HAB]->engineColor);
		circlefill (buffer, //draws the right 'engine'
					100 + craft[HAB]->radius * cos (craft[HAB]->turnRadians - (PI * 1.25) ),
					22 * lineSpace + craft[HAB]->radius * sin (craft[HAB]->turnRadians - (PI * 1.25) ),
					craft[HAB]->engineRadius,
					craft[HAB]->engineColor);
	}

//    long double hyp = sqrt ((craft[HAB]->Vy * craft[HAB]->Vy) + (craft[HAB]->Vx * craft[HAB]->Vx));
//	line (buffer, 100, 22 * lineSpace, (atan2l (craft[HAB]->Vy, craft[HAB]->Vx)) + (100), (atan2l (craft[HAB]->Vy, craft[HAB]->Vx)) + (22 * lineSpace), makecol (255, 255, 255));
//	line (buffer, 100, 22 * lineSpace, 100 + craft[HAB]->radius * tan (craft[HAB]->Vy / craft[HAB]->Vx), 22 * lineSpace + craft[HAB]->radius * tan (craft[HAB]->Vy / craft[HAB]->Vx), makecol (255, 255, 255));
//	line (buffer, 100, 22 * lineSpace, 100 + craft[HAB]->radius * cos (hyp), 22 * lineSpace + craft[HAB]->radius * sin (hyp), makecol (255, 255, 255));
<<<<<<< HEAD
    line (buffer, 100, 22 * lineSpace, craft[HAB]->Vy / craft[HAB]->Vx + 22 * lineSpace, craft[HAB]->Vy / craft[HAB]->Vx + 22 * lineSpace, makecol(255, 0, 0));
=======
>>>>>>> b5cb9bd94248f2b566df5745d7f1d373349e0623

	textprintf_ex (buffer, font, lineSpace, 30 * lineSpace, makecol (200, 200, 200), -1, "Center:"), textprintf_ex (buffer, font, 220, 30 * lineSpace, makecol (200, 200, 200), -1, "%s", camera.target->name.c_str());
	textprintf_ex (buffer, font, lineSpace, 31 * lineSpace, makecol (200, 200, 200), -1, "Target:"), textprintf_ex (buffer, font, 220, 31 * lineSpace, makecol (200, 200, 200), -1, "%s", target->name.c_str());
	textprintf_ex (buffer, font, lineSpace, 32 * lineSpace, makecol (200, 200, 200), -1, "Reference:"), textprintf_ex (buffer, font, 220, 32 * lineSpace, makecol (200, 200, 200), -1, "%s", reference->name.c_str());
<<<<<<< HEAD
	*/
=======
>>>>>>> b5cb9bd94248f2b566df5745d7f1d373349e0623
}

void viewpoint::zoom (short int direction) {

	if (zoomLevel < maxZoom)
		zoomLevel += zoomStep * direction;
	else
		zoomLevel -= zoomStep;
}

long double viewpoint::actualZoom() {

	return (pow (zoomMagnitude, zoomLevel) + minZoom);
}

void viewpoint::panX (short int direction) {

	x += panSpeed / actualZoom() * actualZoom() * direction;
}

void viewpoint::panY (short int direction) {

	y += panSpeed / actualZoom() * actualZoom() * direction;
}

void viewpoint::shift() {

	x = target->x;
	y = target->y;
}

void viewpoint::autoZoom() {

//    zoom = sqrtf ( ( (target->x - reference->x) * (target->x - reference->x) ) + ( (target->y - reference->y) * (target->y - reference->y) ) ) / zoom;
//    zoom = sqrtf ( ( (target->x - reference->x) * (target->x - reference->x) ) + ( (target->y - reference->y) * (target->y - reference->y) ) ) / zoom;
}

void detectCollision () {

//	long double stepDistance;
//
//	vector <physical*>::iterator collider, collided;


//	for (collider = craft.begin(); collider != craft.end(); ++collider)
//
//		stepDistance = (*collider)->distance ( (*collided)->x + (*collided)->Vx, (*collided)->y + (*collided)->Vy)
//		               + ( (*collider)->Vx + (*collider)->Vy)
//		               - ( (*collider)->radius + (*collided)->radius); //the distance the objects will be at the next move
//
//	if (stepDistance < 0) {
//		(*collider)->Vx = (*collided)->Vx;
//		(*collider)->Vy = (*collided)->Vy;
//
//		if (stepDistance < -0.01 ) {
//			long double angle = atan2l ( (*collided)->y - (*collider)->y, (*collided)->x - (*collider)->x);
//			(*collider)->x -= cos (angle);
//			(*collider)->y -= sin (angle);
//		}   //end of stepDistance < -0.01
//	}   //end of stepDistance < 0

//
//	if (stepDistance < 0) {
//		Vx = object.Vx;
//		Vy = object.Vy;
//
//		if (stepDistance < -0.01 ) {
//			long double angle = atan2l (object.y - y, object.x - x);
//			x -= cos (angle);
//			y -= sin (angle);
//		}
//	}

//	for (vector<ship*>::iterator spaceship = craft.begin(); spaceship != craft.end(); ++spaceship) {
//
//		for (vector<solarBody*>::iterator rock = body.begin(); rock != body.end(); ++rock) {
//
//			stepDistance = (*spaceship)->distance ( (*rock)->x + (*rock)->Vx, (*rock)->y + (*rock)->Vy) + ( (*spaceship)->Vx + (*spaceship)->Vy) - ( (*spaceship)->radius + (*rock)->radius); //the distance the objects will be at the next move
//
//			if (stepDistance < 0) {
//				(*spaceship)->Vx = (*rock)->Vx;
//				(*spaceship)->Vy = (*rock)->Vy;
//
//				if (stepDistance < -0.01 ) {
//					long double angle = atan2l ( (*rock)->y - (*spaceship)->y, (*rock)->x - (*spaceship)->x);
//					(*spaceship)->x -= sin (angle);
//					(*spaceship)->y -= sin (angle);
//				}
//			}

//		}
//
//		for (vector<ship*>::iterator flyer = spaceship + 1; flyer != craft.end(); ++flyer) {
//
//			stepDistance = (*spaceship)->distance ( (*flyer)->x + (*flyer)->Vx, (*flyer)->y + (*flyer)->Vy) + ( (*spaceship)->Vx + (*spaceship)->Vy) - ( (*spaceship)->radius + (*flyer)->radius); //the distance the objects will be at the next move
//
//			if (stepDistance < 0) {
//				(*spaceship)->Vx = (*flyer)->Vx;
//				(*spaceship)->Vy = (*flyer)->Vy;
//
//				if (stepDistance < -0.01 ) {
//					long double angle = atan2l ( (*flyer)->y - (*spaceship)->y, (*flyer)->x - (*spaceship)->x);
//					(*spaceship)->x -= cos (angle);
//					(*spaceship)->y -= sin (angle);
//				}
//			}
//
//		}
//
//	}

	/*for (vector<solarBody*>::iterator rock = body.begin(); rock != body.end(); ++rock) {

		for (vector<solarBody*>::iterator otherRock = rock; otherRock != body.end(); ++otherRock) {

			stepDistance = (*rock)->distance ( (*otherRock)->x + (*otherRock)->Vx, (*otherRock)->y + (*otherRock)->Vy) + ( (*rock)->Vx + (*rock)->Vy) - ( (*rock)->radius + (*otherRock)->radius); //the distance the objects will be at the next move

			if (stepDistance < 0) {
				(*rock)->Vx = (*otherRock)->Vx;
				(*rock)->Vy = (*otherRock)->Vy;

				if (stepDistance < -0.01 ) {
					long double angle = atan2l ( (*otherRock)->y - (*rock)->y, (*otherRock)->x - (*rock)->x);
					(*rock)->x -= 1;
					(*rock)->y -= 1;
				}
			}

		}
	}*/
}

void gravitate () { //calculates gravitational forces, and accelerates, between two entities

	/*long double theta, gravity; //theta being the angle at which the object is accelerated, gravity being the rate at which it is accelerated
	//looping pointers, for looping
	vector<ship*>::iterator spaceship, _spaceship;
	vector<solarBody*>::iterator rock, _rock;

	for (rock = body.begin(); rock != body.end(); ++rock) {
        for (spaceship = craft.begin(); spaceship != craft.end(); ++spaceship){

        theta = atan2l ( (*spaceship)->x - (*rock)->x, (*spaceship)->y - (*rock)->y) + PI * 0.5;
			gravity =
			    G *
			    ( (*spaceship)->mass * (*rock)->mass) /
			    ( (*spaceship)->distance ( (*rock)->x, (*rock)->y) * (*spaceship)->distance ( (*rock)->x, (*rock)->y) );
			//finds total gravitational force between hab and earth, in the formula G (m1 * m2) / r^2

//			(*spaceship)->accX (theta, gravity);
//			(*spaceship)->accY (theta, gravity);
			(*rock)->accX (theta, -gravity);
			(*rock)->accY (theta, -gravity);

        }
        for (_rock = body.begin(); _rock != body.end(); ++_rock) {

            if (_rock != rock){
            theta = atan2l ( (*_rock)->x - (*rock)->x, (*_rock)->y - (*rock)->y) + PI * 0.5;
			gravity =
			    G *
			    ( (*_rock)->mass * (*rock)->mass) /
			    ( (*_rock)->distance ( (*rock)->x, (*rock)->y) * (*_rock)->distance ( (*rock)->x, (*rock)->y) );
			//finds total gravitational force between hab and earth, in the formula G (m1 * m2) / r^2

			(*_rock)->accX (theta, gravity);
			(*_rock)->accY (theta, gravity);
			(*rock)->accX (theta, -gravity);
			(*rock)->accY (theta, -gravity);
            }
        }


	}


//	for (vector<ship*>::iterator spaceship = craft.begin(); spaceship != craft.end(); ++spaceship) {
//
//		for (vector<solarBody*>::iterator rock = body.begin(); rock != body.end(); ++rock) {
//			theta = atan2l ( (*spaceship)->x - (*rock)->x, (*spaceship)->y - (*rock)->y) + PI * 0.5;
//			gravity =
//			    G *
//			    ( (*spaceship)->mass * (*rock)->mass) /
//			    ( (*spaceship)->distance ( (*rock)->x, (*rock)->y) * (*spaceship)->distance ( (*rock)->x, (*rock)->y) );
//			//finds total gravitational force between hab and earth, in the formula G (m1 * m2) / r^2
//
//			(*spaceship)->accX (theta, gravity);
//			(*spaceship)->accY (theta, gravity);
//			(*rock)->accX (theta, -gravity);
//			(*rock)->accY (theta, -gravity);
//		}
//
//		for (vector<solarBody*>::iterator rock = body.begin(); rock != body.end(); ++rock) {
//
//			for (vector<solarBody*>::iterator otherRock = body.begin(); otherRock != body.end(); ++otherRock) {
//
//				if (rock == otherRock) {
//
//					theta = atan2l ( (*rock)->x - (*otherRock)->x, (*rock)->y - (*otherRock)->y) + PI * 0.5;
//					gravity =
//					    G *
//					    ( (*rock)->mass * (*otherRock)->mass) /
//					    ( (*rock)->distance ( (*otherRock)->x, (*otherRock)->y) * (*rock)->distance ( (*otherRock)->x, (*otherRock)->y) );
//					//finds total gravitational force between hab and earth, in the formula G (m1 * m2) / r^2
//
//					(*rock)->accX (theta, gravity);
//					(*rock)->accY (theta, gravity);
//					(*otherRock)->accX (theta, -gravity);
//					(*otherRock)->accY (theta, -gravity);
//				}
//
//			}
//
//		}
//
//
//	}
}

void iterate (void transform() ) {

//    vector <ship*>::iterator spaceship, _spaceship;
//	vector <solarBody*>::iterator rock, _rock;
//
//    for (spaceship = craft.begin; spaceship != craft.end; ++spaceship){
//        for (_spaceship = craft.begin; _spaceship != craft.end; ++_spaceship)
//
//
//
//
//
//    }
<<<<<<< HEAD
=======

>>>>>>> b5cb9bd94248f2b566df5745d7f1d373349e0623


*/
}
