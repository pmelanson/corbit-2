#ifndef CALC_HPP
#define CALC_HPP
#include <corbit/globals.hpp>

#include <boost/intrusive/list.hpp>

class object_c;

class calc_c {
private:

	object_c		*_active_ship,	//ship currently controlled
					*_targ,
					*_ref;

	void			_accelerate		(object_c &A, var force, var radians),
					_move			(object_c &A);

					calc_c			(object_c *active_ship_, object_c *target_, object_c *reference_);
public:

	object_c		*active_ship	() const,
					*target			() const,
					*reference		() const;
	void			set_active_ship	(object_c *new_ship),
					set_target		(object_c *new_target),
					set_reference	(object_c *new_reference);

	void			ship_accelerate	(var force, var radians);

	var				distance2		(const object_c &A, const object_c &B) const,
					distance		(const object_c &A, const object_c &B) const,
					theta_object	(const object_c &A, const object_c &B) const,
					gravity			(const object_c &A, const object_c &B) const,
					Vcen			(const object_c &A, const object_c &B) const,
					Vtan			(const object_c &A, const object_c &B) const,
					orbitV			(const object_c &A, const object_c &B) const,
					ship_targ_orbitV() const,
					ship_ref_orbitV	() const,
					eccentricity	(const object_c &A, const object_c &B) const,
					ship_ecc		(const object_c &A) const,
					ship_targ_ecc	() const,
					ship_ref_ecc	() const;

	vector			relative_pos	(const object_c &A, const object_c &B) const,
					relative_v		(const object_c &A, const object_c &B) const,
					ship_relative_pos(const object_c &A) const,
					ship_relative_v	(const object_c &A) const,
					ship_targ_v		() const,
					ship_ref_v		() const;

	void			update			(boost::intrusive::list<object_c> &list);

	static calc_c	&get_instance	(object_c *active_ship_, object_c *target_, object_c *reference_);

					~calc_c			();
};

#endif	//CALC_HPP
