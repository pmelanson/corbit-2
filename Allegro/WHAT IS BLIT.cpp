/*******************
Allegro Newbie Tutorial

by
LoomSoft

http://loomsoft.net
email: jay@loomsoft.net
*******************/
/* Allegro Newbie Tutorial : Lesson 3 - Clipping Bitmaps & Keyboard Input
 * This lesson will do all general initialization, draw two
 * 'clipped' bitmaps, and read a key. Upon the keypress the program will then quit.
 */

/* Always include the header. */
#include <allegro.h> // You must include the Allegro Header file

/* Set up the main function, initialize allegro, set the color depth, set the graphics mode, and install the keyboard.
 * This part of your allegro programs should start getting familiar.
 */
int main(int argc, char *argv[])
{
    allegro_init(); // Initialize Allegro
    install_keyboard(); // Initialize keyboard routines
    set_color_depth(desktop_color_depth()); // Set the color depth
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640,480,0,0); // Change our graphics mode to 640x480

    /* Create a bitmap and load the picture into memory; this should be familiar as well. */
    BITMAP *my_pic = NULL; //Declare a BITMAP called my_pic
    my_pic = load_bitmap("picture.bmp", NULL); // Load our picture

    /* Here we will be blitting the same picture from lesson 2 onto the screen,
     * but this time we will blit 2 'clipped images' of it - so that we can gain
     * some experience using the other parameters of the blit function.
     *
     * The first call to blit will place the picture at (0,0), starting the 'clip' from (0,0): the upper left hand corner.
     * Notice how I changed the size (width/height) of the bitmap? Well, this will draw from (0,0) to (50,50)!
     * That is how you clip the right and bottom sides of the image.
     *
     * The second call to blit simply clips from (50,50) instead of (0,0), and will extend to (150,150) on the bitmap
     * of the picture and draws it to the coordinates (100,100).
     *
     *	Clipping is pretty easy:
     *	Set the upper left hand corner - and then set the lower right hand corner of the 'section' of the bitmap you wish to draw!
     *
     * As you can see, you can kee p drawing the same bitmap (in this case my_pic) as many times as you like.
     * Put in your own extra blit function, and blit another portion of the image.
     */

    blit(my_pic, screen, 10,10,10,10,50,50); //Draw from 0,0 to 50,50 on the screen at (0,0)
    blit(my_pic, screen, 50,50,100,100,150,150); //Draw from 50,50 to 150,150 on the screen at (100,100) release_screen();

    /* Wait for a keypress. . . */
    readkey(); // Wait untill a key is pressed

    /* and then shut down. */
    destroy_bitmap(my_pic); //Release the bitmap data
    return 0; // Exit with no errors
}
END_OF_MAIN() // This must be called right after the closing bracket of your MAIN function.
// It is Allegro specific.
