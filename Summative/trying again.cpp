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
//#include "version.h"

//globals
const int screenWidth = 1040;
const int screenHeight = 820;
const float zoomMagnitude = 2;  //when zooming out, actual zoom level = camera.zoom ^ zoomMagnitude, therefore is an exponential zoom
const float zoomStep = 0.02; //rate at which cameras zoom out
const double maxZoom = 20;
BITMAP *buffer = NULL;
volatile int timer = 0;
volatile int milliseconds = 0;
const float PI = 3.14159265;
const double G = 6.673e-11;
enum craft {HAB, CRAFTMAX};
enum planet {MERCURY, VENUS, EARTH, MARS, JUPITER, SATURN, URANUS, NEPTUNE, PLANETMAX};
const unsigned short int frameRate = 60;


//prototypes
void timeStep();
void input();
void drawHab ();
void drawBuffer ();
void detectCollision ();
void debug();
void gravitate();
void drawGrid();

//beginning of class declarations
struct viewpoint {

	float x;
	float y;
	float zoom;
	float actualZoom();
};

struct entity { //stores data about any physical entity, such as mass and radius, acceleration, velocity, and angle from right

<<<<<<< HEAD
	char name[21];

	double mass;
	unsigned int radius;   //mass of entity, to be used in calculation F=ma, and radius of entity
	void gravitate (struct ship craft);
	float x, y; //the center of the entity
	float a();
	float b();
	float turnRadians;
	float distance (float x, float y);
	void move ();   //moves entity

	void accelerate();
	float acc;  //net acceleration of entity
	float radians;    //the degree at which the entity is velocitying from the right, in radians
	void accX (float radians, float acc); //the entity's acceleration (m/s/s) along the x axis
	void accY (float radians, float acc); //''
	float Vx, Vy;   //the entity's speed (m/s) along each axis

	void turn ();   //turns the entity
	float turnRate; //rate at which the entity turns
	float degrees();  //normal degrees (360 in a circle) at which the entity is rotated from facing right

	void draw();    //draws entity
	unsigned int fillColour;
=======
    char name[21];

    long double mass;
    unsigned int radius;   //mass of entity, to be used in calculation F=ma, and radius of entity
    void gravitate (struct entity object);
    double x, y; //the center of the entity
    float a();
    float b();
    float turnRadians;
    float distance (float x, float y);
    void move ();   //moves entity

    void accelerate();
    float acc;  //net acceleration of entity
    float radians;    //the degree at which the entity is velocitying from the right, in radians
    void accX (long double radians, long double acc); //the entity's acceleration (m/s/s) along the x axis
    void accY (long double radians, long double acc); //''
    long double Vx, Vy;   //the entity's speed (m/s) along each axis

    void turn ();   //turns the entity
    float turnRate; //rate at which the entity turns
    float degrees();  //normal degrees (360 in a circle) at which the entity is rotated from facing right

    void draw();    //draws entity
    unsigned int fillColour;
>>>>>>> 323a06e4b8695ab55428290337b0848bccc53909
};

struct ship : entity {  //stores information about a pilotable ship, in addition to information already stored by an entity

	void fireEngine();
	float engine;
	unsigned int engineColour;

	void draw();
};

struct body : entity {   //stores information about an astronomical body, in addition to information already stored by an entity

	unsigned int atmosphereHeight;
	unsigned int atmosphereDrag;
	unsigned int atmosphereColour;

	void draw();
};

viewpoint camera;
ship craft[CRAFTMAX]; //all ships. Default ship is the Hab, the first one declared
body planet[PLANETMAX];    //all planets in our solar system, ordered in distance from sun. Earth is 3


