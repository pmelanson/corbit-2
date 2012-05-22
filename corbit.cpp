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
#include <boost/iterator/zip_iterator.hpp>
using namespace std;

//globals
const unsigned short int screenWidth = 1280;  //my computer's resolution
//const unsigned short int screenWidth = 1144;    //school resolution
const unsigned short int screenHeight = 980;  //my computer's resolution
//const unsigned short int screenHeight = 830;    //school resolution

BITMAP *buffer = NULL;
volatile int timer = 0;

const long double PI = 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844609550582231725359408128481117450284102701938521105559644622948954930381964428810975665933446128475648233786783165271201909145648566923460348610454326648213393607260249141273724587006606315588174881520920962829254091715364367892590360011330530548820466521384146951941511609433057270365759591953092186117381932611793105118548074462379962749567351885752724891227938183011949129833673362440656643086021394946395224737190702179860943702770539217176293176752384674818467669405132000568127145263560827785771342757789609173637178721468440901224953430146549585371050792279689258923542019956112129021960864034418159813629774771309960518707211349999998372978049951059731732816096318595024459455346908302642522308253344685035261931188171010003137838752886587533208381420617177669147303598253490428755468731159562863882353787593751957781857780532171226806613001927876611195909216420198938095257201065485863278865936153381827968230301952035301852968995773622599413891249721775283479131515574857242454150695950829533116861727855889075098381754637464939319255060400927701671139009848824012858361603563707660104710181942955596198946767;
const long double G = 6.673e-11;
const long double AU = 1495978707e2;

enum playerShips {HAB, CRAFTMAX};
enum solarSystem {SUN, MERCURY, VENUS, EARTH, MARS, JUPITER, SATURN, URANUS, NEPTUNE, PLUTO, BODYMAX};
const unsigned short int frameRate = 60;
const unsigned short int gridSpace = 50;


//prototypes
void timeStep();
void input();
void drawBuffer();
void debug();
void gravitate();
void drawGrid();
void detectCollision();
void drawHUD();

//beginning of class declarations
class viewpoint {

	const int zoomMagnitude;  //when zooming out, actual zoom level = camera.zoom ^ zoomMagnitude, therefore is an exponential zoom
	const float zoomStep; //rate at which cameras zoom out
	const unsigned short int maxZoom;  //the smaller this is, the further you can zoom in
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

	viewpoint (const int _zoomMagnitude, const float _zoomStep, const unsigned short int _maxZoom, const double _minZoom, const unsigned short int _panSpeed) :
		x (0), y (0), zoomLevel (1), track (true), zoomMagnitude (_zoomMagnitude), zoomStep (_zoomStep), maxZoom (_maxZoom), minZoom (_minZoom), panSpeed (_panSpeed)
	{}
};

struct display {

	struct physical *target;
	struct physical *reference;

	const short unsigned int gridSpace;

	void drawHUD();
	void drawGrid();

	display (const short unsigned int _gridSpace) :
		gridSpace (_gridSpace)
	{}
};

struct physical { //stores data about any physical physical, such as mass and radius, acceleration, velocity, and angle from right

	string name;    //I love C++ over C so much for this

	long int mass;
	unsigned int radius;   //mass of physical, to be used in calculation F=ma, and radius of physical
	long double x, y; //the center of the physical
	long int a();
	long int b();
	float turnRadians;
	long int distance (long double x, long double y);
	void move();   //moves physical

	long double radians;    //the degree at which the physical is velocitying [sic] from the right, in radians
	long double accX (long double radians, long double acc); //the physical's acceleration (m/s/s) along the x axis
	long double accY (long double radians, long double acc); //''
	long double Vx, Vy;   //the physical's speed (m/s) along each axis
	long double gravity(const int _x, const int _y, const int _mass);

	void turn();   //turns the physical
	long double turnRate; //rate at which the physical turns
	double degrees();  //normal degrees (360 in a circle) at which the physical is rotated from facing right

	virtual void draw();    //draws physical
	unsigned int fillColor;

