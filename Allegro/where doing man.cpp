#include <allegro.h>


volatile long timer = 0;


void timeStep() {
    timer += 2;
}
END_OF_FUNCTION (timeStep);

int main (int argc, char *argv[]) {
    allegro_init(); // Initialize Allegro
    install_keyboard(); // Initialize keyboard routines
    install_timer(); // Initialize the timer routines

    LOCK_VARIABLE (timer); //Used to set the timer - which regulates the game's
    LOCK_FUNCTION (timeStep); //speed.
    install_int_ex (timeStep, BPS_TO_TIMER (60) ); //Set our BPS

    set_color_depth (desktop_color_depth() ); // Set the color depth
    set_gfx_mode (GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0); // Change our graphics mode to 640x480

    BITMAP *sbahj[5] = {load_bitmap ("sbahj one.bmp", NULL), load_bitmap ("sbahj two.bmp", NULL), load_bitmap ("sbahj three.bmp", NULL), load_bitmap ("sbahj four.bmp", NULL), load_bitmap ("sbahj fin.bmp", NULL) }; // Declare a bitmap and Load our picture in one single statement

    BITMAP *buffer = create_bitmap (640, 480);

    float frame = 0;

    while (!key[KEY_ESC]) { //If the user hits escape, quit the program
        while (timer > 0) {

            timer --;

            frame += 0.01;

            if (frame > 4)
                frame = 0;

        }

        rectfill (buffer, 0, 0, 640, 480, makecol (255, 255, 255) );

        draw_sprite (buffer, sbahj[ int (frame) ], 0, 0);
        blit (buffer, screen, 0, 0, 0, 0, 640, 480); //Draw the buffer to the screen
        clear_bitmap (buffer);
    }

    draw_sprite (screen, sbahj[4], 0, 0);

    readkey();
    readkey();
    readkey();
    readkey();
    readkey();
    readkey();
    readkey();
    readkey();
    readkey();
    readkey();
    while (!key[KEY_ESC]);

    unsigned short int n;
    for (n = 0; n <= 4; n++)
        destroy_bitmap (sbahj[n]);
    destroy_bitmap (buffer); //Release the bitmap data

    return (0); // Exit with no errors
}
END_OF_MAIN()
