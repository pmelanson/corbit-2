#ifndef HAB_HPP
#define HAB_HPP

#include <corbit/entity.hpp>

enum ENTITY_TYPE;
namespace Json {
	class Value;
}


class hab_c : public entity_c {
public:
	var				fuel;
	const int		Isp;
	const var		thrust;
	var				mass		();

	Json::Value		json		();

	hab_c						(ENTITY_TYPE type_, std::string name_, var m, var r,
								var x_, var y_, var Vx_, var Vy_, var accX_, var accY_,
								ALLEGRO_COLOR color_, var fuel_, int Isp_, var thrust_);
};


#endif	//HAB_HPP

