/*******************
Allegro Newbie Tutorial

by
LoomSoft

http://loomsoft.net
email: jay@loomsoft.net
*******************/
/* Allegro Newbie Tutorial : Lesson 2 - Bitmaps
 * This lesson will initialize allegro, set up keyboard support,
 * show a single bitmap, then wait for a keypress. Upon the keypress
 * the program will then quit.
 */

// Here is your first line. You must include the allegro header file or else nothing will work!
#include <allegro.h> // You must include the Allegro Header file

/* Our generic main function. */
int main(int argc, char *argv[])
{

    // Now we want to initialize allegro just as we did in our last lesson.
    allegro_init(); // Initialize Allegro

    // Now we want to install the keyboard just as we did in our last lesson.
    install_keyboard(); // Initialize keyboard routines

    /* This next step is something new. When you actually draw bitmaps to the screen you must
     * specify a color depth that the screen will have.
     *
     * set_color_depth will change the current color depth.
     *
     * Available depths are 8,15,16, 24, and 32.
     * A color depth of 8 requires a palette to be loaded with each bitmap, so we will not use it.
     * A color depth of 32 can use a lot of resources, so we will either use 15 or 16 in order to ensure
     * maximum compatabiliy between machines.
     *
     * Make sure that you change the color depth before you change the screen resolution.
     * If you fail to do so, your bitmaps will not display with the correct colors!
     */
    set_color_depth(desktop_color_depth()); // Set the color depth
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640,480,0,0); // Change our graphics mode to 640x480

    /* This is the BITMAP structure.
     * Every time you want to create an image to display, or to draw to,
     * you must declare it with BITMAP, just like when you want an integer,
     * you declare it with int. Since BITMAP in the allegro libarary is a pointer,
     * every instance of a bitmap you create must be preceded by a *
     */
    BITMAP *my_pic = NULL; // Declare a BITMAP called my_pic, setting it's initial value to NULL

    /* Now, the bitmap my_pic is "empty". In order to load a picture into it,
     * you must call the function load_bitmap();
     *
     * Next, we will load a bitmap image from the hard disk into the bitmap my_pic.
     *
     * Here are what the parameters mean:
     *	load_bitmap("Location of the bitmap", palette);
     *
     * In place of the palette, we will pass NULL - meaning that no palette is used/needed.
     *
     * We can't just call the function load_bitmap by itself, however.
     * We must specify that we are loading the bitmap into the memory of my_pic. It is done like so:
     * (note, the bitmap must actually exist at this location. If not, the program will crash when
     * trying to load it. Functions exist to check for such errors, but they will be left out for
     * now to keep this lesson ultra-simple)
     */
    my_pic = load_bitmap("rainbow.bmp", NULL); // Load our picture

    if (my_pic == NULL)
        allegro_message("allegro failed to load bitmap");
    else
        allegro_message("allegro successful");

    allegro_message("Paul is a stoopid");
    allegro_message("Luchia is a stoopid");
    /* Now that our bitmap is loaded into memory, we want to display it onto the screen.
     * To do this we use a function called blit. Here are the parameters:
     *
     * blit(bitmap to draw, destination bitmap (usually the screen), clip x, clip y, x position, y position, width, height);
     *
     * As you can see, blit offers a lot of functionality.
     * The clipx and clipy will be the upper left hand corner of the bitmap that you want to copy.
     * For instance, if you use 0,0 - you will get the bitmap from the top left corner exactly.
     * However, if you use 50,50 - you will get the bitmap using the position 50,50 as the upper left hand
     * corner. You also need to specity the width and height. This will allow you to clip the image from the
     * other sides. Don't worry, we will demonstrate all the capabilities of blitting in the upcoming lessons.
     *
     * Now, lets just display the entire bitmap in the upper left hand corner of the screen
     */
    line(screen, 0, 0 , 480, 360, 0x100);

    blit(my_pic, screen, 0,0,0,0,480,360); //Draw the whole bitmap to the screen at (0,0)

    readkey();
    //  allegro_message("allegro failed to initialize");
    /* We now want to wait for a keypress before we exit the program. */
    readkey();// Wait untill a key is pressed
    /* The next step is critical before exiting the program.
     * The destroy_bitmap function is used to clear out the space in memory that a bitmap was utilizing.
     * Make sure you destroy all bitmaps that you used in your program, so garbage data doesn't stay behind in memory.
     */
    destroy_bitmap(my_pic); //Release the bitmap data


    /* Return 0 at the end of main, indicating a exit without errors */
    return 0; // Exit with no errors
}
/* Add the allegro specific END_OF_MAIN() */
END_OF_MAIN() // This must be called right after the closing bracket of your MAIN function.
// It is Allegro specific.
