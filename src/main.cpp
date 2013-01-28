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
//typedef boost::intrusive::list <object_c> object_list;
object_c::object_list object_c::objects;
using namespace Json;


ALLEGRO_DISPLAY		*display		=NULL;
ALLEGRO_EVENT_QUEUE	*event_queue	=NULL;
ALLEGRO_TIMER		*timer			=NULL;
bool				key[ALLEGRO_KEY_MAX] ={};
unsigned			mods			=0;
object_c			*ship			=NULL,
					*targ			=NULL,
					*ref			=NULL;

//object_list objects;

object_c *find_object (string name) {

	for(auto &it : object_c::objects) {
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


	Value root;
	Reader reader;
	ifstream fin("objects.json");

	if(!reader.parse(fin, root)) {
		cout << "well sheit.\n" << reader.getFormatedErrorMessages();
		return false;
	}

	const Value json_objects = root["objects"];

	string	name="";
	var		m   =0,	r   =0,
			x   =0,	y   =0,
			Vx  =0,	Vy  =0,
			accX=0,	accY=0;
	col		color(al_color_name("lawngreen"));

	static object_c *json_input = (object_c*) malloc(json_objects.size() * sizeof(object_c));
//	static const int *json_input = (int*) malloc(json_objects.size() * sizeof(const int));
//	static std::vector<object_c> json_input;
	cout << "\n\nSIZE HERE\n\n" << json_objects.size();

	for(unsigned i=0; i < json_objects.size(); ++i) {
		name	= json_objects[i].get("name", "unnamed").asString();
		m		= json_objects[i].get("mass", 100).asDouble();
		r		= json_objects[i].get("radius", 100).asDouble();
		x		= json_objects[i]["pos"].get("x", 100).asDouble();
		y		= json_objects[i]["pos"].get("y", 100).asDouble();
		Vx		= json_objects[i]["v"].get("x", 100).asDouble();
		Vy		= json_objects[i]["v"].get("y", 100).asDouble();
		accX	= json_objects[i]["acc"].get("x", 100).asDouble();
		accY	= json_objects[i]["acc"].get("y", 100).asDouble();
		color	= al_color_name(json_objects[i].get("color", "lawngreen").asCString());

//		json_input[i](name, m, r, x,y, Vx,Vy, accX,accY, color);
//		json_input.emplace_back(*new object_c(name, m, r, x,y, Vx,Vy, accX,accY, color));
		new (&json_input[i]) object_c(name, m, r, x,y, Vx,Vy, accX,accY, color);
//		json_input[i]=4;
	}
//	static object_c mars(name, m, r, x,y, Vx,Vy, accX,accY, color);

//	cin.ignore();


//	input.shrink_to_fit();
//	cout << "\nLOOP\n";
//	for(auto it = input.begin(); it != input.end(); ++it) {
//		cout << (*it)->name << '\t';
//		objects.push_back(**it);
//		cout << object_c::objects.back().name << endl;
//	}
//	cout << object_c::objects.back().name;

//	static object_c earth	("earth",	1e15,	200,	0,0,	0,0,	0,0, al_color_name("green"));				objects.push_back(earth);
//	static object_c iss		("iss",		1e3,	30,		500,0,	0,200,	0,0, al_color_name("blue"));
//	static hab_c 	hab		("hab",		1e8,	50,		0,500,	300,0,	0,0, al_color_name("red"), 1, 1, 1);
//
//	const int max = 1;
//	static object_c ar[max];

//
//	ship = find_object("hab");
//	targ = find_object("ref");
//	ref  = find_object("earth");

	return true;
}

bool cleanup() {

	if(event_queue)
		al_destroy_event_queue(event_queue);
	if(timer)
		al_destroy_timer(timer);
	if(display)
		al_destroy_display(display);

//	objects.erase(objects.begin(), objects.end());

	return true;
}

void input() {

	if(key[ALLEGRO_KEY_PAD_MINUS])
		graphics::camera->zoom_level += 0.1;
	if(key[ALLEGRO_KEY_PAD_PLUS])
		graphics::camera->zoom_level -= 0.1;

	if(key[ALLEGRO_KEY_TAB])
		graphics::camera->tracking = !graphics::camera->tracking;
	if(key[ALLEGRO_KEY_Q])
		graphics::camera->tracking = true;
	if(key[ALLEGRO_KEY_E])
		graphics::camera->tracking = false;

	if(key[ALLEGRO_KEY_RIGHT])
		graphics::camera->pan(10, 0);
	if(key[ALLEGRO_KEY_LEFT])
		graphics::camera->pan(-10, 0);
	if(key[ALLEGRO_KEY_UP])
		graphics::camera->pan(0, -10);
	if(key[ALLEGRO_KEY_DOWN])
		graphics::camera->pan(0, 10);

	if(key[ALLEGRO_KEY_W])
		ship->accelerate(50000000000, 3.14159 * 1.5);
	if(key[ALLEGRO_KEY_A])
		ship->accelerate(50000000000, 3.14159 * 1.0);
	if(key[ALLEGRO_KEY_S])
		ship->accelerate(50000000000, 3.14159 * 0.5);
	if(key[ALLEGRO_KEY_D])
		ship->accelerate(50000000000, 3.14159 * 0.0);

	if(key[ALLEGRO_KEY_H])
		graphics::camera->center = find_object("hab");
	if(key[ALLEGRO_KEY_1])
		graphics::camera->center = find_object("earth");
}

void calculate() {

	unsigned short n;
	for (n = 0; n != ALLEGRO_KEY_MAX; n++)
		if (key[n])
			clog << al_keycode_to_name(n) << '\n';
	cout << "-----------\n";

	auto itX = object_c::objects.begin();

	while(itX != object_c::objects.end()) {
		auto itY = itX;
		++itY;
		while(itY != object_c::objects.end()) {
			itX->accelerate( calc::gravity(*itX, *itY), calc::theta(*itX, *itY));
			itY->accelerate(-calc::gravity(*itX, *itY), calc::theta(*itX, *itY));
			++itY;
		}
		++itX;
	}

//	find_object("hab")->set_accX(50);

	for(auto &it : object_c::objects) {
		it.move();
	}



	graphics::camera->update();

	input();
}

void draw() {

	for(auto &it : object_c::objects) {
		graphics::draw(it);
	}
}

bool run() {

	bool redraw = true;
	ALLEGRO_FONT *font = al_load_font("courier new.ttf", 36, 0);

	while(true) {

		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if		(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { 					///window close
			return true;
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
			al_draw_textf(font, al_map_rgb(200,200,200), 0,0, ALLEGRO_ALIGN_LEFT, "%lf you are poop %lf", graphics::camera->zoom(), graphics::camera->zoom_level);
			al_flip_display();
		}

	}

	return true;
}


int main() {

	int return_code = 0x00000;



	if(!init()) {
		cerr << "Init failed!" << endl;
		return_code += 0x00001;
	}
//	cout << object_c::objects.front().name;
//	cout << ref->name;

	new object_c 		("iss",		1e3,	30,		500,0,	0,200,	0,0, al_color_name("blue"));
	hab_c 	hab		("hab",		1e8,	50,		0,500,	300,0,	0,0, al_color_name("red"), 1, 1, 1);
//	objects.push_back(iss);
	cout << object_c::objects.front().name;

	if(!run()) {
		cerr << "Error in main loop!" << endl;
		return_code += 0x00004;
	}

//	delete ref;

	if(!cleanup()) {
		cerr << "Cleanup failed!" << endl;
		return_code += 0x00002;
		return return_code;
	}

	return return_code;
}
