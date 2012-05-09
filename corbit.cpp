/*******************

CORBIT Program

Designed by Patrick Melanson

Started on 23/03/2012


This program provides a realistic space flight simulation
The player's ship is called the "Hab"
Forces are as realistic as possible
See changelog.txt for changelog past 31/03/2012

*******************/

/*******************

       CONTROLS

   PROGRAM CONTROLS

ESCAPE
exit corbit

    CAMERA CONTROLS

ARROW KEYS     NUMPAD +/-
move camera    zoom in/out


     SHIP CONTROLS

W/S
increase/decrease throttle

A/D            ENTER
spin ccw/cw    set engines to 100

BACKSPACE      SHIFT+BACKSPACE
cut engines    stop turning

*******************/

/*******************

This software is licensed under the WTFPL license, in 2012, as follows:




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

#include <allegro5/allegro.h>
#include <math.h>
#include <vector>
//#include <memory>
#include "version.h"
#include <iostream>
using namespace std;

//globals
const unsigned short int screenWidth = 1280;
//const unsigned short int screenWidth = 1144;    //school resolution
const unsigned short int screenHeight = 980;
//const unsigned short int screenHeight = 830;    //school resolution
const float zoomMagnitude = 2;  //when zooming out, actual zoom level = camera.zoom ^ zoomMagnitude, therefore is an exponential zoom
const float zoomStep = 0.02; //rate at which cameras zoom out
const unsigned short int maxZoom = 20;
BITMAP *buffer = NULL;
volatile int timer = 0;
const long double PI = 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844609550582231725359408128481117450284102701938521105559644622948954930381964428810975665933446128475648233786783165271201909145648566923460348610454326648213393607260249141273724587006606315588174881520920962829254091715364367892590360011330530548820466521384146951941511609433057270365759591953092186117381932611793105118548074462379962749567351885752724891227938183011949129833673362440656643086021394946395224737190702179860943702770539217176293176752384674818467669405132000568127145263560827785771342757789609173637178721468440901224953430146549585371050792279689258923542019956112129021960864034418159813629774771309960518707211349999998372978049951059731732816096318595024459455346908302642522308253344685035261931188171010003137838752886587533208381420617177669147303598253490428755468731159562863882353787593751957781857780532171226806613001927876611195909216420198938095257201065485863278865936153381827968230301952035301852968995773622599413891249721775283479131515574857242454150695950829533116861727855889075098381754637464939319255060400927701671139009848824012858361603563707660104710181942955596198946767;
const long double G = 6.673e-11;
enum playerShips {HAB, CRAFTMAX};
enum solarSystem {SUN, MERCURY, VENUS, EARTH, MARS, JUPITER, SATURN, URANUS, NEPTUNE, BODYMAX};
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

//beginning of class declarations
struct viewpoint {

	long double x;
	long double y;
	long double zoom;
	long double actualZoom();
	void shift();
	struct body *target;
	struct body *reference;
	void autoZoom();
	bool track;
};

struct body { //stores data about any physical body, such as mass and radius, acceleration, velocity, and angle from right

	char name[21];

	long double mass;
	unsigned int radius;   //mass of body, to be used in calculation F=ma, and radius of body
	long double x, y; //the center of the body
	float a();
	float b();
	long double turnRadians;
	long double distance (long double x, long double y);
	void move();   //moves body

	void accelerate();
	long double acc;  //net acceleration of body
	long double radians;    //the degree at which the body is velocitying from the right, in radians
	void accX (long double radians, long double acc); //the body's acceleration (m/s/s) along the x axis
	void accY (long double radians, long double acc); //''
	long double Vx, Vy;   //the body's speed (m/s) along each axis

	void turn ();   //turns the body
	long double turnRate; //rate at which the body turns
	double degrees();  //normal degrees (360 in a circle) at which the body is rotated from facing right

	virtual void draw();    //draws body
	unsigned int fillColour;
};

struct ship : body {  //stores information about a pilotable ship, in addition to information already stored by an body

	void fireEngine();
	float engine;
	unsigned int engineColour;
	unsigned short int engineRadius;

	virtual void draw();
};

struct habitat : ship {

	void draw();
};

struct solarBody : body {   //stores information about an astronomical body, in addition to information already stored by an body

	unsigned int atmosphereHeight;
	unsigned int atmosphereDrag;
	unsigned int atmosphereColour;

	void draw();
};

viewpoint camera;
vector <ship*> craft;
vector <solarBody*> body;


int main () {

	//allegro initializations
	allegro_init();
	install_keyboard();
	set_color_depth (desktop_color_depth() );
	set_gfx_mode (GFX_AUTODETECT_WINDOWED, screenWidth, screenHeight, 0, 0);

	LOCK_VARIABLE (timer);
	LOCK_FUNCTION (timestep);
	install_int_ex (timeStep, BPS_TO_TIMER (frameRate) );

	//bitmap initializations
	buffer = create_bitmap (SCREEN_W, SCREEN_H);

	//data initializations

	//looping variables, and declaring blank
	unsigned short int n;

	for (n = 0; n < BODYMAX; n++)
		body.push_back ( new solarBody() );

	vector<ship*>::iterator spaceship;
	vector<solarBody*>::iterator rock;

	for (rock = body.begin(); rock != body.end(); ++rock) {
		strcpy ( (*rock)->name, "Blank");
		(*rock)->Vx = 0;
		(*rock)->Vy = 0;
		(*rock)->radius = 100;
		(*rock)->mass = 6000;
		(*rock)->fillColour = makecol (100, 100, 100);
		(*rock)->atmosphereColour = makecol (110, 110, 110);
		(*rock)->atmosphereHeight = 5;
		(*rock)->x = 1000;
		(*rock)->y = 0;
	}

	strcpy (body[EARTH]->name, "Earth");
	body[EARTH]->Vx = 0;
	body[EARTH]->Vy = 0;
	body[EARTH]->radius = 200;
	body[EARTH]->mass = pow (5, 8) * 6 * 2.71828;
	body[EARTH]->fillColour = makecol (0, 255, 0);
	body[EARTH]->atmosphereColour = makecol (0, 0, 255);
	body[EARTH]->atmosphereHeight = 3;
	body[EARTH]->x = SCREEN_W / 2;
	body[EARTH]->y = SCREEN_H / 2;

	strcpy (body[MARS]->name, "Mars");
	body[MARS]->x = body[EARTH]->x + body[EARTH]->radius + 800;
	body[MARS]->y = body[EARTH]->y;
	body[MARS]->radius = 150;
	body[MARS]->mass = 6e1;
	body[MARS]->fillColour = makecol (205, 164, 150);
	body[MARS]->atmosphereColour = makecol (160, 40, 40);
	body[MARS]->atmosphereHeight = 7;

	craft.push_back ( new habitat() );

	strcpy (craft[HAB]->name, "Habitat");
	craft[HAB]->fillColour = makecol (211, 211, 211);
	craft[HAB]->engineColour = makecol (139, 0, 0);
	craft[HAB]->radius = 30;
	craft[HAB]->Vx = body[EARTH]->Vx;
	craft[HAB]->Vy = body[EARTH]->Vy;
	craft[HAB]->x = SCREEN_W / 2 + body[EARTH]->radius + craft[HAB]->radius;
	craft[HAB]->y = SCREEN_H / 2;
	craft[HAB]->mass = 5e4;
	craft[HAB]->turnRadians = 0;
	craft[HAB]->turnRate = 0;
	craft[HAB]->engineRadius = 8;

	camera.zoom = 0;
	camera.x = craft[HAB]->x - (SCREEN_W / 4);
	camera.y = craft[HAB]->y - (SCREEN_H / 4);

	camera.target = craft[HAB];
	camera.reference = body[EARTH];

	while (!key[KEY_ESC]) {

		while (timer > 0) {

			input();

//			gravitate();
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

		drawGrid();

		for (rock = body.begin(); rock != body.end(); ++rock)
			(*rock)->draw();

		for (spaceship = craft.begin(); spaceship != craft.end(); ++spaceship) {
			(*spaceship)->draw();
		}

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

	return (0);
}
END_OF_MAIN();


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
		camera.x -= maxZoom + 1 - camera.actualZoom();

	if (key[KEY_RIGHT])
		camera.x += maxZoom + 1 - camera.actualZoom();

	if (key[KEY_UP])
		camera.y -= maxZoom + 1 - camera.actualZoom();

	if (key[KEY_DOWN])
		camera.y += maxZoom + 1 - camera.actualZoom();

	if (key[KEY_PLUS_PAD]) {
		if (camera.actualZoom() < maxZoom)
			camera.zoom += zoomStep;
	}

	if (key[KEY_MINUS_PAD])
		camera.zoom -= zoomStep;

	if (key[KEY_TAB])
		camera.track = !camera.track;
}

void drawBuffer () {

	textprintf_ex (buffer, font, 0, SCREEN_H - 10, makecol (255, 255, 255), -1, "Corbit v%d.%d%d.%d", AutoVersion::MAJOR, AutoVersion::MINOR, AutoVersion::REVISION, AutoVersion::BUILD);

	draw_sprite (buffer, screen, SCREEN_H, SCREEN_W); // Draw the buffer to the screen
	draw_sprite (screen, buffer, 0, 0);
	clear_bitmap (buffer); // Clear the contents of the buffer bitmap
}

void timeStep() {

	timer++;
}
END_OF_FUNCTION (timeStep);

void body::move() {

	x += Vx;
	y += Vy;
}

void ship::fireEngine() {

	accX (turnRadians, engine);
	accY (turnRadians, engine);
}

void debug() {

	textprintf_ex (buffer, font, 0, 0, makecol (255, 255, 255), -1, "DEBUG: hab.x: %Lf", craft[HAB]->x);
	textprintf_ex (buffer, font, 0, 10, makecol (255, 255, 255), -1, "DEBUG: hab.y = %Lf", craft[HAB]->y );
	textprintf_ex (buffer, font, 0, 20, makecol (255, 255, 255), -1, "DEBUG: Mars.x = %Lf", body[MARS]->x );
	textprintf_ex (buffer, font, 0, 30, makecol (255, 255, 255), -1, "DEBUG: Mars.y = %Lf", body[MARS]->y );
	textprintf_ex (buffer, font, 0, 40, makecol (255, 255, 255), -1, "DEBUG: Vx: %Lf", craft[HAB]->Vx);
	textprintf_ex (buffer, font, 0, 50, makecol (255, 255, 255), -1, "DEBUG: Vy: %Lf", craft[HAB]->Vy);
	textprintf_ex (buffer, font, 0, 60, makecol (255, 255, 255), -1, "DEBUG: Earth.x: %Lf", body[EARTH]->x);
	textprintf_ex (buffer, font, 0, 70, makecol (255, 255, 255), -1, "DEBUG: Earth.y: %Lf", body[EARTH]->y);
	textprintf_ex (buffer, font, 0, 80, makecol (255, 255, 255), -1, "DEBUG: arc tan: %Lf", atan2f (craft[HAB]->x - body[EARTH]->x, craft[HAB]->y - body[EARTH]->y) + PI * 0.5 );
	textprintf_ex (buffer, font, 0, 90, makecol (255, 255, 255), -1, "DEBUG: Actual zoom: %Lf", camera.actualZoom() );
	textprintf_ex (buffer, font, 0, 100, makecol (255, 255, 255), -1, "DEBUG: Camera zoom: %Lf", camera.zoom);
	textprintf_ex (buffer, font, 0, 110, makecol (255, 255, 255), -1, "DEBUG: turn Radians: %Lf", craft[HAB]->turnRadians);
	textprintf_ex (buffer, font, 0, 120, makecol (255, 255, 255), -1, "DEBUG: turn Degrees: %Lf", craft[HAB]->turnRadians * 180 / PI);
	textprintf_ex (buffer, font, 0, 130, makecol (255, 255, 255), -1, "DEBUG: turn Rate: %Lf", craft[HAB]->turnRate);
	textprintf_ex (buffer, font, 0, 140, makecol (255, 255, 255), -1, "DEBUG: camera X: %Lf", camera.x);
	textprintf_ex (buffer, font, 0, 150, makecol (255, 255, 255), -1, "DEBUG: camera Y: %Lf", camera.y);
	textprintf_ex (buffer, font, 0, 160, makecol (255, 255, 255), -1, "DEBUG: hab a: %f", craft[HAB]->a() );
	textprintf_ex (buffer, font, 0, 170, makecol (255, 255, 255), -1, "DEBUG: hab b: %f", craft[HAB]->b() );
	textprintf_ex (buffer, font, 0, 180, makecol (255, 255, 255), -1, "DEBUG: hab engine: %f", craft[HAB]->engine );
}

double body::degrees() {

	return (radians * 180 / PI);
}
void body::accX (long double radians, long double acc) {

	Vx += cos (radians) * acc / mass;
}
void body::accY (long double radians, long double acc) {

	Vy += sin (radians) * acc / mass;
}

void body::draw() {

	circlefill (buffer, x - camera.x, y - camera.y, radius * camera.zoom, fillColour ); //draws the body to the buffer
}

void solarBody::draw() {

	circlefill (buffer, a(), b(), radius * camera.actualZoom() + atmosphereHeight, atmosphereColour);   //draws the atmosphere to the buffer

	circlefill (buffer, a(), b(), radius * camera.actualZoom(), fillColour); //draws the body to the buffer
}

void ship::draw() {

	float A = a();  //so that the program doesn't have to calculate a and b every time
	float B = b();

	circlefill (buffer, A, B, radius * camera.actualZoom(), fillColour); //draws the picture to the buffer
	line (buffer, A, B, //draws the 'engine'
	      A + radius * cos (turnRadians) * camera.actualZoom(),
	      B + radius * sin (turnRadians) * camera.actualZoom(),
	      engineColour);
}

void habitat::draw() {

	float A = a();  //so that the program doesn't have to calculate a and b every time
	float B = b();


	circlefill (buffer, A, B, radius * camera.actualZoom(), fillColour); //draws the picture to the buffer
	circlefill (buffer, //draws the center 'engine'
	            A + (radius - engineRadius * camera.actualZoom() / 2) * cos (turnRadians - PI) * camera.actualZoom(),
	            B + (radius - engineRadius * camera.actualZoom() / 2) * sin (turnRadians - PI) * camera.actualZoom(),
	            engineRadius * camera.actualZoom(),
	            engineColour);
	circlefill (buffer, //draws the left 'engine'
	            A + radius * cos (turnRadians - (PI * .75) ) * camera.actualZoom(),
	            B + radius * sin (turnRadians - (PI * .75) ) * camera.actualZoom(),
	            engineRadius * camera.actualZoom(),
	            engineColour);
	circlefill (buffer, //draws the right 'engine'
	            A + radius * cos (turnRadians - (PI * 1.25) ) * camera.actualZoom(),
	            B + radius * sin (turnRadians - (PI * 1.25) ) * camera.actualZoom(),
	            engineRadius * camera.actualZoom(),
	            engineColour);
}

void body::turn () {

	turnRadians += turnRate;

	if (turnRadians < 0)
		turnRadians += 2 * PI;

	if (turnRadians > 2 * PI)
		turnRadians -= 2 * PI;
}

long double viewpoint::actualZoom() {

	return (pow (zoomMagnitude, zoom) );
}

float body::a() { //on-screen x position of body

	return ( (x - camera.x) * camera.actualZoom() );
}

float body::b() { //on-screen y position of body

	return ( (y - camera.y) * camera.actualZoom() );
}

<<<<<<< HEAD
void gravitate () { //calculates gravitational forces, and accelerates, between two entities

	long double theta, gravity; //theta being the angle at which the object is accelerated, gravity being the rate at which it is accelerated
	//looping pointers, for looping

	for (vector<ship*>::iterator spaceship = craft.begin(); spaceship != craft.end(); ++spaceship) {

		for (vector<solarBody*>::iterator rock = body.begin(); rock != body.end(); ++rock) {
			theta = atan2l ( (*spaceship)->x - (*rock)->x, (*spaceship)->y - (*rock)->y) + PI * 0.5;
			gravity =
				G *
				( (*spaceship)->mass * (*rock)->mass) /
				( (*spaceship)->distance ( (*rock)->x, (*rock)->y) * (*spaceship)->distance ( (*rock)->x, (*rock)->y) );
			//finds total gravitational force between hab and earth, in the formula G (m1 * m2) / r^2

			(*spaceship)->accX (theta, gravity);
			(*spaceship)->accY (theta, gravity);
			(*rock)->accX (theta, -gravity);
			(*rock)->accY (theta, -gravity);
		}

		for (vector<solarBody*>::iterator rock = body.begin(); rock != body.end(); ++rock) {

			for (vector<solarBody*>::iterator otherRock = body.begin(); otherRock != body.end(); ++otherRock) {

				if (rock == otherRock) {

					theta = atan2l ( (*rock)->x - (*otherRock)->x, (*rock)->y - (*otherRock)->y) + PI * 0.5;
					gravity =
						G *
						( (*rock)->mass * (*otherRock)->mass) /
						( (*rock)->distance ( (*otherRock)->x, (*otherRock)->y) * (*rock)->distance ( (*otherRock)->x, (*otherRock)->y) );
					//finds total gravitational force between hab and earth, in the formula G (m1 * m2) / r^2

					(*rock)->accX (theta, gravity);
					(*rock)->accY (theta, gravity);
					(*otherRock)->accX (theta, -gravity);
					(*otherRock)->accY (theta, -gravity);
				}

			}

		}


	}
}

long double body::distance (long double targetX, long double targetY) { //finds distance from body to target
=======
long double entity::distance (long double targetX, long double targetY) { //finds distance from entity to target
>>>>>>> 1a810651efd4537a8b92d326c654ce7dc4ccc36e

	return (sqrtf ( ( (targetX - x) * (targetX - x) ) + ( (targetY - y) * (targetY - y) ) ) ); //finds the distance between two entities, using d = sqrt ( (x1 - x2)^2 + (y1 - y2) )
}

void drawGrid () {  //draws a grid to the screen, later on I will be making gravity distort it

	unsigned short int n;

	for (n = 0; n < SCREEN_W; n++)
		line (buffer,
		      n * gridSpace,
		      0,
		      n * gridSpace,
		      SCREEN_H,
		      makecol (100, 100, 100)
		     );

	for (n = 0; n < SCREEN_H; n++)
		line (buffer,
		      0,
		      n * gridSpace,
		      SCREEN_W, n * gridSpace,
		      makecol (100, 100, 100)
		     );
}

void viewpoint::shift() {

	x = target->x - SCREEN_W / 4;
	y = target->y - SCREEN_H / 4;
}

void viewpoint::autoZoom() {

//    zoom = sqrtf ( ( (target->x - reference->x) * (target->x - reference->x) ) + ( (target->y - reference->y) * (target->y - reference->y) ) ) / zoom;
//    zoom = sqrtf ( ( (target->x - reference->x) * (target->x - reference->x) ) + ( (target->y - reference->y) * (target->y - reference->y) ) ) / zoom;
}

void detectCollision () {

	long double stepDistance;

	vector <entity*>::iterator collider, collided;


	for (collider = craft.begin(); collider != craft.end(); ++collider)

		stepDistance = (*collider)->distance ( (*collided)->x + (*collided)->Vx, (*collided)->y + (*collided)->Vy)
		               + ( (*collider)->Vx + (*collider)->Vy)
		               - ( (*collider)->radius + (*collided)->radius); //the distance the objects will be at the next move

	if (stepDistance < 0) {
		(*collider)->Vx = (*collided)->Vx;
		(*collider)->Vy = (*collided)->Vy;

		if (stepDistance < -0.01 ) {
			long double angle = atan2l ( (*collided)->y - (*collider)->y, (*collided)->x - (*collider)->x);
			(*collider)->x -= cos (angle);
			(*collider)->y -= sin (angle);
		}   //end of stepDistance < -0.01
	}   //end of stepDistance < 0

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
//
//	long double theta, gravity; //theta being the angle at which the object is accelerated, gravity being the rate at which it is accelerated
//	//looping pointers, for looping
//
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
