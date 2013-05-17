#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>

#define ALLEGRO_STATICLINK
#include <allegro5/allegro5.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <boost/intrusive/list.hpp>
#include <boost/algorithm/string.hpp>	//for trim(std::string)

#include <json/json.h>

#include <corbit/corbit.hpp>

using std::clog;
using std::cerr;
using std::cout;
using std::cin;
using std::string;
using std::endl;
using std::flush;
using std::ifstream;
using std::ofstream;
using std::thread;
using std::stringstream;
using std::istringstream;
using std::vector;
typedef boost::intrusive::list <entity_c> entity_list_t;


ALLEGRO_DISPLAY		*display		=NULL;
ALLEGRO_EVENT_QUEUE	*event_queue	=NULL;
ALLEGRO_TIMER		*timer			=NULL;
bool				key[ALLEGRO_KEY_MAX] = {};
unsigned			mods			=0;
bool				paused			=false;
bool				redraw			=true;

entity_list_t entities;

bool load (string filename);

entity_c *find_entity (string name) {

	for (auto &it : entities) {
		if (it.name == name) {
			return &it;
		}
	}

	return NULL;
}

void parse_console (string console_input) {

	string buf;
	stringstream args(console_input); 	//insert all arguments into stringstream

	vector<string> argv;	//vector of arguments

	while (args >> buf) {
		argv.push_back(buf);
	}


	if (argv[0] == "load") {
		load (argv[1]);
	}
	else if (argv[0] == "target") {
		nav::targ = find_entity (argv[1]);
	}
	else if (argv[0] == "reference") {
		nav::ref = find_entity (argv[1]);
	}
	else if (argv[0] == "center") {
		graphics::camera->center = find_entity (argv[1]);
	}
}

void get_input() {

	if (!paused) {
		return;
	}

	string console_input ("");

	const string PS1 = "\n$> ";

	cout << PS1 << flush;



	while (true) {
		getline (cin, console_input);
		boost::algorithm::trim (console_input);

		if (console_input == "exit") {
			break;
		}
		parse_console (console_input);

		cout << PS1 << flush;
	}

	paused = false;
}

