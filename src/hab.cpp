#include <corbit/hab.hpp>

hab_c::hab_c(std::string name_, var m, var r,
			 var x_, var y_, var Vx_, var Vy_, var accX_, var accY_,
			 ALLEGRO_COLOR color_, long long fuel_, int Isp_, int thrust_)
	: entity_c (name_, m, r, x_,y_, Vx_,Vy_, accX_,accY_, color_),
	fuel (fuel_), Isp (Isp_), thrust (thrust_) {}
