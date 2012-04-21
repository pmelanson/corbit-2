/*******************
Allegro Newbie Tutorial

  by
LoomSoft

http://loomsoft.net
email: jay@loomsoft.net
*******************/
/* Allegro Newbie Tutorial : Lesson 9 - Datafiles
 *
 *  This tutorial will demonstrate how to use allegro datafiles. Allegro Datafiles are a powerful thing that allow you to store multiple
 *  peices of data (bitmaps, midi, wavs) in a single location, so you don't have to manage creation/loading of multiple items. It's as simple
 *  as loading a datafile once, and unloading it when you're done with it.
 *
 *  In the following comments I will try to describe the process of actually *creating* the datafile. Please note that the utility for creating
 *  datafiles comes with a readme, and it will better explain how to use it than I do. If you get stuck trying to create your own datafile,
 *  load the one supplied with this tutorial and poke around.
 *
 *  The utility to create datafiles is called "grabber". It is build when you compile allegro and can be found
 *  in the "tools" directory. Go launch it up and take a look. You'll notice it's pretty basic. It contains your
 *  standard menus. Next come the "Editing, Header, Prefix, Password" text fields. Below this, to the left is a big white
 *  empty list. This is where a list of items that are in your datafile go. To the upper right is "compression" stuff. Don't worry about
 *  this for now. Below that is a box which will display information for whichever datafile item you currently have selected. Below this appears
 *  a big empty space with nothing in it. When you select an item in your datafile a preview will be shown there, if possible.
 *
 *  Ok. So now onto creating an item for your datafile. I find using the right mouse button for everything much easier than using the main menu.
 *  Move your mouse to the big white block in the left side of the grabber utility. Right click. You will see the option of "new" if you're not
 *  currently selecting an item (which you shouldn't if you started up the grabber without loading a datafile).
 *
 *  Next, open up the new menu; you'll see you have an option of creating all types of items. What we care about is bitmaps. Create a new
 *  bitmap and name it anything. You will notice that there is now a preview of your bitmap that say "Hi." This is normal. All you have done
 *  is create a bitmap structure within your datafile. Now you need to load a real bitmap's data into your structure. Do do this, right click your
 *  newly created item in the list and select "Grab..." A dialog will pop up. Select the image you want, and voila! Your bitmap now contains the image!
 *
 *  You can now save your datafile and use it for displaying this bitmap. I wont explain about other data types like sample, or MIDI, as the work
 *  in much the same way. Just remember: Create the item; then use "Grab" to put the actual data in the item. After that you're all set.
 *
 *  One thing to keep in mind before saving your datafile is whether you want to create a header file to go along with it. Header files make it easier
 *  to use datafiles in code as they will give you names to access a field in the datafile array instead of having to remember the exact location of
 *  where everything resides. The header file simply creates a #define out of the name you gave your items when creating the datafile that represents
 *  that items position in the datafile structure. This will make more sense as we check out the code. Here we go!
 *
 *  The datafile I created for this tutorial consists of only two images. I also used the "Header:" feature to create a header file out of the datafile
 *  items for use in code. *Remember*: NEVER hand edit a header file created by the grabber utility.
 */

#include <allegro.h> // Include the allegro header file.

#include "datafile_header.h" // Include the header file created by the grabber utility.

/* Timer stuff */
volatile long speed_counter = 0; // A long integer which will store the value of the
								 // speed counter.

