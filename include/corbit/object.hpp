#ifndef OBJECT_HPP
#define OBJECT_HPP


#include <string>
#include <boost/intrusive/list.hpp>

#include <corbit/physical.hpp>

class object_c : public physical_c, public boost::intrusive::list_base_hook<> {
public:

	typedef boost::intrusive::list<object_c> object_list;
	static object_list objects;

	const std::string	name;
	const var			mass,
						radius;

	void				accelerate	(var force, var radians);

	const col			color;

	object_c						(std::string name_, var m, var r,
									var x_, var y_, var Vx_, var Vy_, var accX_, var accY_,
									col color_);
	object_c						();

	~object_c						();

	friend void				swap		(object_c &first, object_c &second);

    object_c& operator=	(const object_c &other);
};

#endif	//OBJECT_HPP
