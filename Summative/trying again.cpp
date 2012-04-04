/*******************
CORBIT Program

Designed by Patrick Melanson

Started on 23/03/2012
Last updated on DD/03/2012


This program provides a realistic space flight simulation
The player's ship is called the "Hab"
Forces are as realistic as possible

*******************/

/*******************
     CHANGELOG

23/03/2012: Started program, we'll have some fun times with this yet
24/03/2012: Added momentum physics with WASD keys, and bouncy collision detection off sides. Collision is a bit glitchy
25/03/2012: Added turning physics, looked at http://www.helixsoft.nl/articles/circle/sincos.htm <--- SO COOL, fixed collision glitchiness
26/03/2012: Rewrote to include fixeds and ints, and tidy up code
29/03/2012: Rewrote to take out fixeds, they are silly
31/03/2012: Built project with AutoVersioning, will autogenerate changelog now. Seeya!

*******************/

#include <allegro.h>
#include <math.h>
#include "version.h"
using namespace std;

//globals
int screenWidth = 800;
int screenHeight = 800;
BITMAP *buffer = NULL;
volatile long timer = 0;
const float PI = 3.14159265;

struct pilotable {  //refers to data in relation to objects that are pilotable (e.g. engine level, fuel left, etc.)

	float engine;   //engine level of the ship

};

struct body {   //stores

    int atmosphereHeight;
    int atmosphereDrag;

};

struct object { //stores data about any physical object, such as mass and radius, acceleration, velocity, and angle from right

	int mass, radius;   //mass of object, to be used in calculation F=ma, and radius of object
	float x, y; //the center of the object
	void move ();   //moves object

	float acc;  //net acceleration of object
	float radians();    //the degree at which the object is rotated from the right, in radians
	float accX (float acc); //the object's acceleration (m/s/s)
	float accY (float acc); //''
	float Vx, Vy;   //the object's speed (m/s)
	void accelerate (); //accelerates object by changing its velocities

	void turn ();   //turns the object
	float turnRate; //rate at which the object turns
	float degrees;  //normal degrees (360 in a circle) at which the object is rotated from facing right

	void debug();   //prints debug information for object
	void draw();    //draws object
	void clearValues();

	struct pilotable ship;
	struct body planet;

};

object hab; //the default ship, the Hawking III Habitat
object earth;   //good old earth. What would we ever do without you?

//declarations
void timeStep();
void input();
void drawHab ();
void drawBuffer ();

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

	hab.x = screenWidth / 2;
	hab.y = screenHeight / 2;
	hab.radius = 50;
	hab.mass = 20000;

	while (!key[KEY_ESC]) {

		while (timer > 0) {

			input();

			hab.accelerate();

			hab.move();

			timer --;
		}

		hab.debug();

		hab.draw();

		hab.clearValues();

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
		hab.ship.engine ++;

	}

	if (key[KEY_S]) {
//        hab.acc -= 0.1;
		hab.ship.engine --;
	}

	if (key[KEY_BACKSPACE]) {
		if (key[KEY_LSHIFT])
			hab.turnRate = 0;
		else
			hab.ship.engine = 0;
	}

	if (key[KEY_ENTER])
        hab.ship.engine = 100;


}
END_OF_FUNCTION (input);

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

	if (hab.degrees < 0)
		hab.degrees += 360;

	if (hab.degrees > 360)
		hab.degrees -= 360;

	if (x - hab.radius < 0 - Vx || x + hab.radius > screenWidth - Vx)
		Vx = -Vx + (0.8 * Vx);

	if (y - hab.radius < 0 - Vy || y + hab.radius > screenHeight - Vy)
		Vy = -Vy + (0.8 * Vy);

	x += Vx;
	y += Vy;

	debug();

	hab.acc = 0;
}
END_OF_FUNCTION (ship::move);

void object::accelerate() {

	hab.acc += hab.ship.engine / hab.mass;   //a = F / m (F=ma)

	hab.Vx += hab.accX (hab.acc);
	hab.Vy += hab.accY (hab.acc);
}
END_OF_FUNCTION (ship::accelerate);

void object::debug() {

	textprintf_ex (buffer, font, 0, 0, makecol (255, 255, 255), -1, "DEBUG: turnRate: %f", turnRate);
	textprintf_ex (buffer, font, 0, 10, makecol (255, 255, 255), -1, "DEBUG: degrees = %f", degrees );
	textprintf_ex (buffer, font, 0, 20, makecol (255, 255, 255), -1, "DEBUG: engines = %f", ship.engine );
	textprintf_ex (buffer, font, 0, 30, makecol (255, 255, 255), -1, "DEBUG: acc: %f", acc);
	textprintf_ex (buffer, font, 0, 40, makecol (255, 255, 255), -1, "DEBUG: accX: %f", accX (acc) );
	textprintf_ex (buffer, font, 0, 50, makecol (255, 255, 255), -1, "DEBUG: accY: %f", accY (acc) );
	textprintf_ex (buffer, font, 0, 60, makecol (255, 255, 255), -1, "DEBUG: Vx: %f", Vx);
	textprintf_ex (buffer, font, 0, 70, makecol (255, 255, 255), -1, "DEBUG: Vy: %f", Vy);
}
END_OF_FUNCTION (ship::debug);

float object::radians() {
	return (hab.degrees * PI / 180);
}
float object::accX (float acc) {
	return ( cos (hab.radians() ) * hab.acc);
}
float object::accY (float acc) {
	return ( sin (hab.radians() ) * hab.acc);
}

void object::draw() {

	circlefill (buffer, x, y, radius, makecol (0, 255, 0) ); // Draw the picture to the buffer
	line (buffer, x, y, //draws the 'engine'
	      x + radius * cos (radians() ),
	      y + radius * sin (radians() ),
	      makecol (255, 0, 0) );

}
END_OF_FUNCTION (ship::draw)

void object::turn () {

	degrees += turnRate;

	if (hab.degrees < 0)
		hab.degrees += 360;

	if (hab.degrees > 360)
		hab.degrees -= 360;
}

void object::clearValues() {

	hab.acc = 0;

}
