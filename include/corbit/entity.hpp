#ifndef ENTITY_HPP
#define ENTITY_HPP


#include <string>
#include <iostream>
#include <boost/intrusive/list.hpp>

#include <corbit/physical.hpp>

class entity_c : public physical_c, public boost::intrusive::list_base_hook<> {
public:

	const std::string	name;
	const var			mass,
						radius;

	void				accelerate		(var force, var radians);

	const col			color;

	entity_c							(std::string name_, var m, var r,
										var x_, var y_, var Vx_, var Vy_, var accX_, var accY_,
										col color_);
	entity_c							();

	~entity_c							();
};

#endif	//ENTITY_HPP
