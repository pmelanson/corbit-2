#ifndef ENTITY_HPP
#define ENTITY_HPP


#include <string>
#include <iostream>
#include <boost/intrusive/list.hpp>

#include <corbit/physical.hpp>

namespace Json {
	class Value;
}

class entity_c : public physical_c, public boost::intrusive::list_base_hook<> {
protected:
	var					I;	//moment of inertia
	var					_mass;

	var					_rot_speed;
	var					_pitch;


public:

	const ENTITY_TYPE	type;

	const std::string	name;
	virtual var		mass		() const;
	const var			radius;
	var					pitch		() const;
	var					rot_speed	() const;
	void				spin		();

	void				accelerate	(var force, var radians);

	const ALLEGRO_COLOR	color;

	virtual Json::Value json		();
	void				print		();

	entity_c						(ENTITY_TYPE type_, std::string name_, var m, var r, var rot_speed_,
									var pitch_, var x_, var y_, var Vx_, var Vy_, var accX_, var accY_,
									ALLEGRO_COLOR color_);
	entity_c						();

	~entity_c						();
};

#endif	//ENTITY_HPP
