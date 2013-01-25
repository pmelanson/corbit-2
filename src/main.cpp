#include <fstream>
#include <iostream>

#define ALLEGRO_STATICLINK
#include <allegro5/allegro5.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <boost/intrusive/list.hpp>

//#include <json_cpp/json.h>

#include <corbit/corbit.hpp>
using std::clog;
using std::cerr;
using std::cout;
using std::cin;
using std::string;
using std::endl;


ALLEGRO_DISPLAY		*display		=NULL;
ALLEGRO_EVENT_QUEUE	*event_queue	=NULL;
ALLEGRO_TIMER		*timer			=NULL;
bool				key[ALLEGRO_KEY_MAX] ={};
unsigned			mods			=0;
object_c			*ship			=NULL,
					*targ			=NULL,
					*ref			=NULL;

typedef boost::intrusive::list <object_c> object_list;
object_list object;

object_c *find_object (string name) {

	for(auto &it : object) {
		if(it.name == name) {
			return &it;
		}
	}

	return NULL;
}


bool init_allegro() {

	bool success = true;

	if(!al_init()) {
		cerr << "Failed to initialise Allegro!" << endl;
		success = false;
	}

	if(!al_init_primitives_addon()) {
		cerr << "Failed to initialise primitives addon!" << endl;
		success = false;
	}

	al_init_font_addon();
	if(!al_init_ttf_addon()) {
		cerr << "Failed to initalise ttf addon!" << endl;
		success = false;
	}


	if(!al_install_keyboard()) {
		cerr << "Failed to install keyboard!" << endl;
		success = false;
	}

	timer = al_create_timer(1./FPS);
	if(!timer) {
		cerr << "Failed to create timer!" << endl;
		success = false;
	}

	ALLEGRO_DISPLAY_MODE disp_data;
	al_get_display_mode(0, &disp_data);
	al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
	display = al_create_display(disp_data.width, disp_data.height);
	graphics::camera->size[0] = disp_data.width;
	graphics::camera->size[1] = disp_data.height;
	cout << "[DISPLAY]\n" << "Pixel format: " << disp_data.format << '\n' << disp_data.width << 'x' << disp_data.height << '\n' << disp_data.refresh_rate << "Hz" << endl;
	if(!display) {
		cerr << "Failed to create display!" << endl;
		success = false;
	}

	if(!al_inhibit_screensaver(true)) {
		cerr << "Failed to inhibit screensaver!" << endl;
	}

	//initializes buffer
	al_set_target_bitmap(al_get_backbuffer(display));

	event_queue = al_create_event_queue();
	if(!event_queue) {
		cerr << "Failed to create event_queue!" << endl;
		success = false;
	}

	//registers event sources
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_clear_to_color(al_map_rgb(0,0,0));
	al_flip_display();
	al_start_timer(timer);

	return success;
}

bool init() {

	cout << "Corbit " << AutoVersion::STATUS << " v" << AutoVersion::MAJOR << '.' << AutoVersion::MINOR << '.' << AutoVersion::BUILD << endl;

	if(!init_allegro())
		return false;

	graphics::camera->center = find_object("earth");

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
		graphics::camera->zoom_level += 0.1;
	else if (key[ALLEGRO_KEY_PAD_PLUS])
		graphics::camera->zoom_level -= 0.1;

	else if (key[ALLEGRO_KEY_TAB])
		graphics::camera->tracking = !graphics::camera->tracking;
	else if (key[ALLEGRO_KEY_Q])
		graphics::camera->tracking = true;
	else if (key[ALLEGRO_KEY_E])
		graphics::camera->tracking = false;

	else if (key[ALLEGRO_KEY_RIGHT])
		graphics::camera->pan(100, 0);
	else if (key[ALLEGRO_KEY_LEFT])
		graphics::camera->pan(-100, 0);
	else if (key[ALLEGRO_KEY_UP])
		graphics::camera->pan(0, -100);
	else if (key[ALLEGRO_KEY_DOWN])
		graphics::camera->pan(0, 100);

	if (key[ALLEGRO_KEY_W])
		ship->accelerate(0, -5000000000);
	else if (key[ALLEGRO_KEY_A])
		ship->accelerate(-500, 0);
	if (key[ALLEGRO_KEY_S])
		ship->accelerate(0, 5000000000);
	else if (key[ALLEGRO_KEY_D])
		ship->accelerate(500, 0);

	else if (key[ALLEGRO_KEY_H])
		graphics::camera->center = find_object("hab");
	else if (key[ALLEGRO_KEY_1])
		graphics::camera->center = find_object("earth");
}

