#include <corbit/entity.hpp>

#define _USE_MATH_DEFINES
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


var		entity_c::moment_inertia() const {
	return (2 * mass() * radius*radius) / 5;
}

void	entity_c::accelerate (vect force, var radians) {
	/*
	a = Fcos(theta) / m

	where
	a: linear acceleration
	F: net force applied
	cos(theta): centripetal force only
	m: mass
	*/
	acc += (force * cos (radians)) / mass();

	/*
	w = T / I

	where
	w: angular acceleration
	T: torque
	I: moment of inertia
	*/
	ang_acc += (force.norm() * sin(radians)) / moment_inertia();
}

void	entity_c::move		(var time) {
	v	+=	acc * time;
	acc.setZero();
	pos	+=	v * time;

	ang_v += ang_acc * time;
	ang_acc = 0;
	ang_pos += ang_v * time;
}

var		entity_c::mass() const {
	return _mass;
}

Json::Value entity_c::json() const {

	Json::Value json_blob;

	json_blob["name"] = name;
	json_blob["mass"] = double(_mass);
	json_blob["radius"] = double(radius);
	json_blob["ang_pos"] = double(ang_pos);
	json_blob["ang_v"] = double(ang_v);
	json_blob["ang_acc"] = double(ang_acc);
	json_blob["pos"]["x"] = double(pos[0]);
	json_blob["pos"]["y"] = double(pos[1]);
	json_blob["v"]["x"] = double(v[0]);
	json_blob["v"]["y"] = double(v[1]);
	json_blob["acc"]["x"] = double(acc[0]);
	json_blob["acc"]["y"] = double(acc[1]);
	json_blob["color"] = al_color_rgb_to_name(color.r, color.g, color.b);

	return json_blob;
}


entity_c::entity_c(ENTITY_TYPE type_, string name_, var m, var r,
				   var ang_pos_, var ang_v_, var ang_acc_,
				   var x_, var y_, var Vx_, var Vy_, var accX_, var accY_,
				   ALLEGRO_COLOR color_)
	: type (type_), name (name_), _mass (m>0 ? m : 1), radius (r),
	ang_pos (ang_pos_), ang_v (ang_v_), ang_acc (ang_acc_),
	physical_c (x_,y_, Vx_,Vy_, accX_,accY_),
	color (color_) {

//	clog << endl << "CONSTRUCTING: " << name;
}

entity_c::~entity_c() {

//	clog << endl << "DESTRUCTING: " << name;
}
