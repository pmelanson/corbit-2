#include <corbit/entity.hpp>

#include <cmath>
#include <iostream>

#include <allegro5/allegro_color.h>

#include <json/json.h>

using std::string;
using std::stringstream;
using std::clog;
using std::endl;
using std::hex;
using std::dec;
using std::uppercase;
using std::nouppercase;
using std::cos;
using std::sin;


void	entity_c::accelerate(var force, var radians) {
	acc[0] += cos(radians) * (force / mass);
	acc[1] += sin(radians) * (force / mass);
}

Json::Value entity_c::json() {

	Json::Value json_blob;

	json_blob["name"] = name;
	json_blob["mass"] = double(mass);
	json_blob["radius"] = double(radius);
	json_blob["pos"]["x"] = double(pos[0]);
	json_blob["pos"]["y"] = double(pos[1]);
	json_blob["v"]["x"] = double(v[0]);
	json_blob["v"]["y"] = double(v[1]);
	json_blob["acc"]["x"] = double(acc[0]);
	json_blob["acc"]["y"] = double(acc[1]);
	json_blob["color"] = al_color_rgb_to_name(color.r, color.g, color.b);

	return json_blob;
}

void	entity_c::print() {
	clog << "\n[" << name << "]";
	clog << "\nmass=" << mass;
	clog << "\nradius=" << radius;
	clog << "\nx=" << pos[0];
	clog << "\ny=" << pos[1];
	clog << "\nVx=" << v[0];
	clog << "\nVy=" << v[1];
	clog << "\naccX=" << acc[0];
	clog << "\naccY=" << acc[1];
	clog << "\ncolor=" << al_color_rgb_to_name(color.r, color.g, color.b);
	clog << endl;
}

entity_c::entity_c(string name_, var m, var r,
				   var x_, var y_, var Vx_, var Vy_, var accX_, var accY_,
				   ALLEGRO_COLOR color_)
	: name(name_), mass(m>0 ? m : 1), radius(r),
	physical_c(x_,y_, Vx_,Vy_, accX_,accY_),
	color(color_) {

	clog << endl << "CONSTRUCTING:";
	print();
}

entity_c::entity_c()
	: name("the nameless"), mass(1e10), radius(1e2),
	physical_c(1337,1337, vect::Random()[0],vect::Random()[1], 0,0),
	color(al_color_name("lightgoldenrodyellow")) {

	clog << "\nEmpty entity created, placing default entity at (" << pos[0] << ", " << pos[1] << ")" << endl;
}

entity_c::~entity_c() {

	clog << endl << "DESTRUCTING:";
	print();
}
