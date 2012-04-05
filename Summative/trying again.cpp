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
#include "version.h"

//globals
const int screenWidth = 900;
const int screenHeight = 800;
BITMAP *buffer = NULL;
volatile long timer = 0;
const float PI = 3.14159265;

//beginning of class declarations
struct object { //stores data about any physical object, such as mass and radius, acceleration, velocity, and angle from right

	int mass, radius;   //mass of object, to be used in calculation F=ma, and radius of object
	float x, y; //the center of the object
	void move ();   //moves object

	float acc;  //net acceleration of object
	float radians();    //the degree at which the object is rotated from the right, in radians
	float accX (); //the object's acceleration (m/s/s)
	float accY (); //''
	float Vx, Vy;   //the object's speed (m/s)

	void turn ();   //turns the object
	float turnRate; //rate at which the object turns
	float degrees;  //normal degrees (360 in a circle) at which the object is rotated from facing right

	void debug();   //prints debug information for object
	void draw();    //draws object
	int fillColour;
	void clearValues();

};

struct ship : object {  //stores information about a pilotable ship

	float engine;
	int engineColour;

	void accelerate();

	void debug();

	void draw();

};

struct body : object {   //stores information about an astronomical body

	int atmosphereHeight;
	int atmosphereDrag;

};

ship hab; //the default ship, the Hawking III Habitat
body earth;   //good old earth. What would we ever do without you?

//declarations
void timeStep();
void input();
void drawHab ();
void drawBuffer ();
void detectCollision ();

int main (int argc, char *argv[]) {

	//allegro initializations
	allegro_init();
	install_keyboard();
	set_color_depth (desktop_color_depth() );
	set_gfx_mode (GFX_AUTODETECT_WINDOWED, screenWidth, screenHeight, 0, 0);
	install_int_ex (timeStep, BPS_TO_TIMER (60) );

	//bitmap initializations
	buffer = create_bitmap (screenWidth, screenHeight);

	//data initializations
	earth.x = screenWidth / 2;
	earth.y = screenHeight / 2;
	earth.radius = 80;
	earth.mass = 1;
	earth.fillColour = makecol (0, 0, 255);


	hab.fillColour = makecol (0, 255, 0);
	hab.engineColour = makecol (255, 0, 0);
	hab.radius = 50;
	hab.x = screenWidth / 2 + earth.radius + hab.radius;
	hab.y = screenHeight / 2;
	hab.mass = 20000;

	while (!key[KEY_ESC]) {

		while (timer > 0) {

			input();

			hab.accelerate();

			hab.move();

			timer --;
		}

		earth.draw();

		hab.debug();
		hab.draw();
		hab.clearValues();

		detectCollision();

		drawBuffer();


	}

	//end of program
	destroy_bitmap (buffer);
	return (0);
}
END_OF_MAIN();


void input () {

	if (key[KEY_A]) {
		hab.turnRate -= 0.1;
	}

	if (key[KEY_D]) {
		hab.turnRate += 0.1;
	}

	if (key[KEY_W]) {
//        hab.acc += 0.1;
		hab.engine ++;

	}

	if (key[KEY_S]) {
//        hab.acc -= 0.1;
		hab.engine --;
	}

	if (key[KEY_BACKSPACE]) {
		if (key[KEY_LSHIFT])
			hab.turnRate = 0;
		else
			hab.engine = 0;
	}

	if (key[KEY_ENTER])
		hab.engine = 100;


}

void drawBuffer () {

	textprintf_ex (buffer, font, 0, screenHeight - 10, makecol (255, 255, 255), -1, "Corbit v%d.%d%d.%d", AutoVersion::MAJOR, AutoVersion::MINOR, AutoVersion::REVISION, AutoVersion::BUILD);

	draw_sprite (buffer, screen, 1000, 800); // Draw the buffer to the screen
	draw_sprite (screen, buffer, 0, 0);
	clear_bitmap (buffer); // Clear the contents of the buffer bitmap
}
END_OF_FUNCTION (drawBuffer);

void timeStep() {

	timer++;
}
END_OF_FUNCTION (timeStep);

void object::move() {

	degrees += turnRate;

	if (degrees < 0)
		degrees += 360;

	if (degrees > 360)
		degrees -= 360;

	if (x - radius < 0 - Vx || x + radius > screenWidth - Vx)
		Vx = -Vx + (0.8 * Vx);

	if (y - radius < 0 - Vy || y + radius > screenHeight - Vy)
		Vy = -Vy + (0.8 * Vy);

	x += Vx;
	y += Vy;

	clearValues();
}

void ship::accelerate() {

	acc += engine / mass;   //a = F / m (F=ma)

	Vx += accX ();
	Vy += accY ();
}

void ship::debug() {

	textprintf_ex (buffer, font, 0, 0, makecol (255, 255, 255), -1, "DEBUG: turnRate: %f", turnRate);
	textprintf_ex (buffer, font, 0, 10, makecol (255, 255, 255), -1, "DEBUG: degrees = %f", degrees );
	textprintf_ex (buffer, font, 0, 20, makecol (255, 255, 255), -1, "DEBUG: acc: %f", acc);
	textprintf_ex (buffer, font, 0, 30, makecol (255, 255, 255), -1, "DEBUG: accX: %f", accX () );
	textprintf_ex (buffer, font, 0, 40, makecol (255, 255, 255), -1, "DEBUG: accY: %f", accY () );
	textprintf_ex (buffer, font, 0, 50, makecol (255, 255, 255), -1, "DEBUG: Vx: %f", Vx);
	textprintf_ex (buffer, font, 0, 60, makecol (255, 255, 255), -1, "DEBUG: Vy: %f", Vy);
	textprintf_ex (buffer, font, 0, 70, makecol (255, 255, 255), -1, "DEBUG: Engines: %f", engine);
}
END_OF_FUNCTION (object::debug);

float object::radians() {
	return (degrees * PI / 180);
}
float object::accX () {
	return ( cos (radians() ) * acc);
}
float object::accY () {
	return ( sin (radians() ) * acc);
}

void object::draw() {

	circlefill (buffer, x, y, radius, fillColour ); // Draw the picture to the buffer

}
END_OF_FUNCTION (ship::draw)

void ship::draw() {

	circlefill (buffer, x, y, radius, fillColour ); // Draw the picture to the buffer
	line (buffer, x, y, //draws the 'engine'
	      x + radius * cos (radians() ),
	      y + radius * sin (radians() ),
	      engineColour );

}
END_OF_FUNCTION (ship::draw)

void object::turn () {

	degrees += turnRate;

	if (degrees < 0)
		degrees += 360;

	if (degrees > 360)
		degrees -= 360;
}

void object::clearValues() {

	acc = 0;

}

void detectCollision () {

	if ( (hab.x + hab.radius) < (earth.x + earth.radius) ) {
		hab.acc = 0;
		hab.Vx = 0;
	}

	if (TRUE)
        hab.acc = 0;

	if ( (hab.y + hab.radius) < (earth.x + earth.radius) ) {
		hab.acc = 0;
		hab.Vy = 0;
	}

}
