/*******************
Allegro Newbie Tutorial

by
LoomSoft

http://loomsoft.net
email: jay@loomsoft.net
*******************/
/* Allegro Newbie Tutorial : Lesson 8 - Bounding Box Collision Detection
 *
 * This tutorial will show you how to do a very simple implementation of
 * collision detection using bounding boxes. The user can move both images.
 *
 * Controls:
 * Up/Down/Left/Right : move image 1
 * A/W/S/D : move image 2
 * SPACE: draw bounding boxes.
 * ESC: exit
 *
 * This lesson also covers error checking when creating/loading bitmaps,
 * drawing primitive lines, and displaying text. This is something new that
 * was skipped in all the previous tutorials for simplicity. It is extremely
 * good practice to do error checking for any function that allows you to do so.
 *
 * We also learn about using the function: allegro_message.
 */

/* The include and timer setup, and allegro initialization. This is routine stuff! */
#include <allegro.h> // Include the allegro header file.
#include <stdlib.h> // Include the stdlib header for the exit() function.

/* Timer stuff */
volatile long speed_counter = 0; //A long integer which will store the value of the
								 //speed counter.
void increment_speed_counter() // A function to increment the speed counter
{
	speed_counter++; // This will just increment the speed counter by one.
}
END_OF_FUNCTION(increment_speed_counter); //Make sure you tell it that it's the end of the
										  //function (allegro specific)

