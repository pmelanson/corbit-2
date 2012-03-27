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




*******************/

#include <allegro.h>
#include <math.h>
using namespace std;

//globals
int screenWidth = 800;
int screenHeight = 800;
BITMAP *buffer = NULL;
volatile long timer = 1;
const fixed PI = ftofix (3.14159265);

struct ship {

    fixed accX; //the circle's acceleration (m/s/s)
    fixed accY; //''
    fixed Vx;   //the circle's speed (m/s)
    fixed Vy;   //''
    fixed turnRate; //rate at which the hab turns
    int radius;
    int x;  //the center of the circle
    int y;  //''
    fixed acc;
    fixed allegros; //allegros = allegro degrees (256 in a circle)
    fixed radians;
    int degrees;  //normal degrees (360 in a circle)
    void move ();
    void accelerate ();
    void debug();

};

ship hab;

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
    hab.x = screenWidth / 2;
    hab.y = screenHeight / 2;
    hab.radius = 50;
//    hab.debug();
    while (!key[KEY_ESC]) {

        while (timer > 0) {

            input();


//            hab.accelerate();
            hab.move();

            timer --;
        }
        hab.debug();
//        hab.acc = 0;
//        hab.accX = 0;
//        hab.accY = 0;

        textprintf_ex (buffer, font, 0, 0, makecol (255, 255, 255), -1, "DEBUG: degrees = %d", fixtoi (hab.allegros) * 360 / 256 );
        textprintf_ex (buffer, font, 0, 10, makecol (255, 255, 255), -1, "DEBUG: allegros = %d", fixtoi (hab.allegros) );


        drawHab();

        drawBuffer();

    }

    //end of program
    destroy_bitmap (buffer);
    return (0);
}
END_OF_MAIN();


void input () {

    if (key[KEY_A]) {
        hab.allegros = (hab.allegros - itofix (1) ) & 0xFFFFFF;
    }

    if (key[KEY_D]) {
        hab.allegros = (hab.allegros + itofix (1) ) & 0xFFFFFF;
    }

    if (key[KEY_W]) {
        hab.acc ++;
        hab.accelerate();
    }

    if (key[KEY_S]) {
        hab.acc --;
        hab.accelerate();
    }

    if (key[KEY_UP])
        hab.y --;

    if (key[KEY_DOWN])
        hab.accY = 5;

}
END_OF_FUNCTION (input);

void drawHab () {

    circlefill (buffer, hab.x, hab.y, hab.radius, makecol (0, 255, 0) ); // Draw the picture to the buffer
    line (buffer, hab.x, hab.y, //draws the 'engine'
          fixtoi (hab.x + 50 * fcos (hab.allegros) ) + hab.x,
          fixtoi (hab.y + 50 * fsin (hab.allegros) ) + hab.y,
          makecol (255, 0, 0) );
}
END_OF_FUNCTION (drawHab);

void drawBuffer () {

    draw_sprite (buffer, screen, 1000, 800); // Draw the buffer to the screen
    draw_sprite (screen, buffer, 0, 0);
    clear_bitmap (buffer); // Clear the contents of the buffer bitmap
}
END_OF_FUNCTION (drawBuffer);

void timeStep() {

    timer++;
}
END_OF_FUNCTION (timeStep);

void ship::move() {

    x += fixtoi(Vx);
    y += fixtoi(Vy);

    acc -= acc;
}
END_OF_FUNCTION (ship::move);

void ship::accelerate() {

    accX = fcos (allegros);
    accY = fsin (allegros);

    Vx += accX;
    Vy += accY;
}
END_OF_FUNCTION (ship::accelerate);

void ship::debug() {

    textprintf_ex (buffer, font, 0, 20, makecol (255, 255, 255), -1, "DEBUG: is working");
    textprintf_ex (buffer, font, 0, 30, makecol (255, 255, 255), -1, "DEBUG: acc: %f", fixtof(acc));
    textprintf_ex (buffer, font, 0, 40, makecol (255, 255, 255), -1, "DEBUG: accX: %f", fixtof(accX));
    textprintf_ex (buffer, font, 0, 50, makecol (255, 255, 255), -1, "DEBUG: accY: %f", fixtof(accY));
    textprintf_ex (buffer, font, 0, 60, makecol (255, 255, 255), -1, "DEBUG: Vx: %f", fixtof(Vx));
    textprintf_ex (buffer, font, 0, 70, makecol (255, 255, 255), -1, "DEBUG: Vy: %f", fixtof(Vy));
}
END_OF_FUNCTION (ship::debug() );
