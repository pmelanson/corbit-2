/*******************
Allegro Newbie Tutorial

by
LoomSoft

http://loomsoft.net
email: jay@loomsoft.net
*******************/
/* Allegro Newbie Tutorial : Lesson 6 - Using Timers
 *
 * In this lesson we will learn how to use timers in order
 * to make Allegro programs run at the same speed on any system
 */

#include <allegro.h> // You must include the Allegro Header file

/* This is the timer function and integer.
 * Since they will be used throughout the entire program, basically
 * being tested each loop, we will just declare them globally.
 * There really isn't any waste of memory seeing as how they are used so often.
 */
volatile long speed_counter = 0; //A long integer which will store the value of the
//speed counter.

void increment_speed_counter() { //A function to increment the speed counter
    speed_counter++; // This will just increment the speed counter by one. :)
}
END_OF_FUNCTION (increment_speed_counter); //Make sure you tell it that it's the end of the
//function

/* Set up the general main function, and init allegro. */
int main (int argc, char *argv[]) {
    allegro_init(); // Initialize Allegro
    install_keyboard(); // Initialize keyboard routines

    /* You need to tell allegro to install the timer routines in order to use timers. */
    install_timer(); // Initialize the timer routines

    /* Ok, now heres some new code. First, we need to, as the function suggests,
     * lock our speed counter. Then, we need to lock the function which increments
     * the speed counter. Locking is used maily for historical reasons (eg: Pure DOS
     * Allegro applications) It doesn't hurt to lock the variable and function anyway
     * even if we're not running DOS only mode.
     */
    LOCK_VARIABLE (speed_counter); //Used to set the timer - which regulates the game's
    LOCK_FUNCTION (increment_speed_counter); //speed.

    /* Next, we need to tell the computer to keep incrementing the speed counter
     * independent of what the program is currenlty doing with the following function.
     * We will also tell it how fast to go. The argument to the function BPS_TO_TIMER
     * will tell the computer how many 'beat's per second. Experiment with this number
     * to get the overall 'speed' you would like in your game. 60 works well, generally.
     */
    install_int_ex (increment_speed_counter, BPS_TO_TIMER (60) ); //Set our BPS

    /* This next chunk of code is exactly the same as lesson 5. Create a buffer,
     * and a bitmap (and load an image into the bitmap).
     */
    set_color_depth (desktop_color_depth() ); // Set the color depth
    set_gfx_mode (GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0); // Change our graphics mode to 640x480

    BITMAP *my_pic; //Declare a BITMAP called my_pic
    my_pic = load_bitmap ("picture.bmp", NULL); // Load our picture

    BITMAP *buffer; //Declare a BITMAP called buffer.

    buffer = create_bitmap (640, 480); //Create an empty bitmap.

    /* Declare some integers for later use */
    int my_pic_x = 0; // Holds our pictures X coorinate
    int my_pic_y = 0; // Holds our picture's Y coordinate

    while (!key[KEY_ESC]) { //If the user hits escape, quit the program

        /* Here is the last part of the timer routines. We want to separate the logic
         * and the drawing portions of the program, so we crate a while loop to test
         * for the value of speed counter

         * While the value is greater than 0 it will do the logical part, and once it
         * drops down to equal to (or lower than) 0, it will draw the screen. Generally
         * the values of speed_counter fluctuate only between 0 and 1
         *
         * What this does is allow for the computer to do the all the logical part of the
         * program before it blits to the screen, so you don't miss any keypresses, etc.
         */
        while (speed_counter > 0) {
            /* This part of the code, the "series of tests" is considered the logical
             * part of the program. It figures out all calculations, etc. The non-logical
             * part of the program would be any image blitting, or sprite(ing). Therefore,
             * since it is a logical part of the program, it will be within the while loop
             * that tests to see if the counter is greater than 0.
             */

            /* Here is a series of tests - so we can move our bitmap around the screen */
            if (key[KEY_RIGHT] && my_pic_x < 610) // If the user hits the right key, change the picture's X coordinate
                my_pic_x ++;// Moving right so up the X coordinate by 1
            if (key[KEY_LEFT] && my_pic_x > 0) // Ditto' - only for left key
                my_pic_x --;// Moving left, so lower the X coordinate by 1
            if (key[KEY_UP] && my_pic_y > 0) // If the user hits the up key, change the picture's Y coordinate
                my_pic_y --;// Moving up, so lower the Y coordinate by 1
            if (key[KEY_DOWN] && my_pic_y < 448) // Ditto' - only for down
                my_pic_y ++;// Moving down, so up the Y coordinate by 1


            /* At the end of your logic loop you will want to tell the program to decrease
             * your speed_counter variable. If the program passes through this loop enough times,
             * then it can do all your drawing sequences without missing any keypresses!
             * Note, however, that if your logical portion of the code takes so long and the speed_counter
             * variable is increased (via our timer incrementing funciton) more times than it is
             * decreased, we will remain in the logical part of the code infinitely.
             */
            speed_counter --;
        } // This is the closing bracket for the (speed_counter > 0) test

        /* Notice that we do all blitting and drawing in this next section of code.
         * We only want to draw when the speed counter is less than 0, when the computer
         * is finished doing enough logic. Make sure you put it before the closing bracket
         * for the (key[KEY_ESC]) test, or else nothing will be drawn (until you hit Escape,
         * and then the program will shut down so fast you still won't be able to see anything!
         */
        draw_sprite (buffer, my_pic, my_pic_x, my_pic_y); //Draw the picture to the buffer
        blit (buffer, screen, 0, 0, 0, 0, 640, 480); //Draw the buffer to the screen
        clear_bitmap (buffer); // Clear the contents of the buffer bitmap
        release_screen();// Release it
    } // This is the closing bracket for the (key[KEY_ESC]) test

    /* Do all the destroying, finishing parts of the program. */
    destroy_bitmap (my_pic); //Release the bitmap data
    destroy_bitmap (buffer); //Release the bitmap data
    return 0; // Exit with no errors
}
END_OF_MAIN() // This must be called right after the closing bracket of your MAIN function.
// It is Allegro specific.
