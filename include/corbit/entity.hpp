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
public:

	const ENTITY_TYPE	type;

	const std::string	name;

protected:
	var					_mass;

public:
	virtual var		mass		() const;
	const var			radius;
	var					ang_pos;
	var					ang_v;
	var					ang_acc;

	virtual var		moment_inertia() const;
	virtual void		accelerate	(vect force, var radians);
	virtual void		move		();

	const ALLEGRO_COLOR	color;

	virtual Json::Value json		() const;
	void				print		() const;

	entity_c						(ENTITY_TYPE type_, std::string name_, var m, var r,
									var ang_pos_, var ang_speed_, var ang_acc_,
									var x_, var y_, var Vx_, var Vy_, var accX_, var accY_,
									ALLEGRO_COLOR color_);

	~entity_c						();
};

#endif	//ENTITY_HPP
