#include <iostream>

#define ALLEGRO_STATICLINK
#include <allegro5/allegro.h>

#include <corbit/object.h>
#include <corbit/version.h>

using namespace std;


const unsigned short	FPS				=30;

ALLEGRO_DISPLAY*		display			=NULL;
ALLEGRO_EVENT_QUEUE*	event_queue		=NULL;
ALLEGRO_TIMER*			timer			=NULL;
bool					key[ALLEGRO_KEY_MAX];


bool initAllegro() {

	bool success = true;

	///initializes allegro///
	if(!al_init()) {
		cerr << "Failed to initialize Allegro!" << endl;
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

	cout << "Corbit " << AutoVersion::STATUS << " v" << AutoVersion::MAJOR << '.' << AutoVersion::MINOR;

	if(!initAllegro())
		return false;

	return true;
}

bool cleanup() {

	if(timer)
		al_destroy_timer(timer);
	if(display)
		al_destroy_display(display);
	if(event_queue)
		al_destroy_event_queue(event_queue);

	return true;
}


void calculate() {

	static int n;
	for (n = 0; n != ALLEGRO_KEY_MAX; n++)
		if (key[n])
			cout << n << endl;
}

void draw() {


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

	run();

	if(cleanup())
		return 0;
	else
		return 2;
}
