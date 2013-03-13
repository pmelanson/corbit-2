#include <corbit/hab.hpp>

hab_c::hab_c(std::string name_, var m, var r,
			 var x_, var y_, var Vx_, var Vy_, var accX_, var accY_,
			 col color_, long long fuel_, float burn_rate_, long long thrust_)
	: entity_c(name_, m, r, x_,y_, Vx_,Vy_, accX_,accY_, color_),
	_fuel(fuel_), _burn_rate(burn_rate_), _thrust(thrust_) {}
