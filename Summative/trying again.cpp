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
#include <stdlib.h>
using namespace std;

//globals
int screenWidth = 800;
int screenHeight = 800;
BITMAP *buffer = NULL;
volatile long timer = 1;
const fixed PI = ftofix (3.14159265);

struct ship {

    <<< <<< < HEAD
    float accX; //the circle's acceleration (m/s/s)
    float accY; //''
    float Vx;   //the circle's speed (m/s)
    float Vy;   //''
    fixed turnRate; //rate at which the hab turns
    int radius;
    int x;  //the center of the circle
    int y;  //''
    float acc;
    float radians();
    float degrees;  //normal degrees (360 in a circle)
    void move ();
    void accelerate ();
    void debug();

};

== == == =
    float accX(); //the circle's acceleration (m/s/s)
float accY(); //''
float Vx;   //the circle's speed (m/s)
float Vy;   //''
float turnRate; //rate at which the hab turns
int radius;
float x;  //the center of the circle
float y;  //''
float acc;
float radians();
int degrees;  //normal degrees (360 in a circle)
void move ();
void accelerate ();
void debug();

};

float ship::radians() {
    return (degrees * PI / 180);
}
float ship::accX() {
    return ( cos (radians() ) * acc);
}
float ship::accY() {
    return ( sin (radians() ) * acc);
}

>>> >>> > 26c94e0219fe9ec07df82e45a3be9d04fa3ed60d
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

//			hab.accelerate();

            <<< <<< < HEAD
//            hab.accelerate();
            hab.move();

            timer --;
        }
        hab.debug();
//        hab.acc = 0;
//        hab.accX = 0;
//        hab.accY = 0;

        textprintf_ex (buffer, font, 0, 0, makecol (255, 255, 255), -1, "DEBUG: degrees = %f", hab.degrees);

        == == == =
            hab.move();

        timer --;
    }

    hab.debug();
    >>> >>> > 26c94e0219fe9ec07df82e45a3be9d04fa3ed60d

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
        hab.degrees ++;
        if (hab.degrees > 360)
            hab.degrees -= 360;
    }

    if (key[KEY_D]) {
        hab.degrees --;
        if (hab.degrees < 0)
            hab.degrees += 360;
    }

    if (key[KEY_W]) {
        hab.acc ++;
        hab.accelerate();
    }

    <<< <<< < HEAD
    if (key[KEY_S]) {
        hab.acc --;
        hab.accelerate();
    }

    if (key[KEY_UP])
        hab.y --;

    if (key[KEY_DOWN])
        hab.accY = 5;

    == == == =
    if (key[KEY_W]) {
        hab.acc += 0.1;
        hab.accelerate();
    }

    if (key[KEY_S]) {
        hab.acc -= 0.1;
        hab.accelerate();
    }

    >>> >>> > 26c94e0219fe9ec07df82e45a3be9d04fa3ed60d
}
END_OF_FUNCTION (input);

void drawHab () {

    circlefill (buffer, hab.x, hab.y, hab.radius, makecol (0, 255, 0) ); // Draw the picture to the buffer
    line (buffer, hab.x, hab.y, //draws the 'engine'
          hab.x + 50 * cos (hab.radians() ),
          hab.y + 50 * sin (hab.radians() ),
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

    <<< <<< < HEAD
    x += Vx;
    y += Vy;

    acc -= acc;
    == == == =
        if (x - hab.radius < 0 - Vx || x + hab.radius > screenWidth - Vx)
            Vx = -Vx + (0.8 * Vx);

    if (y - hab.radius < 0 - Vy || y + hab.radius > screenHeight - Vy)
        Vy = -Vy + (0.8 * Vy);

    x += Vx;
    y += Vy;
    >>> >>> > 26c94e0219fe9ec07df82e45a3be9d04fa3ed60d
}
END_OF_FUNCTION (ship::move);

void ship::accelerate() {

    <<< <<< < HEAD
    accX = fixtof (fcos (degrees) );
    accY = fixtof (fsin (degrees) );

    Vx += accX;
    Vy += accY;
    == == == =
        Vx += accX();
    Vy += accY();

    acc = 0;
    >>> >>> > 26c94e0219fe9ec07df82e45a3be9d04fa3ed60d
}
END_OF_FUNCTION (ship::accelerate);

void ship::debug() {

    <<< <<< < HEAD
    textprintf_ex (buffer, font, 0, 20, makecol (255, 255, 255), -1, "DEBUG: is working");
    textprintf_ex (buffer, font, 0, 30, makecol (255, 255, 255), -1, "DEBUG: acc: %f", fixtof (acc) );
    textprintf_ex (buffer, font, 0, 40, makecol (255, 255, 255), -1, "DEBUG: accX: %f", fixtof (accX) );
    textprintf_ex (buffer, font, 0, 50, makecol (255, 255, 255), -1, "DEBUG: accY: %f", fixtof (accY) );
    textprintf_ex (buffer, font, 0, 60, makecol (255, 255, 255), -1, "DEBUG: Vx: %f", fixtof (Vx) );
    textprintf_ex (buffer, font, 0, 70, makecol (255, 255, 255), -1, "DEBUG: Vy: %f", fixtof (Vy) );
    == == == =
        textprintf_ex (buffer, font, 0, 0, makecol (255, 255, 255), -1, "DEBUG: is working");
    textprintf_ex (buffer, font, 0, 10, makecol (255, 255, 255), -1, "DEBUG: degrees = %d", degrees );
    textprintf_ex (buffer, font, 0, 20, makecol (255, 255, 255), -1, "DEBUG: radians = %f", radians() );
    textprintf_ex (buffer, font, 0, 30, makecol (255, 255, 255), -1, "DEBUG: acc: %f", acc);
    textprintf_ex (buffer, font, 0, 40, makecol (255, 255, 255), -1, "DEBUG: accX: %f", accX() );
    textprintf_ex (buffer, font, 0, 50, makecol (255, 255, 255), -1, "DEBUG: accY: %f", accY() );
    textprintf_ex (buffer, font, 0, 60, makecol (255, 255, 255), -1, "DEBUG: Vx: %f", Vx);
    textprintf_ex (buffer, font, 0, 70, makecol (255, 255, 255), -1, "DEBUG: Vy: %f", Vy);
    >>> >>> > 26c94e0219fe9ec07df82e45a3be9d04fa3ed60d
}
END_OF_FUNCTION (ship::debug() );

float ship::radians() {

    return (degrees * PI / 180);

}
