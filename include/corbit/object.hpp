#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <string>
#include <allegro5/allegro_color.h>
#include <boost/intrusive/list.hpp>

#include <corbit/globals.hpp>

class object_c : public boost::intrusive::list_base_hook<> {
private:

	const std::string	_name;
	const var			_mass,
						_radius;

	vector				_pos,
						_v,
						_acc;

public:
	std::string			name			() const;
	virtual var			mass			() const,
						radius			() const,
						x				() const,
						y				() const,
						Vx				() const,
						Vy				() const,
						accX			() const,
						accY			() const;
	virtual vector		pos				() const,
						v				() const,
						acc				() const;

	virtual void		set_x			(var x_),
						set_y			(var y_),
						set_Vx			(var Vx_),
						set_Vy			(var Vy_),
						set_accX		(var accX_),
						set_accY		(var accY_),
						set_pos			(vector pos_),
						set_v			(vector v_),
						set_acc			(vector acc_);

	const ALLEGRO_COLOR	color;

	object_c							(std::string name_, var m, var r,
										var X, var Y, var Vx, var Vy, var accX, var accY,
										ALLEGRO_COLOR color_);
	object_c							();

	~object_c							();
};

#endif	//OBJECT_HPP
