#ifndef CALC_HPP
#define CALC_HPP
#include <corbit/globals.hpp>

#include <boost/intrusive/list.hpp>
#define _USE_MATH_DEFINES
#include <cmath>

class object_c;
const float	G = 6.674e-11;

class calc_c {
public:

	object_c		*_active_ship,	//ship currently controlled
					*_target,
					*_reference;

	void			accelerate		(object_c& A, var force, var radians),
					move			(object_c& A);
	var				distance2		(const object_c& A, const object_c& B) const,
					distance		(const object_c& A, const object_c& B) const,
					gravity			(const object_c& A, const object_c& B) const,
					theta_object	(const object_c& A, const object_c& B) const,
					Vcen			(const object_c& A, const object_c& B) const,
					Vtan			(const object_c& A, const object_c& B) const,
					orbitV			(const object_c& A, const object_c& B) const;

					calc_c			(object_c* active_ship_, object_c* target_, object_c* reference_);
public:

	object_c		*active_ship	() const,
					*target			() const,
					*reference		() const;
	void			set_active_ship	(object_c* new_ship),
					set_target		(object_c* new_target),
					set_reference	(object_c* new_reference);

	void			update			(boost::intrusive::list<object_c> &list);

	static calc_c&	get_instance	(object_c* active_ship_, object_c* target_, object_c* reference_);

					~calc_c			();
};

#endif	//CALC_HPP
