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
					gravity			(const entity_c &A, const entity_c &B),
					orbitV			(const entity_c &A, const entity_c &B),
					Vcen			(const entity_c &A, const entity_c &B),
					Vtan			(const entity_c &A, const entity_c &B),
					ecc				(const entity_c &A, const entity_c &B);
	vect			position		(const entity_c &A, const entity_c &B),
					velocity		(const entity_c &A, const entity_c &B);
	bool			approaching		(const entity_c &A, const entity_c &B);

	void			detect_collision(entity_c &A, entity_c &B);
};


#endif	//CALC_HPP
