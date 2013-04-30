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

#include <curses.h>

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


ALLEGRO_DISPLAY		*display		=NULL;
ALLEGRO_EVENT_QUEUE	*event_queue	=NULL;
ALLEGRO_TIMER		*timer			=NULL;
bool				key[ALLEGRO_KEY_MAX] = {};
unsigned			mods			=0;
stringstream		console_input	("");

entity_list_t entities;

entity_c *find_entity (string name) {

	for (auto &it : entities) {
		if (it.name == name) {
			return &it;
		}
	}

	return NULL;
}

bool parse_console (ALLEGRO_KEYBOARD_EVENT keyboard) {

	const string PS1 = "\n\n$> ";

	if (keyboard.keycode == ALLEGRO_KEY_ENTER) {
		if (keyboard.modifiers && ALLEGRO_KEYMOD_SHIFT) {
			console_input << '\n';
			cout << "\n> ";
			cout.flush();
		}
		else {
			return true;
		}
	}

	else if (keyboard.keycode == ALLEGRO_KEY_BACKSPACE) {
//		if (
		string erased = console_input.str();
		erased.pop_back();

		console_input.str("");
		console_input << erased;

		cout << "\b \b";
		cout.flush();
	}


	else {
		console_input << char(keyboard.unichar);
		cout << char(keyboard.unichar);
		cout.flush();
	}

	return false;
}

bool save(string filename) {

	Json::Value root;

	Json::Value json_entities(Json::arrayValue);
	for (auto &it : entities) {
		json_entities.append(it.json());
	}

	root["entities"] = json_entities;

	Json::StyledStreamWriter writer;
//	writer.write(cout, root);
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

bool init() {

	WINDOW *console;

	if ((console = initscr()) == NULL) {
		cout << "\n\njohn is kill\n\n";
	}

	addstr("\n\nJohn is no kill\n\n");
	refresh();
	sleep(3);

	delwin(console);
	endwin();
	refresh();

	cout << "Corbit " << AutoVersion::STATUS
		 << " v" << AutoVersion::MAJOR
		 << '.' << AutoVersion::MINOR
		 << '.' << AutoVersion::BUILD << endl;

	if (!init_allegro() )
		return false;

	Json::Value root;
	Json::Reader reader;
	ifstream fin ("res/OCESS.json");

	if (!reader.parse (fin, root) ) {
		cout << "failed to read JSON.\n" << reader.getFormatedErrorMessages();
		return false;
	}

	const Json::Value json_entities = root["entities"];

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


	nav::ship = find_entity ("flab");
	nav::ref = find_entity ("earth");
	nav::targ = find_entity ("hab");
	graphics::camera->center = find_entity ("flab");

	graphics::hud.font = al_load_ttf_font ("res/DejaVuSansMono.ttf", 15, 0);

	graphics::console.font = al_load_ttf_font ("res/DejaVuSansMono.ttf", 15, 0);

	return true;
}

bool cleanup() {

	if (event_queue)
		al_destroy_event_queue (event_queue);
	if (timer)
		al_destroy_timer (timer);
	endwin();
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
		graphics::camera->pan (2000, 0);
	if (key[ALLEGRO_KEY_LEFT])
		graphics::camera->pan (-2000, 0);
	if (key[ALLEGRO_KEY_UP])
		graphics::camera->pan (0, 2000);
	if (key[ALLEGRO_KEY_DOWN])
		graphics::camera->pan (0, -2000);

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
		graphics::camera->center = find_entity ("flab");
	if (key[ALLEGRO_KEY_2])
		graphics::camera->center = find_entity ("hab");
	if (key[ALLEGRO_KEY_1])
		graphics::camera->center = find_entity ("earth");

	if (key[ALLEGRO_KEY_F5])
		save("res/quicksave.json");
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
	cout << "-----------\n";


	auto itX = entities.begin();
	while (itX != entities.end() ) {
		auto itY = itX;
		++itY;
		while (itY != entities.end() ) {
			if (calc::distance2 (*itX, *itY) > (itX->radius + itY->radius) * (itX->radius + itY->radius) ) {
				itX->accelerate ( calc::gravity (*itX, *itY), calc::theta (*itX, *itY) );
				itY->accelerate (-calc::gravity (*itX, *itY), calc::theta (*itX, *itY) );
			}
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
	graphics::console.draw();

	al_flip_display();
}

bool run() {

	bool redraw	= true;
	bool paused	= false;

	const string PS1 = "\n\n$> ";

	while (true) {

		ALLEGRO_EVENT ev;
		al_wait_for_event (event_queue, &ev);



		if	(ev.type == ALLEGRO_EVENT_TIMER && !paused) {				///tick
			calculate();
			redraw = true;
		}

		else if	(ev.type == ALLEGRO_EVENT_KEY_CHAR) {					///keypress
			if (ev.keyboard.keycode == ALLEGRO_KEY_TILDE) {
				paused = !paused;
				if (paused) {
					cout << PS1;
					cout.flush();
				}
			}
			else if (paused) {

				if (parse_console (ev.keyboard)) {
					cout << endl << "INPUT:\n" << console_input.str() << "\nDONE";
					console_input.str("");
					cout << PS1;
					cout.flush();
				}
			}
			else {
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


