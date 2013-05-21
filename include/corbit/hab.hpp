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
	var				throttle;
	const int		I_sp;
	const var		thrust;
	void			burn		();
	void			move		();
	var				mass		() const;

	Json::Value		json		() const;

	hab_c						(ENTITY_TYPE type_, std::string name_, var m, var r,
								var ang_pos_, var ang_v_, var ang_acc_,
								var x_, var y_, var Vx_, var Vy_, var accX_, var accY_,
								ALLEGRO_COLOR color_, var fuel_, int I_sp_, var thrust_);
};


#endif	//HAB_HPP