void calculate() {

	unsigned short n;
	for (n = 0; n != ALLEGRO_KEY_MAX; n++)
		if (key[n])
			clog << al_keycode_to_name(n) << '\n';
	cout << "-----------\n";

	auto itX = object.begin();

	while(itX != object.end()) {
		auto itY = itX;
		++itY;
		while(itY != object.end()) {
			itX->accelerate( calc::gravity(*itX, *itY), calc::theta(*itX, *itY));
			itY->accelerate(-calc::gravity(*itX, *itY), calc::theta(*itX, *itY));
			++itY;
		}
		++itX;
	}

//	find_object("hab")->set_accX(50);

	for(auto &it : object) {
		it.move();
	}

	graphics::camera->update();

	input();
}

void draw() {

	for(auto &it : object) {
		graphics::draw(it);
	}
}

void run() {

	bool redraw = true;
	unsigned short i;
	ALLEGRO_FONT *font = al_load_font("courier new.ttf", 36, 0);

	while(true) {

		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if		(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { 					///window close
			return;
		}

		else if	(ev.type == ALLEGRO_EVENT_TIMER) {							///tick
			calculate();
			redraw = true;
		}

		else if	(ev.type == ALLEGRO_EVENT_KEY_CHAR) {						///keypress
			key[ev.keyboard.keycode] = true;
			mods += ev.keyboard.modifiers;
		}
		else if	(ev.type == ALLEGRO_EVENT_KEY_UP) {							///key release
			key[ev.keyboard.keycode] = false;
			mods -= ev.keyboard.modifiers;
		}
		else if	(ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE) {					///display resize
			if(!al_acknowledge_resize(display)) {
				clog << "Could not acknowledge resize( " << al_get_time() << "s from startup)" << endl;
			}
		}

		if(redraw && al_is_event_queue_empty(event_queue)) {				///redraw
			redraw = false;
			al_clear_to_color(al_map_rgb(0,0,0));
			draw();
			al_draw_textf(font, al_map_rgb(200,200,200), 0,0, ALLEGRO_ALIGN_LEFT, "%lf you are poop %lf", calc::ecc(*ship, *ref), calc::distance(*ship, *ref));
			al_flip_display();
		}

	}
}


int main() {

	int return_code = 0x00000;


	if(!init()) {
		cerr << "Init failed!" << endl;
		return_code += 0x00001;
		if(!cleanup()) {
			cerr << "Cleanup failed!" << endl;
			return_code += 0x00004;
			return return_code;
		}
	}

	object_c poop ("earth", 1e18, 200, 0,0, -.1,.1, 0,0, al_color_name("green"));
//	object_c fart ("iss", 1e1,10, 50,700, 0,1, 0,0, al_color_name("blue"));
	object_c butt ("hab", 1e8,50, 500,0, 0,39, 0,0, al_color_name("red"));

	object.push_back(poop);
//	object.push_back(fart);
	object.push_back(butt);



	int max = 0;

	object_c ar[max];
	int x = 0;
	while(x != max)
		object.push_back(ar[x++]);

	ship = find_object("hab");
	targ = find_object("earth");
	ref  = find_object("earth");

//	find_object("hab")->accelerate(calc::orbitV(*ship, *ref) * find_object("hab")->mass, 3.14159);
	find_object("hab")->accelerate(50000000000, 0);

//	ofstream file("objectout.json");
//	js::Object obj;
//	obj.push_back(Pair("mass", 122));
//	obj.push_back(Pair("radius", 15));
//	js::write(obj, file, pretty_print);
//	file.close();
//	js::Value value;
//	js::read(file, value);

	run();

	if(!cleanup()) {
		cerr << "Cleanup failed!" << endl;
		return_code += 0x00004;
		return return_code;
	}

	return return_code;
}
