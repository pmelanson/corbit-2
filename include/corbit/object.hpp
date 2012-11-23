#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <string>
#include <allegro5/allegro_color.h>
#include <boost/intrusive/list.hpp>

#include <corbit/globals.hpp>

class object_c : public boost::intrusive::list_base_hook<> {
private:

	const std::string	_name;
	const data			_mass,
						_radius;

	vector				_pos,
						_v,
						_acc;

public:
	std::string			name			() const;
	virtual data		mass			() const,
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

	virtual void		set_x			(data x_),
						set_y			(data y_),
						set_Vx			(data Vx_),
						set_Vy			(data Vy_),
						set_accX		(data accX_),
						set_accY		(data accY_),
						set_pos			(vector pos_),
						set_v			(vector v_),
						set_acc			(vector acc_);

	const ALLEGRO_COLOR	color;

	object_c							(std::string name_, data m, data r,
									 data X, data Y, data Vx, data Vy, data accX, data accY,
									 ALLEGRO_COLOR color_);
	object_c							();

	~object_c							();
};

#endif	//OBJECT_HPP
