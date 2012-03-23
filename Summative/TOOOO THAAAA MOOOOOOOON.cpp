/*******************
CORBIT Program

Designed by Patrick Melanson

Started on 23/03/2012
Last updated on DD/03/2012


Description

*******************/

/*******************
     CHANGELOG
*******************/

#include <stdlib.h>
#include <iostream>
using namespace std;

//globals

//declarations



int main(int argc, char *argv[]) {


	/**
	Allegro-related initializations
	**/
	allegro_init();
	install_keyboard();
	install_timer();

	LOCK_VARIABLE(speed_counter);
	LOCK_FUNCTION(increment_speed_counter);
	install_int_ex(increment_speed_counter, BPS_TO_TIMER(60));

	set_color_depth(desktop_color_depth());

	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640,480,0,0);

	BITMAP *buffer = create_bitmap(640,480);

	if(buffer == NULL)
	{
		set_gfx_mode(GFX_TEXT,0,0,0,0);
		allegro_message("Could not create buffer!");
		exit(EXIT_FAILURE);
	}


}
