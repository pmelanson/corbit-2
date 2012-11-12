#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>

#define ALLEGRO_STATICLINK
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include <corbit/corbit.hpp>

using namespace std;


const unsigned short	FPS				=30;

ALLEGRO_DISPLAY*		display			=NULL;
ALLEGRO_EVENT_QUEUE*	event_queue		=NULL;
ALLEGRO_TIMER*			timer			=NULL;
bool					key[ALLEGRO_KEY_MAX];

object_c poop ("poop", 100.32, 233, 1, 2, 0, 0, 0, 0, al_color_name("red"));
object_c doober ("doober", 100.32, 233, 20, 2, 11, 12, 0, 0, al_color_name("green"));

typedef boost::intrusive::list <object_c> objectlist;
objectlist object;


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
	al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
	display = al_create_display(disp_data.width, disp_data.height-50);
	if(!display) {
		cerr << "Failed to create display!" << endl;
		success = false;
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

	return true;
}

bool cleanup() {

	if(event_queue)
		al_destroy_event_queue(event_queue);
	if(timer)
		al_destroy_timer(timer);
	if(display)
		al_destroy_display(display);

	return true;
}


void calculate() {

	static unsigned short n;
	for (n = 0; n != ALLEGRO_KEY_MAX; n++)
		if (key[n])
			cout << al_keycode_to_name(n) << endl;

	if (key[ALLEGRO_KEY_RIGHT])
		poop.accelerate (1, 0);
	if (key[ALLEGRO_KEY_LEFT])
		poop.accelerate (-1, 0);

	poop.move();
}

void draw() {

	al_draw_circle (poop.x(), poop.y(), poop.radius(), poop.color, 0);
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

	if(!initialize()) {
		cleanup();
		return 1;
	}

	object.push_back(poop);
	object.push_back(doober);

	objectlist::iterator it(object.begin()), itend(object.end());

	cout << it->x();

	run();

	if(cleanup())
		return 0;
	else
		return 2;
}
