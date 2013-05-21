#include <corbit/hab.hpp>

#include <json/json.h>


var			hab_c::mass() {
	return _mass + fuel;
}

void		hab_c::burn() {

	fuel -= 1;

	fuel -= throttle * Isp;

	accelerate (vect (throttle * thrust * cos (ang_pos),
				throttle * thrust * sin (ang_pos)),
				0);
}

Json::Value	hab_c::json() {

	Json::Value json_blob;

	json_blob = entity_c::json();

	json_blob["fuel"] = double(fuel);
	json_blob["Isp"] = Isp;
	json_blob["thrust"] = double(thrust);

	return json_blob;
}


hab_c::hab_c (ENTITY_TYPE type_, std::string name_, var m, var r,
			 var ang_pos_, var ang_v_, var ang_acc_,
			 var x_, var y_, var Vx_, var Vy_, var accX_, var accY_,
			 ALLEGRO_COLOR color_, var fuel_, int Isp_, var thrust_)
	: entity_c (type_, name_, m, r, ang_pos_, ang_v_, ang_acc_, x_,y_, Vx_,Vy_, accX_,accY_, color_),
	fuel (fuel_), throttle (0), Isp (Isp_), thrust (thrust_) {}
