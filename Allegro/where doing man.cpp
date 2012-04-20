#include <allegro.h>


volatile unsigned short int timer = 0;

void timeStep () {
	timer++;
}
END_OF_FUNCTION (timeStep);

int main () {

	allegro_init(); // Initialize Allegro
	install_keyboard(); // Initialize keyboard routines
	install_timer(); // Initialize the timer routines

	LOCK_VARIABLE (timer);
	LOCK_FUNCTION (timeStep);
	install_int_ex (timeStep, BPS_TO_TIMER (600) ); //Set our BPS

	set_color_depth (desktop_color_depth() ); // Set the color depth
	set_gfx_mode (GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0); // Change our graphics mode to 640x480

	BITMAP *where = load_bitmap ("sbahj one.bmp", NULL); // Load bitmaps
	BITMAP *making = load_bitmap ("sbahj two.bmp", NULL);
	BITMAP *it = load_bitmap ("sbahj three.bmp", NULL);
	BITMAP *hapen = load_bitmap ("sbahj four.bmp", NULL);
	BITMAP *fin = load_bitmap ("sbahj fin.bmp", NULL);

	BITMAP *buffer = create_bitmap (640, 480);  // Create buffer

	unsigned short int xPos, yPos, frame = 0;   // Declare x, y positions and fram number

	while (!key[KEY_ESC]) { //If the user hits escape, quit the program
		while (timer > 0) {

			if (key[KEY_RIGHT]) // If the user hits the right key, change the picture's X coordinate
				xPos++;// Moving right so up the X coordinate by 1

			if (key[KEY_LEFT]) // Ditto' - only for left key
				xPos--;// Moving left, so lower the X coordinate by 1

			if (key[KEY_UP]) // If the user hits the up key, change the picture's Y coordinate
				yPos--;// Moving up, so lower the Y coordinate by 1

			if (key[KEY_DOWN]) // Ditto' - only for down
				yPos++;
		}

		timer--;

		frame++;

		draw_sprite (buffer, screen, 640, 480); //Draw the buffer to the screen
		clear_bitmap (buffer);
	}

    draw_sprite (fin, screen, 640, 480);

    while (!key[KEY_ESC]);

	/* Don't forget to destroy all the bitmaps we created, and do the rest of the deinitializing. */
	destroy_bitmap (where); //Release the bitmap data
	destroy_bitmap (making); //Release the bitmap data
	destroy_bitmap (it); //Release the bitmap data
	destroy_bitmap (hapen); //Release the bitmap data
	destroy_bitmap (fin);

	return (0); // Exit with no errors

}