	physical (const string _name, const long double _x, const long double _y, const long double _Vx, const long double _Vy, long int _mass, unsigned int _radius, unsigned int _fillColor) :
		name (_name), x (_x), y (_y), Vx (_Vx), Vy (_Vy), mass (_mass), radius (_radius), fillColor (_fillColor)
	{}
};

struct solarBody : physical {   //stores information about an astronomical body, in addition to information already stored by an physical

	unsigned short int atmosphereDrag;
	unsigned int atmosphereColor;
	unsigned short int atmosphereHeight;

	void draw();

	solarBody (const string _name, const long double _x, const long double _y, const long double _Vx, const long double _Vy, long int _mass, unsigned int _radius, unsigned int _fillColor, unsigned int _atmosphereColor, unsigned short int _atmosphereHeight) :
		physical (_name, _x, _y, _Vx, _Vy, _mass, _radius, _fillColor),
		atmosphereColor (_atmosphereColor), atmosphereHeight (_atmosphereHeight)
	{}
};

struct ship : physical {  //stores information about a pilotable ship, in addition to information already stored by an physical

	void fireEngine();
	float engine;
	unsigned int engineColor;
	unsigned short int engineRadius;

	virtual void draw();

	ship (const string _name, const long double _x, const long double _y, const long double _Vx, const long double _Vy, long int _mass, unsigned int _radius, unsigned int _fillColor, float _engine, unsigned int _engineColor, unsigned short int _engineRadius) :
		physical (_name, _x, _y, _Vx, _Vy, _mass, _radius, _fillColor),
		engine (_engine), engineColor (_engineColor), engineRadius (_engineRadius)
	{}
};

struct habitat : ship {

	void draw();

	habitat (const string _name, const long double _x, const long double _y, const long double _Vx, const long double _Vy, long int _mass, unsigned int _radius, unsigned int _fillColor, float _engine, unsigned int _engineColor, unsigned short int _engineRadius) :
		ship (_name, _x, _y, _Vx, _Vy, _mass, _radius, _fillColor, _engine, _engineColor, _engineRadius)
	{}
};

viewpoint camera (  18,             0.01,       30,     1e-10,  10);   //constructor initializes consts in the order they are declared, which is...
//                  zoomMagnitude   zoomStep    maxZoom minZoom panSpeed

display HUD (   18);    //constructor initializes consts in the order they are declared, which is...
//              gridSpace

vector <ship*> craft;
vector <solarBody*> body;

pair <vector <ship*>, vector<solarBody*> > entity (craft, body);


