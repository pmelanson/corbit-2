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




*******************/

#include <allegro.h>
#include <math.h>
using namespace std;

//globals

BITMAP *buffer = NULL;
volatile long timer = 0;
float Vx = 0;   //the circle's speed (m/s)
float Vy = 0;   //''
float accX = 0; //the circle's acceleration (m/s/s)
float accY = 0; //''
float x = 500;  //the center of the circle
float y = 500;  //''
fixed allegros = itofix (0);    //allegros = allegro degrees (256 in a circle)
fixed turnRate = 0; //rate at which the hab turns
float degrees = 0;  //normal degrees (360 in a circle)
//float level = 0;

struct ship {
//public:
    float x;
    float y;
    float Vx;
    float Vy;
    float accX;
    float accY;
    fixed allegros;
    fixed turnRate;
    float degrees;
    int radius;
    float vectorLength;
    float acc;

};

//declarations
void timeStep();
//int move (float &iPos, float iAcceleration, float &V, int ScreenSize);
int move (float &x, float &y, float vectorLength, fixed theta);
int detectCollision (float &position, int &theta, int radius, int screenSize);
void print (BITMAP buffer);

ship hab;

//void ship::draw () {
//
//    circlefill (buffer, 50, 50, 50, makecol (255, 0, 0) );
//
//    circlefill (buffer, ship::x, ship::y, ship::radius, makecol (0, 255, 0) ); // Draw the ship to the buffer
//    line (buffer, ship::x, ship::y, //draws the 'engine'
//          fixtof (ship::x + ship::radius * fcos (ship::allegros) ) + ship::x,
//          fixtof (ship::y + ship::radius * fsin (ship::allegros) ) + ship::y,
//          makecol (255, 0, 0) );
//
//}
//END_OF_FUNCTION (ship::draw);

int main (int argc, char *argv[]) {

    allegro_init();
    install_keyboard();
    set_color_depth (desktop_color_depth() );
    set_gfx_mode (GFX_AUTODETECT_WINDOWED, 1000, 800, 0, 0);

    buffer = create_bitmap (1000, 800);

//    BITMAP *hab = NULL;
//    hab = load_bitmap ("hab.bmp", NULL);

    install_int_ex (timeStep, BPS_TO_TIMER (60) );

    hab.x = 500;
    hab.y = 500;

    while (!key[KEY_ESC]) {

        while (timer > 0) {


            if (key[KEY_LEFT]) {
                hab.allegros = (hab.allegros - itofix (1) ) & 0xFFFFFF;
            }

            if (key[KEY_RIGHT]) {
                hab.allegros = (hab.allegros + itofix (1) ) & 0xFFFFFF;
            }

            if (key[KEY_W]) {
                hab.acc --;
            }

            if (key[KEY_S]) {
                hab.acc ++;
            }

            if (key[KEY_UP])
                y--;

            if (key[KEY_DOWN])
                y++;

            if (key[KEY_A]) {
                hab.accX --;
            }

            if (key[KEY_D]) {
                hab.accX ++;
            }

            hab.vectorLength += hab.acc;

            move (hab.x, hab.y, hab.vectorLength, hab.allegros);

//            move (hab.x, hab.accX, hab.Vx, SCREEN_W - 50);
//            move (hab.y, hab.accY, hab.Vy, SCREEN_H - 50);

            timer--;
        }

        hab.accY = 0;
        hab.accX = 0;
        hab.acc = 0;

        textprintf_ex (buffer, font, 0, 0, makecol (255, 255, 255), -1, "%d", fixtoi (hab.allegros) * 360 / 256 );
//        draw_sprite (hab, buffer, x, y);
        circlefill (buffer, hab.x, hab.y, 50, makecol (0, 255, 0) ); // Draw the picture to the buffer
        line (buffer, hab.x, hab.y, //draws the 'engine'
              fixtoi (hab.x + 50 * fcos (hab.allegros) ) + hab.x,
              fixtoi (hab.y + 50 * fsin (hab.allegros) ) + hab.y,
              makecol (255, 0, 0) );
//        hab.draw();
        draw_sprite (buffer, screen, 1000, 800); // Draw the buffer to the screen
        draw_sprite (screen, buffer, 0, 0);
        clear_bitmap (buffer); // Clear the contents of the buffer bitmap

    }

    /* Do all of the program exit stuff. */
    destroy_bitmap (buffer); //Release the bitmap data
    return 0; // Exit with no errors
}

END_OF_MAIN()


void timeStep() {

    timer++;

}
END_OF_FUNCTION (timeStep);

int move (float &x, float &y, float vectorLength, fixed theta) {

    x += vectorLength * fixcos (theta);
    y += vectorLength * fixsin (theta);

}


//int move (float &iPos, float iAcceleration, float &V, int ScreenSize) {
//
//    if (iPos <= 50 || iPos >= ScreenSize) {
//        V = - (0.5 * V);
//
//        if (iPos <= 50)
//            iPos = 51;
//        if (iPos >= ScreenSize)
//            iPos = ScreenSize - 1;
//    }
//
//    else {
//        V += iAcceleration;
//    }
//
//    iPos += V;
//
//}
END_OF_FUNCTION (move);
