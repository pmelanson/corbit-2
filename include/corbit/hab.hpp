#ifndef HAB_HPP
#define HAB_HPP

#include <corbit/entity.hpp>

class hab_c : public entity_c {
private:

	long long		_fuel;
	const float		_burn_rate;
	const long long	_thrust;

public:
	hab_c						(std::string name_, var m, var r,
								var x_, var y_, var Vx_, var Vy_, var accX_, var accY_,
								col color_, long long fuel_, float burn_rate_, long long thrust_);
};


#endif	//HAB_HPP

