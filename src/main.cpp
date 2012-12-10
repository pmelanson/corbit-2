#include <iostream>

#define ALLEGRO_STATICLINK
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>

#include <boost/intrusive/list.hpp>
#include <boost/program_options.hpp>

//#include <eigen/dense>

#include <corbit/corbit.hpp>
using namespace std;
namespace po = boost::program_options;


ALLEGRO_DISPLAY			*display		=NULL;
ALLEGRO_EVENT_QUEUE		*event_queue	=NULL;
ALLEGRO_TIMER			*timer			=NULL;
bool					key[ALLEGRO_KEY_MAX];
unsigned				mods;

typedef boost::intrusive::list <object_c> object_list;
object_list object;

camera_c	camera		= camera_c::get_instance	(0,0, 0,0, 0,0, NULL, 1, 1);
graphics_c	graphics	= graphics_c::get_instance	(&camera);
calc_c		calc		= calc_c::get_instance		(NULL, NULL, NULL);

object_c *find_object (string name, object_list &list) {

	static object_list::iterator it;
	for (it = list.begin(); it != list.end(); ++it)
		if (it->name() == name) {
			return &*it;
		}

	return NULL;
}


bool initAllegro() {

	bool success = true;

	///initializes allegro///
	if(!al_init()) {
		cerr << "Failed to initialize Allegro!" << endl;
		success = false;
	}

	///initializes primitives addon///
	if(!al_init_primitives_addon()) {
		cerr << "Failed to initialize primitives addon!" << endl;
		success = false;
	}

	///initializes keyboard///
	if(!al_install_keyboard()) {
		cerr << "Failed to install keyboard!" << endl;
		success = false;
	}

	///initializes timer///
	timer = al_create_timer(1./FPS);
	if(!timer) {
		cerr << "Failed to create timer!" << endl;
		success = false;
	}

	///initializes display///
	ALLEGRO_DISPLAY_MODE disp_data;
	al_get_display_mode(al_get_num_display_modes()-1, &disp_data);
	al_set_new_display_flags(ALLEGRO_WINDOWED);
	display = al_create_display(disp_data.width, disp_data.height-50);
	if(!display) {
		cerr << "Failed to create display!" << endl;
		success = false;
	}

	///inhibits screensaver///
	if(!al_inhibit_screensaver(true)) {
		cerr << "Failed to inhibit screensaver!" << endl;
	}

	///initializes buffer///
	al_set_target_bitmap(al_get_backbuffer(display));

	///initializes event queue///
	event_queue = al_create_event_queue();
	if(!event_queue) {
		cerr << "Failed to create event_queue!" << endl;
		success = false;
	}

	///registers event sources///
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_clear_to_color(al_map_rgb(0,0,0));
	al_flip_display();
	al_start_timer(timer);

	return success;
}

bool initialize() {

	cout << "Corbit " << AutoVersion::STATUS << " v" << AutoVersion::MAJOR << '.' << AutoVersion::MINOR << endl;

	if(!initAllegro())
		return false;

	ALLEGRO_DISPLAY_MODE disp_data;
	al_get_display_mode(al_get_num_display_modes()-1, &disp_data);
	graphics.set_dimensions(disp_data.width, disp_data.height);

	graphics.set_center(find_object("butt", object));

	return true;
}

bool cleanup() {

	if(event_queue)
		al_destroy_event_queue(event_queue);
	if(timer)
		al_destroy_timer(timer);
	if(display)
		al_destroy_display(display);

	object.erase(object.begin(), object.end());

	return true;
}

void input() {

	if (key[ALLEGRO_KEY_PAD_MINUS])
		graphics.change_zoom(0.1);
	if (key[ALLEGRO_KEY_PAD_PLUS])
		graphics.change_zoom(-0.1);

	if (key[ALLEGRO_KEY_RIGHT])
		graphics.pan(0.5, 0);
	if (key[ALLEGRO_KEY_LEFT])
		graphics.pan(-0.5, 0);
	if (key[ALLEGRO_KEY_UP])
		graphics.pan(0, -0.5);
	if (key[ALLEGRO_KEY_DOWN])
		graphics.pan(0, 0.5);

	if (key[ALLEGRO_KEY_A])
		calc.accelerate(*calc.active_ship(), -500, 0);
	if (key[ALLEGRO_KEY_D])
		calc.accelerate(*calc.active_ship(), 500, 0);

	if (key[ALLEGRO_KEY_TAB])
		graphics.toggle_track();
	if (key[ALLEGRO_KEY_Q])
		graphics.track(true);
	if (key[ALLEGRO_KEY_W])
		graphics.track(false);

	if (key[ALLEGRO_KEY_H])
		graphics.set_center(*find_object("poop", object));
	if (key[ALLEGRO_KEY_1])
		graphics.set_center(*find_object("fart", object));
}

void calculate() {

//	static unsigned short n;
//	for (n = 0; n != ALLEGRO_KEY_MAX; n++)
//		if (key[n])
//			clog << endl << al_keycode_to_name(n);

	graphics.update();

	calc.update(object);

	input();
}

void draw() {

	graphics.draw_all(object);
}

void run() {

	bool redraw = true;
	unsigned short n;


	while(true) {

		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);


		if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { 						///window close
			return;
		}

		else if(ev.type == ALLEGRO_EVENT_TIMER) {							///tick
			calculate();
			redraw = true;

			for(n = 0; n != ALLEGRO_KEY_MAX; n++)							//clear key states
				key[n] = false;
		}

		else if(ev.type == ALLEGRO_EVENT_KEY_CHAR) {						///keypress
			key[ev.keyboard.keycode] = true;
			mods += ev.keyboard.modifiers;
		}

		if(redraw && al_is_event_queue_empty(event_queue)) {				///redraw
			redraw = false;
			al_clear_to_color(al_map_rgb(0,0,0));
			draw();
			al_flip_display();
		}

	}
}


int main() {

	object_c poop ("poop", 1e10,200, 750,500, 0,0, 0,0, al_color_name("red"));
	object_c fart ("fart", 1e5,10, 50,700, 0,0, 0,0, al_color_name("green"));
	object_c butt ("butt", 1e8,50, 80,200, 0,0, 0,0, al_color_name("blue"));

	object.push_back(poop);
	object.push_back(fart);
	object.push_back(butt);

	calc.set_active_ship(find_object("poop", object));

	if(!initialize()) {
		if(cleanup())
			return 1;
		else
			return 11;
	}

	run();

	if(cleanup())
		return 0;
	else
		return 10;
}
