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


void	entity_c::accelerate (var force, var radians) {
	acc[0] += cos(radians) * (force / mass());
	acc[1] += sin(radians) * (force / mass());
}

var		entity_c::mass() const {
	return _mass;
}

var		entity_c::pitch() const {
	return _pitch;
}

var		entity_c::rot_speed() const {
	return _rot_speed;
}

Json::Value entity_c::json() {

	Json::Value json_blob;

	json_blob["name"] = name;
	json_blob["mass"] = double(_mass);
	json_blob["radius"] = double(radius);
	json_blob["rot_speed"] = double(_rot_speed);
	json_blob["pitch"] = double(_pitch);
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
	clog << "\ntype=" << type;
	clog << "\nmass=" << _mass;
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

entity_c::entity_c(ENTITY_TYPE type_, string name_, var m, var r, var rot_speed_,
				   var pitch_, var x_, var y_, var Vx_, var Vy_, var accX_, var accY_,
				   ALLEGRO_COLOR color_)
	: type (type_), name (name_), _mass (m>0 ? m : 1), radius (r),
	_rot_speed (rot_speed_), _pitch (pitch_), I ((mass() * radius * radius) / 4),
	physical_c (x_,y_, Vx_,Vy_, accX_,accY_),
	color (color_) {

	clog << endl << "CONSTRUCTING:";
	print();
}

entity_c::~entity_c() {

	clog << endl << "DESTRUCTING:";
	print();
}
