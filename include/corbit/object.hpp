#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <string>
#include <allegro5/allegro_color.h>
extern const unsigned short FPS;

class object_c {

	struct				pos_t {

		long double		x, y;
						pos_t		(long double _x, long double _y);
	} pos;
	struct				v_t : pos_t {

						v_t			(long double _Vx, long double _Vy);
	} v;

	const long double	mass;
	long double			x			() const,
						y			() const;
	long double			Vx			() const,
						Vy			() const;

public:
	//properties
	const std::string	name;
	virtual long double	totalmass	() const;
	const long long		radius;

	void				acc			(long double force, long double radians),
						move		();
	long double			distance2	(const object_c& targ) const,
						distance	(const object_c& targ) const;
	long double			gravity		(const object_c& targ) const;
	void				gravitate	(const object_c& targ);

	long double			Vcen		(const object_c* targ) const,
						Vtan		(const object_c* targ) const;
	long double			orbitV		(const object_c* targ) const;

	const ALLEGRO_COLOR	color;

	object_c (std::string _name, long double _mass, long long _radius,
	          long double _x, long double _y, long double _Vx, long double _Vy,
	          ALLEGRO_COLOR _color);
	~object_c();
protected:

};

#endif // OBJECT_HPP