int main () {

	//looping variable initialization
	unsigned short int n;
	boost::make_zip_iterator (pair <vector <ship*>, vector<solarBody*> > entity (craft, body));

	//allegro initializations
	allegro_init();
	install_keyboard();
	set_color_depth (desktop_color_depth() );
	set_gfx_mode (GFX_AUTODETECT_WINDOWED, screenWidth, screenHeight, 0, 0);

	LOCK_VARIABLE (timer);
	LOCK_FUNCTION (timestep);
	install_int_ex (timeStep, BPS_TO_TIMER (frameRate) );
	buffer = create_bitmap (SCREEN_W, SCREEN_H);

	//file initialization
	ifstream datafile;

	//data initializations
	/*if (datafile.peek != '#')
		switch (datafile.getline) {
			case "solarBody" :
	//				datafile.getline
				break;
			case "craft" :
	//				dosomething;
				break;
		default case :
				break;
		}*/

//	for (n = 0; n < BODYMAX; n++)
//		entity.second.push_back ( new solarBody() );

	//radii are in meters, and are equatorial radii
	/*entity.second[SUN]->name = "Sun";
	entity.second[SUN]->x = 0 * AU;
	entity.second[SUN]->y = 0;
	entity.second[SUN]->Vx = 0;
	entity.second[SUN]->Vy = 0;
	entity.second[SUN]->mass = 3.301e20;
	entity.second[SUN]->radius = 1.392e9;
	entity.second[SUN]->fillColor = makecol (169, 169, 169);
	entity.second[SUN]->atmosphereColor = makecol (169, 169, 169);
	entity.second[SUN]->atmosphereHeight = 7;*/

	camera.zoomLevel = 0;
	camera.x = 0;
	camera.y = 0;

//	camera.target = entity.first[HAB];
	camera.target = entity.second[EARTH];
	camera.reference = entity.second[EARTH];

	///PROGRAM STARTS HERE///
	while (!key[KEY_ESC]) {

		while (timer > 0) {

			input();

//            gravitate();
//            detectCollision();
			/*
			for (rock = entity.second.begin(); rock != entity.second.end(); ++rock)
				(*rock)->move();

			for (spaceship = entity.first.begin(); spaceship != entity.first.end(); ++spaceship) {
				(*spaceship)->turn();
				(*spaceship)->fireEngine();
				(*spaceship)->move();

			}

			//            camera.autoZoom();

			if (camera.track == true)
				camera.shift();

			timer--;
			*/
		}

		HUD.drawGrid();
		/*
				for (rock = entity.second.begin(); rock != entity.second.end(); ++rock)
					(*rock)->draw();

				for (spaceship = entity.first.begin(); spaceship != entity.first.end(); ++spaceship) {
					(*spaceship)->draw();
				}
		*/
		debug();

		drawBuffer();

	}

	//end of program
	destroy_bitmap (buffer);
	release_screen();

	/*
	for (rock = entity.second.begin(); rock != entity.second.end(); ++rock)
		delete *rock;

	entity.second.clear();

	for (spaceship = entity.first.begin(); spaceship != entity.first.end(); ++spaceship)
		delete *spaceship;

	entity.first.clear();
	*/

	return (0);
}
END_OF_MAIN();

void timeStep() {

	timer++;
}
END_OF_FUNCTION (timeStep);

void drawBuffer () {

	textprintf_ex (buffer, font, 0, SCREEN_H - 10, makecol (255, 255, 255), -1, "Corbit v%d.%d%d.%d", AutoVersion::MAJOR, AutoVersion::MINOR, AutoVersion::REVISION, AutoVersion::BUILD);

	draw_sprite (buffer, screen, SCREEN_H, SCREEN_W); // Draw the buffer to the screen
	draw_sprite (screen, buffer, 0, 0);
	clear_bitmap (buffer); // Clear the contents of the buffer bitmap
}

void input () {

	if (key[KEY_A])
		entity.first[HAB]->turnRate -= 0.1 * PI / 180;

	if (key[KEY_D])
		entity.first[HAB]->turnRate += 0.1 * PI / 180;

	if (key[KEY_W])
		entity.first[HAB]->engine ++;

	if (key[KEY_S])
		entity.first[HAB]->engine --;

	if (key[KEY_BACKSPACE]) {
		if (key[KEY_LSHIFT] || key[KEY_RSHIFT])
			entity.first[HAB]->turnRate = 0;
		else
			entity.first[HAB]->engine = 0;
	}

	if (key[KEY_ENTER])
		entity.first[HAB]->engine = 100;

	if (key[KEY_LEFT])
		camera.panX (-1);

	if (key[KEY_RIGHT])
		camera.panX (1);

	if (key[KEY_UP])
		camera.panY (-1);

	if (key[KEY_DOWN])
		camera.panY (1);

	if (key[KEY_PLUS_PAD])
		camera.zoom (1);

	if (key[KEY_MINUS_PAD])
		camera.zoom (-1);

	if (key[KEY_TAB])
		camera.track = !camera.track;
}

