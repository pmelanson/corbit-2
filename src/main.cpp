#include <iostream>

#define ALLEGRO_STATICLINK
#include <allegro5/allegro5.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <boost/intrusive/list.hpp>

#include <fstream>
#include <json/json.h>

#include <regex>

#include <corbit/corbit.hpp>

using std::clog;
using std::cerr;
using std::cout;
using std::cin;
using std::string;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::malloc;
typedef boost::intrusive::list <entity_c> entity_list_t;
using namespace Json;


ALLEGRO_DISPLAY		*display		=NULL;
ALLEGRO_EVENT_QUEUE	*event_queue	=NULL;
ALLEGRO_TIMER		*timer			=NULL;
bool				key[ALLEGRO_KEY_MAX] = {};
unsigned			mods			=0;
ALLEGRO_USTR		*console_input	=al_ustr_new("");

entity_list_t entities;

entity_c *find_entity (string name) {

for (auto &it : entities) {
		if (it.name == name) {
			return &it;
		}
	}

	return NULL;
}

bool init_allegro() {

	if (!al_init() ) {
		cerr << "Failed to initialise Allegro!" << endl;
		return false;
	}

	if (!al_init_primitives_addon() ) {
		cerr << "Failed to initialise primitives addon!" << endl;
		return false;
	}

	al_init_font_addon();
	if (!al_init_ttf_addon() ) {
		cerr << "Failed to initalise ttf addon!" << endl;
		return false;
	}


	if (!al_install_keyboard() ) {
		cerr << "Failed to install keyboard!" << endl;
		return false;
	}

	timer = al_create_timer (1./FPS);
	if (!timer) {
		cerr << "Failed to create timer!" << endl;
		return false;
	}

	ALLEGRO_DISPLAY_MODE disp_data;
	al_get_display_mode (0, &disp_data);
	al_set_new_display_flags (ALLEGRO_FULLSCREEN_WINDOW);

	al_set_new_display_option (ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option (ALLEGRO_SAMPLES, 4, ALLEGRO_SUGGEST);
	al_set_new_display_option (ALLEGRO_VSYNC, 1, ALLEGRO_SUGGEST);

	display = al_create_display (disp_data.width, disp_data.height);
	graphics::camera->size[0] = disp_data.width;
	graphics::camera->size[1] = disp_data.height;
	cout << "[DISPLAY]\n" << "Pixel format: " << disp_data.format << '\n'
		 << disp_data.width << 'x' << disp_data.height << '\n'
		 << disp_data.refresh_rate << "Hz" << endl;
	if (!display) {
		cerr << "Failed to create display!" << endl;
		return false;
	}
	al_acknowledge_resize (display);

	if (!al_inhibit_screensaver (true) ) {
		cerr << "Failed to inhibit screensaver!" << endl;
	}

	//initializes buffer
	al_set_target_bitmap (al_get_backbuffer (display) );

	event_queue = al_create_event_queue();
	if (!event_queue) {
		cerr << "Failed to create event_queue!" << endl;
		return false;
	}

	//registers event sources
	al_register_event_source (event_queue, al_get_display_event_source (display) );
	al_register_event_source (event_queue, al_get_timer_event_source (timer) );
	al_register_event_source (event_queue, al_get_keyboard_event_source() );

	al_clear_to_color (al_map_rgb (0,0,0) );
	al_flip_display();
	al_start_timer (timer);

	return true;
}

bool init() {

	cout << "Corbit " << AutoVersion::STATUS
		 << " v" << AutoVersion::MAJOR
		 << '.' << AutoVersion::MINOR
		 << '.' << AutoVersion::BUILD << endl;

	if (!init_allegro() )
		return false;

	Value root;
	Reader reader;
	ifstream fin ("entities.json");

	if (!reader.parse (fin, root) ) {
		cout << "well sheit.\n" << reader.getFormatedErrorMessages();
		return false;
	}

	const Value json_entities = root["entities"];

	string	name="";
	var		m   =0,	r   =0,
			x   =0,	y   =0,
			Vx  =0,	Vy  =0,
			accX=0,	accY=0;
	ALLEGRO_COLOR color (al_color_name ("lawngreen") );


	entity_c *json_input = (entity_c*) malloc (json_entities.size() * sizeof (entity_c) );

	for (unsigned i=0; i != json_entities.size(); ++i) {
		name	= json_entities[i].get ("name", "unnamed").asString();
		m		= json_entities[i].get ("mass", 100).asDouble();
		r		= json_entities[i].get ("radius", 100).asDouble();
		x		= json_entities[i]["pos"].get ("x", 100).asDouble();
		y		= json_entities[i]["pos"].get ("y", 100).asDouble();
		Vx		= json_entities[i]["v"].get ("x", 100).asDouble();
		Vy		= json_entities[i]["v"].get ("y", 100).asDouble();
		accX	= json_entities[i]["acc"].get ("x", 100).asDouble();
		accY	= json_entities[i]["acc"].get ("y", 100).asDouble();
		color	= al_color_name (json_entities[i].get ("color", "lawngreen").asCString() );

		new (&json_input[i]) entity_c (name, m, r, x,y, Vx,Vy, accX,accY, color);
		entities.push_back (json_input[i]);
		assert (&entities.back() == &json_input[i]);
	}

	assert (&entities.front() == &json_input[0]);


	nav::ship = find_entity ("mars");
	nav::ref = find_entity ("earth");
	nav::targ = find_entity ("earth");
	graphics::camera->center = find_entity ("mars");

	graphics::hud.font = al_load_ttf_font ("res/DejaVuSansMono.ttf", 15, 0);

	graphics::console.font = al_load_ttf_font ("res/DejaVuSansMono.ttf", 15, 0);

	return true;
}

bool cleanup() {

	if (event_queue)
		al_destroy_event_queue (event_queue);
	if (timer)
		al_destroy_timer (timer);
	if (display)
		al_destroy_display (display);


	entities.erase (entities.begin(), entities.end() );

	return true;
}

void input() {

	if (key[ALLEGRO_KEY_PAD_MINUS])
		graphics::camera->zoom_level += 0.1;
	if (key[ALLEGRO_KEY_PAD_PLUS])
		graphics::camera->zoom_level -= 0.1;

	if (key[ALLEGRO_KEY_TAB])
		graphics::camera->tracking = !graphics::camera->tracking;
	if (key[ALLEGRO_KEY_Q])
		graphics::camera->tracking = true;
	if (key[ALLEGRO_KEY_E])
		graphics::camera->tracking = false;

	if (key[ALLEGRO_KEY_RIGHT])
		graphics::camera->pan (200, 0);
	if (key[ALLEGRO_KEY_LEFT])
		graphics::camera->pan (-200, 0);
	if (key[ALLEGRO_KEY_UP])
		graphics::camera->pan (0, -200);
	if (key[ALLEGRO_KEY_DOWN])
		graphics::camera->pan (0, 200);

	if (key[ALLEGRO_KEY_W])
		if (nav::ship) nav::ship->accelerate (1e6, 3.14159 * 0.5);
	if (key[ALLEGRO_KEY_A])
		if (nav::ship) nav::ship->accelerate (1e6, 3.14159 * 1.0);
	if (key[ALLEGRO_KEY_S])
		if (nav::ship) nav::ship->accelerate (1e6, 3.14159 * 1.5);
	if (key[ALLEGRO_KEY_D])
		if (nav::ship) nav::ship->accelerate (1e6, 3.14159 * 0.0);
	if (key[ALLEGRO_KEY_H])
		if (nav::ship) nav::ship->accelerate (1e8, 3.14159 * 1.0);
	if (key[ALLEGRO_KEY_J])
		if (nav::ship) nav::ship->accelerate (1e8, 3.14159 * 1.5);
	if (key[ALLEGRO_KEY_K])
		if (nav::ship) nav::ship->accelerate (1e8, 3.14159 * 0.5);
	if (key[ALLEGRO_KEY_L])
		if (nav::ship) nav::ship->accelerate (1e8, 3.14159 * 0.0);

//	if(key[ALLEGRO_KEY_H])
//		graphics::camera->center = find_entity("hab");
	if (key[ALLEGRO_KEY_4])
		graphics::camera->center = find_entity ("mars");
	if (key[ALLEGRO_KEY_1])
		graphics::camera->center = find_entity ("earth");
}

void calculate() {

	unsigned short n;
	for (n = 0; n != ALLEGRO_KEY_MAX; n++)
		if (key[n]) {
//			clog << al_keycode_to_name (n) << '\n';
		}
	cout << "-----------\n";



	for (auto &it : entities) {
		it.move();
	}

	auto itX = entities.begin();
	while (itX != entities.end() ) {
		auto itY = itX;
		++itY;
		while (itY != entities.end() ) {
			if (calc::distance2 (*itX, *itY) > (itX->radius + itY->radius) * (itX->radius + itY->radius) ) {
				itX->accelerate ( calc::gravity (*itX, *itY), calc::theta (*itX, *itY) );
				itY->accelerate (-calc::gravity (*itX, *itY), calc::theta (*itX, *itY) );
			}
			calc::detect_collision (*itX, *itY);
			++itY;
		}
		++itX;
	}

	graphics::camera->update();

	input();
}

void draw() {

	al_clear_to_color (al_map_rgb (0,0,0) );


	for (auto &it : entities) {
		graphics::draw (it);
	}

	graphics::hud.draw();
	graphics::console.draw();

	al_flip_display();
}

bool run() {

	bool redraw	= true;
	bool paused	= false;

	while (true) {

		ALLEGRO_EVENT ev;
		al_wait_for_event (event_queue, &ev);


		if	(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { 					///window close
			return true;
		}

		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN						///open console
				&& ev.keyboard.keycode == ALLEGRO_KEY_TILDE) {
			graphics::console.open = !graphics::console.open;
			paused = graphics::console.open;
		}

		else if	(ev.type == ALLEGRO_EVENT_TIMER && !paused) {			///tick
			calculate();
			redraw = true;
		}

		else if (graphics::console.open									///console text entry
				&& ev.type == ALLEGRO_EVENT_KEY_CHAR) {
			ALLEGRO_USTR *buf = al_ustr_new("");
			al_ustr_append_chr(buf, ev.keyboard.unichar);
			graphics::console.input << char(ev.keyboard.unichar);
			clog << char(ev.keyboard.unichar) << ev.keyboard.repeat;
			redraw = true;
		}

		else if	(ev.type == ALLEGRO_EVENT_KEY_CHAR) {					///keypress
			key[ev.keyboard.keycode] = true;
			mods += ev.keyboard.modifiers;
		}

		else if	(ev.type == ALLEGRO_EVENT_KEY_UP) {						///key release
			key[ev.keyboard.keycode] = false;
			mods -= ev.keyboard.modifiers;
		}

		else if	(ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE) {				///display resize
			if (!al_acknowledge_resize (display) ) {
				cerr << "Could not acknowledge resize [ " << al_get_time() << "]" << endl;
			}
		}

		if (redraw && al_is_event_queue_empty (event_queue) ) {			///redraw
			redraw = false;
			draw();
		}

	}

	return true;
}


int main() {

	int return_code = 0x00000;



	if (!init() ) {
		cerr << "Init failed!" << endl;
		return_code += 0x00001;
	}

	else if (!run() ) {
		cerr << "Error in main loop!" << endl;
		return_code += 0x00004;
	}

	if (!cleanup() ) {
		cerr << "Cleanup failed!" << endl;
		return_code += 0x00002;
		return return_code;
	}

	return return_code;
}


