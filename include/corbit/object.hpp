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
	std::string			name		() const;

	void				accelerate	(data force, data radians),
						move		();
	data				distance2	(const object_c& targ) const,
						distance	(const object_c& targ) const,
						gravity		(const object_c& targ) const,
						thetaobject	(const object_c& targ) const,
						Vcen		(const object_c& targ) const,
						Vtan		(const object_c& targ) const,
						orbitv		(const object_c& targ) const,

						x			() const,
						y			() const,
						Vx			() const,
						Vy			() const,
						accX		() const,
						accY		() const,
						radius		() const;
	virtual data		mass		() const;
	vector				pos			() const,
						v			() const,
						acc			() const;

	ALLEGRO_COLOR		color;

	object_c						(std::string name_, data m, data r,
									 data X, data Y, data Vx, data Vy, data accX, data accY,
									 ALLEGRO_COLOR color_);
	object_c						();

	~object_c						();
};

#endif	//OBJECT_HPP
