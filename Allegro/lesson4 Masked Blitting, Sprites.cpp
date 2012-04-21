/*******************
Allegro Newbie Tutorial

by
LoomSoft

http://loomsoft.net
email: jay@loomsoft.net
*******************/
/* Allegro Newbie Tutorial : Lesson 4 - Masked Blitting, Drawing Sprites, 
 * Clearing Bitmaps and the Keyboard Buffer
 *
 * In this lesson we will be blitting a masked bitmap and a sprite as well 
 * as learning how to clear a bitmap and the keybuffer.
 */

/* Initialize the general stuff. */
#include <allegro.h>  // You must include the Allegro Header file
int main(int argc, char *argv[]) 
{ 
	allegro_init(); // Initialize Allegro 
	install_keyboard(); // Initialize keyboard routines 
	set_color_depth(desktop_color_depth()); // Set the color depth 
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0); // Change our graphics mode to 640x480
	
	/* Create a bitmap, and load the picture into memory */
		
	BITMAP *my_pic; //Declare a BITMAP called my_pic 
	my_pic = load_bitmap("picture.bmp", NULL); // Load our picture
	
	/* Here we will blit a masked bitmap; that is, it contains pixels that we want will
	 * be shown as transparent. In order to make transparent parts in a bitmap, you must 
	 * use the RGB color:
	 *  Red 255 
	 *  Green 0 
	 *  Blue 255
	 *
	 * This color will be the brightest pink possible. Any pixel in a bitmap that is this 
	 * color when you draw a bitmap using masked_bitmap or draw_sprite will be transparent. 
	 * masked_blit takes the same parameters as blit, so you can perform clipping if you wish.
	 */
	masked_blit(my_pic, screen, 0,0,0,0,572,473); //Draw the whole bitmap to the screen at (0,0)
	readkey(); // Wait untill a key is pressed
			
	/* The clear_keybuf function will just clear the keybuffer of any information. 
	 * We are calling this because we don't want the program to think a key is pressed when we 
	 * call readkey for the second time. Some computers are so fast that they may draw the upcoming 
	 * sprite and think that a key is still pressed from the previous keypress. 
	 * 
	 * Note that this isn't an efficient way of preventing the next chunk of code from thinking
	 * they key is still pressed. You will notice that if you hold down the key long enough, clearing
	 * the keybuffer seemingly has no effect (it still does clear the keyboard buffer, though) 
	 * and the program will simply look like it exits.
	 * There are better methods for preventing execution of portions of code until a key is actually
	 * released. This topic is not covered in this lesson.
	 */
	clear_keybuf();
			
	/* Here we will be using two new functions. 
	 * The function clear_bitmap does exactly what its name implies: it will clear a bitmap to color 0 (black).
	 *
	 * In this case, we want to clear the screen, which is a special kind of bitmap (but a bitmap nonetheless), 
	 * 
	 * After clearing the screen, we will be using the function draw_sprite. 
	 * Draw sprite is exactly the same as masked_blit except for the fact that it has no clipping capabilities.
	 * draw_sprite() will always draw the full, masked, bitmap. Also, notice that the source bitmap and 
	 * destination bitmap are in reverse order from the blit series of functions. When calling draw_sprite put 
	 * the bitmap you wish to draw to first, and the bitmap you are actually going to be drawing second.
	 *
	 *	When you run the program, you won't be able to tell the difference between masked_blit and draw_sprite.
	 */
	clear_bitmap(screen); 
	draw_sprite(screen, my_pic, 0,0);//Draw the bitmap at 0,0. 
	readkey(); //Wait for a keypress
	
	/* Destroy the bitmap, and exit the program. */
	destroy_bitmap(my_pic); //Release the bitmap data 
	return 0; // Exit with no errors 
} 
END_OF_MAIN() // This must be called right after the closing bracket of your MAIN function. 
			  // It is Allegro specific.
