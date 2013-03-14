#ifndef CALC_HPP
#define CALC_HPP


#include <corbit/globals.hpp>

class entity_c;

namespace calc {

	var				distance2		(const entity_c &A, const entity_c &B),
					step_distance2	(const entity_c &A, const entity_c &B),
					distance		(const entity_c &A, const entity_c &B),
					step_distance	(const entity_c &A, const entity_c &B),
					theta			(const entity_c &A, const entity_c &B),
					theta			(const entity_c &A, const entity_c &B, const entity_c &C),
					gravity			(const entity_c &A, const entity_c &B),
					v_orbit			(const entity_c &A, const entity_c &B),
					v_cen			(const entity_c &A, const entity_c &B),
					v_tan			(const entity_c &A, const entity_c &B),
					pitch			(const entity_c &A, const entity_c &B),
					stopping_acc	(const entity_c &A, const entity_c &B),
					ecc				(const entity_c &A, const entity_c &B),
					periapsis		(const entity_c &A, const entity_c &B),
					apoapsis		(const entity_c &A, const entity_c &B);
	vect			position		(const entity_c &A, const entity_c &B),
					velocity		(const entity_c &A, const entity_c &B);
	bool			approaching		(const entity_c &A, const entity_c &B);

	void			detect_collision(entity_c &A, entity_c &B);
};


#endif	//CALC_HPP
