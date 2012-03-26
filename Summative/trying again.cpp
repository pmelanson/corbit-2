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
int Vx = 0;   //the circle's speed (m/s)
int Vy = 0;   //''
int accX = 0; //the circle's acceleration (m/s/s)
int accY = 0; //''
int x = 500;  //the center of the circle
int y = 500;  //''
fixed allegros = itofix (0);    //allegros = allegro degrees (256 in a circle)
fixed turnRate = itofix (0); //rate at which the hab turns
int degrees = 0;  //normal degrees (360 in a circle)

struct ship {

    int accX; //the circle's acceleration (m/s/s)
    int accY; //''
    int Vx;   //the circle's speed (m/s)
    int Vy;   //''
    fixed turnRate; //rate at which the hab turns
    int degrees;  //normal degrees (360 in a circle)
    int radius;

public:

    int x;  //the center of the circle
    int y;  //''
    int acc;
    fixed allegros; //allegros = allegro degrees (256 in a circle)



};

ship hab;

//declarations
void timeStep();
void move();
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
    buffer = create_bitmap (1000, 800);

    //data initializations
    hab.x = screenWidth / 2;
    hab.y = screenHeight / 2;
    hab.radius = 50;

    while (!key[KEY_ESC]) {

        while (timer > 0) {

            input();

            timer --;
        }


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











void move () {


}
END_OF_FUNCTION (move);

void input () {

    if (key[KEY_A]) {
        hab.allegros = (hab.allegros - itofix (1) ) & 0xFFFFFF;
    }

    if (key[KEY_D]) {
        hab.allegros = (hab.allegros + itofix (1) ) & 0xFFFFFF;
    }

    if (key[KEY_W]) {
        hab.acc --;
    }

    if (key[KEY_S]) {
        hab.acc ++;
    }

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
