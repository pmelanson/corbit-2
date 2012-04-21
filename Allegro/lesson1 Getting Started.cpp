/*******************
Allegro Newbie Tutorial

by
LoomSoft

http://loomsoft.net
email: jay@loomsoft.net
*******************/
/* Lesson 1 - Getting Started
 * This lesson will initialize allegro, set up keyboard support, show an empty screen and wait for a keypress - then quit.
 */

/* Here is your first line. You must include the allegro header file or else nothing will work! */
#include <allegro.h> // You must include the Allegro Header file

/* Make sure your main function always looks like the following.
 * Don't worry if you aren't used to it, soon enough you will memorize this.
 */
int main(int argc, char *argv[])
{

	/* Next, we will initialize Allegro.
	 * This will set up all the components it needs in order to run.
	 * This function basically does all the hard stuff for you.
	 */
	allegro_init(); // Initialize Allegro

	/* Now that allegro is initialized we will want to set up an input device so the
	 * user will be able to control how the program will execute. For this program we
	 * want to include support only for the keyboard. This function will set up all
	 * keyboard related functions and variables.
	 */
	install_keyboard(); // Initialize keyboard routines

	/* Ok. Almost everything is set up and ready to roll.
	 * Now that the keyboard is initialized we will want to set up the graphics mode. This next part can be a little
	 * tricky, and may take some time to get used to.
	 *
	 *	set_gfx_mode will set the graphics mode that you will use in your program. Here is what each paramater is:
	 *
	 *	set_gfx_mode(GRAPHICS MODE, Width, Height, Virtual Width, Virtual Height);
	 *
	 * - Using GFX_AUTODETECT for a graphics mode will set everything for you, using the machine's preinstalled drivers.
	 * - The width and the height are simple that, the width and the height of the screen.
	 * - The V_Width and V_Height are the virtual (not visible) width and height of the screen.
	 *   (We don't need to worry about these for the moment, so entering zeros will just create NO virtual screen.)
	 */
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640,780,0,0); // Change our graphics mode to 640x480


	/* This almost concludes Lesson 1. Now the screen will change to the desired
	 * resolution. In order to see the effect, we need to stall the program for a
	 * moment. This next function simply waits for any keypress from the keyboard.
	 * Once a key is presed it will then continue through the program.
	 */
   allegro_message("You just ran your first Allegro Program");
	//readkey();// Wait untill a key is pressed  This is an alternate way of pausing the program

	// After the key is pressed, we must tell the program that everything executed without error and then exit.
	return 0; // Exit with no errors
}

/* We are not done yet, however. There is still one more thing that needs to be done.
 * This part is very important. In every allegro program you write you must include this
 * function. The following function is Allegro specific - and your programs will not work
 * unless it is found right after the closing bracket of your main function.
 */
END_OF_MAIN() // This must be called right after the closing bracket of your MAIN function.
			  // It is Allegro specific.
