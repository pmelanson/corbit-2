#ifndef CALC_HPP
#define CALC_HPP

#include <boost/intrusive/list.hpp>

#include <corbit/globals.hpp>

class calc_c {
public:

	class object_c	*_active_ship,	//ship currently controlled
					*_target,
					*_reference;

	void			accelerate		(object_c& A, data force, data radians),
					move			(object_c& A);
	data			distance2		(const object_c& A, const object_c& B) const,
					distance		(const object_c& A, const object_c& B) const,
					gravity			(const object_c& A, const object_c& B) const,
					theta_object	(const object_c& A, const object_c& B) const,
					Vcen			(const object_c& A, const object_c& B) const,
					Vtan			(const object_c& A, const object_c& B) const,
					orbitV			(const object_c& A, const object_c& B) const;

					calc_c			(class object_c* active_ship_, class object_c* target_, class object_c* reference_);
public:

	class object_c	*active_ship	() const,
					*target			() const,
					*reference		() const;
	void			set_active_ship	(class object_c* new_ship),
					set_target		(class object_c* new_target),
					set_reference	(class object_c* new_reference);

	void			update			(boost::intrusive::list<object_c> &list);

	static calc_c&	get_instance	(class object_c* active_ship_, class object_c* target_, class object_c* reference_);

					~calc_c			();

//	static calc_c&	get_instance	(class object_c* activeship_, class object_c* target_, class object_c* reference_);
};

#endif	//CALC_HPP