void debug() {

	textprintf_ex (buffer, font, 0, 0, makecol (255, 255, 255), -1, "DEBUG: hab.x: %Lf", entity.first[HAB]->x);
	textprintf_ex (buffer, font, 0, 10, makecol (255, 255, 255), -1, "DEBUG: hab.y = %Lf", entity.first[HAB]->y );
	textprintf_ex (buffer, font, 0, 20, makecol (255, 255, 255), -1, "DEBUG: hab a: %Li", entity.first[HAB]->a() );
	textprintf_ex (buffer, font, 0, 30, makecol (255, 255, 255), -1, "DEBUG: hab b: %Li", entity.first[HAB]->b() );
	textprintf_ex (buffer, font, 0, 40, makecol (255, 255, 255), -1, "DEBUG: Vx: %Lf", entity.first[HAB]->Vx);
	textprintf_ex (buffer, font, 0, 50, makecol (255, 255, 255), -1, "DEBUG: Vy: %Lf", entity.first[HAB]->Vy);
	textprintf_ex (buffer, font, 0, 60, makecol (255, 255, 255), -1, "DEBUG: Venus.a: %Li", entity.second[VENUS]->a() );
	textprintf_ex (buffer, font, 0, 70, makecol (255, 255, 255), -1, "DEBUG: Venus.b: %Li", entity.second[VENUS]->b() );
	textprintf_ex (buffer, font, 0, 80, makecol (255, 255, 255), -1, "DEBUG: Earth.a: %Li", entity.second[EARTH]->a() );
	textprintf_ex (buffer, font, 0, 90, makecol (255, 255, 255), -1, "DEBUG: Earth.b: %Li", entity.second[EARTH]->b() );
	textprintf_ex (buffer, font, 0, 100, makecol (255, 255, 255), -1, "DEBUG: Venus.x: %Lf", entity.second[VENUS]->x);
	textprintf_ex (buffer, font, 0, 110, makecol (255, 255, 255), -1, "DEBUG: Venus.y: %Lf", entity.second[VENUS]->y);
	textprintf_ex (buffer, font, 0, 120, makecol (255, 255, 255), -1, "DEBUG: arc tan: %Lf", atan2f (entity.first[HAB]->x - entity.second[EARTH]->x, entity.first[HAB]->y - entity.second[EARTH]->y) + PI * 0.5 );
	textprintf_ex (buffer, font, 0, 130, makecol (255, 255, 255), -1, "DEBUG: Actual zoom: %Lf", camera.actualZoom() );
	textprintf_ex (buffer, font, 0, 140, makecol (255, 255, 255), -1, "DEBUG: camera X: %Li", camera.x);
	textprintf_ex (buffer, font, 0, 150, makecol (255, 255, 255), -1, "DEBUG: camera Y: %Li", camera.y);
	textprintf_ex (buffer, font, 0, 160, makecol (255, 255, 255), -1, "DEBUG: hab engine: %f", entity.first[HAB]->engine );
}

void physical::move() {

	x += Vx;
	y += Vy;
}

void ship::fireEngine() {

	accX (turnRadians, engine);
	accY (turnRadians, engine);
}

double physical::degrees() {

	return (radians * 180 / PI);
}

void physical::turn () {

	turnRadians += turnRate;

	if (turnRadians < 0)
		turnRadians += 2 * PI;

	if (turnRadians > 2 * PI)
		turnRadians -= 2 * PI;
}

long double physical::accX (long double radians, long double acc) {

	Vx += cos (radians) * acc / mass;
}

long double physical::accY (long double radians, long double acc) {

	Vy += sin (radians) * acc / mass;
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

	circlefill (buffer, a(), b(), radius * camera.actualZoom(), fillColor); //draws the physical to the buffer

	textprintf_ex (buffer, font, a(), b(), makecol (255, 255, 255), -1, "poop" );
}

void ship::draw() {

	float A = a();  //so that the program doesn't have to calculate a and b every time
	float B = b();

	circlefill (buffer, A, B, radius * camera.actualZoom(), fillColor); //draws the picture to the buffer
	line (buffer, A, B, //draws the 'engine'
		  A + radius * cos (turnRadians) * camera.actualZoom(),
		  B + radius * sin (turnRadians) * camera.actualZoom(),
		  engineColor);
}

