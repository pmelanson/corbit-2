#ifndef HAB_HPP
#define HAB_HPP

#include <corbit/entity.hpp>

class hab_c : public entity_c {
public:
	long long		fuel;
	const int		Isp;
	const int		thrust;

public:
	hab_c						(std::string name_, var m, var r,
								var x_, var y_, var Vx_, var Vy_, var accX_, var accY_,
								ALLEGRO_COLOR color_, long long fuel_, int Isp_, int thrust_);
};


#endif	//HAB_HPP