bool save (string filename) {

	Json::Value root;

	Json::Value json_entities (Json::arrayValue);
	Json::Value json_habs (Json::arrayValue);

	for (auto &it : entities) {
		if (it.type == ENTITY) {
			json_entities.append (it.json());
		}
		else if (it.type == HAB) {
			json_habs.append (it.json());
		}
	}

	root["entities"] = json_entities;
	root["habs"] = json_habs;

	Json::StyledStreamWriter writer;
	ofstream file(filename);
	writer.write(file, root);
	file.close();
	//TODO: check all is good
	return true;
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
	al_set_new_display_flags (ALLEGRO_WINDOWED);

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

	stringstream title("");
	title	<< "Corbit " << AutoVersion::STATUS
	<< " v" << AutoVersion::MAJOR
	<< '.' << AutoVersion::MINOR
	<< '.' << AutoVersion::BUILD;
	al_set_window_title (display, title.str().c_str());

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

bool init_from_file (string filename) {

	Json::Value root;
	Json::Reader reader;
	ifstream fin (filename);

	if (fin.fail()) {
		cerr << "Error while reading from file!" << endl;
		return false;
	}

	if (!reader.parse (fin, root) ) {
		cerr << "Error while reading JSON:\n" << reader.getFormatedErrorMessages() << endl;
		return false;
	}

	fin.close();

	const Json::Value json_entities = root["entities"];
	string	name="";
	var		m   =0,	r   =0,
			ang_pos		=0,
			ang_v		=0,
			ang_acc		=0,
			x   =0,	y   =0,
			Vx  =0,	Vy  =0,
			accX=0,	accY=0;
	ALLEGRO_COLOR color (al_color_name ("lawngreen") );
	static vector <entity_c> json_entites_data;
	json_entites_data.reserve (json_entities.size());

	clog << "\n\ngetting entities\n\n";

	for (unsigned i=0; i != json_entities.size(); ++i) {
		name	= json_entities[i].get ("name", "unnamed").asString();
		m		= json_entities[i].get ("mass", 100).asDouble();
		r		= json_entities[i].get ("radius", 100).asDouble();
		ang_pos	= json_entities[i].get ("ang_pos", 0).asDouble();
		ang_v	= json_entities[i].get ("ang_v", 0).asDouble();
		ang_acc	= json_entities[i].get ("ang_acc", 0).asDouble();
		x		= json_entities[i]["pos"].get ("x", 100).asDouble();
		y		= json_entities[i]["pos"].get ("y", 100).asDouble();
		Vx		= json_entities[i]["v"].get ("x", 100).asDouble();
		Vy		= json_entities[i]["v"].get ("y", 100).asDouble();
		accX	= json_entities[i]["acc"].get ("x", 100).asDouble();
		accY	= json_entities[i]["acc"].get ("y", 100).asDouble();
		color	= al_color_name (json_entities[i].get ("color", "lawngreen").asCString() );

		json_entites_data.emplace_back (ENTITY, name, m, r, ang_pos, ang_v, ang_acc, x,y, Vx,Vy, accX,accY, color);
		entities.push_back (json_entites_data.back());
		assert (&entities.back() == &json_entites_data.back());
	}


	const Json::Value json_habs = root["habs"];
	var fuel =0, Isp =0, thrust =0;
	static vector <hab_c> json_habs_data;
	json_habs_data.reserve (json_habs.size());

	clog << "\n\ngetting habs\n\n";

	for (unsigned i=0; i != json_habs.size(); ++i) {
		name	= json_habs[i].get ("name", "unnamed").asString();
		m		= json_habs[i].get ("mass", 100).asDouble();
		r		= json_habs[i].get ("radius", 100).asDouble();
		ang_pos	= json_entities[i].get ("ang_pos", 0).asDouble();
		ang_v	= json_entities[i].get ("ang_v", 0).asDouble();
		ang_acc	= json_entities[i].get ("ang_acc", 0).asDouble();
		x		= json_habs[i]["pos"].get ("x", 100).asDouble();
		y		= json_habs[i]["pos"].get ("y", 100).asDouble();
		Vx		= json_habs[i]["v"].get ("x", 100).asDouble();
		Vy		= json_habs[i]["v"].get ("y", 100).asDouble();
		accX	= json_habs[i]["acc"].get ("x", 100).asDouble();
		accY	= json_habs[i]["acc"].get ("y", 100).asDouble();
		color	= al_color_name (json_habs[i].get ("color", "lawngreen").asCString() );
		fuel	= json_habs[i].get ("fuel", 10000).asDouble();
		Isp		= json_habs[i].get ("Isp", 1000).asDouble();
		thrust	= json_habs[i].get ("thrust", 50000).asDouble();

		json_habs_data.emplace_back (HAB, name, m, r, ang_pos, ang_v, ang_acc, x,y, Vx,Vy, accX,accY, color, fuel, Isp, thrust);
		entities.push_back (json_habs_data.back());
		assert (&entities.back() == &json_habs_data.back());
	}

	return true;
}

bool init() {

	cout << "Corbit " << AutoVersion::STATUS
		 << " v" << AutoVersion::MAJOR
		 << '.' << AutoVersion::MINOR
		 << '.' << AutoVersion::BUILD
		 << endl;

	if (!init_allegro() ) {
		return false;
	}

	if (!load("res/OCESS.json")) {
		return false;
	}

	graphics::hud.font = al_load_ttf_font ("res/DejaVuSansMono.ttf", 15, 0);

	return true;
}

bool cleanup() {

	if (event_queue)
		al_destroy_event_queue (event_queue);
	if (timer)
		al_destroy_timer (timer);
	if (display)
		al_destroy_display (display);


	entities.erase (entities.begin(), entities.end());

	return true;
}

bool load (string filename) {

	entities.erase (entities.begin(), entities.end());

	if (!init_from_file (filename)) {
		return false;
	}

	nav::ship = find_entity ("Habitat");
	nav::ref = find_entity ("AYSE");
	nav::targ = find_entity ("AYSE");
	graphics::camera->center = find_entity ("Habitat");

	return true;
}

void input() {

	if (key[ALLEGRO_KEY_PAD_MINUS])
		graphics::camera->zoom_level += 0.1;
	if (key[ALLEGRO_KEY_PAD_PLUS])
		graphics::camera->zoom_level -= 0.1;

	if (key[ALLEGRO_KEY_T])
		graphics::camera->tracking = !graphics::camera->tracking;
	if (key[ALLEGRO_KEY_TAB]) {

		if (mods & ALLEGRO_KEYMOD_SHIFT) {
			auto it = entities.iterator_to (*graphics::camera->center);
			if (it == entities.begin()) {
				it = entities.end();
			}
			--it;

			graphics::camera->center = &(*it);
		}
		else {
			auto it = entities.iterator_to (*graphics::camera->center);
			++it;
			if (it == entities.end()) {
				it = entities.begin();
			}

			graphics::camera->center = &(*it);
		}

	}


	if (key[ALLEGRO_KEY_RIGHT])
		graphics::camera->pan (2000, 0);
	if (key[ALLEGRO_KEY_LEFT])
		graphics::camera->pan (-2000, 0);
	if (key[ALLEGRO_KEY_UP])
		graphics::camera->pan (0, 2000);
	if (key[ALLEGRO_KEY_DOWN])
		graphics::camera->pan (0, -2000);

	if (key[ALLEGRO_KEY_W])
		if (nav::ship) nav::ship->accelerate (vect (0, 1e8), M_PI);
	if (key[ALLEGRO_KEY_A])
		if (nav::ship) nav::ship->accelerate (vect (-1e8, 0), 0);
	if (key[ALLEGRO_KEY_S])
		if (nav::ship) nav::ship->accelerate (vect (0, -1e8), 0);
	if (key[ALLEGRO_KEY_D])
		if (nav::ship) nav::ship->accelerate (vect (1e8, 0), 0);

//	if(key[ALLEGRO_KEY_H])
//		graphics::camera->center = find_entity("hab");
	if (key[ALLEGRO_KEY_4])
		graphics::camera->center = find_entity ("flab");
	if (key[ALLEGRO_KEY_2])
		graphics::camera->center = find_entity ("hab");
	if (key[ALLEGRO_KEY_1])
		graphics::camera->center = find_entity ("earth");

	if (key[ALLEGRO_KEY_Q])
		if (nav::ship) nav::ship->accelerate (vect (0, -100000), -M_PI/2);
	if (key[ALLEGRO_KEY_E])
		if (nav::ship) nav::ship->accelerate (vect (0, 100000), M_PI/2);

	if (key[ALLEGRO_KEY_F5])
		save("res/quicksave.json");
	if (key[ALLEGRO_KEY_F9]) {
		load("res/quicksave.json");
		graphics::camera->center = nav::ship = find_entity ("Hawking III");
		cout << "\n\n\n\n\n" << nav::ship->name << "\n\n\n\n\n";
	}
}

void calculate() {

	input();

	unsigned short n;
	for (n = 0; n != ALLEGRO_KEY_MAX; n++) {
		if (key[n]) {
//			clog << al_keycode_to_name (n) << '\n';
		}
		key[n] = false;
	}


	auto itX = entities.begin();
	while (itX != entities.end() ) {
		auto itY = itX;
		++itY;
		while (itY != entities.end() ) {
//			if (calc::distance2 (*itX, *itY) > (itX->radius + itY->radius) * (itX->radius + itY->radius) ) {
				vect grav (cos (calc::theta (*itX, *itY)), sin (calc::theta (*itX, *itY)));
				grav *= calc::gravity (*itX, *itY);
				itX->accelerate ( grav, 0);
				itY->accelerate (-grav, 0);
//			}
			++itY;
		}
		++itX;
	}

	itX = entities.begin();
	while (itX != entities.end() ) {
		auto itY = itX;
		++itY;
		while (itY != entities.end() ) {
			calc::detect_collision (*itX, *itY);
			++itY;
		}
		++itX;
	}

	for (auto &it : entities) {
		it.move();
	}


	graphics::camera->update();
}

void draw() {

	al_clear_to_color (al_map_rgb (0,0,0) );

	for (auto &it : entities) {
		graphics::draw (it);
	}

	graphics::hud.draw();

	al_flip_display();
}

bool run() {

	while (true) {

		ALLEGRO_EVENT ev;
		al_wait_for_event (event_queue, &ev);



		if	(ev.type == ALLEGRO_EVENT_TIMER && !paused) {				///tick
			calculate();
			redraw = true;
		}

		else if	(ev.type == ALLEGRO_EVENT_KEY_CHAR) {					///keypress
			if (ev.keyboard.keycode == ALLEGRO_KEY_TILDE) {
				paused = true;
				thread input_thread (get_input);
				input_thread.detach();
			} else {
				key[ev.keyboard.keycode] = true;
				mods = ev.keyboard.modifiers;
			}
		}

		else if	(ev.type == ALLEGRO_EVENT_KEY_UP) {						///key release
			key[ev.keyboard.keycode] = false;
			mods = ev.keyboard.modifiers;
		}

		else if	(ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE) {				///display resize
			if (!al_acknowledge_resize (display) ) {
				cerr << "[" << al_get_time() << "] Could not acknowledge resize" << endl;
			}
			redraw = true;
		}

		else if	(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { 				///window close
			return true;
		}

		if (redraw && al_is_event_queue_empty (event_queue) ) {			///redraw
			redraw = false;
			draw();
		}

	}

	return true;
}


int main() {

	enum codes {
		SUCCESS		= 0x00000,
		INIT_FAIL	= 0x00001,
		CLEANUP_FAIL= 0x00002,
		RUN_FAIL	= 0x00004
	};

	int return_code = SUCCESS;


	if (!init() ) {
		cerr << "Init failed!" << endl;
		return_code += INIT_FAIL;
	}

	else if (!run() ) {
		cerr << "Error in main loop!" << endl;
		return_code += RUN_FAIL;
	}

	if (!cleanup() ) {
		cerr << "Cleanup failed!" << endl;
		return_code += CLEANUP_FAIL;
		return return_code;
	}

	return return_code;
}
