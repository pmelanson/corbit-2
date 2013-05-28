#include <corbit/hab.hpp>

#include <json/json.h>


var			hab_c::mass() const {
	return _mass + fuel;
}

void		hab_c::burn() {

	if (throttle == 0) {
		return;
	}

	const var g = 9.80665;

	var fuel_rate = thrust / (g * I_sp);

	var fuel_use = fuel / fuel_rate;	//ratio of how much fuel was used
						//when 1, maximum amount of fuel was used

	if (fuel_use > 1) {
		fuel_use = 1;
	}

	var acc = fuel_use * throttle * thrust;

	accelerate (vect (fuel_use * throttle * thrust * cos (ang_pos),
				fuel_use * throttle * thrust * sin (ang_pos)),
				0);

	fuel -= fuel_use * fuel_rate;
}

void		hab_c::move	(var time) {
	burn();

	entity_c::move (time);
}

Json::Value	hab_c::json() const {

	Json::Value json_blob;

	json_blob = entity_c::json();

	json_blob["fuel"] = double(fuel);
	json_blob["I_sp"] = I_sp;
	json_blob["thrust"] = double(thrust);

	return json_blob;
}


hab_c::hab_c (ENTITY_TYPE type_, std::string name_, var m, var r,
			 var ang_pos_, var ang_v_, var ang_acc_,
			 var x_, var y_, var Vx_, var Vy_, var accX_, var accY_,
			 ALLEGRO_COLOR color_, var fuel_, int I_sp_, var thrust_)
	: entity_c (type_, name_, m, r, ang_pos_, ang_v_, ang_acc_, x_,y_, Vx_,Vy_, accX_,accY_, color_),
	fuel (fuel_), throttle (0), I_sp (I_sp_), thrust (thrust_) {}