void increment_speed_counter() // A function to increment the speed counter
{
	speed_counter++; // This will just increment the speed counter by one.
}
END_OF_FUNCTION(increment_speed_counter); // Make sure you tell it that it's the end of the
                                          // function (allegro specific)

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

	BITMAP *buffer = create_bitmap(640,480); // Create a buffer for smooth animation.

	/* Check that the buffer was created correctly */
	if(buffer == NULL)
	{
		set_gfx_mode(GFX_TEXT,0,0,0,0);
		allegro_message("Could not create buffer!");
		exit(EXIT_FAILURE);
	}

	/* Here we will learn about the DATAFILE structure.
	 * A DATAFILE structure is much like a BITMAP structure. You
	 * use this to create a pointer to a datafile, and then using
	 * that pointer you can load a datafile and then use it in your code.
	 * Let's create a datafile called dat.
	 */
	DATAFILE *my_datafile = NULL;

	/* Next, we want to actually load the datafile so we can use it.
	 * That's as simple as loading bitmaps: use the load_datafile
	 * function. All you pass to this function is the path to your
	 * datafile.
	 */
	my_datafile = load_datafile("datafile.dat");

	/* You can check for errors while loading datafiles just like
	 * with bitmaps, so lets do that here. load_datafile returns
	 * NULL on error
	 */

	/* Check that the datafile was created correctly */
	if(my_datafile == NULL)
	{
		set_gfx_mode(GFX_TEXT,0,0,0,0);
		allegro_message("Could not load datafile!");
		exit(EXIT_FAILURE);
	}

	/* Great, now we've got our datafile loaded, we can use it for
	 * whatever we want. In this tutorial I'm only going to use the
	 * datafile to draw items to the screen. Nothing else spiffy
	 * is going to happen, so we can leave the innards of the logical
	 * loop empty save the speed_counter --; line.
	 */
	while(!key[KEY_ESC]) // Keep going until we hit escape.
	{
		while(speed_counter > 0) // Do the logic loop while the speed counter is > 0.
		{
			speed_counter --; // Decrement the speed counter.
		}

		/* Here's where things get fun! Time to use our datafile.
		 * The first thing you need to know is that (to think about it
		 * in a super simplistic style) a datafile is simply an array of
		 * items. So, each item that we have put into the datafile will
		 * have a corresponding "cell" in the array. This is what that
		 * datafile I created is good for. If you have a huge datafile, you
		 * can reference your items by their name in the datafile as opposed
		 * to their position number. I'll show you both methods.
		 *
		 * First, to access any item in a datafile you do it like so
		 * my_datafile[ Item Number ].dat
		 *
		 * This represents the item numbered "Item Number" in the datafile,
		 * where my_datafile is the name that we used when we created the datafile
		 * variable using DATAFILE *. The .dat part is required to pull out
		 * the actual data in that cell.
		 *
		 * Let's draw the item at position 0 in the datafile to the screen
		 */

		/* (BITMAP*) is required before my_datafile[0].dat when
		 * using C++ to cast the item in order to prevent warnings/errors
		 * about the type of my_datafile[0].dat being unknown. Try for yourself
		 * to omit it and see if your compiler cares or not.
		 */
		draw_sprite(buffer, (BITMAP*)my_datafile[0].dat, 0, 0);

		/* Alright! So we drew the first item in the datafile to the screen.
		 * Let's try the second one!
		 */
		draw_sprite(buffer, (BITMAP*)my_datafile[1].dat, 100, 100);

		/* Easy, huh? Very. Now, what if you had hundreds of items
		 * in your datafile? How could you possibly remember the number
		 * corresponding to each one? It would be very difficult. This is
		 * why the grabber utility can create a header file for you. The header
		 * file simply creates a #define of each item's name in the datafile
		 * and sets it to the number that corresponds to that item.
		 */

		/* The names of the two bitmaps I created are
		 * little_guy
		 * and
		 * space_ship
		 *
		 * So, when I'm drawing stuff from my datafile to the buffer,
		 * I can use these names instead of numbers. Watch!
		 */
		draw_sprite(buffer, (BITMAP*)my_datafile[little_guy].dat, 200, 200);
		draw_sprite(buffer, (BITMAP*)my_datafile[space_ship].dat, 300, 300);

		/* One thing to remember when creating datafiles is the names of
		 * your items. Don't create names that are going to conflict with
		 * other variables in your code. The grabber utility will probably
		 * let you name a bitmap "int", but when you try to use a header
		 * file created by the grabber utility that tries to #define int,
		 * you're going to run into all sorts of problems.
		 */

		blit(buffer, screen, 0, 0, 0, 0, 640, 480);//Blit the buffer
		clear(buffer);//Clear the buffer
		release_screen();//Release the screen
	}

	/* Destroy all bitmaps */
	destroy_bitmap(buffer);

	/* The last thing to remember about datafiles, like bitmaps, is
	 * you need to get rid of them when you're done with them. Just
	 * like destroy_bitmap for a bitmap, unload_datafile does the
	 * same thing for a datafile. Datafiles take up space in memory
	 * when you load them, so always make sure you unload your datafiles!
	 */
	unload_datafile(my_datafile);

	return 0;
}
END_OF_MAIN()