void habitat::draw() {

	float A = a();  //so that the program doesn't have to calculate a and b every time
	float B = b();


	circlefill (buffer, A, B, radius * camera.actualZoom(), fillColor); //draws the picture to the buffer
	circlefill (buffer, //draws the center 'engine'
				A + (radius - engineRadius * camera.actualZoom() / 2) * cos (turnRadians - PI) * camera.actualZoom(),
				B + (radius - engineRadius * camera.actualZoom() / 2) * sin (turnRadians - PI) * camera.actualZoom(),
				engineRadius * camera.actualZoom(),
				engineColor);
	circlefill (buffer, //draws the left 'engine'
				A + radius * cos (turnRadians - (PI * .75) ) * camera.actualZoom(),
				B + radius * sin (turnRadians - (PI * .75) ) * camera.actualZoom(),
				engineRadius * camera.actualZoom(),
				engineColor);
	circlefill (buffer, //draws the right 'engine'
				A + radius * cos (turnRadians - (PI * 1.25) ) * camera.actualZoom(),
				B + radius * sin (turnRadians - (PI * 1.25) ) * camera.actualZoom(),
				engineRadius * camera.actualZoom(),
				engineColor);
}

void display::drawGrid () {  //draws a grid to the screen, later on I will be making gravity distort it

	unsigned short int x, y;

	for (x = 0; x < SCREEN_W; x += gridSpace)
		for (y = 0; y < SCREEN_H; y += gridSpace)
			putpixel (buffer, x, y, makecol (255, 255, 255));
}

void display::drawHUD () {

//will fill this in later


}

void viewpoint::zoom (short int direction) {

	zoomLevel += zoomStep * direction;
}

long double viewpoint::actualZoom() {

	return (pow (zoomMagnitude, zoomLevel) + minZoom);
}

void viewpoint::panX (short int direction) {

	x += panSpeed / actualZoom() * actualZoom() * direction;
}

void viewpoint::panY (short int direction) {

	y += panSpeed / (actualZoom() * actualZoom() ) * direction;
}

void viewpoint::shift() {

	x = target->x + SCREEN_H / 2;
	y = target->y + SCREEN_W / 2;
}

void viewpoint::autoZoom() {

//    zoom = sqrtf ( ( (target->x - reference->x) * (target->x - reference->x) ) + ( (target->y - reference->y) * (target->y - reference->y) ) ) / zoom;
//    zoom = sqrtf ( ( (target->x - reference->x) * (target->x - reference->x) ) + ( (target->y - reference->y) * (target->y - reference->y) ) ) / zoom;
}

void detectCollision () {

//	long double stepDistance;
//
//	vector <physical*>::iterator collider, collided;


//	for (collider = entity.first.begin(); collider != entity.first.end(); ++collider)
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

//	for (vector<ship*>::iterator spaceship = entity.first.begin(); spaceship != entity.first.end(); ++spaceship) {
//
//		for (vector<solarBody*>::iterator rock = entity.second.begin(); rock != entity.second.end(); ++rock) {
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
//		for (vector<ship*>::iterator flyer = spaceship + 1; flyer != entity.first.end(); ++flyer) {
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

	/*for (vector<solarBody*>::iterator rock = entity.second.begin(); rock != entity.second.end(); ++rock) {

		for (vector<solarBody*>::iterator otherRock = rock; otherRock != entity.second.end(); ++otherRock) {

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








//
//	long double theta, gravity; //theta being the angle at which the object is accelerated, gravity being the rate at which it is accelerated
//	//looping pointers, for looping
//
//	for (vector<ship*>::iterator spaceship = entity.first.begin(); spaceship != entity.first.end(); ++spaceship) {
//
//		for (vector<solarBody*>::iterator rock = entity.second.begin(); rock != entity.second.end(); ++rock) {
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
//		for (vector<solarBody*>::iterator rock = entity.second.begin(); rock != entity.second.end(); ++rock) {
//
//			for (vector<solarBody*>::iterator otherRock = entity.second.begin(); otherRock != entity.second.end(); ++otherRock) {
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