int main () {

	//allegro initializations
	allegro_init();
	install_keyboard();
	set_color_depth (desktop_color_depth() );
	set_gfx_mode (GFX_AUTODETECT_WINDOWED, screenWidth, screenHeight, 0, 0);

	LOCK_VARIABLE (timer);
	LOCK_FUNCTION (timestep);
	install_int_ex (timeStep, BPS_TO_TIMER (frameRate) );
//    install_int_ex (input, BPS_TO_TIMER (10) );

<<<<<<< HEAD
	//bitmap initializations
	buffer = create_bitmap (screenWidth, screenHeight);

	//data initializations

	bool gravitated[CRAFTMAX-1][PLANETMAX-1] = {};
	unsigned short int n = 0;
	unsigned short int i = 0;

	strcpy (planet[EARTH].name, "Earth");
	planet[EARTH].x = screenWidth / 2;
	planet[EARTH].y = screenHeight / 2;
	planet[EARTH].radius = 200;
	planet[EARTH].mass = 5.9742e8;
	planet[EARTH].fillColour = makecol (0, 255, 0);
	planet[EARTH].atmosphereColour = makecol (0, 0, 255);
	planet[EARTH].atmosphereHeight = 3;

	strcpy (craft[HAB].name, "Habitat");
	craft[HAB].fillColour = makecol (211, 211, 211);
	craft[HAB].engineColour = makecol (139, 0, 0);
	craft[HAB].radius = 30;
	craft[HAB].x = screenWidth / 2 + planet[EARTH].radius + craft[HAB].radius;
	craft[HAB].y = screenHeight / 2;
	craft[HAB].mass = 35000;

//	strcpy (planet[MARS].name, "Mars");
//	planet[MARS].x = 0;
//	planet[MARS].y = 0;
//	planet[MARS].radius = 200;
//	planet[MARS].mass = 5.9742e8;
//	planet[MARS].fillColour = makecol (205, 164, 150);
//	planet[MARS].atmosphereColour = makecol (160, 40, 40);
//	planet[MARS].atmosphereHeight = 7;
	planet[MARS] = planet[EARTH];
	planet[MARS].x = 50;
	planet[MARS].y = 50;

	camera.zoom = pow (camera.actualZoom(), 1 / zoomMagnitude);
	camera.x = craft[HAB].x - (screenWidth / 4);
	camera.y = craft[HAB].y - (screenHeight / 4);

	while (!key[KEY_ESC]) {

		while (timer > 0) {

			input();

//            craft[HAB].turn();
//            craft[HAB].fireEngine();
//            craft[HAB].move();
//            craft[HAB].gravitate();
			detectCollision();

=======
    //bitmap initializations
    buffer = create_bitmap (screenWidth, screenHeight);

    //data initializations

    bool gravitated[CRAFTMAX-1][PLANETMAX-1] = {};
    unsigned int n = 0;
    unsigned int i = 0;

    strcpy (planet[EARTH].name, "Earth");
    planet[EARTH].x = screenWidth / 2;
    planet[EARTH].y = screenHeight / 2;
    planet[EARTH].radius = 200;
    planet[EARTH].mass = 5.9742e8;
    planet[EARTH].fillColour = makecol (0, 255, 0);
    planet[EARTH].atmosphereColour = makecol (0, 0, 255);
    planet[EARTH].atmosphereHeight = 3;

    strcpy (craft[HAB].name, "Habitat");
    craft[HAB].fillColour = makecol (211, 211, 211);
    craft[HAB].engineColour = makecol (139, 0, 0);
    craft[HAB].radius = 30;
    craft[HAB].x = screenWidth / 2 + planet[EARTH].radius + craft[HAB].radius;
    craft[HAB].y = screenHeight / 2;
    craft[HAB].mass = 35000;

    strcpy (planet[MARS].name, "Mars");
    planet[MARS].x = 0;
    planet[MARS].y = 0;
    planet[MARS].radius = 200;
    planet[MARS].mass = 5.9742e8;
    planet[MARS].fillColour = makecol (205, 164, 150);
    planet[MARS].atmosphereColour = makecol (160, 40, 40);
    planet[MARS].atmosphereHeight = 7;

    camera.zoom = pow (camera.actualZoom(), 1 / zoomMagnitude);
    camera.x = craft[HAB].x - (screenWidth / 4);
    camera.y = craft[HAB].y - (screenHeight / 4);

    while (!key[KEY_ESC]) {

        while (timer > 0) {

            input();

//            craft[HAB].gravitate (planet[EARTH]);
            detectCollision();
>>>>>>> 323a06e4b8695ab55428290337b0848bccc53909
//            planet[EARTH].move();
			for (n = 0; n < PLANETMAX; n++)
				planet[n].move();

			for (n = 0; n < CRAFTMAX; n++) {
				craft[n].turn();
				craft[n].fireEngine();
				craft[n].move();
			}

<<<<<<< HEAD
			for (i = 0; i < PLANETMAX; i++)
				for (n = 0; n < CRAFTMAX; n++)
					planet[i].gravitate (craft[n]);
=======
//            for (n = 0; n < PLANETMAX; n++)
//                for (i = n; i < CRAFTMAX; i++)
//                    planet[n].gravitate (planet[i]);
>>>>>>> 323a06e4b8695ab55428290337b0848bccc53909

			timer--;
		}

<<<<<<< HEAD
		drawGrid();
=======
        drawGrid();
//        for (n = 0; n < PLANETMAX; n++)
//            planet[n].draw();
>>>>>>> 323a06e4b8695ab55428290337b0848bccc53909

		for (n = 0; n < PLANETMAX; n++)
			planet[n].draw();

<<<<<<< HEAD
		for (n = 0; n < CRAFTMAX; n++)
			craft[n].draw();
=======
//        for (n = 0; n < CRAFTMAX; n++)
//            craft[n].draw();

        debug();
>>>>>>> 323a06e4b8695ab55428290337b0848bccc53909

//		planet[EARTH].draw();
//		planet[MARS].draw();
		craft[HAB].draw();

		debug();

		drawBuffer();

	}

	//end of program
	destroy_bitmap (buffer);
	return (0);
}
END_OF_MAIN();


void input () {

	if (key[KEY_A]) {
		craft[HAB].turnRate -= 0.01 * PI / 180;
	}

	if (key[KEY_D]) {
		craft[HAB].turnRate += 0.01 * PI / 180;
	}

	if (key[KEY_W]) {
		craft[HAB].engine ++;
	}

	if (key[KEY_S]) {
		craft[HAB].engine --;
	}

	if (key[KEY_BACKSPACE]) {
		if (key[KEY_LSHIFT] || key[KEY_RSHIFT])
			craft[HAB].turnRate = 0;
		else
			craft[HAB].engine = 0;
	}

	if (key[KEY_ENTER])
		craft[HAB].engine = 100;

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

<<<<<<< HEAD
	if (key[KEY_MINUS] || key[KEY_MINUS_PAD]) {
		camera.zoom -= zoomStep;
	}

=======
    if (key[KEY_MINUS] || key[KEY_MINUS_PAD]) {
        camera.zoom -= zoomStep;
    }
>>>>>>> 323a06e4b8695ab55428290337b0848bccc53909
}

void drawBuffer () {

<<<<<<< HEAD
//	textprintf_ex (buffer, font, 0, screenHeight - 10, makecol (255, 255, 255), -1, "Corbit v%d.%d%d.%d", AutoVersion::MAJOR, AutoVersion::MINOR, AutoVersion::REVISION, AutoVersion::BUILD);
=======
//    textprintf_ex (buffer, font, 0, screenHeight - 10, makecol (255, 255, 255), -1, "Corbit v%d.%d%d.%d", AutoVersion::MAJOR, AutoVersion::MINOR, AutoVersion::REVISION, AutoVersion::BUILD);
>>>>>>> 323a06e4b8695ab55428290337b0848bccc53909

	draw_sprite (buffer, screen, screenHeight, screenWidth); // Draw the buffer to the screen
	draw_sprite (screen, buffer, 0, 0);
	clear_bitmap (buffer); // Clear the contents of the buffer bitmap
}

void timeStep() {

	timer++;
}
END_OF_FUNCTION (timeStep);

void entity::move() {

	x += Vx;
	y += Vy;
}

void ship::fireEngine() {

	accX (turnRadians, engine);
	accY (turnRadians, engine);
}

void debug() {

<<<<<<< HEAD
	textprintf_ex (buffer, font, 0, 0, makecol (255, 255, 255), -1, "DEBUG: hab.x: %f", craft[HAB].x);
	textprintf_ex (buffer, font, 0, 10, makecol (255, 255, 255), -1, "DEBUG: hab.y = %f", craft[HAB].y );
	textprintf_ex (buffer, font, 0, 20, makecol (255, 255, 255), -1, "DEBUG: camera.x = %f", camera.x );
	textprintf_ex (buffer, font, 0, 30, makecol (255, 255, 255), -1, "DEBUG: camera.y = %f", camera.y );
	textprintf_ex (buffer, font, 0, 40, makecol (255, 255, 255), -1, "DEBUG: Vx: %f", craft[HAB].Vx);
	textprintf_ex (buffer, font, 0, 50, makecol (255, 255, 255), -1, "DEBUG: Vy: %f", craft[HAB].Vy);
	textprintf_ex (buffer, font, 0, 60, makecol (255, 255, 255), -1, "DEBUG: Earth.Vx: %f", planet[EARTH].Vx);
	textprintf_ex (buffer, font, 0, 70, makecol (255, 255, 255), -1, "DEBUG: Earth.Vy: %f", planet[EARTH].Vy);
	textprintf_ex (buffer, font, 0, 80, makecol (255, 255, 255), -1, "DEBUG: arc tan: %f", atan2 (craft[HAB].x - planet[EARTH].x, craft[HAB].y - planet[EARTH].y) + PI * 0.5 );
	textprintf_ex (buffer, font, 0, 90, makecol (255, 255, 255), -1, "DEBUG: Actual zoom: %f", pow (zoomMagnitude, camera.zoom) );
	textprintf_ex (buffer, font, 0, 100, makecol (255, 255, 255), -1, "DEBUG: Camera zoom: %f", camera.zoom);
	textprintf_ex (buffer, font, 0, 110, makecol (255, 255, 255), -1, "DEBUG: turn Radians: %f", craft[HAB].turnRadians);
	textprintf_ex (buffer, font, 0, 120, makecol (255, 255, 255), -1, "DEBUG: turn Degrees: %f", craft[HAB].turnRadians * 180 / PI);
	textprintf_ex (buffer, font, 0, 130, makecol (255, 255, 255), -1, "DEBUG: turn Rate: %f", craft[HAB].turnRate);
=======
    textprintf_ex (buffer, font, 0, 0, makecol (255, 255, 255), -1, "DEBUG: hab.x: %f", craft[HAB].x);
    textprintf_ex (buffer, font, 0, 10, makecol (255, 255, 255), -1, "DEBUG: hab.y = %f", craft[HAB].y );
    textprintf_ex (buffer, font, 0, 20, makecol (255, 255, 255), -1, "DEBUG: camera.x = %f", camera.x );
    textprintf_ex (buffer, font, 0, 30, makecol (255, 255, 255), -1, "DEBUG: camera.y = %f", camera.y );
    textprintf_ex (buffer, font, 0, 40, makecol (255, 255, 255), -1, "DEBUG: Vx: %Lf", craft[HAB].Vx);
    textprintf_ex (buffer, font, 0, 50, makecol (255, 255, 255), -1, "DEBUG: Vy: %Lf", craft[HAB].Vy);
    textprintf_ex (buffer, font, 0, 60, makecol (255, 255, 255), -1, "DEBUG: Earth.Vx: %Lf", planet[EARTH].Vx);
    textprintf_ex (buffer, font, 0, 70, makecol (255, 255, 255), -1, "DEBUG: Earth.Vy: %Lf", planet[EARTH].Vy);
    textprintf_ex (buffer, font, 0, 80, makecol (255, 255, 255), -1, "DEBUG: arc tan: %f", atan2 (craft[HAB].x - planet[EARTH].x, craft[HAB].y - planet[EARTH].y) + PI * 0.5 );
    textprintf_ex (buffer, font, 0, 90, makecol (255, 255, 255), -1, "DEBUG: Actual zoom: %f", pow (zoomMagnitude, camera.zoom) );
    textprintf_ex (buffer, font, 0, 100, makecol (255, 255, 255), -1, "DEBUG: Camera zoom: %f", camera.zoom);
    textprintf_ex (buffer, font, 0, 110, makecol (255, 255, 255), -1, "DEBUG: turn Radians: %f", craft[HAB].turnRadians);
    textprintf_ex (buffer, font, 0, 120, makecol (255, 255, 255), -1, "DEBUG: turn Degrees: %f", craft[HAB].turnRadians * 180 / PI);
    textprintf_ex (buffer, font, 0, 130, makecol (255, 255, 255), -1, "DEBUG: turn Rate: %f", craft[HAB].turnRate);
>>>>>>> 323a06e4b8695ab55428290337b0848bccc53909
}

float entity::degrees() {

	return (radians * 180 / PI);
}
void entity::accX (long double radians, long double acc) {

	Vx += cos (radians) * acc / mass;
}
void entity::accY (long double radians, long double acc) {

	Vy += sin (radians) * acc / mass;
}

void entity::draw() {

	circlefill (buffer, x - camera.x, y - camera.y, radius * camera.zoom, fillColour ); //draws the entity to the buffer
}

void body::draw() {

	circlefill (buffer, a(), b(), radius * camera.actualZoom() + atmosphereHeight, atmosphereColour);   //draws the atmosphere to the buffer

<<<<<<< HEAD
	circlefill (buffer, a(), b(), radius * camera.actualZoom(), fillColour ); //draws the entity to the buffer
=======
    circlefill (buffer, a(), b(), radius * camera.actualZoom(), fillColour); //draws the entity to the buffer
>>>>>>> 323a06e4b8695ab55428290337b0848bccc53909
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

void entity::turn () {

	turnRadians += turnRate;

	if (turnRadians < 0)
		turnRadians += 2 * PI;

	if (turnRadians > 2 * PI)
		turnRadians -= 2 * PI;
}

void detectCollision () {

	if (craft[HAB].distance (planet[EARTH].x, planet[EARTH].y) + craft[HAB].Vx + craft[HAB].Vy < craft[HAB].radius + planet[EARTH].radius) {  //if the entity will touch the earth next move
//        craft[HAB].Vx = -craft[HAB].Vx;  //this will make the entity gradually slow down, instead of suddenly stopping
//        craft[HAB].Vy = sin ;
		craft[HAB].Vx = 0;
		craft[HAB].Vy = 0;
//        craft[HAB].engine = 0;
	}
}
//		planet[MARS].draw();
float viewpoint::actualZoom() {

	return (pow (zoomMagnitude, zoom) );
}

float entity::a() { //on-screen x position of entity

	return ( (x - camera.x) * camera.actualZoom() );
}

float entity::b() { //on-screen y position of entity

	return ( (y - camera.y) * camera.actualZoom() );
}

void entity::gravitate (struct entity object) { //calculates gravitational forces, and accelerates, between two entities

<<<<<<< HEAD
	float theta = atan2f (craft.y - y, craft.x - x);    //finds angle at which hab is from earth
	float gravity = G * (craft.mass * mass) / (distance (craft.x, craft.y) * distance (craft.x, craft.y) ); //finds total gravitational force between hab and earth, in the formula G (m1 * m2) / r^2

	accX (theta, -gravity);
	accY (theta, -gravity);

	craft.accX (theta, gravity);
	craft.accY (theta, gravity);
=======
//    float theta = atan2f (object.y - y, object.x - x);    //finds angle at which hab is from earth
    float theta = PI;
//    float gravity = G * ( (object.mass * mass) / (distance (object.x, object.y) * distance (object.x, object.y) ) ); //finds total gravitational force between hab and earth, in the formula G (m1 * m2) / r^2
    float gravity = 2.3;

//    accX (PI, 1);
//    accY (PI, 5);

//    object.accX (PI, gravity);
//    object.accY (PI, gravity);
>>>>>>> 323a06e4b8695ab55428290337b0848bccc53909
}

float entity::distance (float targetX, float targetY) { //finds distance from entity to target

	return (sqrtf ( ( (targetX - x) * (targetX - x) ) + ( (targetY - y) * (targetY - y) ) ) ); //finds the distance between two entities, using d = sqrt ( (x1 - x2)^2 + (y1 - y2) )
}

void drawGrid () {  //draws a grid to the screen, later on I will be making gravity distort it

	unsigned int n;

	for (n = 0; n < screenWidth; n++)
		line (buffer, n * 20 * fabs (camera.zoom), 0, n * 20 * fabs (camera.zoom), screenHeight, makecol (100, 100, 100) );

	for (n = 0; n < screenHeight * camera.actualZoom(); n++)
		line (buffer, 0, n * 20 * fabs (camera.actualzoom), screenWidth, n * 20 * fabs (camera.zoom), makecol (100, 100, 100) );
}
