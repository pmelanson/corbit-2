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




*******************/

#include <allegro.h>
#include <math.h>
using namespace std;

//globals
volatile long timer = 0;
float Vx = 0;
float Vy = 0;
float accX = 0;
float accY = 0;
float x = 500;
float y = 500;
float angle = 0;
float turnRate = 0;
//float level = 0;

//declarations
void timeStep();

int main (int argc, char *argv[]) {
    allegro_init();
    install_keyboard();
    set_color_depth (desktop_color_depth() );
    set_gfx_mode (GFX_AUTODETECT_WINDOWED, 1000, 800, 0, 0);

    BITMAP *buffer = NULL;
    buffer = create_bitmap (1000, 800);

    install_int_ex (timeStep, BPS_TO_TIMER (60) );

    while (!key[KEY_ESC]) {

        while (timer > 0) {


            if (key[KEY_RIGHT])
                turnRate ++;

            if (key[KEY_LEFT])
                turnRate --;

            if (key[KEY_W]) {
                accY --;
            }

            if (key[KEY_S]) {
                accY ++;
            }

            if (key[KEY_UP])
                y--;

            if (key[KEY_DOWN])
                y++;

            if (key[KEY_A]) {
                accX --;
            }

            if (key[KEY_D]) {
                accX ++;
            }


            if (y <= 40 || y >= SCREEN_H - 40) {
                Vy = -Vy + (0.1 * Vy);
            } else
                Vy += accY;

            if (x <= 40 || x >= SCREEN_W - 40)
                Vx = -Vx + (0.1 * Vx);
            else
                Vx += accX;


            y += Vy;
            x += Vx;

            timer--;
        }


        circlefill (buffer, x, y, 40, (makecol (0, 255, 0) ) ); // Draw the picture to the buffer
        draw_sprite (buffer, screen, 1000, 800); // Draw the buffer to the screen
        draw_sprite (screen, buffer, 0, 0);
        clear_bitmap (buffer); // Clear the contents of the buffer bitmap
        accY = 0;
        accX = 0;

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
