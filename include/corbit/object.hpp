#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <string>
#include <allegro5/allegro_color.h>
#include <boost/intrusive/list.hpp>

#include <corbit/globals.hpp>
extern const unsigned short FPS;

class object_c : public boost::intrusive::list_base_hook<> {
private:

	const data			_mass,
						_radius;

	#include <corbit/dynamics.hpp>

public:
	const std::string	name;

	void				accelerate	(data force, data radians),
						move		();
	data				distance2	(const object_c& targ) const,
						distance	(const object_c& targ) const,
						gravity		(const object_c& targ) const,
						thetaobject	(const object_c& targ) const,
						Vcen		(const object_c& targ) const,
						Vtan		(const object_c& targ) const,
						orbitV		(const object_c& targ) const,

						x			() const,
						y			() const,
						Vx			() const,
						Vy			() const,
						accX		() const,
						accY		() const,
						radius		() const;
	virtual data		mass		() const;

	const ALLEGRO_COLOR	color;

	boost::intrusive::list_member_hook<> hook;

	object_c (std::string name_, data m, data r,
			  data X, data Y, data Vx, data Vy, data accX, data accY,
			  ALLEGRO_COLOR color_);
};

#endif	//OBJECT_HPP
