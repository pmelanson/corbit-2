/*******************
Allegro Newbie Tutorial

by
LoomSoft

http://loomsoft.net
email: jay@loomsoft.net
*******************/
/* Allegro Newbie Tutorial : Lesson 7 - Simple Sprite Animation
 *
 * This tutorial will just play a simple animation over and over again, 
 * regarless of what key you are pressing, or in what direction you are 
 * moving. It will keep playing even if you don't press anything.
 * To acheive different effects, you could create different variables to 
 * hold the states as to what direction the user is heading, or whether the 
 * user is even moving at all!
 */

/* This stuff should be pretty much memorized by now: */
#include <allegro.h> // You must include the Allegro Header file

/* Timer stuff */
volatile long speed_counter = 0; //A long integer which will store the value of the
								 //speed counter.

void increment_speed_counter() // A function to increment the speed counter
{
	speed_counter++; // This will just increment the speed counter by one.
}
END_OF_FUNCTION(increment_speed_counter);// Make sure you tell it that it's the end of the
										 // function

int main(int argc, char *argv[])
{
	allegro_init(); // Initialize Allegro
	install_keyboard(); // Initialize keyboard routines
	install_timer(); // Initialize the timer routines
	
	LOCK_VARIABLE(speed_counter); //Used to set the timer - which regulates the game's
	LOCK_FUNCTION(increment_speed_counter); //speed.
	install_int_ex(increment_speed_counter, BPS_TO_TIMER(60)); //Set our BPS
			
	set_color_depth(desktop_color_depth()); // Set the color depth
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640,480,0,0); // Change our graphics mode to 640x480
			
	/* Here we create 3 bitmaps. One bitmap per frame of animation, and we have a total of
	 * three different frames for the animation, so hence: frame1, frame2, and frame3.
	 */
	BITMAP *frame1 = load_bitmap("frame1.bmp", NULL); // Declare a bitmap and Load our picture in one single statement
	BITMAP *frame2 = load_bitmap("frame2.bmp", NULL); 
	BITMAP *frame3 = load_bitmap("frame3.bmp", NULL);
			
	/* This code is the same as in the previous lesson. Create the buffer, and location
	 * holders for the position of the frames.
	 */
	BITMAP *buffer = create_bitmap(640,480); // Declare and create an empty bitmap in one statment.
			
	/* Declare some integers for later use */
	int my_pic_x = 0; // Holds our pictures X coorinate
	int my_pic_y = 0; // Holds our picture's Y coordinate
					
	/* Here is a simple integer that we are going to increment on each logic loop,
	 * therefore it will be incremented at the speed of the speed counter, which 
	 * (in this program) is set at 60bps. That means when our frame_counter == 60,
	 * one full second has passed. Keep that in mind, as you will see how we use it later on.
	 */
	int frame_counter = 0;// A counter for which frame to draw
						
	/* This code stays the same as the previous lessons */
	while(!key[KEY_ESC])//If the user hits escape, quit the program
	{
		while(speed_counter > 0)
		{
			if(key[KEY_RIGHT])// If the user hits the right key, change the picture's X coordinate
			{
				my_pic_x ++;// Moving right so up the X coordinate by 1
			}
			else if(key[KEY_LEFT])// Ditto' - only for left key
			{
				my_pic_x --;// Moving left, so lower the X coordinate by 1
			} 
			else if(key[KEY_UP])// If the user hits the up key, change the picture's Y coordinate
			{
				my_pic_y --;// Moving up, so lower the Y coordinate by 1
			}
			else if(key[KEY_DOWN])// Ditto' - only for down
			{
				my_pic_y ++;// Moving down, so up the Y coordinate by 1
			}
								
			speed_counter --;
									
			/* Increment our frame counter at the same speed of the speed counter. */
			frame_counter ++;
									
			/* Here we test to see if the frame counter is over 4 seconds. 
			 * If it is, we set it back to zero in order to restart the animation. We are only 
			 * testing for four seconds because we are only drawing 1 frame a second, and we're
			 * drawing a total of four frames. The next big chunk of explanation will clear 
			 * up any confusion regarding this. 
			 */
										
			if(frame_counter > 240) // 60 * 4 = 240 (When frame counter = 60, 1 second has passed)
			{
				frame_counter = 0;
			}
			
		} //Closing bracket for the while(speed_counter > 0) statment
												
		/* Here is where things get a little tricky. There are multiple ways to do this, 
		 * but I'll show you the one requiring the least lines of code.
		 *
		 * As you read earlier, when frame_counter == 60, one second
		 * has passed, so we will draw each frame at one second.
		 */
											
		// In the first second, draw the first frame
		if(frame_counter < 60) // Less than a full second
		{
			draw_sprite(buffer, frame1, my_pic_x, my_pic_y);//Draw the first frame
		}
		else if(frame_counter >= 60 && frame_counter < 120)//Between 1 and 2 seconds
		{
			draw_sprite(buffer, frame2, my_pic_x, my_pic_y);//Draw the second frame
		}
		else if(frame_counter >= 120 && frame_counter < 180)//If we are between 2 and 3 seconds
		{
			draw_sprite(buffer, frame1, my_pic_x, my_pic_y);//Draw the first frame again,
															//to acheive better effect
		}
		else // If we are over 3 seconds
		{
			draw_sprite(buffer, frame3, my_pic_x, my_pic_y); //Draw the last frame.
		}
		
		/* End of the drawing portion -- same as the previous lesson's */
		blit(buffer, screen, 0,0,0,0,640,480); //Draw the buffer to the screen
		clear_bitmap(buffer);
	}
								
	/* Don't forget to destroy all the bitmaps we created, and do the rest of the deinitializing. */
	destroy_bitmap(frame1); //Release the bitmap data
	destroy_bitmap(frame2); //Release the bitmap data
	destroy_bitmap(frame3); //Release the bitmap data
	destroy_bitmap(buffer); //Release the bitmap data 
											
	return(0); // Exit with no errors
}
END_OF_MAIN()