int main(int argc, char *argv[])
{
	allegro_init(); // Initialize Allegro
	install_keyboard(); // Initialize keyboard routines
	install_timer(); // Initialize the timer routines

	LOCK_VARIABLE(speed_counter); //Used to set the timer - which regulates the game's
	LOCK_FUNCTION(increment_speed_counter);//speed.
	install_int_ex(increment_speed_counter, BPS_TO_TIMER(60));//Set our BPS

	set_color_depth(desktop_color_depth()); // Set the color depth

	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640,480,0,0); // Change our graphics mode to 640x480

	/* This bitmap is for double buffering. This is covered in a previous tutorial.
	 * However, this time, we do error checking - just to make sure the buffer was
	 * actually created properly. This is something you'll always want to do, since
	 * you can output an error and let the user know what didn't work!
	 */
	BITMAP *buffer = create_bitmap(640,480); // Create a buffer for smooth animation.

	/* We do a little test here to see if the bitmap was created. If the pointer is
	 * NULL after calling create_bitmap, then that means it's not pointing to anything,
	 * thus the create_bitmap did not work.
	 */
	if(buffer == NULL)
	{
		/* Here's some new stuff. This sets up allegro to go into a text only mode.
		 * This ensures that the message you want to display will be shown on any platform.
		 */
		set_gfx_mode(GFX_TEXT,0,0,0,0);

		/* This is a new function. It's called allegro_message. Obviously enough,
		 * it outputs a message.
		 */
		allegro_message("Error: Could not create buffer!");

		/* This function simply will stop the program since the buffer could not
		 * be created we do not want to continue.
		 */

		exit(EXIT_FAILURE);
	}

	/* Here we load the two bitmap images which we want to test collision for.
	 * We test to see they were loaded correctly. If not, output an error message
	 * and exit. This is important. If your images can't load, when you try to
	 * blit them, your program will crash. Safely exiting is the better choice, by far!
	 */
	BITMAP *image1 = load_bitmap("picture 2.bmp", NULL);//Load image 1
	if(image1 == NULL)
	{
		set_gfx_mode(GFX_TEXT,0,0,0,0); //Set the screen mode for allegro messages
		allegro_message("Error: Could not load image1.bmp");
		exit(EXIT_FAILURE);
	}

	BITMAP *image2 = load_bitmap("picture.bmp", NULL); //Load image 2
	if(image2 == NULL)
	{
		set_gfx_mode(GFX_TEXT,0,0,0,0); //Set the screen mode for allegro messages
		allegro_message("Error: Could not load image2.bmp");
		exit(EXIT_FAILURE);
	}

	/* Next we're going to do a bit of variable declaring. First off, we're going
	 * to want to know the x and y positions of both images.
	 */
	int image1_x_position = 0; //Set the x position of image 1 to 0.
	int image1_y_position = 0; //Set the y position of image 1 to 0.
	int image2_x_position = 100; //Set the x position of image 2 to 100.
	int image2_y_position = 100; //Set the y position of image 2 to 100.

	/* Next, we need to know where the bounding boxes are for these
	 * images. This is going to be a little complicated:
	 * A box (2D square) has four sides, so we will need to have 4 variables.
	 * Two y coordinates, and two x coordinates.
	 * The two y coordinates will represent the top and bottom "sides" of the box,
	 * while the x coordinates will represent the left and right "sides" of the box.
	 *
	 * For the purposes of this lesson, we will have the bounding box be the same
	 * size as the image we are going to apply it to. So, lets set these variables.
	 * We will need a total of 8. 4 for each image.
	 *
	 * Note - bb stands for bounding box
	 * Here we set the left side of the bounding box to Image 1's x position.
	 * Why do we do this? Well, we want the bounding box to surround the image at all times,
	 * so we have to set it to the images current position. The x position of the image just
	 * so happens to be the same x coordinate of the left side of the box.
	 */
	int image1_bb_left = image1_x_position;

	/* The same idea goes for the top "side" */
	int image1_bb_top = image1_y_position;

	/* Next we need to set the bottom and right "sides" of the box. Since we are going to
	 * use the bitmap's dimensions for the bounding boxes, this is simple. Since we don't want
	 * to have to keep track of the actual size of the image any time we change the image itself
	 * we can use a data member of the BITMAP structure to find out the width or height of
	 * a bitmap image dynamically! We can do this using the pointer->w or pointer->h data members.
	 *
	 * We know the width of the image. That is: (image1->w)
	 * If we have the left side of the box, well, the right side is just the image width away
	 * from the left side.
	 */
	int image1_bb_right = (image1_bb_left + image1->w);

	/* Use the same idea for the bottom. */
	int image1_bb_bottom = (image1_bb_top + image1->h);

	/* Now, we will create the box for image 2 using the same principles. */
	int image2_bb_left = image2_x_position;
	int image2_bb_top = image2_y_position;
	int image2_bb_right = (image2_bb_left + image2->w);
	int image2_bb_bottom = (image2_bb_top + image2->h);

	/* This is just a variable to hold the value of whether we want to show the bounding boxes
	 * or not. We'll see more about it later.
	 * When setting the initial value of this variable we will use an Allegro constant FALSE.
	 * There is also the constant TRUE. FALSE and TRUE are useful for doing variable testing,
	 * as they take away the ambiguity of doing something like
	 *
	 * if(variable == 0)
	 */
	int show_bbox = FALSE;

	/* This is just a variable to hold the value of whether there is a collision taking
	 * place or not. We'll see more about it later.
	 */
	int collision = FALSE;

	/* Okay, now that we've set up all our variables, we need to make our loop. You've
	 * seen this before; it should be familiar.
	 */
	while(!key[KEY_ESC]) // Keep going until we hit escape.
	{
		while(speed_counter > 0) // Do the logic loop while the speed counter is > 0.
		{
			/* Things get a little tricky in here. We want to be able to move both images.
			 * The controls are described at the beginning of this lesson. Lets make some
			 * statements to take care of moving the images.
			 *
			 * Notice how each test is a standalone "if" statement? This will allow for
			 * diagonal movement without explicitly coding it in.
			 */

			/* Key checks for image1 */
			if(key[KEY_LEFT])
				image1_x_position --; //Move image 1 left.

			if(key[KEY_RIGHT])
				image1_x_position ++; //Move image 1 right.

			if(key[KEY_DOWN])
				image1_y_position ++; //Move image 1 down.

			if(key[KEY_UP])
				image1_y_position --; //Move image 1 up.

			/* Key checks for image2 */
			if(key[KEY_A])
				image2_x_position --; //Move image 2 left.

			if(key[KEY_D])
				image2_x_position ++; //Move image 2 right.

			if(key[KEY_S])
				image2_y_position ++; //Move image 2 down.

			if(key[KEY_W])
				image2_y_position --; //Move image 2 up.

			/* Here's where we use show_bbox.
             * If the space key is held down, it will show. Otherwise, it wont.
			 * This only works when the space key is held down.
			 */
			if(key[KEY_SPACE])
				show_bbox = TRUE;

			else if(!key[KEY_SPACE])
				show_bbox = FALSE;

			/* Things get a little tricky here. Since the player can move the images,
			 * we need to update the locations of the bounding boxes. Lets do it just
			 * like we did when we initialized them, using the same ideas and principles
			 */

			/* Update image 1's bounding box */
			image1_bb_left = image1_x_position;
			image1_bb_top = image1_y_position;
			image1_bb_right = (image1_bb_left + image1->w);
			image1_bb_bottom = (image1_bb_top + image1->h);

			/* Update image 2's bounding box. */
			image2_bb_left = image2_x_position;
			image2_bb_top = image2_y_position;
			image2_bb_right = (image2_bb_left + image2->w);
			image2_bb_bottom = (image2_bb_top + image2->h);

			/* Alright, so now we finally get to actually detecting collisions.
			 * This could easily be written to it's own function, and probably should
			 * be. For the sake of simplicity, however, it's inlined here.
			 *
			 * The way we check for collisions is simple. First, we assume there is a collision.
			 * Next we do a series of checks which can determine if there is actually no
			 * collision taking place. This is quite simple, and the logic for one side
			 * of the box follows for the rest of the sides. Since we're dealing with boxes,
			 * we can say that if the bottom bound of the first box is less than the top
			 * bound of the second box, there is no way that the boxes can be colliding.
			 * That is, since the bottom bound of the first box is the absolute *lowest*
			 * point of all pixels on that box, and that point is less than the top bound
			 * of the second box (the absolute *highest*) point, there can be no other point
			 * on the first box that intersects the second (and vice versa).
			 *
			 * The same logic follows for the other three side comparisons
			 */
			collision = TRUE; // Assume that there is no collision

			// NOTE: This could easily be concatenated into one big if statement across
			//	 four 'or' clauses -- but it's split up this way for easy reading
			if(image1_bb_bottom < image2_bb_top)
			{
				collision = FALSE;
			}
			else if(image1_bb_top > image2_bb_bottom)
			{
				collision = FALSE;
			}
			else if(image1_bb_right < image2_bb_left)
			{
				collision = FALSE;
			}
			else if(image1_bb_left > image2_bb_right)
			{
				collision = FALSE;
			}

			speed_counter --; // Decrement the speed counter.
		} // This is the closing bracket to the (speed_counter > 0) test.

		/* Time for the drawing stuff! */
		draw_sprite(buffer, image1, image1_x_position, image1_y_position); //Draw image1
		draw_sprite(buffer, image2, image2_x_position, image2_y_position); //Draw image2

		/* Here we do the drawing of the bounding box if the player is hitting space. */
		if(show_bbox == TRUE)
		{
			/* This uses the simple line drawing function of allegro.
			 * It should be quite self explanitory.
			 *
			 * line(BITMAP *bmp, int x1, int y1, int x2, int y2, int color);
			 *
			 * How we draw the box is quite simple. We draw four lines:
			 *
			 * A line from the left coordinate to the right coordinate with it's y
			 * coordinate corrosponding to the top y coordinate.
			 *
			 * Another line with the same idea, except using the bottom y coordinate.
			 *
			 * A line from the top coordinate to the bottom coordinate with it's x
			 * coordinate corrosponding to the left x coordinate.
			 *
			 * Another line with the same idea, except using the right x coordinate.
			 */

			/* Draw image1's bounding box. */
			line(buffer, image1_bb_left, image1_bb_top, image1_bb_right, image1_bb_top, makecol(255,0,0));
			line(buffer, image1_bb_left, image1_bb_bottom, image1_bb_right, image1_bb_bottom, makecol(255,0,0));
			line(buffer, image1_bb_left, image1_bb_top, image1_bb_left, image1_bb_bottom, makecol(255,0,0));
			line(buffer, image1_bb_right, image1_bb_top, image1_bb_right, image1_bb_bottom, makecol(255,0,0));

			/* Draw image2's bounding box. */
			line(buffer, image2_bb_left, image2_bb_top, image2_bb_right, image2_bb_top, makecol(255,0,0));
			line(buffer, image2_bb_left, image2_bb_bottom, image2_bb_right, image2_bb_bottom, makecol(255,0,0));
			line(buffer, image2_bb_left, image2_bb_top, image2_bb_left, image2_bb_bottom, makecol(255,0,0));
			line(buffer, image2_bb_right, image2_bb_top, image2_bb_right, image2_bb_bottom, makecol(255,0,0));

		}	/* Just the closing bracket... */

		/* Now, if there's a collision, we want to state it! */
		if(collision == TRUE)
		{
			/* This is the allegro text printing function. It prints text to the screen.
			 * It's pretty self explanitory
			 *
			 * textprintf_ex(bitmap, font, x_pos, y_pos, color, transparency, "string", formatting stuff); It works just like C style printf.
			 */
			textprintf_ex(buffer, font, 0,0, makecol(255,255,255), -1, "Collision!");
		}

		/* Do the normal blitting of the buffer. */
		blit(buffer, screen, 0, 0, 0, 0, 640, 480); // Blit the buffer
		clear(buffer); // Clear the buffer
	}

	/* Destroy bitmaps and quit. */
	destroy_bitmap(buffer);
	destroy_bitmap(image1);
	destroy_bitmap(image2);
	return 0;
}
END_OF_MAIN()
