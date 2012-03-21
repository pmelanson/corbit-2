/*******************
Allegro Newbie Tutorial

by
LoomSoft

http://loomsoft.net
email: jay@loomsoft.net
*******************/
/* Allegro Newbie Tutorial : Lesson 5 - The Double Buffering Technique
 *
 * In this lesson we will learn how to use the double buffering technique to 
 * make things look smooth. We will also learn about the key[] array as well 
 * as the clear_bitmap function.
 */

/* Set up the main initializing stuff. */
#include <allegro.h> // You must include the Allegro Header file

int main(int argc, char *argv[]) 
{ 
    allegro_init(); // Initialize Allegro 
    install_keyboard(); // Initialize keyboard routines 
    set_color_depth(desktop_color_depth()); // Set the color depth 
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640,480,0,0); // Change our graphics mode to 640x480
	
	/* Declare a bitmap my_pic and load the picture into it. */
	BITMAP *my_pic = NULL; //Declare a BITMAP called my_pic 
	my_pic = load_bitmap("picture.bmp", NULL); // Load our picture
	
	/* Declare another bitmap. This will be our 'buffer'. */
	BITMAP *buffer = NULL; //Declare a BITMAP called buffer.
	
	/* Here is a new function. As you can see, we have already declared a BITMAP called 'buffer'. 
	 * This bitmap will be our buffer which will make things look smooth. We obviously don't want 
	 * to load a bitmap into its memory -- we are just going to be using it to draw bitmaps to it. 
	 * So, we use the function create_bitmap.
	 * create_bitmap takes two parameters, 
	 *
	 *   create_bitmap(width, height);
	 *
	 * and will just create an empty bitmap pointed to by the pointer set to its return value.
	 *
	 * Pretty simple. So, lets create a bitmap with the dimensions of the screen.
	 */
	buffer = create_bitmap(640,480); //Create an empty bitmap.
	
	/* Delcare some integers for later use... */
		
	int my_pic_x = 0;// Holds our pictures X coorinate 
	int my_pic_y = 0;// Holds our picture's Y coordinate
	
	/* As you can see in this following while loop, we will be using the key[] structure. 
	 *
	 * The key[] structure is simple. You can use it for testing - such as 
	 * 
	 * if(key[KEY_UP]) 
	 * { 
	 *    do_something(); 
	 * } 
	 *
	 * For this if clause, if the user hits the UP key, the clause will execute.
	 * The same goes for a while loop, such as the one following this comment. 
	 * While the escape key is not pressed, keep looping. A list of all the keys 
	 * can be found in the Allegro documentation files
	 */
	while(!key[KEY_ESC]) //If the user hits escape, quit the program 
	{
		/* Here is a series of tests so that we can move our bitmap around the screen. 
		 * Note that if you remove the if/else if structure of them and convert them
		 * to all ifs, you will be able to move the ball in a diagonal fasion!
		 */
		
		if(key[KEY_RIGHT]) // If the user hits the right key, change the picture's X coordinate 
		{ 
			my_pic_x ++; // Moving right so up the X coordinate by 1 
		} 
		else if(key[KEY_LEFT]) // Ditto' - only for left key 
		{ 
			my_pic_x --; // Moving left, so lower the X coordinate by 1 
		}
		else if(key[KEY_UP]) // If the user hits the up key, change the picture's Y coordinate 
		{
			my_pic_y --; // Moving up, so lower the Y coordinate by 1 
		} 
		else if(key[KEY_DOWN]) // Ditto' - only for down 
		{ 
			my_pic_y ++; // Moving down, so up the Y coordinate by 1 
		}
		
		/* Now, here is where the buffering comes into play. 
		 * As you can see, there are 3 lines of code. 2 regular lines, and 1 commented out.
		 *
		 * draw_sprite(buffer, my_pic, my_pic_x, my_pic_y); 
		 * blit(buffer, screen, 0,0,0,0,640,480);
		 *
		 * //draw_sprite(screen, my_pic, my_pic_x, my_pic_y);
		 *				
		 * The first two lines do the buffering. 
		 * If you comment out those two lines, and uncomment this line:
		 *
		 * //draw_sprite(screen, my_pic, my_pic_x, my_pic_y); 
		 *
		 * The program will run without buffering.
		 *			
		 * Here is how buffering works:
		 * If you just draw everything to the screen, you will notice that you get a lot of 
		 * flicker. This is because you are drawing directly to the screen, which is updated 
		 * by the operating system at an unknown rate! Buffering gets rid of flicker because
		 * it minimizes direct drawing to the screen. If you draw each component onto the 
		 * 'buffer', and then the complete screen sized buffer to the screen: flicker is 
		 * eliminated since you draw everything at once in one pass. It's that simple. 
		 * You will also notice, in the unbuffered mode, that the ball will zoom across the 
		 * screen (if you have a relatively fast computer). Even in buffered mode, things 
		 * can move pretty fast (or slow, again depending on the speed of your computer). 
		 * This problem is fixed using timers, which will be explained in the next lesson, 
		 * so don't worry about this for now.
		 */
								
		draw_sprite(buffer, my_pic, my_pic_x, my_pic_y); // Draw the picture to the buffer 
		blit(buffer, screen, 0,0,0,0,640,480); // Draw the buffer to the screen 
		clear_bitmap(buffer); // Clear the contents of the buffer bitmap 
			
		/* Uncomment the following line, and comment out the 3 previous lines in order to view an UNBUFFERED mode  of drawing. */ 
		// draw_sprite(screen, my_pic, my_pic_x, my_pic_y); // Draw the picture to the screen 
	}
						
	/* Do all of the program exit stuff. */
	destroy_bitmap(my_pic); //Release the bitmap data 
	destroy_bitmap(buffer); //Release the bitmap data 
	return 0; // Exit with no errors 
} 
END_OF_MAIN() // This must be called right after the closing bracket of your MAIN function. 
			  // It is Allegro specific.
